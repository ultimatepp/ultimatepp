class SshSession : public Ssh {
public:
    enum Methods : int {
        METHOD_EXCHANGE = 0,
        METHOD_HOSTKEY,
        METHOD_CENCRYPTION,
        METHOD_SENCRYPTION,
        METHOD_CMAC,
        METHOD_SMAC,
        METHOD_CCOMPRESSION,
        METHOD_SCOMPRESSION,
        METHOD_CLANGUAGE,
        METHOD_SLANGUAGE
    };

    enum Phase : int {
        PHASE_DNS,
        PHASE_CONNECTION,
        PHASE_HANDSHAKE,
        PHASE_AUTHORIZATION,
        PHASE_SUCCESS
    };
    
public:
    SshSession&         Timeout(int ms)                         { ssh->timeout = ms; return *this; }
 
    SshSession&         Compression(bool b = true)              { session->compression = b; return *this; }
    SshSession&         NoCompression()                         { return Compression(false); }

    SshSession&         Keys(const String& prikey, const String& pubkey, const String& phrase, bool fromfile = true);
    SshSession&         Method(int type, Value method)          { session->iomethods(type) << pick(method); return *this; }
    SshSession&         Methods(ValueMap methods)               { session->iomethods = pick(methods); return *this; }
 
    SshSession&         PasswordAuth()                          { session->authmethod = PASSWORD;  return *this; }
    SshSession&         PublicKeyAuth()                         { session->authmethod = PUBLICKEY; return *this; }
    SshSession&         HostBasedAuth()                         { session->authmethod = HOSTBASED; return *this; }
    SshSession&         KeyboardAuth()                          { session->authmethod = KEYBOARD;  return *this; }
    SshSession&         AgentAuth()                             { session->authmethod = SSHAGENT;  return *this; }
 
    LIBSSH2_SESSION*    GetHandle()                             { return ssh->session; }
    
    String              GetBanner() const                       { return ssh->session ? pick(String(libssh2_session_banner_get(ssh->session))) : Null; }
    String              GetMD5Fingerprint() const               { return GetHostKeyHash(LIBSSH2_HOSTKEY_HASH_MD5, 16);    }
    String              GetSHA1Fingerprint() const              { return GetHostKeyHash(LIBSSH2_HOSTKEY_HASH_SHA1, 20);   }
    String              GetSHA256Fingerprint() const            { return GetHostKeyHash(LIBSSH2_HOSTKEY_HASH_SHA256, 32); }
    Vector<String>      GetAuthMethods()                        { return pick(Split(session->authmethods, ',')); }
    TcpSocket&          GetSocket()                             { return session->socket;  }
    ValueMap            GetMethods() const;

    bool                Connect(const String& url);
    bool                Connect(const String& host, int port, const String& user, const String& password);
    void                Disconnect();
    
    Event<>             WhenConfig;
    Event<>             WhenAuth;
    Function<String()>  WhenPasswordChange;
    Event<int>          WhenPhase;
    Gate<String, int>   WhenVerify;
    Gate<>              WhenProxy;
    Event<SshX11Handle> WhenX11;
    Function<String(String, String, String)>  WhenKeyboard;
    
    SshSession();
    virtual ~SshSession();

    SshSession(SshSession&&) = default;

    // Deprecations.
    [[deprecated("This method is deprecated and will be removed in U++ 2025.1. Use the class constructor instead.")]] SFtp                CreateSFtp();
    [[deprecated("This method is deprecated and will be removed in U++ 2025.1. Use the class constructor instead.")]] SshChannel          CreateChannel();
    [[deprecated("This method is deprecated and will be removed in U++ 2025.1. Use the class constructor instead.")]] SshExec             CreateExec();
    [[deprecated("This method is deprecated and will be removed in U++ 2025.1. Use the class constructor instead.")]] Scp                 CreateScp();
    [[deprecated("This method is deprecated and will be removed in U++ 2025.1. Use the class constructor instead.")]] SshTunnel           CreateTunnel();
    [[deprecated("This method is deprecated and will be removed in U++ 2025.1. Use the class constructor instead.")]] SshShell            CreateShell();

private:
    void                Exit() override;
    String              GetHostKeyHash(int type, int length) const;
    String              GetMethodNames(int type) const;
    int                 TryAgent(const String& username);
    void                FreeAgent(SshAgent* agent);
    
    struct SessionData {
        TcpSocket       socket;
        String          authmethods;
        int             authmethod;
        String          prikey;
        String          pubkey;
        bool            keyfile;
        String          phrase;
        ValueMap        iomethods;
        bool            connected;
        bool            compression;
    };
    One<SessionData> session;

    enum AuthMethod     { PASSWORD, PUBLICKEY, HOSTBASED, KEYBOARD, SSHAGENT };
    enum HostkeyType    { RSAKEY, DSSKEY };
};
