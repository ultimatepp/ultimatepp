/***************************************************************************/
/*									   */
/* ftplib.h - header file for callable ftp access routines                 */
/* Copyright (C) 1996, 1997 Thomas Pfau, pfau@cnj.digex.net                */
/*	73 Catherine Street, South Bound Brook, NJ, 08880		   */
/*									   */
/* This library is free software; you can redistribute it and/or	   */
/* modify it under the terms of the GNU Library General Public		   */
/* License as published by the Free Software Foundation; either		   */
/* version 2 of the License, or (at your option) any later version.	   */
/* 									   */
/* This library is distributed in the hope that it will be useful,	   */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of	   */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	   */
/* Library General Public License for more details.			   */
/* 									   */
/* You should have received a copy of the GNU Library General Public	   */
/* License along with this progam; if not, write to the			   */
/* Free Software Foundation, Inc., 59 Temple Place - Suite 330,		   */
/* Boston, MA 02111-1307, USA.						   */
/*									   */
/***************************************************************************/

#if !defined(__FTPLIB_H)
#define __FTPLIB_H

#define GLOBALDEF
#define GLOBALREF extern

/* FtpAccess() type codes */
#define FTPLIB_DIR 1
#define FTPLIB_DIR_VERBOSE 2
#define FTPLIB_FILE_READ 3
#define FTPLIB_FILE_WRITE 4

/* FtpAccess() mode codes */
#define FTPLIB_ASCII 'A'
#define FTPLIB_IMAGE 'I'
#define FTPLIB_TEXT FTPLIB_ASCII
#define FTPLIB_BINARY FTPLIB_IMAGE

/* connection modes */
#define FTPLIB_PASSIVE 1
#define FTPLIB_PORT 2

/* connection option names */
#define FTPLIB_CONNMODE 1
#define FTPLIB_CALLBACK 2
#define FTPLIB_IDLETIME 3
#define FTPLIB_CALLBACKARG 4
#define FTPLIB_CALLBACKBYTES 5
#define FTPLIB_IDLETIMEOUT 6

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NetBuf netbuf;
typedef int (*FtpCallback)(netbuf *nControl, int xfered, void *arg);

GLOBALREF void FtpInit(void);
GLOBALREF char *FtpLastResponse(netbuf *nControl);
GLOBALREF int FtpConnect(const char *host, netbuf **nControl, char perror[512],
	FtpCallback idlecb, void *idledata, int idletime_msecs, int idletimeout_secs);
GLOBALREF int FtpOptions(int opt, uintptr_t val, netbuf *nControl);
GLOBALREF int FtpLogin(const char *user, const char *pass, netbuf *nControl);
GLOBALREF int FtpAccess(const char *path, int typ, int mode, netbuf *nControl,
    netbuf **nData);
GLOBALREF int FtpRead(void *buf, int max, netbuf *nData);
GLOBALREF int FtpWrite(void *buf, int len, netbuf *nData);
GLOBALREF int FtpClose(netbuf *nData);
GLOBALREF int FtpSite(const char *cmd, netbuf *nControl);
GLOBALREF int FtpSysType(char *buf, int max, netbuf *nControl);
GLOBALREF int FtpMkdir(const char *path, netbuf *nControl);
GLOBALREF int FtpChdir(const char *path, netbuf *nControl);
GLOBALREF int FtpCDUp(netbuf *nControl);
GLOBALREF int FtpRmdir(const char *path, netbuf *nControl);
GLOBALREF int FtpPwd(char *path, int max, netbuf *nControl);
GLOBALREF int FtpSize(const char *path, int *size, char mode, netbuf *nControl);
GLOBALREF int FtpModDate(const char *path, char *dt, int max, netbuf *nControl);
GLOBALREF int FtpRename(const char *src, const char *dst, netbuf *nControl);
GLOBALREF int FtpDelete(const char *fnm, netbuf *nControl);
GLOBALREF void FtpQuit(netbuf *nControl);
GLOBALREF const char *FtpError(netbuf *nControl);

#ifdef __cplusplus
};
#endif

#endif /* __FTPLIB_H */
