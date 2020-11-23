class Ssh {
public:
    void                Abort()                                 { ssh->status = ABORTED; }
    Ssh&                WaitStep(int ms)                        { ssh->waitstep = clamp(ms, 0, INT_MAX); return *this; }
    int                 GetTimeout() const                      { return ssh->timeout; }
    int                 GetWaitStep() const                     { return ssh->waitstep; }
    bool                InProgress() const                      { return ssh->status == WORKING; }
    bool                IsError() const                         { return ssh->status == FAILED || ssh->status == ABORTED; }
    int                 GetError() const                        { return ssh->error.Get<int>(); }
    String              GetErrorDesc() const                    { return ssh->error.Get<String>(); }
    int64               GetId() const                           { return ssh->oid;   }
    int                 GetType() const                         { return ssh->otype; }
    template <class T>  T&   To()                               { auto* t = dynamic_cast<T*>(this); ASSERT(t); return *t; }
    template <class T>  bool Is() const                         { return dynamic_cast<const T*>(this); }

    operator bool() const                                       { return ssh; }

    Event<>             WhenWait;
    
    static void         Trace(bool b = true)                    { SSH::sTrace = b; }
    static void         TraceVerbose(int level)                 { Trace((bool)level); SSH::sTraceVerbose = level; }

    Ssh();
    virtual ~Ssh();

    Ssh(Ssh&&) = default;

    struct Error : Exc {
        int code;
        Error() : Exc(Null), code(-1) {}
        Error(const String& reason) : Exc(reason), code(-1) {}
        Error(int rc, const String& reason) : Exc(reason), code(rc) {}
    };
    enum Type  { CORE, SESSION, SFTP, CHANNEL, SCP, EXEC, SHELL, TUNNEL };

protected:
    struct CoreData {
        LIBSSH2_SESSION*    session;
        TcpSocket*          socket;
        Tuple<int, String>  error;
        Event<>             whenwait;
        bool                init;
        int64               oid;
        int                 otype;
        int                 timeout;
        int                 start_time;
        int                 waitstep;
        int                 chunk_size;
        int                 status;
    };
    One<CoreData> ssh;

    const int           CHUNKSIZE = 1024 * 64;

    enum Status         { IDLE, WORKING, FAILED, ABORTED };

    virtual bool        Init()                                  { return true; }
    virtual void        Exit()                                  {}
    void                Wait();
    bool                Run(Gate<>&& fn, bool abortable = true);
    bool                WouldBlock(int rc)                      { return rc == LIBSSH2_ERROR_EAGAIN; }
    bool                WouldBlock()                            { return ssh->session && WouldBlock(libssh2_session_last_errno(ssh->session)); }
    bool                IsTimeout() const                       { return !IsNull(ssh->timeout) && ssh->timeout > 0 &&  msecs(ssh->start_time) >= ssh->timeout; }
    void                ThrowError(int rc, const String& reason = Null);
    void                SetError(int rc, const String& reason);
    void                UpdateClient()                          { WhenWait  ? WhenWait() : ssh->whenwait(); }
    
private:
    static int64        GetNewId();

};
