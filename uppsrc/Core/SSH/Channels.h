class SshChannel : public Ssh {
public:
    SshChannel&         Timeout(int ms)                                                     { ssh->timeout = ms; return *this; }
    SshChannel&         ChunkSize(int sz)                                                   { ssh->chunk_size = clamp(sz, 1, INT_MAX); return *this; }

    LIBSSH2_CHANNEL*    GetHandle() const                                                   { return channel ? *channel : nullptr; }
    int                 GetDone() const                                                     { return done; }

    bool                IsOpen() const                                                      { return channel; }

    bool                Open();
    bool                Close();
    bool                WaitClose();

    bool                Request(const String& request, const String& params = Null);
    bool                RequestExec(const String& cmdline)                                  { return Request("exec", cmdline); }
    bool                RequestShell()                                                      { return Request("shell", Null); }
    bool                RequestSubsystem(const String& subsystem)                           { return Request("subsystem", subsystem); }
    bool                RequestTerminal(const String& term, int width, int height, const String& tmodes);
    bool                RequestTerminal(const String& term, Size sz, const String& tmodes)   { return RequestTerminal(term, sz.cx, sz.cy, tmodes); }
    bool                SetEnv(const String& variable, const String& value);

    int                 Get(void *ptr, int size, int sid = 0);
    String              Get(int size, int sid = 0);
    String              GetLine(int maxlen = 65536, int sid = 0);
    String              GetStdErr(int size)                                                 { return Get(size, SSH_EXTENDED_DATA_STDERR); }
    int                 Put(const void *ptr, int size, int sid = 0);
    int                 Put(const String& s, int sid = 0)                                   { return Put(~s, s.GetLength(), sid); }
    int                 PutStdErr(const String& err)                                        { return Put(err, SSH_EXTENDED_DATA_STDERR); }

    bool                PutEof();
    bool                GetEof();
    bool                PutGetEof()                                                         { return PutEof() && GetEof(); }
    bool                IsEof();

    bool                SetTerminalSize(int width, int height);
    bool                SetTerminalSize(Size sz)                                            { return SetTerminalSize(sz.cx, sz.cy); }
    bool                SetReadWindowSize(uint32 size, bool force = false);
    uint32              GetReadWindowSize()                                                 { return libssh2_channel_window_read(*channel); }
    uint32              GetWriteWindowSize()                                                { return libssh2_channel_window_write(*channel); }
    int                 GetExitCode();
    String              GetExitSignal();

    SshChannel(SshSession& session);
    virtual ~SshChannel();

    SshChannel(SshChannel&&) = default;

protected:
    bool                Init() override;
    void                Exit() override;
    int                 Read(void *ptr, int size, int sid = 0);
    int                 Read(int sid = 0);
    int                 Write(const void *ptr, int size, int sid = 0);
    bool                Write(char c, int sid = 0);
    bool                SetWndSz(uint32 size, bool force = false);
    int                 SetPtySz(int w, int h);
    int                 SetPtySz(Size sz)                                                   { return SetPtySz(sz.cx, sz.cy); }


    dword               EventWait(int fd, dword events, int tv = 10);
    bool                ProcessEvents(String& input);
    virtual void        ReadWrite(String& in, const void* out, int out_len)                 {}

    bool                Shut(const String& msg, bool nowait = true);
    
    One<LIBSSH2_CHANNEL*>  channel;
    int                    done;
};

class SshExec : public SshChannel {
public:
    int Execute(const String& cmd, String& out, String& err);
    int operator()(const String& cmd, String& out, String& err)                             { return Execute(cmd, out, err); }

    SshExec(SshSession& session) : SshChannel(session)                                      { ssh->otype = EXEC; };
private:
	void ReadStdOut(String& out);
	void ReadStdErr(String& err);
};

int     SshExecute(SshSession& session, const String& cmd, String& out, String& err);
int     SshExecute(SshSession& session, const String& cmd, String& out);
String  SshExecute(SshSession& session, const String& cmd);

class Scp : public SshChannel {
public:
    bool   SaveFile(const char *path, const String& data);
    String LoadFile(const char *path);
    bool   SaveFile(const char *path, Stream& in);
    bool   LoadFile(Stream& out, const char *path);

    Gate<int64, int64> WhenProgress;
    
    Scp(SshSession& session) : SshChannel(session)                                          { ssh->otype = SCP; ssh->init = true; }

private:
    bool   OpenRead(const String& path, ScpAttrs& attrs);
    bool   OpenWrite(const String& path, int64 size, long mode = 0644);
    bool   Load(Stream& s, ScpAttrs a, int64 maxsize = INT64_MAX);
    bool   Save(Stream& s);
};

class SshTunnel : public SshChannel {
public:
    bool Connect(const String& host, int port);
    bool Connect(const String& url);
    bool Listen(int port, int listen_count = 5)                                             { return Listen(Null, port, nullptr, listen_count); }
    bool Listen(const String& host, int port, int* bound_port, int listen_count = 5);
    bool Accept(SshTunnel& listener);

    SshTunnel(SshSession& session) : SshChannel(session)                                    { ssh->otype = TUNNEL; mode = NONE; ssh->init = true; }
    virtual ~SshTunnel()                                                                    { Exit(); }

    SshTunnel(SshTunnel&&) = default;

private:
    void Exit() override;
    bool IsValid();

    int  mode;
    One<LIBSSH2_LISTENER*>  listener;

    enum Modes { NONE, CONNECT, LISTEN, ACCEPT };
};


class SshShell : public SshChannel {
public:
    bool        Run(const String& terminal, Size pagesize, const String& tmodes = Null)                 { return Run(GENERIC, terminal, pagesize, tmodes);  }
    bool        Run(const String& terminal, int width, int height, const String& tmodes = Null)         { return Run(GENERIC, terminal, {width, height}, tmodes); }
    
    bool        Console(const String& terminal, const String& tmodes = Null)                            { return Run(CONSOLE, terminal, GetConsolePageSize(), tmodes); }

    SshShell&   ForwardX11(const String& host = Null, int display = 0, int screen = 0, int bufsize = 1024 * 1024);
    bool        AcceptX11(SshX11Handle xhandle);

    void        Send(int c)                     { queue.Cat(c);   }
    void        Send(const char* s)             { Send(String(s));}
    void        Send(const String& s)           { queue.Cat(s);   }

    SshShell&   PageSize(Size sz)               { resized = sz != psize; if(resized) psize = sz; return *this;}
    Size        GetPageSize() const             { return psize; }
    Size        GetConsolePageSize();

    Event<>                  WhenInput;
    Event<const void*, int>  WhenOutput;

    SshShell(SshSession& session);
    virtual ~SshShell();

    SshShell(SshShell&&) = default;

protected:
    void    ReadWrite(String& in, const void* out, int out_len) override;
    virtual bool Run(int mode_, const String& terminal, Size pagesize, const String& tmodes);

    void    Resize();
    bool    ConsoleInit();
    void    ConsoleRead();
    void    ConsoleWrite(const void* buffer, int len);
    void    ConsoleRawMode(bool b = true);

    bool    X11Init();
    void    X11Loop();

    enum Modes { GENERIC, CONSOLE };

private:
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
    Vector<Tuple<SshX11Handle, SOCKET>> xrequests;
#endif
};