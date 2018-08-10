/*
class SshChannel : public Ssh {
public:
    SshChannel&         Timeout(int ms)                                             { ssh->timeout = ms; return *this;  }
    SshChannel&         NonBlocking(bool b = true)                                  { return Timeout(b ? 0 : Null); }
    SshChannel&         WaitStep(int ms)                                            { ssh->waitstep = clamp(ms, 0, INT_MAX); return *this; }
    SshChannel&         ChunkSize(int sz)                                           { ssh->chunk_size = clamp(sz, 128, INT_MAX); return *this; }


    LIBSSH2_CHANNEL*    GetHandle() const                                           { return *channel; }
    Value               GetResult()                                                 { return pick(result); }

    bool                Open();
    bool                Close();
    bool                CloseWait();
    bool                Request(const String& request, const String& params = Null);
    bool                Exec(const String& cmdline)                                 { return Request("exec", cmdline); }
    bool                Shell()                                                     { return Request("shell", Null); }
    bool                Subsystem(const String& subsystem)                          { return Request("subsystem", subsystem); }
    bool                SetEnv(const String& variable, const String& value);
    bool                Terminal(const String& term, int width, int height);
    bool                Terminal(const String& term, Size sz)                       { return Terminal(term, sz.cx, sz.cy); }

    String              Get(int64 size, int sid = 0);
    int64               Get(Stream& out, int64 size, int sid = 0);
    String              GetLine(int maxlen = 65536, int sid = 0);
    int                 GetNow(int sid = 0);
    int                 GetNow(void* buffer, int sid = 0);
    String              GetStdErr(int64 size = 65536)                               { return Get(size, SSH_EXTENDED_DATA_STDERR); }
    int64               GetStdErr(Stream& out, int64 size = 65536)                  { return Get(out, size, SSH_EXTENDED_DATA_STDERR); }
    int64               Put(const String& s, int sid = 0);
    int64               Put(Stream& in, int64 size = 65536, int sid = 0);
    bool                PutNow(char c, int sid = 0);
    int                 PutNow(const void* buffer, int64 size, int sid = 0);
    int64               PutStdErr(const String& err)                                { return Put(err, SSH_EXTENDED_DATA_STDERR); }
    int64               PutStdErr(Stream& err)                                      { return Put(err, err.GetSize(), SSH_EXTENDED_DATA_STDERR); }

    bool                SendEof();
    bool                RecvEof();
    bool                SendRecvEof();
    bool                IsEof();

    bool                SetTerminalSize(int width, int height);
    bool                SetTerminalSize(Size sz)                                    { return SetTerminalSize(sz.cx, sz.cy); }
    bool                SetReadWindowSize(int64 size, bool force = false);
    int64               GetReadWindowSize();
    int64               GetWriteWindowSize();
    int                 GetExitCode();
    String              GetExitSignal();

    bool                IsNullInstance() const                                      { return !channel || !ssh; }
    inline operator     bool() const                                                { return IsNullInstance();}

    Event<const void*, int> WhenContent;
    Gate<int64, int64>      WhenProgress;

    SshChannel() : Ssh(false) {}
    SshChannel(SshSession& session);
    virtual ~SshChannel();

    SshChannel(SshChannel&&) = default;
    SshChannel& operator=(SshChannel&&) = default;

protected:
    bool                Init() override;
    void                Exit() override;
    bool                Cleanup(Error& e) override;

    int                 AdjustChunkSize(int64 sz);
    void                Clear();

    int                 Read(void *buffer, int64 len, int sid = 0);
    int                 Read(int sid = 0);
    bool                ReadString(String& s, int64 len, int sid = 0, bool nb = false);
    bool                ReadStream(Stream& s, int64 len, int sid = 0, bool nb = false);
    bool                ReadContent(Event<const void*,int>&& consumer, int64 len, int sid = 0, bool nb =  false);

    int                 Write(const void* buffer, int64 len, int sid = 0);
    bool                Write(char c, int sid = 0);
    bool                WriteString(const String& s, int64 len, int sid = 0, bool nb = false);
    bool                WriteStream(Stream& s, int64 len, int sid = 0, bool nb = false);

    bool                SendEof0();
    bool                RecvEof0();

    bool                SetWndSz(int64 size, bool force = false);

    int                 SetPtySz(int w, int h);
    int                 SetPtySz(Size sz)                                           { return SetPtySz(sz.cx, sz.cy); }

    dword               EventWait(int fd, dword events, int tv = 10);
    bool                ProcessEvents(String& input);
    virtual void        ReadWrite(String& in, const void* out, int out_len);

    bool                Lock();
    void                Unlock();

    One<LIBSSH2_CHANNEL*>    channel;
    LIBSSH2_LISTENER*        listener;
    libssh2_struct_stat      filestat;
    Value                    result;
    int                      exitcode;
    String                   exitsignal;
    int64                    done;
    int64                    total;
    std::atomic<int64>*      lock;

    enum OpCodes {
        CHANNEL_INIT,
        CHANNEL_EXIT,
        CHANNEL_OPEN,
        CHANNEL_CLOSE,
        CHANNEL_REQUEST,
        CHANNEL_SET_ENV,
        CHANNEL_READ,
        CHANNEL_WRITE,
        CHANNEL_WAIT,
        CHANNEL_EOF,
        CHANNEL_STDERR,
        CHANNEL_RC,
        CHANNEL_SIGNAL,
        CHANNEL_WIN_SIZE,
        CHANNEL_PTY_SIZE,
        CHANNEL_SCP,
        CHANNEL_SCP_GET,
        CHANNEL_SCP_PUT,
        CHANNEL_EXEC,
        CHANNEL_SHELL,
        CHANNEL_TUNNEL,
        CHANNEL_TUNNEL_CONNECT,
        CHANNEL_TUNNEL_LISTEN,
        CHANNEL_TUNNEL_ACCEPT
    };
};

// Channels.
class Scp : public SshChannel {
public:
    bool    Get(const String& path, Stream& out);
    bool    operator()(const String& path, Stream& out)                                                 { return Get(path, out); }
    String  Get(const String& path);
    String  operator()(const String& path)                                                              { return Get(path); }
    bool    Put(Stream& in, const String& path, long mode = 0744);
    bool    Put(const String& in, const String& path, long mode = 0744);
    bool    operator()(Stream& in, const String& path, long mode = 0744)                                 { return Put(in, path, mode); }
    bool    operator()(const String& in, const String& path, long mode = 0744)                           { return Put(in, path, mode); }

    static  AsyncWork<String> AsyncGet(SshSession& session, const String& path, Gate<int64, int64, int64> progress = Null);
    static  AsyncWork<void>   AsyncGet(SshSession& session, const String& path, Stream& out, Gate<int64, int64, int64> progress = Null);
    static  AsyncWork<void>   AsyncPut(SshSession& session, String& in, const String& path, long mode = 0744, Gate<int64, int64, int64> progress = Null);
    static  AsyncWork<void>   AsyncPut(SshSession& session, Stream& in, const String& path, long mode = 0744, Gate<int64, int64, int64> progress = Null);
    static  AsyncWork<void>   AsyncGetToFile(SshSession& session, const String& src, const String& dest, Gate<int64, int64, int64> progress = Null);
    static  AsyncWork<void>   AsyncPutToFile(SshSession& session, const String& src, const String& dest, long mode = 0744, Gate<int64, int64, int64> progress = Null);
    static  AsyncWork<void>   AsyncConsumerGet(SshSession& session, const String& path, Event<int64, const void*, int> consumer);

    Scp(SshSession& session) : SshChannel(session)                                                      { ssh->otype = SCP; }

private:
    bool Init() override                                                                        { return Lock(); }
    bool Open(int opcode, const String& path, int64 size, long mode);
    static void StartAsync(int cmd, SshSession& session, const String& path, Stream& io, long mode,
                            Gate<int64, int64, int64> progress, Event<int64, const void*, int> consumer = Null);
};

class SshExec : public SshChannel {
public:
    int         Execute(const String& cmd, Stream& out, Stream& err);
    int         operator()(const String& cmd, Stream& out, Stream& err)                                 { return Execute(cmd, out, err); }

    static      AsyncWork<Tuple<int, String, String>> AsyncRun(SshSession& session, const String& cmd);

    SshExec(SshSession& session) : SshChannel(session)                                                  { ssh->otype = EXEC; };

private:
    bool        Init() override                                                                        { return Lock(); }
};

class SshTunnel : public SshChannel {
public:
    bool        Connect(const String& host, int port);
    bool        Connect(const String& url);
    bool        Listen(int port, int listen_count = 5)                                                  { return Listen(Null, port, NULL, listen_count); }
    bool        Listen(const String& host, int port, int* bound_port, int listen_count = 5);
    bool        Accept(SshTunnel& listener);

    SshTunnel(SshSession& session) : SshChannel(session)                                                { ssh->otype = TCPTUNNEL; mode = -1; }
    SshTunnel() : SshChannel()                                                                          {}

private:
    bool         Init() override                                                                        { return true; }
    void         Validate();
    int mode;
};

class SshShell : public SshChannel {
public:
    bool        Run(const String& terminal, Size pagesize)                                              { return Run(GENERIC, terminal, pagesize); }
    bool        Run(const String& terminal, int width, int height)                                      { return Run(GENERIC, terminal, {width, height}); }

    bool        Console(const String& terminal)                                                         { return Run(CONSOLE, terminal, GetConsolePageSize()); }

    SshShell&   ForwardX11(const String& host = Null, int display = 0, int screen = 0, int bufsize = 1024 * 1024);
    bool        AcceptX11(SshX11Connection* x11conn);

    void        Send(int c)                     { queue.Cat(c);   }
    void        Send(const char* s)             { Send(String(s));}
    void        Send(const String& s)           { queue.Cat(s);   }

    SshShell&   PageSize(Size sz)               { if((resized = sz != psize)) psize = sz; return *this;}
    Size        GetPageSize() const             { return psize; }
    Size        GetConsolePageSize();

    Event<>                  WhenInput;
    Event<const void*, int>  WhenOutput;

    static AsyncWork<void> AsyncRun(SshSession& session, String terminal, Size pagesize,
                                        Event<SshShell&> in, Event<const String&> out);

    SshShell(SshSession& session);
    virtual ~SshShell();

    SshShell(SshShell&&) = default;
    SshShell& operator=(SshShell&&) = default;

protected:
    void    ReadWrite(String& in, const void* out, int out_len) override;
    virtual bool Run(int mode_, const String& terminal, Size pagesize);

    void    Resize();
    bool    ConsoleInit();
    void    ConsoleRead();
    void    ConsoleWrite(const void* buffer, int len);
    void    ConsoleRawMode(bool b = true);

    bool    X11Init();
    void    X11Loop();

    enum Modes { GENERIC, CONSOLE };

private:
    bool    Init() override                                                                        { return Lock(); }

    String  queue;
    Size    psize;
    int     mode;
    bool    rawmode;
    bool    resized;
    bool    xenabled;
#ifdef PLATFORM_WIN32
    DWORD   tflags;
    HANDLE  stdinput;
    HANDLE  stdoutput;
#elif  PLATFORM_POSIX
    termios tflags;
    byte    xdisplay;
    byte    xscreen;
    String  xhost;
    Buffer<char> xbuffer;
    int          xbuflen;
    Vector<Tuple<SshX11Connection*, SOCKET>> xrequests;
#endif
};
*/