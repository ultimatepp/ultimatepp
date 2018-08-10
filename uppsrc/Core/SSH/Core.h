class Ssh {
public:
    void                Abort()                                 { if(ssh) ssh->status = ABORTED; }
    int                 GetTimeout() const                      { return ssh->timeout; }
    int					GetWaitStep() const						{ return ssh->waitstep; }
    bool                InProgress() const                      { return ssh->status == WORKING; }
    bool                IsError() const                         { return ssh->status == FAILED || ssh->status == ABORTED; }
    int                 GetError() const                        { return ssh->error.Get<int>(); }
    String              GetErrorDesc() const                    { return ssh->error.Get<String>(); }
    int64               GetId() const                           { return ssh->oid;   }
    int                 GetType() const                         { return ssh->otype; }
    template <class T>  T&   To()                               { auto* t = dynamic_cast<T*>(this); ASSERT(t); return *t; }
    template <class T>  bool Is() const                         { return dynamic_cast<const T*>(this); }

    operator bool()												{ return ssh; }

    static void         Trace(bool b = true)                    { SSH::sTrace = b; }
    static void         TraceVerbose(int level)                 { Trace((bool)level); SSH::sTraceVerbose = level; }

    Ssh();
    virtual ~Ssh();

    Ssh(Ssh&&) = default;
    Ssh& operator=(Ssh&&) = default;

    struct Error : Exc {
        int code;
        Error() : Exc(Null), code(-1) {}
        Error(const String& reason) : Exc(reason), code(-1) {}
        Error(int rc, const String& reason) : Exc(reason), code(rc) {}
    };
    enum Type  { CORE, SESSION, SFTP, CHANNEL, SCP, EXEC, SHELL, TCPTUNNEL };

protected:
    struct CoreData {
        LIBSSH2_SESSION*    session;
        TcpSocket*          socket;
        Tuple<int, String>  error;
        Event<>             wait;
        bool                init;
        int64               oid;
        int                 otype;
        int                 timeout;
        int                 start_time;
        int                 waitstep;
        int                 chunk_size;
        int                 status;
        bool				noloop;
    };
    One<CoreData> ssh;

    constexpr static int CHUNKSIZE = 1024 * 64;

    enum Status         { IDLE, WORKING, FAILED, ABORTED };

    virtual bool        Init()                                  { return true; }
    virtual void        Exit()                                  {}
    virtual void        Check();
    void                Wait();
    bool                Run(Gate<>&& fn);
    bool                WouldBlock(int rc)                      { return rc == LIBSSH2_ERROR_EAGAIN; }
    bool                WouldBlock()                            { return ssh->session && WouldBlock(libssh2_session_last_errno(ssh->session)); }
    bool                IsTimeout() const                       { return ssh->timeout > 0 && msecs(ssh->start_time) >= ssh->timeout; }
    void                SetError(int rc, const String& reason = Null);

    void                AddTo(SocketWaitEvent& e)               { e.Add(*ssh->socket, GetWaitEvents()); }
    dword               GetWaitEvents();
    
private:
    static int64        GetNewId();

};
