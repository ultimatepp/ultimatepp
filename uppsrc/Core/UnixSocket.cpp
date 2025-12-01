#include "Core.h"

#define LLOG(x) // RLOG(x)

namespace Upp {

#ifdef PLATFORM_POSIX

#include <sys/un.h>

namespace {
	
int GetErrorCode()
{
	return errno;
}

void SetSockType(const String& path, sockaddr_un& addr, UnixSocket::Type t)
{
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    
    switch(t) {
    case UnixSocket::Type::FileSystem:
        if(path.GetLength() > 0) {
            ASSERT(path.GetLength() < sizeof(addr.sun_path));
            strncpy(addr.sun_path, ~path, sizeof(addr.sun_path) - 1);
            addr.sun_path[sizeof(addr.sun_path) - 1] = '\0';
        }
        break;
    case UnixSocket::Type::Abstract:
        addr.sun_path[0] = '\0';
        if(path.GetLength() > 0) {
            ASSERT(path.GetLength() < sizeof(addr.sun_path) - 1);
            strncpy(addr.sun_path + 1, ~path, sizeof(addr.sun_path) - 2);
        }
        break;
    default:
        NEVER();
    }
}

}

UnixSocket::UnixSocket()
{
	socket.WhenWait << WhenWait;
}

int UnixSocket::GetPeerPid() const
{
	if(!IsOpen())
		return -1;
	
#if defined(PLATFORM_LINUX)
    struct ucred ucred;
    socklen_t len = sizeof(ucred);
    if(getsockopt(socket.GetSOCKET(), SOL_SOCKET, SO_PEERCRED, &ucred, &len) == 0)
        return ucred.pid;
        
#elif (defined(PLATFORM_MACOS) || defined(PLATFORM_FREEBSD)) && defined(LOCAL_PEERPID)
    pid_t pid;
    socklen_t len = sizeof(pid);
    if(getsockopt(socket.GetSOCKET(), SOL_LOCAL, LOCAL_PEERPID, &pid, &len) == 0)
        return pid;

#endif
 
   return -1; // Not supported or failed
}

bool UnixSocket::Listen(const String& path, int listen_count, bool reuse, Type socktype)
{
	Close();
	socket.Init();
	socket.Reset();

	if(!socket.Open(AF_UNIX, SOCK_STREAM, 0))
		return false;

	struct sockaddr_un addr;
	SetSockType(path, addr, socktype);
	
	if(reuse) {
		int optval = 1;
		setsockopt(socket.GetSOCKET(), SOL_SOCKET, SO_REUSEADDR, (const char *) &optval, sizeof(optval));
	}

	if(bind(socket.GetSOCKET(), (const sockaddr *) &addr, sizeof(addr))) {
		socket.SetSockError(Format("bind(path=%s)", path));
		return false;
	}

	if(listen(socket.GetSOCKET(), listen_count)) {
		socket.SetSockError(Format("listen(path=%s, count=%d)", path, listen_count));
		return false;
	}

	return true;
}

bool UnixSocket::Accept(UnixSocket& ls)
{
	Close();
	socket.Init();
	socket.Reset();
	
	ASSERT(ls.IsOpen());

	int et = socket.GetEndTime();

	for(;;) {
		int h = ls.GetTimeout();
		bool b = ls.Timeout(GetTimeout()).socket.Wait(WAIT_READ, et);
		ls.Timeout(h);
		if(!b) // timeout
			return false;
		socket.socket = accept(ls.GetSOCKET(), nullptr, nullptr);
		if(socket.socket != INVALID_SOCKET)
			break;
		if(!socket.WouldBlock() && GetErrorCode() != EINTR) { // In prefork condition, Wait is not enough, as other process can accept
			socket.SetSockError("accept");
			return false;
		}
	}

	socket.mode = TcpSocket::ACCEPT;
	return socket.SetupSocket();
}

bool UnixSocket::Connect(const String& path, Type socktype)
{
	Close();
	socket.Init();
	socket.Reset();

	if(!socket.Open(AF_UNIX, SOCK_STREAM, 0))
		return false;

	struct sockaddr_un addr;
	SetSockType(path, addr, socktype);

	if(connect(socket.GetSOCKET(), (sockaddr *) &addr, sizeof(addr)) == 0 ||
		GetErrorCode() == EINPROGRESS || GetErrorCode() == EWOULDBLOCK) {
			socket.mode = TcpSocket::CONNECT;
			return true;
	}

	SetSockError("connect", -1, strerror(GetErrorCode()));
	Close();
	return false;
}

#endif
}
