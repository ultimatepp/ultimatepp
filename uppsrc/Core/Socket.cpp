#include "Core.h"

#ifdef PLATFORM_WIN32
#include <winsock2.h>
#include <Ws2ipdef.h>
#include <Ws2tcpip.h>
#endif

#ifdef PLATFORM_POSIX
#include <arpa/inet.h>
#endif

NAMESPACE_UPP

#ifdef PLATFORM_WIN32
#pragma comment(lib, "ws2_32.lib")
#endif

#define LLOG(x)  // DLOG("TCP " << x)

#ifdef PLATFORM_POSIX

#define SOCKERR(x) x

const char *TcpSocketErrorDesc(int code)
{
	return strerror(code);
}

int TcpSocket::GetErrorCode()
{
	return errno;
}

#else

#define SOCKERR(x) WSA##x

const char *TcpSocketErrorDesc(int code)
{
	static Tuple2<int, const char *> err[] = {
		{ WSAEINTR,                 "Interrupted function call." },
		{ WSAEACCES,                "Permission denied." },
		{ WSAEFAULT,                "Bad address." },
		{ WSAEINVAL,                "Invalid argument." },
		{ WSAEMFILE,                "Too many open files." },
		{ WSAEWOULDBLOCK,           "Resource temporarily unavailable." },
		{ WSAEINPROGRESS,           "Operation now in progress." },
		{ WSAEALREADY,              "Operation already in progress." },
		{ WSAENOTSOCK,              "TcpSocket operation on nonsocket." },
		{ WSAEDESTADDRREQ,          "Destination address required." },
		{ WSAEMSGSIZE,              "Message too long." },
		{ WSAEPROTOTYPE,            "Protocol wrong type for socket." },
		{ WSAENOPROTOOPT,           "Bad protocol option." },
		{ WSAEPROTONOSUPPORT,       "Protocol not supported." },
		{ WSAESOCKTNOSUPPORT,       "TcpSocket type not supported." },
		{ WSAEOPNOTSUPP,            "Operation not supported." },
		{ WSAEPFNOSUPPORT,          "Protocol family not supported." },
		{ WSAEAFNOSUPPORT,          "Address family not supported by protocol family." },
		{ WSAEADDRINUSE,            "Address already in use." },
		{ WSAEADDRNOTAVAIL,         "Cannot assign requested address." },
		{ WSAENETDOWN,              "Network is down." },
		{ WSAENETUNREACH,           "Network is unreachable." },
		{ WSAENETRESET,             "Network dropped connection on reset." },
		{ WSAECONNABORTED,          "Software caused connection abort." },
		{ WSAECONNRESET,            "Connection reset by peer." },
		{ WSAENOBUFS,               "No buffer space available." },
		{ WSAEISCONN,               "TcpSocket is already connected." },
		{ WSAENOTCONN,              "TcpSocket is not connected." },
		{ WSAESHUTDOWN,             "Cannot send after socket shutdown." },
		{ WSAETIMEDOUT,             "Connection timed out." },
		{ WSAECONNREFUSED,          "Connection refused." },
		{ WSAEHOSTDOWN,             "Host is down." },
		{ WSAEHOSTUNREACH,          "No route to host." },
		{ WSAEPROCLIM,              "Too many processes." },
		{ WSASYSNOTREADY,           "Network subsystem is unavailable." },
		{ WSAVERNOTSUPPORTED,       "Winsock.dll version out of range." },
		{ WSANOTINITIALISED,        "Successful WSAStartup not yet performed." },
		{ WSAEDISCON,               "Graceful shutdown in progress." },
		{ WSATYPE_NOT_FOUND,        "Class type not found." },
		{ WSAHOST_NOT_FOUND,        "Host not found." },
		{ WSATRY_AGAIN,             "Nonauthoritative host not found." },
		{ WSANO_RECOVERY,           "This is a nonrecoverable error." },
		{ WSANO_DATA,               "Valid name, no data record of requested type." },
		{ WSASYSCALLFAILURE,        "System call failure." },
	};
	const Tuple2<int, const char *> *x = FindTuple(err, __countof(err), code);
	return x ? x->b : "Unknown error code.";
}

int TcpSocket::GetErrorCode()
{
	return WSAGetLastError();
}

#endif


void TcpSocket::Init()
{
#if defined(PLATFORM_WIN32)
	ONCELOCK {
		WSADATA wsadata;
		WSAStartup(MAKEWORD(2, 2), &wsadata);
	}
#endif
}

void TcpSocket::Reset()
{
	is_eof = false;
	socket = INVALID_SOCKET;
	ipv6 = false;
	ptr = end = buffer;
	is_error = false;
	is_timeout = false;
	is_abort = false;
}

TcpSocket::TcpSocket()
{
	ClearError();
	Reset();
	timeout = Null;
	waitstep = 20;
	global = false;
}

bool TcpSocket::Open(int family, int type, int protocol)
{
	Init();
	CloseRaw();
	ClearError();
	if((socket = ::socket(family, type, protocol)) == INVALID_SOCKET)
		return false;
	LLOG("TcpSocket::Data::Open() -> " << (int)socket);
#ifdef PLATFORM_WIN32
	u_long arg = 1;
	if(ioctlsocket(socket, FIONBIO, &arg))
		SetSockError("ioctlsocket(FIO[N]BIO)");
#else
	if(fcntl(socket, F_SETFL, (fcntl(socket, F_GETFL, 0) | O_NONBLOCK)))
		SetSockError("fcntl(O_[NON]BLOCK)");
#endif
	return true;
}

bool TcpSocket::Listen(int port, int listen_count, bool ipv6_, bool reuse)
{
	ipv6 = ipv6_;
	if(!Open(ipv6 ? AF_INET6 : AF_INET, SOCK_STREAM, 0))
		return false;
	sockaddr_in sin;
#ifdef PLATFORM_WIN32
	SOCKADDR_IN6 sin6;
	if(ipv6 && IsWinVista())
#else
	sockaddr_in6 sin6;
	if(ipv6)
#endif
	{
		Zero(sin6);
		sin.sin_family = AF_INET6;
		sin.sin_port = htons(port);
		sin.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else {
		Zero(sin);
		sin.sin_family = AF_INET;
		sin.sin_port = htons(port);
		sin.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	if(reuse) {
		int optval = 1;
		setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (const char *)&optval, sizeof(optval));
	}
	if(bind(socket, ipv6 ? (const sockaddr *)&sin6 : (const sockaddr *)&sin,
	        ipv6 ? sizeof(sin6) : sizeof(sin))) {
		SetSockError(Format("bind(port=%d)", port));
		return false;
	}
	if(listen(socket, listen_count)) {
		SetSockError(Format("listen(port=%d, count=%d)", port, listen_count));
		return false;
	}
	return true;
}

bool TcpSocket::Accept(TcpSocket& ls)
{
	CloseRaw();
	if(timeout && !ls.WaitRead())
		return false;
	if(!Open(ls.ipv6 ? AF_INET6 : AF_INET, SOCK_STREAM, 0))
		return false;
	socket = accept(ls.GetSOCKET(), NULL, NULL);
	if(socket == INVALID_SOCKET) {
		SetSockError("accept");
		return false;
	}
	return true;
}

String TcpSocket::GetPeerAddr() const
{
	if(!IsOpen())
		return Null;
	sockaddr_in addr;
	socklen_t l = sizeof(addr);
	if(getpeername(socket, (sockaddr *)&addr, &l) != 0)
		return Null;
	if(l > sizeof(addr))
		return Null;
#ifdef PLATFORM_WIN32
	return inet_ntoa(addr.sin_addr);
#else
	char h[200];
	return inet_ntop(AF_INET, &addr.sin_addr, h, 200);
#endif
}

void TcpSocket::NoDelay()
{
	ASSERT(IsOpen());
	int __true = 1;
	LLOG("NoDelay(" << (int)socket << ")");
	if(setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (const char *)&__true, sizeof(__true)))
		SetSockError("setsockopt(TCP_NODELAY)");
}

void TcpSocket::Linger(int msecs)
{
	ASSERT(IsOpen());
	linger ls;
	ls.l_onoff = !IsNull(msecs) ? 1 : 0;
	ls.l_linger = !IsNull(msecs) ? (msecs + 999) / 1000 : 0;
	if(setsockopt(socket, SOL_SOCKET, SO_LINGER,
		reinterpret_cast<const char *>(&ls), sizeof(ls)))
		SetSockError("setsockopt(SO_LINGER)");
}

void TcpSocket::Attach(SOCKET s)
{
	CloseRaw();
	socket = s;
}

bool TcpSocket::Connect(const char *host, int port)
{
	LLOG("TcpSocket::Data::OpenClient(" << host << ':' << port << ')');

	Init();

	addrinfo hints;
	memset(&hints, 0, sizeof(addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	addrinfo *result;
	if(getaddrinfo(host, ~AsString(port), &hints, &result) || !result) {
		SetSockError(Format("getaddrinfo(%s) failed", host));
		return false;
	}
	
	addrinfo *rp = result;
	for(;;) {
		if(Open(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) {
			if(connect(socket, rp->ai_addr, rp->ai_addrlen) == 0 ||
			   GetErrorCode() == SOCKERR(EINPROGRESS) || GetErrorCode() == SOCKERR(EWOULDBLOCK))
				break;
			CloseRaw();
		}
		rp = rp->ai_next;
		if(!rp) {
			SetSockError(Format("unable to open or bind socket for %s", host));
			freeaddrinfo(result);
			return false;
		}
    }

	freeaddrinfo(result);
	return true;
}

bool TcpSocket::CloseRaw()
{
	SOCKET old_socket = socket;
	socket = INVALID_SOCKET;
	if(old_socket != INVALID_SOCKET) {
		LLOG("TcpSocket::CloseRaw(" << (int)old_socket << ")");
		int res;
#if defined(PLATFORM_WIN32)
		res = closesocket(old_socket);
#elif defined(PLATFORM_POSIX)
		res = close(old_socket);
#else
	#error Unsupported platform
#endif
		if(res && !IsError()) {
			SetSockError("close");
			return false;
		}
	}
	return true;
}

bool TcpSocket::Close()
{
	if(socket == INVALID_SOCKET)
		return false;
	return !IsError() && WaitWrite() && CloseRaw();
}

bool TcpSocket::WouldBlock()
{
	int c = GetErrorCode();
#ifdef PLATFORM_POSIX
	return c == SOCKERR(EWOULDBLOCK) || c == SOCKERR(EAGAIN);
#endif
#ifdef PLATFORM_WIN32
	return c == SOCKERR(EWOULDBLOCK);
#endif
}

int TcpSocket::Send(const void *buf, int amount)
{
	int res = send(socket, (const char *)buf, amount, 0);
	if(res < 0 && WouldBlock())
		res = 0;
	else
	if(res == 0 || res < 0)
		SetSockError("send");
	return res;
}

void TcpSocket::Shutdown()
{
	ASSERT(IsOpen());
	if(shutdown(socket, SD_SEND))
		SetSockError("shutdown(SD_SEND)");
}

String TcpSocket::GetHostName()
{
	Init();
	char buffer[256];
	gethostname(buffer, __countof(buffer));
	return buffer;
}

TcpSocket& TcpSocket::GlobalTimeout(int ms)
{
	global = true;
	starttime = msecs();
	timeout = ms;
	return *this;
}

bool TcpSocket::Wait(dword flags)
{
	LLOG("Wait(" << timeout << ", " << flags << ")");
	if((flags & WAIT_READ) && ptr != end)
		return true;
	int end_time = (global ? starttime : msecs()) + timeout;
	if(socket == INVALID_SOCKET)
		return false;
	is_timeout = false;
	for(;;) {
		if(IsError() || IsAbort())
			return false;
		int to = end_time - msecs();
		if(WhenWait)
			to = waitstep;
		timeval *tvalp = NULL;
		timeval tval;
		if(!IsNull(timeout) || WhenWait) {
			to = max(to, 0);
			tval.tv_sec = to / 1000;
			tval.tv_usec = 1000 * (to % 1000);
			tvalp = &tval;
		}
		fd_set fdset[1];
		FD_ZERO(fdset);
		FD_SET(socket, fdset);
		int avail = select((int)socket + 1,
		                   flags & WAIT_READ ? fdset : NULL,
		                   flags & WAIT_WRITE ? fdset : NULL, NULL, tvalp);
		LLOG("Wait select avail: " << avail);
		if(avail < 0) {
			SetSockError("wait");
			return false;
		}
		if(avail > 0)
			return true;
		if(to <= 0) {
			is_timeout = true;
			return false;
		}
		WhenWait();
	}
}

int TcpSocket::Put(const char *s, int length)
{
	LLOG("Put(@ " << socket << ": " << length);
	ASSERT(IsOpen());
	if(length < 0 && s)
		length = (int)strlen(s);
	if(!s || length <= 0 || IsError() || IsAbort())
		return 0;
	done = 0;
	bool peek = false;
	while(done < length) {
		if(peek && !WaitWrite())
			return done;
		peek = false;
		int count = Send(s + done, length - done);
		if(IsError() || timeout == 0)
			return done;
		if(count > 0)
			done += count;
		else
			peek = true;
	}
	LLOG("//Put() -> " << done);
	return done;
}

int TcpSocket::Recv(void *buf, int amount)
{
	int res = recv(socket, (char *)buf, amount, 0);
	if(res == 0)
		is_eof = true;
	else
	if(res < 0 && WouldBlock())
		res = 0;
	else
	if(res < 0)
		SetSockError("recv");
	LLOG("recv(" << socket << "): " << res << " bytes: "
	     << AsCString((char *)buf, (char *)buf + min(res, 16))
	     << (res ? "" : IsEof() ? ", EOF" : ", WOULDBLOCK"));
	return res;
}

void TcpSocket::ReadBuffer()
{
	ptr = buffer;
	end = buffer + Recv(buffer, BUFFERSIZE);
	if(ptr == end && timeout) {
		WaitRead();
		end = buffer + Recv(buffer, BUFFERSIZE);
	}
}

int TcpSocket::Get_()
{
	if(!IsOpen() || IsError() || IsEof() || IsAbort())
		return -1;
	ReadBuffer();
	return ptr < end ? *ptr++ : -1;
}

int TcpSocket::Peek_()
{
	if(!IsOpen() || IsError() || IsEof() || IsAbort())
		return -1;
	ReadBuffer();
	return ptr < end ? *ptr : -1;
}

int TcpSocket::Get(void *buffer, int count)
{
	LLOG("Get " << count);

	if(!IsOpen() || IsError() || IsEof() || IsAbort())
		return 0;
	
	String out;
	int l = end - ptr;
	done = 0;
	if(l > 0)
		if(l < count) {
			memcpy(buffer, ptr, l);
			done += l;
			ptr = end;
		}
		else {
			memcpy(buffer, ptr, count);
			ptr += count;
			return count;
		}
	int part = Recv((char *)buffer + done, count - done);
	if(part > 0)
		done += part;
	while(timeout != 0 && part >= 0 && done < count && !IsError() && !IsEof()) {
		if(!WaitRead())
			break;
		part = Recv((char *)buffer + done, count - done);
		if(part > 0)
			done += part;
	}
	return done;
}

String TcpSocket::Get(int count)
{
	if(count == 0)
		return Null;
	StringBuffer out(count);
	int done = Get(out, count);
	if(!done && IsEof())
		return String::GetVoid();
	out.SetLength(done);
	return out;
}

String TcpSocket::GetLine(int maxlen)
{
	String ln;
	for(;;) {
		int c = Peek();
		if(c < 0)
			return String::GetVoid();
		Get();
		if(c == '\n')
			return ln;
		if(c != '\r')
			ln.Cat(c);
	}
}

void TcpSocket::SetSockError(const char *context, const char *errdesc)
{
	String err;
	errorcode = GetErrorCode();
	if(socket != INVALID_SOCKET)
		err << "socket(" << (int)socket << ") / ";
	err << context << ": " << errdesc;
	errordesc = err;
	is_error = true;
}

void TcpSocket::SetSockError(const char *context)
{
	SetSockError(context, TcpSocketErrorDesc(GetErrorCode()));
}

END_UPP_NAMESPACE
