class SshSession : public Ssh {
public:
  enum Methods {
        METHOD_EXCHANGE = 0,
        METHOD_HOSTKEY,
        METHOD_CENCRYPTION,
        METHOD_SENCRYPTION,
        METHOD_CMAC,
        METHOD_SMAC,
        METHOD_CCOMPRESSION,
        METHOD_SCOMPRESSION
    };

public:
    SshSession&         Timeout(int ms)                         { ssh->timeout = ms; return *this; }
    SshSession&         NonBlocking(bool b = true)              { return Timeout(b ? 0 : Null);}
    SshSession&         WaitStep(int ms)                        { ssh->waitstep = clamp(ms, 0, INT_MAX); return *this; }

    SshSession&         Keys(const String& prikey, const String& pubkey, const String& phrase = Null, bool fromfile = true);
    SshSession&         Method(int type, Value method)          { session->iomethods(type) = pick(method); return *this; }
    SshSession&         Methods(ValueMap methods)               { session->iomethods = pick(methods); return *this; }

    SshSession&         PasswordAuth()                          { session->authmethod = PASSWORD;  return *this; }
    SshSession&         PublicKeyAuth()                         { session->authmethod = PUBLICKEY; return *this; }
    SshSession&         HostBasedAuth()                         { session->authmethod = HOSTBASED; return *this; }
    SshSession&         KeyboardAuth()                          { session->authmethod = KEYBOARD;  return *this; }
    SshSession&         AgentAuth()                             { session->authmethod = SSHAGENT;  return *this; }

    LIBSSH2_SESSION*    GetHandle()                             { return ssh->session; }
    String              GetBanner() const                       { return ssh->session ? pick(String(libssh2_session_banner_get(ssh->session))) : Null; }
    String              GetFingerprint() const                  { return session->fingerprint; }
    Vector<String>      GetAuthMethods()                        { return pick(Split(session->authmethods, ' ')); }
    TcpSocket&          GetSocket()                             { return session->socket;  }
    ValueMap            GetMethods();
    std::atomic<int64>* GetLockPtr()                            { return &session->lock; }

    SFtp                CreateSFtp();
    SshChannel          CreateChannel();
    SshExec             CreateExec();
    Scp                 CreateScp();
    SshTunnel           CreateTcpTunnel();
    SshShell            CreateShell();

    bool                Connect(const String& url);
    bool                Connect(const String& host, int port, const String& user, const String& password);
    void                Disconnect();
    
//    Event<>             WhenDo;
    Event<>             WhenWait;
    Event<>             WhenConfig;
    Event<>             WhenAuth;
    Gate<>              WhenVerify;
    Gate<>              WhenProxy;
    Event<SshX11Connection*> WhenX11;
    Function<String(String, String, String)>  WhenKeyboard;

    SshSession();
    virtual ~SshSession();

    SshSession(SshSession&&) = default;
    SshSession& operator=(SshSession&&) = default;

private:
    void                Exit() override;
    void                Check() override;
    String              GetMethodNames(int type);
    int                 TryAgent(const String& username);
    void                FreeAgent(SshAgent* agent);
    
    struct SessionData {
        TcpSocket       socket;
        IpAddrInfo      ipinfo;
        String          fingerprint;
        String          authmethods;
        int             authmethod;
        String          prikey;
        String          pubkey;
        bool            keyfile;
        String          phrase;
        ValueMap        iomethods;
        bool            connected;
        std::atomic<int64>   lock;
    };
    One<SessionData> session;

    enum AuthMethod     { PASSWORD, PUBLICKEY, HOSTBASED, KEYBOARD, SSHAGENT };
    enum HostkeyType    { RSAKEY, DSSKEY };
    enum OpCodes        { CONNECT, LOGIN, DISCONNECT };
};
