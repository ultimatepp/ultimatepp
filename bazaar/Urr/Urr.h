#ifndef _Urr_Urr_h
#define _Urr_Urr_h

#include <Core/Core.h>

#if defined(PLATFORM_WIN32)
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
typedef int socklen_t;
#endif

#ifdef PLATFORM_POSIX
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
typedef int SOCKET;
inline void closesocket(int fd) { close(fd); }
#endif

NAMESPACE_UPP

inline int b_sendto(
	SOCKET s,
	const char *buf,
 	int len,
	int flags,
	const struct sockaddr *to,
	int tolen
) {
	Sleep(rand() % 10);
	int bug = rand() % 10;
	if(bug == 0) LOG("[Emulated drop]");
	return bug ? sendto(s, buf, len, flags, to, tolen) : 0;
}

// #define sendto b_sendto

int Rcv(SOCKET skt, void *buffer, int bsize, int timeout, void *adr = NULL, socklen_t *adrlen = NULL);

/*
struct UrrRequestHdr {   // size = 20
	byte        version; // 0
	byte        flags;   // 1
	word        blksize; // 2
	Uuid        id;      // 4
};

struct UrrResponseHdr { // size = 20
	int         part;
	Uuid        id;
};
*/

enum {
	URR_LAST       = -1,
	URR_FIN        = -2,
	URR_SINGLE     = -3,
	URR_PROCESSING = -4,
	
	URR_ONEWAY     = 0x01,
	URR_COMPRESS   = 0x02,
};

class UrrServer;

class UrrRequest : Moveable<UrrRequest> {
	Uuid           id;
	byte           adr[256];
	socklen_t      adrsz;
	int            blksize;
	String         request;
	byte           flags;
	Ptr<UrrServer> server;
	
	friend class UrrServer;

public:
	operator const String&() const                    { return request; }
	const String& operator~() const                   { return request; }

	//WARNING: infos are unreliable due to NAT in many cases
	int SourcePort() const { sockaddr_in& srcadr = *(sockaddr_in*)&adr; return ntohs(srcadr.sin_port); }
	String SourceIp() const { sockaddr_in& srcadr = *(sockaddr_in*)&adr; return String(inet_ntoa(srcadr.sin_addr)); }
	
	void Return(const String& s, bool linger = true);
};

class UrrServer : public Pte<UrrServer> {
	Mutex                   mutex;

	struct TimeI : Moveable<TimeI> {
		int time;
		int ii;
	};

	Index<Uuid>             processing;

	BiVector<TimeI>         done;
	VectorMap<Uuid, String> donemap;
	
	BiVector<TimeI>         sent;
	Index<Uuid>             sentindex;
	
	Vector<SOCKET>          skts;
	
	SOCKET                  srvskt;
	int                     lingertime;
	int                     senttime;
	int                     acktimeout;
	int                     blksize;
	int                     retries;
	int                     inputqueue;

	int                     loss;

	void                    Acceptor();
	void                    Reset();

	void        SendBack(UrrRequest& r, String data, bool linger);
	static void SendTo(SOCKET skt, char *h, const void *data, int sz, UrrRequest& r);
	void        Return(UrrRequest& r, const String& data, bool linger);
	
	friend class UrrRequest;
	
	typedef UrrServer CLASSNAME;

public:
	bool       Create(int port, const char *addr = NULL);
	void       Close();

	bool       IsRequest();
	bool       Accept(UrrRequest& r);
	int        GetLoss() const                       { return loss; }

	UrrServer& Linger(int msec)                      { lingertime = msec; return *this; }
	UrrServer& BlockSentQuery(int msec)              { senttime = msec; return *this; }
	UrrServer& AckTimeout(int msec)                  { acktimeout = msec; return *this; }
	UrrServer& MaxBlkSize(int sz)                    { blksize = sz; return *this; }
	UrrServer& Retries(int n)                        { retries = n; return *this; }
	UrrServer& InputQueue(int n)                     { inputqueue = n; return *this; }
	
	UrrServer()                                      { Reset(); }
	UrrServer(int port, const char *addr = NULL)     { Reset(); Create(port, addr); }
	~UrrServer()                                     { Close(); }
};

class UrrClient {
	sockaddr_in srvadr;
	int         retries;
	int         qtimeout;
	int         timeout;
	int         loss;
	int         blksize;
	SOCKET      skt;
	byte        flags;
	
	void        Init();

public:
	String     Call(const String& request);
	
	int        GetLoss() const                            { return loss; }
	
	UrrClient& SetServer(const char *host, int port);
	UrrClient& QueryTimeout(int ms)                       { qtimeout = ms; return *this; }
	UrrClient& Timeout(int ms)                            { timeout = ms; return *this; }
	UrrClient& Retries(int n)                             { retries = n; return *this; }
	UrrClient& MaxBlkSize(int sz)                         { blksize = sz; return *this; }
	UrrClient& Flags(byte f)                              { flags = f; return *this; }

	UrrClient()                                           { Init(); }
	UrrClient(const char *addr, int port, byte f = 0)     { Init(); SetServer(addr, port); flags = f; }
	~UrrClient();
};

END_UPP_NAMESPACE

#endif
