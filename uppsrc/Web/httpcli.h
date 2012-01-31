#ifndef _Web_httpcli_h_
#define _Web_httpcli_h_

class HttpClient
{
public:
	HttpClient();
	HttpClient(const char *url);
	virtual ~HttpClient();

	HttpClient&  TimeoutMsecs(int t)              { timeout_msecs = t; return *this; }
	HttpClient&  MaxHeaderSize(int m)             { max_header_size = m; return *this; }
	HttpClient&  MaxContentSize(int m)            { max_content_size = m; return *this; }

	HttpClient&  Host(String h)                   { host = h; return *this; }
	HttpClient&  Port(int p)                      { port = p; return *this; }
	HttpClient&  Path(String p)                   { path = p; return *this; }
	HttpClient&  User(String u, String p)         { username = u; password = p; return *this; }
	HttpClient&  Digest()                         { force_digest = true; return *this; }
	HttpClient&  Digest(String d)                 { digest = d; return *this; }
	HttpClient&  URL(const char *url);
	HttpClient&  Url(const char *id, const String& data);
	HttpClient&  KeepAlive(bool k)                { keepalive = k; return *this; }
	HttpClient&  Proxy(String host, int port)     { proxy_host = host; proxy_port = port; return *this; }
	HttpClient&  Proxy(const char *url);
	HttpClient&  ProxyAuth(String usr, String pwd){  proxy_username = usr; proxy_password = pwd; return *this; }

	HttpClient&  Headers(String h)                { client_headers = h; return *this; }
	HttpClient&  ClearHeaders()                   { return Headers(Null); }
	HttpClient&  AddHeaders(String h)             { client_headers.Cat(h); return *this; }
	HttpClient&  Header(const char *id, const String& data);

	HttpClient&  StdHeaders(bool sh)              { std_headers = sh; return *this; }
	HttpClient&  NoStdHeaders()                   { return StdHeaders(false); }
	HttpClient&  Accept(String a)                 { accept = a; return *this; }
	HttpClient&  Agent(String a)                  { agent = a; return *this; }
	HttpClient&  ContentType(String a)            { contenttype = a; return *this; }

	HttpClient&  Method(int m)                    { method = m; return *this; }
	HttpClient&  Get()                            { return Method(METHOD_GET); }
	HttpClient&  Post()                           { return Method(METHOD_POST); }
	HttpClient&  Head()                           { return Method(METHOD_HEAD); }
	HttpClient&  Put()                            { return Method(METHOD_PUT); }

	HttpClient&  PostData(String pd)              { postdata = pd; return *this; }
	HttpClient&  PostUData(String pd)             { return PostData(UrlEncode(pd)); }
	HttpClient&  Post(const String& data)         { Post(); return PostData(data); }
	HttpClient&  Post(const char *id, const String& data);

	HttpClient&  UrlVar(const char *id, const String& data);
	HttpClient&  operator()(const char *id, const String& data) { return UrlVar(id, data); }

	String       Execute(Gate2<int, int> progress = false);
	String       ExecuteRedirect(int max_redirect = DEFAULT_MAX_REDIRECT,
		int retries = DEFAULT_RETRIES, Gate2<int, int> progress = false);

	bool         IsError() const                  { return !IsNull(error); }
	String       GetError() const                 { return error; }
	bool         IsAborted() const                { return aborted; }

	int          GetStatusCode() const            { return status_code; }
	String       GetStatusLine() const            { return status_line; }
	String       GetHeaders() const               { return server_headers; }
	String       GetBody() const                  { return body; }

	bool         IsRedirect() const               { return is_redirect; }
	String       GetRedirectURL() const           { return redirect_url; }

	String       CalculateDigest(String authenticate) const;

	void         Close()                          { socket.Close(); }

	static void  Trace(bool b = true);

	
	virtual bool CreateClientSocket();
	virtual bool IsSecure();

public:
	Socket       socket;
	bool         keepalive;
	bool         aborted;
	bool         force_digest;
	String       error;
	String       body;

	int          timeout_msecs;
	int          max_header_size;
	int          max_content_size;

	String       host;
	int          port;
	int          method;
	String       proxy_host;
	int          proxy_port;
	String       proxy_username;
	String       proxy_password;
	String       path;
	String       username;
	String       password;
	String       digest;
	String       client_headers;
	String       accept;
	String       agent;
	String       contenttype;
	String       postdata;

	bool         is_post;
	bool         std_headers;
	bool         hasurlvar;
	bool         is_redirect;

	int          status_code;
	String       status_line;
	String       server_headers;

	String       redirect_url;
	String       authenticate;
	
	String       socket_host;
	int          socket_port;

	enum {
		DEFAULT_HTTP_PORT        = 80,
		DEFAULT_TIMEOUT_MSECS    = 120000,
		DEFAULT_MAX_HEADER_SIZE  = 1000000,
		DEFAULT_MAX_CONTENT_SIZE = 10000000,
		DEFAULT_MAX_REDIRECT     = 5,
		DEFAULT_RETRIES          = 3,
	};

	enum {
		METHOD_GET,
		METHOD_POST,
		METHOD_HEAD,
		METHOD_PUT,
	};

private:
	void         Init();

protected:
	bool         use_proxy;
	String       ReadUntilProgress(char until, int start_time, int end_time, Gate2<int, int> progress);
};

String HttpClientGet(String url, String *server_headers = NULL, String *error = NULL,
	Gate2<int, int> progress = false, int timeout = HttpClient::DEFAULT_TIMEOUT_MSECS,
	int max_redirect = HttpClient::DEFAULT_MAX_REDIRECT, int retries = HttpClient::DEFAULT_RETRIES);

String HttpClientGet(String url, String username, String password,
	String *server_headers = NULL, String *error = NULL,
	Gate2<int, int> progress = false, int timeout = HttpClient::DEFAULT_TIMEOUT_MSECS,
	int max_redirect = HttpClient::DEFAULT_MAX_REDIRECT, int retries = HttpClient::DEFAULT_RETRIES);

String HttpClientGet(String url, String proxy,
	String *server_headers = NULL, String *error = NULL,
	Gate2<int, int> progress = false, int timeout = HttpClient::DEFAULT_TIMEOUT_MSECS,
	int max_redirect = HttpClient::DEFAULT_MAX_REDIRECT, int retries = HttpClient::DEFAULT_RETRIES);

String HttpClientGet(String url, String proxy, String username, String password,
	String *server_headers = NULL, String *error = NULL,
	Gate2<int, int> progress = false, int timeout = HttpClient::DEFAULT_TIMEOUT_MSECS,
	int max_redirect = HttpClient::DEFAULT_MAX_REDIRECT, int retries = HttpClient::DEFAULT_RETRIES);

#endif
