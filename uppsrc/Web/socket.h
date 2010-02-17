//#define NOFAKEERROR

static const int DEFAULT_CONNECT_TIMEOUT = 5000;

static const int SOCKKIND_STD = 1; // GetKind() for ordinary socket

class Socket : Moveable<Socket>
{
public:
	class Data
	{
	public:
		Data();
		virtual ~Data() { CloseRaw(0); }

		bool                    Open(bool is_blocking);
		bool                    OpenServer(int port, bool nodelay, int listen_count, bool is_blocking, bool reuse = true);
		bool                    OpenClient(const char *host, int port, bool nodelay, dword *my_addr, int timeout, bool is_blocking);
		bool                    IsOpen() const     { return socket != INVALID_SOCKET; }
		bool                    CloseRaw(int timeout_msec);

		bool                    IsEof() const          { return is_eof && leftover.IsEmpty(); }
		bool                    IsError() const        { return is_error; }
		void                    SetError()             { is_error = true; }
		void                    ClearError()           { is_error = false; }
		void                    SetSockError(String context);

		void                    NoDelay();
		void                    Linger(int msecs);
		void                    Block(bool b = true);
		void                    Reuse();
//		void                    WriteTimeout(int msecs);
//		void                    ReadTimeout(int msecs);
		void                    StopWrite();
		
		String                  GetPeerAddr() const;

		virtual int             GetKind() const        { return SOCKKIND_STD; }

		virtual bool            Peek(int timeout_msec, bool write);
		virtual int             Read(void *buf, int amount);
		virtual int             Write(const void *buf, int amount);
		virtual bool            Accept(Socket& socket, dword *ipaddr, bool nodelay, int timeout_msec);
		virtual bool            Close(int timeout_msec);
		virtual Value           GetInfo(String info) const;

	public:
		SOCKET                  socket;
		String                  leftover;
		bool                    is_blocking;
		bool                    is_error;
		bool                    is_eof;
#ifndef NOFAKEERROR
		int                     fake_error;
#endif

	protected:
		SOCKET                  AcceptRaw(dword *ipaddr, int timeout_msec);
		void                    Attach(SOCKET socket, bool nodelay, bool is_blocking);
		void                    SetSockResError(String context);
		void                    AttachRaw(SOCKET s, bool blocking);

		friend void AttachSocket(Socket& socket, SOCKET hsocket, bool blocking);
	};

	Socket() {}
	Socket(One<Data> data) : data(data) {}

	static void     Init();

	void            Attach(One<Data> d)                      { data = d; }
	void            Clear()                                  { data.Clear(); }

	bool            IsOpen() const                           { return data && data->IsOpen(); }

	bool            IsEof() const                            { return !data || data->IsEof(); }
	bool            IsError() const                          { return data->IsError(); }
	void            ClearError()                             { data->ClearError(); }

	SOCKET          GetSocket() const                        { return IsOpen() ? data->socket : INVALID_SOCKET; }
	int             GetNumber() const                        { return (int)GetSocket(); }

	int             GetKind() const                          { return data->GetKind(); }
	Value           GetInfo(String info) const               { return data->GetInfo(info); }
	
	String          GetPeerAddr() const                      { return data->GetPeerAddr(); }

	bool            Accept(Socket& socket, dword *ipaddr = 0, bool nodelay = true, int timeout_msec = DEFAULT_CONNECT_TIMEOUT) { return data->Accept(socket, ipaddr, nodelay, timeout_msec); }
	bool            Close(int msecs_timeout = 0);

	void            NoDelay()                                { data->NoDelay(); }
	void            Linger(int msecs)                        { data->Linger(msecs); }
	void            NoLinger()                               { data->Linger(Null); }
	void            Block(bool b = true)                     { data->Block(b); }
	void            NoBlock()                                { data->Block(false); }
	void            Reuse()                                  { data->Reuse(); }

	static bool     Wait(const Vector<SOCKET>& read, const Vector<SOCKET>& write, int timeout_msec);
	static bool     Wait(const Vector<Socket *>& read, const Vector<Socket *>& write, int timeout_msec);

	bool            Peek(int timeout_msec = 0)               { return data->Peek(timeout_msec, false); }
	bool            PeekWrite(int timeout_msec = 0)          { return data->Peek(timeout_msec, true); }
	bool            PeekAbort(int timeout_msec = 0);
	String          PeekCount(int count, int timeout_msec = Null);
	String          PeekUntil(char term, int timeout_msec = Null, int maxlen = 1000000);

	int             ReadRaw(void *buffer, int maxlen)        { return data->Read(buffer, maxlen); }
	String          Read(int timeout_msec = Null, int maxlen = 1000000);
	int             ReadCount(void *buffer, int count, int timeout_msec = Null);
	String          ReadCount(int count, int timeout_msec = Null);
	String          ReadUntil(char term, int timeout_msec = Null, int maxlen = 1000000);
	String          ReadUntil(Gate1<int> term, int& termchar, int timeout = Null, int maxlen = 1000000);
	void            UnRead(const void *buffer, int len);
	void            UnRead(String data)                      { UnRead(data.Begin(), data.GetLength()); }

	int             WriteRaw(const void *buffer, int maxlen) { return data->Write(buffer, maxlen); }
	int             WriteWait(const char *s, int length, int timeout_msec);
	void            Write(const char *s, int length)         { WriteWait(s, length, Null); }
	void            Write(String s)                          { Write(s.Begin(), s.GetLength()); }

	void            StopWrite()                              { data->StopWrite(); }

	static String   GetHostName();

	static String   GetErrorText();
	static void     SetErrorText(String text);
	static void     ClearErrorText()                         { SetErrorText(Null); }
	static void     SetSockError(const char *context)        { SetSockError(INVALID_SOCKET, context); }
	static void     SetSockError(SOCKET socket, const char *context);
	static void     SetSockError(SOCKET socket, const char *context, const char *errordesc);

#if defined(PLATFORM_WIN32)
	static int      GetErrorCode()                           { return WSAGetLastError(); }
#define SOCKERR(x) COMBINE(WSA, x)
#elif defined(PLATFORM_POSIX)
	static int      GetErrorCode()                           { return errno; }
#define SOCKERR(x) x
#else
#error Unsupported platform
#endif

private:
	One<Data>       data;
};

#ifdef PLATFORM_WIN32
class SocketEvent : public Event {
public:
	SocketEvent() {}

	void     Read(Socket& socket);
	void     Write(Socket& socket);
	void     Accept(Socket& socket);
	void     Select(Socket& socket, int fd_flags);
};
#endif

bool ServerSocket(Socket& socket, int port, bool nodelay = true, int listen_count = 5, bool is_blocking = true, bool reuse = true);
bool ClientSocket(Socket& socket, const char *host, int port, bool nodelay = true, dword *my_addr = NULL, int timeout = DEFAULT_CONNECT_TIMEOUT, bool is_blocking = true);
void AttachSocket(Socket& socket, SOCKET s, bool blocking);
