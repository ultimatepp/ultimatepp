String        FormatIP(dword _ip);

String        UrlEncode(const String& s);
String        UrlEncode(const String& s, const char *specials);
String        UrlDecode(const char *b, const char *e);
inline String UrlDecode(const String& s)          { return UrlDecode(s.Begin(), s.End() ); }

String        Base64Encode(const char *b, const char *e);
inline String Base64Encode(const String& data)    { return Base64Encode(data.Begin(), data.End()); }
String        Base64Decode(const char *b, const char *e);
inline String Base64Decode(const String& data)    { return Base64Decode(data.Begin(), data.End()); }

enum { WAIT_READ = 1, WAIT_WRITE = 2, WAIT_EXCEPTION = 3 };

class TcpSocket {
	enum { BUFFERSIZE = 512 };
	SOCKET                  socket;
	char                    buffer[BUFFERSIZE];
	char                   *ptr;
	char                   *end;
	bool                    is_eof;
	bool                    is_error;
	bool                    is_timeout;
	bool                    is_abort;
	bool                    ipv6;

	bool                    global;
	int                     timeout;
	int                     starttime;
	int                     waitstep;
	int                     done;

	int                     errorcode;
	String                  errordesc;


	bool                    CloseRaw();
	SOCKET                  AcceptRaw(dword *ipaddr, int timeout_msec);
	bool                    Open(int family, int type, int protocol);
	int                     Recv(void *buffer, int maxlen);
	int                     Send(const void *buffer, int maxlen);

	void                    ReadBuffer();
	int                     Get_();
	int                     Peek_();

	void                    Reset();

	void SetSockError(const char *context, const char *errdesc);
	void SetSockError(const char *context);

	static int              GetErrorCode();
	static bool             WouldBlock();

public:
	Callback        WhenWait;

	static String   GetHostName();
	
	int             GetDone() const                          { return done; }

	static void     Init();

	bool            IsOpen() const                           { return socket != INVALID_SOCKET; }
	bool            IsEof() const                            { return is_eof && ptr == end; }

	bool            IsError() const                          { return is_error; }
	void            ClearError()                             { is_error = false; errorcode = 0; errordesc.Clear(); }
	int             GetError() const                         { return errorcode; }
	String          GetErrorDesc() const                     { return errordesc; }

	bool            IsTimeout() const                        { return is_timeout; }
	void            ClearTimeout()                           { is_timeout = false; }
	
	void            Abort()                                  { is_abort = true; }
	bool            IsAbort() const                          { return is_abort; }
	void            ClearAbort()                             { is_abort = false; }
	
	SOCKET          GetSOCKET() const                        { return socket; }
	String          GetPeerAddr() const;

	void            Attach(SOCKET socket);
	bool            Connect(const char *host, int port);
	bool            Listen(int port, int listen_count, bool ipv6 = false, bool reuse = true);
	bool            Accept(TcpSocket& listen_socket);
	bool            Close();
	void            Shutdown();

	void            NoDelay();
	void            Linger(int msecs);
	void            NoLinger()                               { Linger(Null); }
	void            Reuse(bool reuse = true);
	
	bool            Wait(dword events);
	bool            WaitRead()                               { return Wait(WAIT_READ); }
	bool            WaitWrite()                              { return Wait(WAIT_WRITE); }

	int             Peek()                                   { return ptr < end ? *ptr : Peek_(); }
	int             Term()                                   { return Peek(); }
	int             Get()                                    { return ptr < end ? *ptr++ : Get_(); }
	int             Get(void *buffer, int len);
	String          Get(int len);
	int             GetAll(void *buffer, int len)            { return Get(buffer, len) == len; }
	String          GetAll(int len)                          { String s = Get(len); return s.GetCount() == len ? s : String::GetVoid(); }
	String          GetLine(int maxlen = 2000000);

	int             Put(const char *s, int len);
	int             Put(const String& s)                     { return Put(s.Begin(), s.GetLength()); }
	bool            PutAll(const char *s, int len)           { return Put(s, len) == len; }
	bool            PutAll(const String& s)                  { return Put(s) == s.GetCount(); }

	TcpSocket&      Timeout(int ms)                          { timeout = ms; global = false; return *this; }
	TcpSocket&      GlobalTimeout(int ms);
	TcpSocket&      Blocking()                               { return Timeout(Null); }

	TcpSocket();
	~TcpSocket()                                             { Close(); }
};

class SocketWaitEvent {
	Vector< Tuple2<SOCKET, dword> > socket;

public:
	void Clear()                                             { socket.Clear(); }
	void Add(SOCKET s, dword events)                         { socket.Add(MakeTuple(s, events)); }
	void Add(TcpSocket& s, dword events)                     { Add(s.GetSOCKET(), events); }

	int  Wait(int timeout);
};

struct HttpHeader {
	String                    first_line;
	VectorMap<String, String> fields;
	
	String operator[](const char *id)                        { return fields.Get(id, Null); }

	bool   Response(String& protocol, int& code, String& reason);
	bool   Request(String& method, String& uri, String& version);

	void   Clear();	
	bool   Parse(const String& hdrs);
};

class HttpRequest : public TcpSocket {
	int          phase;
	String       data;
	int          count;

	HttpHeader   header;

	String       error;
	String       body;

	enum {
		DEFAULT_HTTP_PORT        = 80,
	};

	enum {
		METHOD_GET,
		METHOD_POST,
		METHOD_HEAD,
		METHOD_PUT,
	};

	int          max_header_size;
	int          max_content_size;
	int          max_redirects;
	int          max_retries;

	String       host;
	int          port;
	String       proxy_host;
	int          proxy_port;
	String       proxy_username;
	String       proxy_password;
	String       path;

	int          method;
	String       accept;
	String       agent;
	bool         force_digest;
	bool         is_post;
	bool         std_headers;
	bool         hasurlvar;
	String       contenttype;
	String       username;
	String       password;
	String       digest;
	String       request_headers;
	String       postdata;

	String       protocol;
	int          status_code;
	String       response_phrase;
	
	int          retry_count;
	int          redirect_count;
	
	int          chunk;

	int          bodylen;
	bool         gzip;	
	Zlib         z;

	void         Init();

	void         StartPhase(int s);
	void         StartBody();
	bool         SendingData();
	bool         ReadingHeader();
	bool         ReadingBody();
	void         StartRequest();
	void         ReadingChunkHeader();
	void         Finish();

	void         HttpError(const char *s);
	void         ContentOut(const void *ptr, dword size);
	void         Out(const void *ptr, dword size);

	String       CalculateDigest(const String& authenticate) const;

public:
	Callback2<const void *, dword> WhenContent;

	HttpRequest&  MaxHeaderSize(int m)                   { max_header_size = m; return *this; }
	HttpRequest&  MaxContentSize(int m)                  { max_content_size = m; return *this; }
	HttpRequest&  MaxRedirect(int n)                     { max_redirects = n; return *this; }
	HttpRequest&  MaxRetries(int n)                      { max_retries = n; return *this; }
	HttpRequest&  ChunkSize(int n)                       { chunk = n; return *this; }

	HttpRequest&  Method(int m)                          { method = m; return *this; }
	HttpRequest&  GET()                                  { return Method(METHOD_GET); }
	HttpRequest&  POST()                                 { return Method(METHOD_POST); }
	HttpRequest&  HEAD()                                 { return Method(METHOD_HEAD); }
	HttpRequest&  PUT()                                  { return Method(METHOD_PUT); }

	HttpRequest&  Host(const String& h)                  { host = h; return *this; }
	HttpRequest&  Port(int p)                            { port = p; return *this; }
	HttpRequest&  Path(const String& p)                  { path = p; return *this; }
	HttpRequest&  User(const String& u, const String& p) { username = u; password = p; return *this; }
	HttpRequest&  Digest()                               { force_digest = true; return *this; }
	HttpRequest&  Digest(const String& d)                { digest = d; return *this; }
	HttpRequest&  Url(const char *url);
	HttpRequest&  UrlVar(const char *id, const String& data);
	HttpRequest&  operator()(const char *id, const String& data) { return UrlVar(id, data); }
	HttpRequest&  PostData(const String& pd)              { postdata = pd; return *this; }
	HttpRequest&  PostUData(const String& pd)             { return PostData(UrlEncode(pd)); }
	HttpRequest&  Post(const String& data)                { POST(); return PostData(data); }
	HttpRequest&  Post(const char *id, const String& data);

	HttpRequest&  Headers(const String& h)                { request_headers = h; return *this; }
	HttpRequest&  ClearHeaders()                          { return Headers(Null); }
	HttpRequest&  AddHeaders(const String& h)             { request_headers.Cat(h); return *this; }
	HttpRequest&  Header(const char *id, const String& data);

	HttpRequest&  StdHeaders(bool sh)                     { std_headers = sh; return *this; }
	HttpRequest&  NoStdHeaders()                          { return StdHeaders(false); }
	HttpRequest&  Accept(const String& a)                 { accept = a; return *this; }
	HttpRequest&  Agent(const String& a)                  { agent = a; return *this; }
	HttpRequest&  ContentType(const String& a)            { contenttype = a; return *this; }

	HttpRequest&  Proxy(const String& host, int port)     { proxy_host = host; proxy_port = port; return *this; }
	HttpRequest&  Proxy(const char *url);
	HttpRequest&  ProxyAuth(const String& u, const String& p) {  proxy_username = u; proxy_password = p; return *this; }

	bool         IsSocketError() const                    { return TcpSocket::IsError(); }
	bool         IsHttpError() const                      { return !IsNull(error) ; }
	bool         IsError() const                          { return IsSocketError() || IsHttpError(); }
	String       GetErrorDesc() const                     { return IsSocketError() ? TcpSocket::GetErrorDesc() : error; }
	void         ClearError()                             { TcpSocket::ClearError(); error.Clear(); }

	String       GetHeader(const char *s)                 { return header[s]; }
	String       operator[](const char *s)                { return GetHeader(s); }
	String       GetRedirectUrl();
	int          GetContentLength();
	int          GetStatusCode() const                    { return status_code; }
	String       GetResponsePhrase() const                { return response_phrase; }

	String       GetContent() const                       { return body; }
	String       operator~() const                        { return GetContent(); }
	operator String() const                               { return GetContent(); }
	void         ClearContent()                           { body.Clear(); }

	enum Phase {
		START, REQUEST, HEADER, BODY, CHUNK_HEADER, CHUNK_BODY, TRAILER, FINISHED, FAILED
	};

	bool    Do();
	int     GetPhase() const                      { return phase; }
	bool    InProgress() const                    { return phase != FAILED && phase != FINISHED; }
	bool    IsFailure() const                     { return phase == FAILED; }
	bool    IsSuccess() const                     { return phase == FINISHED && status_code >= 200 && status_code < 300; }

	String  Execute();
	
	HttpRequest();
	HttpRequest(const char *url);

	
	static void  Trace(bool b = true);
};
