/***************************************************************************/
/*                                                                         */
/* ftplib.c - callable ftp access routines                                 */
/* Copyright (C) 1996-2001 Thomas Pfau, pfau@eclipse.net                   */
/*      1407 Thomas Ave, North Brunswick, NJ, 08902                        */
/*                                                                         */
/* This library is free software; you can redistribute it and/or           */
/* modify it under the terms of the GNU Library General Public             */
/* License as published by the Free Software Foundation; either            */
/* version 2 of the License, or (at your option) any later version.        */
/*                                                                         */
/* This library is distributed in the hope that it will be useful,         */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of          */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU       */
/* Library General Public License for more details.                        */
/*                                                                         */
/* You should have received a copy of the GNU Library General Public       */
/* License along with this progam; if not, write to the                    */
/* Free Software Foundation, Inc., 59 Temple Place - Suite 330,            */
/* Boston, MA 02111-1307, USA.                                             */
/*                                                                         */
/***************************************************************************/

#include <Core/Core.h>
#if defined(_WIN32)
	#include <winsock2.h>
	#include <WS2tcpip.h>
#else
	#include <sys/time.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netdb.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
#endif

#define BUILDING_LIBRARY
#include "ftplib.h"

#define LLOG(x)      //RLOG(x)
#define LLOGBLOCK(x) //RLOGBLOCK(x)
#define LDUMP(x)     //RDUMP(x)

#if defined(_WIN32)
#define SETSOCKOPT_OPTVAL_TYPE (const char *)
#define ERRPENDING WSAEWOULDBLOCK
#else
#define SETSOCKOPT_OPTVAL_TYPE (void *)
#define ERRPENDING EINPROGRESS
#endif

#define FTPLIB_BUFSIZ 8192
#define ACCEPT_TIMEOUT 30

#define FTPLIB_CONTROL 0
#define FTPLIB_READ 1
#define FTPLIB_WRITE 2

#if !defined FTPLIB_DEFMODE
#define FTPLIB_DEFMODE FTPLIB_PASSIVE
#endif

struct NetBuf {
	char *cput,*cget;
	int handle;
	int cavail,cleft;
	char *buf;
	int dir;
	netbuf *ctrl;
	netbuf *data;
	int cmode;
	struct timeval idletime;
	int idletimeout_secs;
	FtpCallback idlecb;
	void *idlearg;
	int xfered;
	int cbbytes;
	int xfered1;
	char response[256];
	char perror[256];
};

/*
static const char version[] =
	"ftplib Release 3.1-1 9/16/00, copyright 1996-2000 Thomas Pfau";
*/

GLOBALDEF int ftplib_debug = 0;

#ifdef _WIN32
#define net_read(x,y,z) recv(x,y,z,0)
#define net_write(x,y,z) send(x,y,z,0)
#define net_close closesocket
#else
#define net_read read
#define net_write write
#define net_close close
#endif

#if defined(NEED_MEMCCPY)
/*
* VAX C does not supply a memccpy routine so I provide my own
*/
void *memccpy(void *dest, const void *src, int c, size_t n)
{
	int i=0;
	const unsigned char *ip=src;
	unsigned char *op=dest;

	while (i < n)
	{
		if ((*op++ = *ip++) == c)
			break;
		i++;
	}
	if (i == n)
		return NULL;
	return op;
}
#endif

/*
* socket_wait - wait for socket to receive or flush data
*
* return 1 if no user callback, otherwise, return value returned by
* user callback
*/
static int socket_wait(netbuf *ctl, int write)
{
	fd_set fd,*rfd = NULL,*wfd = NULL;
	struct timeval tv;
	int rv = 0;
	if (ctl->idlecb == NULL)
		return 1;
	if (write)
		wfd = &fd;
	else
		rfd = &fd;
	FD_ZERO(&fd);
	int idle_end = UPP::msecs() + 1000 * ctl->idletimeout_secs;
	do
	{
		FD_SET(ctl->handle,&fd);
		tv = ctl->idletime;
		rv = select(ctl->handle+1, rfd, wfd, NULL, &tv);
		if (rv == -1)
		{
			rv = 0;
			strcpy(ctl->perror, strerror(errno));
			break;
		}
		else if (rv > 0)
		{
			rv = 1;
			break;
		}
		else if(UPP::msecs(idle_end) >= 0) {
			rv = 0;
			sprintf(ctl->perror, "idle timeout expired (%d secs)", ctl->idletimeout_secs);
			break;
		}
	}
	while ((rv = ctl->idlecb(ctl, ctl->xfered, ctl->idlearg)));
	return rv;
}

/*
* read a line of text
*
* return -1 on error or bytecount
*/
static int readline(char *buf,int max,netbuf *ctl)
{
	int x,retval = 0;
	char *end,*bp=buf;
	int eof = 0;

	if ((ctl->dir != FTPLIB_CONTROL) && (ctl->dir != FTPLIB_READ))
		return -1;
	if (max == 0)
		return 0;
	do
	{
		if (ctl->cavail > 0)
		{
			x = (max >= ctl->cavail) ? ctl->cavail : max-1;
			end = (char *)memccpy(bp,ctl->cget,'\n',x);
			if (end != NULL)
				x = end - bp;
			retval += x;
			bp += x;
			*bp = '\0';
			max -= x;
			ctl->cget += x;
			ctl->cavail -= x;
			if (end != NULL)
			{
				bp -= 2;
				if (strcmp(bp,"\r\n") == 0)
				{
					*bp++ = '\n';
					*bp++ = '\0';
					--retval;
				}
				break;
			}
		}
		if (max == 1)
		{
			*buf = '\0';
			break;
		}
		if (ctl->cput == ctl->cget)
		{
			ctl->cput = ctl->cget = ctl->buf;
			ctl->cavail = 0;
			ctl->cleft = FTPLIB_BUFSIZ;
		}
		if (eof)
		{
			if (retval == 0)
				retval = -1;
			break;
		}
		if (!socket_wait(ctl, 0))
			return retval;
		if ((x = net_read(ctl->handle,ctl->cput,ctl->cleft)) == -1)
		{
			strcpy(ctl->perror, "read");
			retval = -1;
			break;
		}
		if (x == 0)
			eof = 1;
		ctl->cleft -= x;
		ctl->cavail += x;
		ctl->cput += x;
	}
	while (1);
	LLOG("readline(" << retval << ") -> " << buf);
	return retval;
}

/*
* write lines of text
*
* return -1 on error or bytecount
*/
static int writeline(char *buf, int len, netbuf *nData)
{
	int x, nb=0, w;
	char *ubp = buf, *nbp;
	char lc=0;

	if (nData->dir != FTPLIB_WRITE)
		return -1;
	nbp = nData->buf;
	for (x=0; x < len; x++)
	{
		if ((*ubp == '\n') && (lc != '\r'))
		{
			if (nb == FTPLIB_BUFSIZ)
			{
				if (!socket_wait(nData, 1))
					return x;
				w = net_write(nData->handle, nbp, FTPLIB_BUFSIZ);
				if (w != FTPLIB_BUFSIZ) {
					RLOG("net_write(1) returned " << w << ", errno = " << errno);
					return(-1);
				}
				nb = 0;
			}
			nbp[nb++] = '\r';
		}
		if (nb == FTPLIB_BUFSIZ)
		{
			if (!socket_wait(nData, 1))
				return x;
			w = net_write(nData->handle, nbp, FTPLIB_BUFSIZ);
			if (w != FTPLIB_BUFSIZ) {
				RLOG("net_write(2) returned " << w << ", errno = " << errno);
				return(-1);
			}
			nb = 0;
		}
		nbp[nb++] = lc = *ubp++;
	}
	if (nb)
	{
		if (!socket_wait(nData, 1))
			return x;
		w = net_write(nData->handle, nbp, nb);
		if (w != nb) {
			RLOG("net_write(3) returned " << w << ", errno = " << errno);
			return(-1);
		}
	}
	return len;
}

/*
* read a response from the server
*
* return 0 if first char doesn't match
* return 1 if first char matches
*/
static int readresp(char c, netbuf *nControl)
{
	char match[5];
	if (readline(nControl->response,256,nControl) == -1)
	{
		strcpy(nControl->perror, "Control socket read failed");
		return 0;
	}
	if (ftplib_debug > 1)
		fprintf(stderr,"%s",nControl->response);
	if (nControl->response[3] == '-')
	{
		strncpy(match,nControl->response,3);
		match[3] = ' ';
		match[4] = '\0';
		do
		{
			if (readline(nControl->response,256,nControl) == -1)
			{
				strcpy(nControl->perror, "Control socket read failed");
				return 0;
			}
			if (ftplib_debug > 1)
				fprintf(stderr,"%s",nControl->response);
		}
		while (strncmp(nControl->response,match,4));
	}
	if (nControl->response[0] == c)
		return 1;
	strcpy(nControl->perror, nControl->response);
	return 0;
}

/*
* FtpInit for stupid operating systems that require it (Windows NT)
*/
GLOBALDEF void FtpInit(void)
{
#if defined(_WIN32)
	WORD wVersionRequested;
	WSADATA wsadata;
	int err;
	wVersionRequested = MAKEWORD(1,1);
	if ((err = WSAStartup(wVersionRequested,&wsadata)) != 0)
		fprintf(stderr,"Network failed to start: %d\n",err);
#endif
}

/*
* FtpLastResponse - return a pointer to the last response received
*/
GLOBALDEF char *FtpLastResponse(netbuf *nControl)
{
	if ((nControl) && (nControl->dir == FTPLIB_CONTROL))
		return nControl->response;
	return NULL;
}

static int FtpLastError()
{
#ifdef flagWIN32
	return WSAGetLastError();
#define SOCKERR(x) WSA##x
#else
	return errno;
#define SOCKERR(x) x
#endif
}

static int FtpBlock(int sockno, int block, char perror[])
{
#ifdef flagWIN32
	u_long arg = (block ? 0 : 1);
	if(ioctlsocket(sockno, FIONBIO, &arg)) {
		strcpy(perror, "ioctlsocket(FIONBIO)");
		return 0;
	}
#else
	if(fcntl(sockno, F_SETFL, (fcntl(sockno, F_GETFL, 0) & ~O_NONBLOCK) | (block ? 0 : O_NONBLOCK))) {
		strcpy(perror, "fcntl(O_NONBLOCK)");
		return 0;
	}
#endif
	return 1;
}

static int FtpCheckWrite(int sockno, struct timeval *tv)
{
	fd_set mask;
	FD_ZERO(&mask);
	FD_SET(sockno, &mask);
	return select(sockno + 1, NULL, &mask, &mask, tv) > 0 ? 1 : 0;
}

/*
* FtpConnect - connect to remote server
*
* return 1 if connected, 0 if not
*/
GLOBALDEF int FtpConnect(const char *host, netbuf **nControl, char perror[],
	FtpCallback idlecb, void *idledata, int idletime_msecs, int idletimeout_secs)
{
	int sControl;
	struct sockaddr_in sin;
	struct hostent *phe;
	struct servent *pse;
	int on=1;
	netbuf *ctrl;
	struct timeval tm;

	tm.tv_sec = idletime_msecs / 1000;
	tm.tv_usec = (idletime_msecs % 1000) * 1000;
	memset(&sin,0,sizeof(sin));
	sin.sin_family = AF_INET;
	UPP::String lhost = host;
	int pnum = lhost.Find(':');
	if (pnum < 0)
	{
#if defined(VMS)
		sin.sin_port = htons(21);
#else
		if((pse = getservbyname("ftp", "tcp")))
			sin.sin_port = pse->s_port;
		else
			sin.sin_port = htons(21);
#endif
	}
	else
	{
		UPP::String port = lhost.Mid(pnum + 1);
		lhost.Trim(pnum);
		if(UPP::IsDigit(*port))
			sin.sin_port = htons((short)atoi(port));
		else {
			pse = getservbyname(port,"tcp");
			sin.sin_port = pse->s_port;
		}
	}
	if ((sin.sin_addr.s_addr = inet_addr(lhost)) == -1)
	{
		if ((phe = gethostbyname(lhost)) == NULL)
		{
			strcpy(perror, "gethostbyname");
			return 0;
		}
		memcpy((char *)&sin.sin_addr, phe->h_addr, phe->h_length);
	}
	sControl = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sControl == -1)
	{
		strcpy(perror, "socket");
		return 0;
	}
	if (setsockopt(sControl,SOL_SOCKET,SO_REUSEADDR,
				SETSOCKOPT_OPTVAL_TYPE &on, sizeof(on)) == -1)
	{
		strcpy(perror, "setsockopt");
		net_close(sControl);
		return 0;
	}
	if(idlecb && !FtpBlock(sControl, 0, perror)) {
		net_close(sControl);
		return 0;
	}
	LLOG("connect(" << FormatIntHex(sin.sin_addr.s_addr) << ", port " << FormatIntHex(sin.sin_port, 4) << ")");
	if (connect(sControl, (struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		if(idlecb && FtpLastError() == ERRPENDING) {
		#ifdef PLATFORM_POSIX
			int idle_end = UPP::GetTickCount() + 1000 * idletimeout_secs;
		#else
			int idle_end = GetTickCount() + 1000 * idletimeout_secs;
		#endif
			while(!FtpCheckWrite(sControl, &tm)) {
				if(!idlecb(NULL, -1, idledata)) {
					strcpy(perror, "connect aborted");
					net_close(sControl);
					return 0;
				}
		#ifdef PLATFORM_POSIX
				if((int)(UPP::GetTickCount() - idle_end) >= 0)
		#else
				if((int)(GetTickCount() - idle_end) >= 0)
		#endif
				{
					sprintf(perror, "connect timed out (%d secs)", idletimeout_secs);
					net_close(sControl);
					return 0;
				}
			}
		}
		else {
			strcpy(perror, "connect");
			net_close(sControl);
			return 0;
		}
	}
	if(idlecb && !FtpBlock(sControl, 1, perror)) {
		net_close(sControl);
		return 0;
	}
	ctrl = new NetBuf;
	UPP::Zero(*ctrl);
	ctrl->buf = new char[FTPLIB_BUFSIZ];
	ctrl->handle = sControl;
	ctrl->dir = FTPLIB_CONTROL;
	ctrl->ctrl = NULL;
	ctrl->cmode = FTPLIB_DEFMODE;
	ctrl->idlecb = idlecb;
	ctrl->idletime = tm;
	ctrl->idletimeout_secs = idletimeout_secs;
	ctrl->idlearg = idledata;
	ctrl->xfered = 0;
	ctrl->xfered1 = 0;
	ctrl->cbbytes = 1;
	if (readresp('2', ctrl) == 0)
	{
		net_close(sControl);
		if(ctrl->buf)
			delete[] ctrl->buf;
		delete ctrl;
		return 0;
	}   
	*nControl = ctrl;
	return 1;
}

/*
* FtpOptions - change connection options
*
* returns 1 if successful, 0 on error
*/
GLOBALDEF int FtpOptions(int opt, uintptr_t val, netbuf *nControl)
{
	int v,rv=0;
	switch (opt)
	{
	case FTPLIB_CONNMODE:
		v = (int) val;
		if ((v == FTPLIB_PASSIVE) || (v == FTPLIB_PORT))
		{
			nControl->cmode = v;
			rv = 1;
		}
		break;
	case FTPLIB_CALLBACK:
		nControl->idlecb = (FtpCallback) val;
		rv = 1;
		break;
	case FTPLIB_IDLETIME:
		v = (int) val;
		rv = 1;
		nControl->idletime.tv_sec = v / 1000;
		nControl->idletime.tv_usec = (v % 1000) * 1000;
		break;
	case FTPLIB_CALLBACKARG:
		rv = 1;
		nControl->idlearg = (void *) val;
		break;
	case FTPLIB_CALLBACKBYTES:
		rv = 1;
		nControl->cbbytes = (int) val;
		break;
	case FTPLIB_IDLETIMEOUT:
		rv = 1;
		nControl->idletimeout_secs = (int) val;
		break;
	}
	return rv;
}

/*
* FtpSendCmd - send a command and wait for expected response
*
* return 1 if proper response received, 0 otherwise
*/
static int FtpSendCmd(const char *cmd, char expresp, netbuf *nControl)
{
	LLOG("FtpSendCmd(" << cmd << ", expresp = " << expresp << ")");
	char buf[256];
	if (nControl->dir != FTPLIB_CONTROL) {
		LLOG("-> dir != FTPLIB_CONTROL");
		return 0;
	}
	if (ftplib_debug > 2)
		fprintf(stderr,"%s\n",cmd);
	if ((strlen(cmd) + 3) > sizeof(buf)) {
		LLOG("-> buffer size exceeded");
		return 0;
	}
	sprintf(buf,"%s\r\n",cmd);
	if (!socket_wait(nControl, 1)) {
		strcpy(nControl->perror, "write aborted");
		LLOG("-> write aborted in socket_wait");
		return 0;
	}
	if (net_write(nControl->handle,buf,strlen(buf)) <= 0)
	{
		strcpy(nControl->perror, "write");
		LLOG("-> write error");
		return 0;
	}
	return readresp(expresp, nControl);
}

/*
* FtpLogin - log in to remote server
*
* return 1 if logged in, 0 otherwise
*/
GLOBALDEF int FtpLogin(const char *user, const char *pass, netbuf *nControl)
{
	char tempbuf[64];

	if (((strlen(user) + 7) > sizeof(tempbuf)) ||
		((strlen(pass) + 7) > sizeof(tempbuf)))
		return 0;
	sprintf(tempbuf,"USER %s",user);
	if (!FtpSendCmd(tempbuf,'3',nControl))
	{
		if (nControl->response[0] == '2')
			return 1;
		return 0;
	}
	sprintf(tempbuf,"PASS %s",pass);
	return FtpSendCmd(tempbuf,'2',nControl);
}

/*
* FtpOpenPort - set up data connection
*
* return 1 if successful, 0 otherwise
*/
static int FtpOpenPort(netbuf *nControl, netbuf **nData, int mode, int dir)
{
	LLOGBLOCK("FtpOpenPort");

	int sData;
	union {
		struct sockaddr sa;
		struct sockaddr_in in;
	} sin;
	struct linger lng = { 0, 0 };
	socklen_t l;
	int on=1;
	netbuf *ctrl;
	char *cp;
	unsigned int v[6];
	char buf[256];

	if (nControl->dir != FTPLIB_CONTROL)
		return -1;
	if ((dir != FTPLIB_READ) && (dir != FTPLIB_WRITE))
	{
		sprintf(nControl->response, "Invalid direction %d\n", dir);
		return -1;
	}
	if ((mode != FTPLIB_ASCII) && (mode != FTPLIB_IMAGE))
	{
		sprintf(nControl->response, "Invalid mode %c\n", mode);
		return -1;
	}
	l = sizeof(sin);
	if (nControl->cmode == FTPLIB_PASSIVE)
	{
		memset(&sin, 0, l);
		sin.in.sin_family = AF_INET;
		if (!FtpSendCmd("PASV",'2',nControl))
			return -1;
		cp = strchr(nControl->response,'(');
		if (cp == NULL)
			return -1;
		cp++;
		sscanf(cp,"%u,%u,%u,%u,%u,%u",&v[2],&v[3],&v[4],&v[5],&v[0],&v[1]);
		sin.sa.sa_data[2] = v[2];
		sin.sa.sa_data[3] = v[3];
		sin.sa.sa_data[4] = v[4];
		sin.sa.sa_data[5] = v[5];
		sin.sa.sa_data[0] = v[0];
		sin.sa.sa_data[1] = v[1];
	}
	else
	{
		if (getsockname(nControl->handle, &sin.sa, &l) < 0)
		{
			strcpy(nControl->perror, "getsockname");
			return 0;
		}
	}
	sData = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (sData == -1)
	{
		strcpy(nControl->perror, "socket");
		return -1;
	}
	if (setsockopt(sData,SOL_SOCKET,SO_REUSEADDR,
				SETSOCKOPT_OPTVAL_TYPE &on,sizeof(on)) == -1)
	{
		strcpy(nControl->perror, "setsockopt");
		net_close(sData);
		return -1;
	}
	if (setsockopt(sData,SOL_SOCKET,SO_LINGER,
				SETSOCKOPT_OPTVAL_TYPE &lng,sizeof(lng)) == -1)
	{
		strcpy(nControl->perror, "setsockopt");
		net_close(sData);
		return -1;
	}
	if (nControl->cmode == FTPLIB_PASSIVE)
	{
		LLOGBLOCK("FTPLIB_PASSIVE data port connect");
//*
		if(nControl->idlecb && !FtpBlock(sData, 0, nControl->perror)) {
			net_close(sData);
			return -1;
		}
//*/

/*
		for(int retry = 0;; retry++) {
			if(nControl->idlecb && !nControl->idlecb(nControl, -2, nControl->idlearg)) {
				strcpy(nControl->perror, "data connect aborted");
				net_close(sData);
				return -1;
			}
			if(connect(sData, &sin.sa, sizeof(sin.sa)) == 0)
				break;
			int err = FtpLastError();
			if(err != SOCKERR(ETIMEDOUT) || retry >= 10) {
				sprintf(nControl->perror, "data connect error: %d", err);
				net_close(sData);
				return -1;
			}
		}
//*/

//*
		if (connect(sData, &sin.sa, sizeof(sin.sa)) == -1) {
			if(nControl->idlecb && FtpLastError() == ERRPENDING) {
				while(!FtpCheckWrite(sData, &nControl->idletime)) {
					if(!nControl->idlecb(nControl, -2, nControl->idlearg)) {
						strcpy(nControl->perror, "data connect aborted");
						net_close(sData);
						return -1;
					}
				}
			}
			else {
				strcpy(nControl->perror, "connect");
				net_close(sData);
				return -1;
			}
		}
//*/
//*
		if(nControl->idlecb && !FtpBlock(sData, 1, nControl->perror)) {
			net_close(sData);
			return -1;
		}
//*/
	}
	else
	{
		sin.in.sin_port = 0;
		if (bind(sData, &sin.sa, sizeof(sin)) == -1)
		{
			strcpy(nControl->perror, "bind");
			net_close(sData);
			return 0;
		}
		if (listen(sData, 1) < 0)
		{
			strcpy(nControl->perror, "listen");
			net_close(sData);
			return 0;
		}
		if (getsockname(sData, &sin.sa, &l) < 0)
			return 0;
		sprintf(buf, "PORT %d,%d,%d,%d,%d,%d",
				(unsigned char) sin.sa.sa_data[2],
				(unsigned char) sin.sa.sa_data[3],
				(unsigned char) sin.sa.sa_data[4],
				(unsigned char) sin.sa.sa_data[5],
				(unsigned char) sin.sa.sa_data[0],
				(unsigned char) sin.sa.sa_data[1]);
		if (!FtpSendCmd(buf,'2',nControl))
		{
			net_close(sData);
			return 0;
		}
	}
	ctrl = new NetBuf;
	UPP::Zero(*ctrl);
	if (mode == 'A')
		ctrl->buf = new char[FTPLIB_BUFSIZ];
	ctrl->handle = sData;
	ctrl->dir = dir;
	ctrl->idletime = nControl->idletime;
	ctrl->idlearg = nControl->idlearg;
	ctrl->idletimeout_secs = nControl->idletimeout_secs;
	ctrl->xfered = 0;
	ctrl->xfered1 = 0;
	ctrl->cbbytes = nControl->cbbytes;
	if (ctrl->idletime.tv_sec || ctrl->idletime.tv_usec || ctrl->cbbytes)
		ctrl->idlecb = nControl->idlecb;
	else
		ctrl->idlecb = NULL;
	*nData = ctrl;
	return 1;
}

/*
* FtpAcceptConnection - accept connection from server
*
* return 1 if successful, 0 otherwise
*/
static int FtpAcceptConnection(netbuf *nData, netbuf *nControl)
{
	int sData;
	struct sockaddr addr;
	socklen_t l;
	int i;
	struct timeval tv;

	if(nControl->idlecb)
		tv = nControl->idletime;
	else {
		tv.tv_usec = 0;
		tv.tv_sec = nData->idletimeout_secs;
	}

	int end_time = UPP::msecs() + 1000 * nData->idletimeout_secs;
	for(;;) {
		fd_set mask;
		FD_ZERO(&mask);
		FD_SET(nControl->handle, &mask);
		FD_SET(nData->handle, &mask);

		i = nControl->handle;
		if (i < nData->handle)
			i = nData->handle;
		i = select(i+1, &mask, NULL, NULL, &tv);
		if (i == -1) {
			strncpy(nControl->response, strerror(errno),
					sizeof(nControl->response));
			net_close(nData->handle);
			nData->handle = 0;
			return 0;
		}
		else if (i == 0) {
			if(UPP::msecs(end_time) >= 0) {
				strcpy(nControl->response, "timed out waiting for connection");
				net_close(nData->handle);
				nData->handle = 0;
				return 0;
			}
			if(!nControl->idlecb(nControl, -3, nControl->idlearg)) {
				strcpy(nControl->response, "accept aborted");
				net_close(nData->handle);
				nData->handle = 0;
				return 0;
			}
			continue;
		}
		else {
			if (FD_ISSET(nData->handle, &mask)) {
				l = sizeof(addr);
				sData = accept(nData->handle, &addr, &l);
				i = errno;
				net_close(nData->handle);
				if (sData > 0) {
					nData->handle = sData;
					return 1;
				}
				else {
					strncpy(nControl->response, strerror(i),
							sizeof(nControl->response));
					nData->handle = 0;
					return 0;
				}
			}
			else if (FD_ISSET(nControl->handle, &mask)) {
				net_close(nData->handle);
				nData->handle = 0;
				readresp('2', nControl);
				return 0;
			}
		}
	}
}

/*
* FtpAccess - return a handle for a data stream
*
* return 1 if successful, 0 otherwise
*/
GLOBALDEF int FtpAccess(const char *path, int typ, int mode, netbuf *nControl,
	netbuf **nData)
{
	LLOG("FtpAccess");

	char buf[256];
	int dir;
	if ((path == NULL) &&
		((typ == FTPLIB_FILE_WRITE) || (typ == FTPLIB_FILE_READ)))
	{
		sprintf(nControl->response,
				"Missing path argument for file transfer\n");
		return 0;
	}
	sprintf(buf, "TYPE %c", mode);
	if (!FtpSendCmd(buf, '2', nControl))
		return 0;
	switch (typ)
	{
	case FTPLIB_DIR:
		strcpy(buf,"NLST");
		dir = FTPLIB_READ;
		break;
	case FTPLIB_DIR_VERBOSE:
		strcpy(buf,"LIST");
		dir = FTPLIB_READ;
		break;
	case FTPLIB_FILE_READ:
		strcpy(buf,"RETR");
		dir = FTPLIB_READ;
		break;
	case FTPLIB_FILE_WRITE:
		strcpy(buf,"STOR");
		dir = FTPLIB_WRITE;
		break;
	default:
		sprintf(nControl->response, "Invalid open type %d\n", typ);
		return 0;
	}
	if (path != NULL)
	{
		int i = strlen(buf);
		buf[i++] = ' ';
		if ((strlen(path) + i) >= sizeof(buf))
			return 0;
		strcpy(&buf[i],path);
	}
	if (FtpOpenPort(nControl, nData, mode, dir) == -1)
		return 0;
	if (!FtpSendCmd(buf, '1', nControl))
	{
		FtpClose(*nData);
		*nData = NULL;
		return 0;
	}
	(*nData)->ctrl = nControl;
	nControl->data = *nData;
	if (nControl->cmode == FTPLIB_PORT)
	{
		if (!FtpAcceptConnection(*nData,nControl))
		{
			FtpClose(*nData);
			*nData = NULL;
			nControl->data = NULL;
			return 0;
		}
	}
	return 1;
}

/*
* FtpRead - read from a data connection
*/
GLOBALDEF int FtpRead(void *buf, int max, netbuf *nData)
{
	LLOG("FtpRead");

	int i;
	if (nData->dir != FTPLIB_READ)
		return 0;
	if (nData->buf)
		i = readline((char *)buf, max, nData);
	else
	{
		i = socket_wait(nData, 0);
		if (i != 1)
			return 0;
		i = net_read(nData->handle, (char *)buf, max);
	}
	if (i == -1)
		return 0;
	nData->xfered += i;
	if (nData->idlecb && nData->cbbytes)
	{
		nData->xfered1 += i;
		if (nData->xfered1 > nData->cbbytes)
		{
			if (nData->idlecb(nData, nData->xfered, nData->idlearg) == 0)
				return 0;
			nData->xfered1 = 0;
		}
	}
	LLOG("-> done " << i);
	return i;
}

/*
* FtpWrite - write to a data connection
*/
GLOBALDEF int FtpWrite(void *buf, int len, netbuf *nData)
{
	LLOG("FtpWrite");

	int i;
	if (nData->dir != FTPLIB_WRITE)
		return 0;
	if (nData->buf)
		i = writeline((char *)buf, len, nData);
	else
	{
		socket_wait(nData, 1);
		i = net_write(nData->handle, (const char *)buf, len);
	}
	if (i == -1)
		return 0;
	nData->xfered += i;
	if (nData->idlecb && nData->cbbytes)
	{
		nData->xfered1 += i;
		if (nData->xfered1 > nData->cbbytes)
		{
			nData->xfered1 = 0;
			if(!nData->idlecb(nData, nData->xfered, nData->idlearg))
				return 0;
		}
	}
	LLOG("-> done " << i);
	return i;
}

/*
* FtpClose - close a data connection
*/
GLOBALDEF int FtpClose(netbuf *nData)
{
	netbuf *ctrl;
	switch (nData->dir)
	{
	case FTPLIB_WRITE:
		/* potential problem - if buffer flush fails, how to notify user? */
		if (nData->buf != NULL)
			writeline(NULL, 0, nData);
	case FTPLIB_READ:
		if (nData->buf)
			delete [] nData->buf;
		shutdown(nData->handle,2);
		net_close(nData->handle);
		ctrl = nData->ctrl;
		delete nData;
		if (ctrl)
		{
			ctrl->data = NULL;
			return(readresp('2', ctrl));
		}
		return 1;
	case FTPLIB_CONTROL:
		if (nData->data)
		{
			nData->ctrl = NULL;
			FtpClose(nData);
		}
		net_close(nData->handle);
		delete nData;
		return 0;
	}
	return 1;
}

/*
* FtpSite - send a SITE command
*
* return 1 if command successful, 0 otherwise
*/
GLOBALDEF int FtpSite(const char *cmd, netbuf *nControl)
{
	char buf[256];

	if ((strlen(cmd) + 7) > sizeof(buf))
		return 0;
	sprintf(buf,"SITE %s",cmd);
	if (!FtpSendCmd(buf,'2',nControl))
		return 0;
	return 1;
}

/*
* FtpSysType - send a SYST command
*
* Fills in the user buffer with the remote system type.  If more
* information from the response is required, the user can parse
* it out of the response buffer returned by FtpLastResponse().
*
* return 1 if command successful, 0 otherwise
*/
GLOBALDEF int FtpSysType(char *buf, int max, netbuf *nControl)
{
	int l = max;
	char *b = buf;
	char *s;
	if (!FtpSendCmd("SYST",'2',nControl))
		return 0;
	s = &nControl->response[4];
	while ((--l) && (*s != ' '))
		*b++ = *s++;
	*b++ = '\0';
	return 1;
}

/*
* FtpMkdir - create a directory at server
*
* return 1 if successful, 0 otherwise
*/
GLOBALDEF int FtpMkdir(const char *path, netbuf *nControl)
{
	char buf[256];

	if ((strlen(path) + 6) > sizeof(buf))
		return 0;
	sprintf(buf,"MKD %s",path);
	if (!FtpSendCmd(buf,'2', nControl))
		return 0;
	return 1;
}

/*
* FtpChdir - change path at remote
*
* return 1 if successful, 0 otherwise
*/
GLOBALDEF int FtpChdir(const char *path, netbuf *nControl)
{
	char buf[256];

	if ((strlen(path) + 6) > sizeof(buf))
		return 0;
	sprintf(buf,"CWD %s",path);
	if (!FtpSendCmd(buf,'2',nControl))
		return 0;
	return 1;
}

/*
* FtpCDUp - move to parent directory at remote
*
* return 1 if successful, 0 otherwise
*/
GLOBALDEF int FtpCDUp(netbuf *nControl)
{
	if (!FtpSendCmd("CDUP",'2',nControl))
		return 0;
	return 1;
}

/*
* FtpRmdir - remove directory at remote
*
* return 1 if successful, 0 otherwise
*/
GLOBALDEF int FtpRmdir(const char *path, netbuf *nControl)
{
	char buf[256];

	if ((strlen(path) + 6) > sizeof(buf))
		return 0;
	sprintf(buf,"RMD %s",path);
	if (!FtpSendCmd(buf,'2',nControl))
		return 0;
	return 1;
}

/*
* FtpPwd - get working directory at remote
*
* return 1 if successful, 0 otherwise
*/
GLOBALDEF int FtpPwd(char *path, int max, netbuf *nControl)
{
	int l = max;
	char *b = path;
	char *s;
	if (!FtpSendCmd("PWD",'2',nControl))
		return 0;
	s = strchr(nControl->response, '"');
	if (s == NULL)
		return 0;
	s++;
	while ((--l) && (*s) && (*s != '"'))
		*b++ = *s++;
	*b++ = '\0';
	return 1;
}

/*
* FtpXfer - issue a command and transfer data
*
* return 1 if successful, 0 otherwise
*/
static int FtpXfer(const char *localfile, const char *path,
		netbuf *nControl, int typ, int mode)
{
	int l,c;
	char *dbuf;
	FILE *local = NULL;
	netbuf *nData;
	int rv=1;

	if (localfile != NULL)
	{
		char ac[4] = "w";
		if (typ == FTPLIB_FILE_WRITE)
			ac[0] = 'r';
		if (mode == FTPLIB_IMAGE)
			ac[1] = 'b';
		local = fopen(localfile, ac);
		if (local == NULL)
		{
			strncpy(nControl->response, strerror(errno),
					sizeof(nControl->response));
			return 0;
		}
	}
	if (local == NULL)
		local = (typ == FTPLIB_FILE_WRITE) ? stdin : stdout;
	if (!FtpAccess(path, typ, mode, nControl, &nData))
		return 0;
	dbuf = new char[FTPLIB_BUFSIZ];
	if (typ == FTPLIB_FILE_WRITE)
	{
		while ((l = fread(dbuf, 1, FTPLIB_BUFSIZ, local)) > 0)
			if ((c = FtpWrite(dbuf, l, nData)) < l)
			{
				RLOG("short write: passed " << l << ", wrote " << c);
				rv = 0;
				break;
			}
	}
	else
	{
		while ((l = FtpRead(dbuf, FTPLIB_BUFSIZ, nData)) > 0)
			if (fwrite(dbuf, 1, l, local) <= 0)
			{
				strcpy(nControl->perror, "localfile write");
				rv = 0;
				break;
			}
	}
	delete[] dbuf;
	fflush(local);
	if (localfile != NULL)
		fclose(local);
	FtpClose(nData);
	return rv;
}

/*
* FtpNlst - issue an NLST command and write response to output
*
* return 1 if successful, 0 otherwise
*/
GLOBALDEF int FtpNlst(const char *outputfile, const char *path,
		netbuf *nControl)
{
	return FtpXfer(outputfile, path, nControl, FTPLIB_DIR, FTPLIB_ASCII);
}

/*
* FtpDir - issue a LIST command and write response to output
*
* return 1 if successful, 0 otherwise
*/
GLOBALDEF int FtpDir(const char *outputfile, const char *path, netbuf *nControl)
{
	return FtpXfer(outputfile, path, nControl, FTPLIB_DIR_VERBOSE, FTPLIB_ASCII);
}

/*
* FtpSize - determine the size of a remote file
*
* return 1 if successful, 0 otherwise
*/
GLOBALDEF int FtpSize(const char *path, int *size, char mode, netbuf *nControl)
{
	char cmd[256];
	int resp,sz,rv=1;

	if ((strlen(path) + 7) > sizeof(cmd))
		return 0;
	sprintf(cmd, "TYPE %c", mode);
	if (!FtpSendCmd(cmd, '2', nControl))
		return 0;
	sprintf(cmd,"SIZE %s",path);
	if (!FtpSendCmd(cmd,'2',nControl))
		rv = 0;
	else
	{
		if (sscanf(nControl->response, "%d %d", &resp, &sz) == 2)
			*size = sz;
		else
			rv = 0;
	}
	return rv;
}

/*
* FtpModDate - determine the modification date of a remote file
*
* return 1 if successful, 0 otherwise
*/
GLOBALDEF int FtpModDate(const char *path, char *dt, int max, netbuf *nControl)
{
	char buf[256];
	int rv = 1;

	if ((strlen(path) + 7) > sizeof(buf))
		return 0;
	sprintf(buf,"MDTM %s",path);
	if (!FtpSendCmd(buf,'2',nControl))
		rv = 0;
	else
		strncpy(dt, &nControl->response[4], max);
	return rv;
}

/*
* FtpGet - issue a GET command and write received data to output
*
* return 1 if successful, 0 otherwise
*/
GLOBALDEF int FtpGet(const char *outputfile, const char *path,
		char mode, netbuf *nControl)
{
	return FtpXfer(outputfile, path, nControl, FTPLIB_FILE_READ, mode);
}

/*
* FtpPut - issue a PUT command and send data from input
*
* return 1 if successful, 0 otherwise
*/
GLOBALDEF int FtpPut(const char *inputfile, const char *path, char mode,
		netbuf *nControl)
{
	return FtpXfer(inputfile, path, nControl, FTPLIB_FILE_WRITE, mode);
}

/*
* FtpRename - rename a file at remote
*
* return 1 if successful, 0 otherwise
*/
GLOBALDEF int FtpRename(const char *src, const char *dst, netbuf *nControl)
{
	char cmd[256];

	if (((strlen(src) + 7) > sizeof(cmd)) ||
		((strlen(dst) + 7) > sizeof(cmd)))
		return 0;
	sprintf(cmd,"RNFR %s",src);
	if (!FtpSendCmd(cmd,'3',nControl))
		return 0;
	sprintf(cmd,"RNTO %s",dst);
	if (!FtpSendCmd(cmd,'2',nControl))
		return 0;
	return 1;
}

/*
* FtpDelete - delete a file at remote
*
* return 1 if successful, 0 otherwise
*/
GLOBALDEF int FtpDelete(const char *fnm, netbuf *nControl)
{
	char cmd[256];

	if ((strlen(fnm) + 7) > sizeof(cmd))
		return 0;
	sprintf(cmd,"DELE %s",fnm);
	if (!FtpSendCmd(cmd,'2', nControl))
		return 0;
	return 1;
}

/*
* FtpQuit - disconnect from remote
*
* return 1 if successful, 0 otherwise
*/
GLOBALDEF void FtpQuit(netbuf *nControl)
{
	if (nControl->dir != FTPLIB_CONTROL)
		return;
	FtpSendCmd("QUIT",'2',nControl);
	net_close(nControl->handle);
	if(nControl->buf)
		delete[] nControl->buf;
	delete nControl;
}

GLOBALDEF const char *FtpError(netbuf *nControl)
{
	return nControl->perror;
}
