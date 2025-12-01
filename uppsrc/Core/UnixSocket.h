#ifndef _Core_UnixSocket_h_
#define _Core_UnixSocket_h_

#ifdef PLATFORM_POSIX

class UnixSocket : NoCopy { // AF_UNIX
public:
	enum class Type {
		FileSystem,
		Abstract
	};
    
	UnixSocket();
	virtual ~UnixSocket()                                    { Close(); }

	Event<>         WhenWait;

	static String   GetHostName()                            { return TcpSocket::GetHostName(); }
	int             GetPeerPid() const;

	int             GetDone() const                          { return socket.GetDone(); }

	bool            IsOpen() const                           { return socket.IsOpen(); }
	bool            IsEof() const                            { return socket.IsEof();  }

	bool            IsError() const                          { return socket.IsError(); }
	void            ClearError()                             { socket.ClearError(); }
	int             GetError() const                         { return socket.GetError(); }
	String          GetErrorDesc() const                     { return socket.GetErrorDesc(); }

	void            Abort()                                  { socket.Abort(); }
	bool            IsAbort() const                          { return socket.IsAbort(); }
	void            ClearAbort()                             { socket.ClearAbort(); }
	
	bool            IsTimeout() const                        { return socket.IsTimeout(); }
	
	SOCKET          GetSOCKET() const                        { return socket.GetSOCKET(); }

	void            Attach(SOCKET sock)                      { socket.Attach(sock); }
	
	void            Close()                                  { socket.Close(); }
	void            Shutdown()                               { socket.Shutdown(); }

	void            Linger(int ms)                           { socket.Linger(ms); }
	void            NoLinger()                               { socket.NoLinger(); }
	
	bool            Wait(dword events)                       { return socket.Wait(events); }
	bool            WaitRead()                               { return socket.WaitRead();   }
	bool            WaitWrite()                              { return socket.WaitWrite();  }

	int             Peek()                                   { return socket.Peek(); }
	int             Term()                                   { return socket.Peek(); }
	int             Get()                                    { return socket.Get();  }
	int             Get(void *buffer, int len)               { return socket.Get(buffer, len); }
	String          Get(int len);

	int             Put(const void *s, int len)              { return socket.Put(s, len); }
	int             Put(const String& s)                     { return socket.Put(s); }

	bool            GetAll(void *buffer, int len)            { return socket.GetAll(buffer, len); }
	String          GetAll(int len)                          { return socket.GetAll(len); }
	String          GetLine(int maxlen = 65536)              { return socket.GetLine(maxlen); }

	bool            PutAll(const void *s, int len)           { return socket.PutAll(s, len); }
	bool            PutAll(const String& s)                  { return socket.PutAll(s); }
	
	void            Clear()                                  { socket.Clear(); }

	int             GetTimeout() const                       { return socket.GetTimeout(); }
	bool            IsBlocking()                             { return socket.IsBlocking(); }
	int             GetWaitStep() const                      { return socket.GetWaitStep(); }

	UnixSocket&     Timeout(int ms)                          { socket.Timeout(ms); return *this; }
	UnixSocket&     GlobalTimeout(int ms)					 { socket.GlobalTimeout(ms); return *this; }
	UnixSocket&     NoGlobalTimeout()                        { socket.NoGlobalTimeout(); return *this;  }
	UnixSocket&     Blocking()                               { socket.Blocking(); return *this; }
	UnixSocket&     WaitStep(int ms)                         { socket.WaitStep(ms); return *this; }
					
	bool            Connect(const String& path, Type t = Type::FileSystem);
	bool            WaitConnect()                            { return socket.WaitConnect(); }
	
	bool            Listen(const String& path, int listen_count = 5, bool reuse = true, Type t = Type::FileSystem);
	bool            Accept(UnixSocket& listen_socket);

	void            SetSockError(const char *context, int code, const char *errdesc) { socket.SetSockError(context, code, errdesc); }
	
private:
	TcpSocket       socket;
};

#endif

#endif
