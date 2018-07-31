class Ssh {
public:
    bool                Do();
    void                Cancel()                                { if(ssh) ssh->status = CANCELLED; }
    int                 GetTimeout() const                      { return ssh->timeout; }
    int					GetWaitStep() const						{ return ssh->waitstep; }
    bool                IsWorking() const                       { return ssh->status == WORKING || ssh->status == CLEANUP; }
    bool                IsBlocking() const                      { return ssh->timeout != 0; }
    bool                IsError() const                         { return ssh->status == FAILED; }
    int                 GetError() const                        { return ssh->error.Get<int>(); }
    String              GetErrorDesc() const                    { return ssh->error.Get<String>(); }
    void                AddTo(SocketWaitEvent& e)               { e.Add(*ssh->socket, GetWaitEvents()); }
    dword               GetWaitEvents();
    int64               GetId() const                           { return ssh->oid;   }
    int                 GetType() const                         { return ssh->otype; }
    template <class T>  T&   To()                               { auto* t = dynamic_cast<T*>(this); ASSERT(t); return *t; }
    template <class T>  bool Is() const                         { return dynamic_cast<const T*>(this); }
    static void         Trace(bool b = true)                    { SSH::sTrace = b; }
    static void         TraceVerbose(int level)                 { Trace((bool)level); SSH::sTraceVerbose = level; }

    Ssh();
    Ssh(bool dummy)    {}
    virtual ~Ssh();

    Ssh(Ssh&&) = default;
    Ssh& operator=(Ssh&&) = default;

    struct Error : Exc {
        int code;
        Error() : Exc(Null), code(-1) {}
        Error(const String& reason) : Exc(reason), code(-1) {}
        Error(int rc, const String& reason) : Exc(reason), code(rc) {}
    };
    enum Type  { SESSION, SFTP, CHANNEL, SCP, EXEC, SHELL, TCPTUNNEL };

protected:
    struct CoreData {
        BiVector<Tuple<int, Gate<>>> queue;
        LIBSSH2_SESSION*    session;
        TcpSocket*          socket;
        Tuple<int, String>  error;
        Event<>             wait;
        int                 ccmd;
        bool                init;
        int64               oid;
        int                 otype;
        int                 timeout;
        int                 start_time;
        int                 waitstep;
        int                 chunk_size;
        int                 status;
        dword               events;
        bool				noloop;
    };
    One<CoreData> ssh;

    constexpr static int CHUNKSIZE = 1024 * 64;

    enum Status         { WORKING, FINISHED, CLEANUP, CANCELLED, FAILED };

    virtual bool        Init()                                  { return true; }
    virtual void        Exit();
    virtual bool        Cmd(int code, Function<bool()>&& fn);
    virtual bool        ComplexCmd(int code, Function<void()>&& fn);
    inline  bool        IsComplexCmd() const                    { return ssh->ccmd != -1; }
    virtual void        Check();
    virtual bool        Cleanup(Error& e);
    void                Wait();

    bool                WouldBlock(int rc)                      { return rc == LIBSSH2_ERROR_EAGAIN; }
    bool                WouldBlock()                            { return ssh->session && WouldBlock(libssh2_session_last_errno(ssh->session)); }
    bool                IsTimeout() const                       { return ssh->timeout > 0 && msecs(ssh->start_time) >= ssh->timeout; }
    int&                OpCode()                                { return ssh->queue.Head().Get<int>(); }
    void                SetError(int rc, const String& reason = Null);
private:
    bool                 Do0();
    static int64         GetNewId();
};
