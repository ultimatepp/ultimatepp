inline String HttpContentType(const String& type)           { return String().Cat() << "Content-Type: " << type << "\r\n"; }
inline String HttpContentDisposition(const char *disp, const String& filename)
{ return String().Cat() << "Content-Disposition: " << disp << "; filename=\"" << filename << "\"\r\n"; }
inline String HttpContentInline(const String& fn)           { return HttpContentDisposition("inline", fn); }
inline String HttpContentAttachment(const String& fn)       { return HttpContentDisposition("attachment", fn); }

inline String HttpTextHtml(const String& encoding)          { return "text/html; charset=" + encoding; }
inline String HttpTextHtml(byte charset = CHARSET_DEFAULT)  { return HttpTextHtml(MIMECharsetName(charset)); }
//inline String HttpTextHtml1250()                          { return "text/html; charset=windows-1250"; }
inline String HttpTextPlain(const String& encoding)         { return "text/plain; charset=" + encoding; }
//inline String HttpTextPlain1250()                         { return "text/plain; charset=windows-1250"; }
inline String HttpTextPlain(byte charset = CHARSET_DEFAULT) { return HttpTextPlain(MIMECharsetName(charset)); }
inline String HttpTextXml()                                 { return "text/xml"; }
inline String HttpImage(const String& format)               { return "image/" + format; }
inline String HttpImageGif()                                { return "image/gif"; }
inline String HttpImageJpg()                                { return "image/jpeg"; }
inline String HttpImagePng()                                { return "image/png"; }
inline String HttpBinary()                                  { return "application/octet-stream"; }

class HttpServer;

class HttpServerRequest
{
	friend class HttpServer;

public:
	HttpServerRequest(HttpServer& server, pick_ Socket& socket, HttpQuery query);

	HttpServer&   GetServer()                              { return server; }
	Socket&       GetSocket()                              { return socket; }
#ifdef PLATFORM_WIN32
	Event&        GetEvent()                               { return event; }
#endif
	HttpQuery     GetQuery() const                         { return query; }

	bool          IsLogging() const;
	void          LogTime(const char *s, int level);
	int           GetDuration() const;

	void          Write(String header, String body, int result_code, String result_text);
	void          Write(String header, String body);
	void          Write(String body)                       { Write(Null, body); }
	void          Redirect(String url);
	void          Error(String err);

private:
	HttpServer&   server;
	Socket        socket;
#ifdef PLATFORM_WIN32
	SocketEvent   event;
#endif
	HttpQuery     query;
	dword         request_ticks;
};

class HttpServer
{
public:
	HttpServer();

	bool               Open(int port, int listen_count = 5);
	bool               Reopen();
	void               Close();

	bool               IsOpen() const                 { return socket.IsOpen(); }
	bool               IsError() const                { return socket.IsError(); }
	int                GetPort() const                { return server_port; }
	Socket&            GetSocket()                    { return socket; }
	Socket&            GetConnSocket()                { return connection; }
#ifdef PLATFORM_WIN32
	Event&             GetEvent()                     { return sock_event; }
	Event&             GetConnEvent()                 { return conn_event; }
#endif

	void               Logging(const char *log = NULL, int max_log_size = 500000);
	void               NoLogging()                    { logfile = Null; }
	bool               IsLogging() const              { return !logfile.IsEmpty(); }
	void               Log(const char *s, int level);
	void               LogTime(const char *s, int level);
	HttpServer&        operator << (const char *s)    { Log(s, 0); return *this; }

	static HttpServer *Wait(const Vector<HttpServer *>& list, int msec);
	bool               Wait(int msec);
	void               GetReadSockets(Vector<Socket *>& sockets);
	void               GetWriteSockets(Vector<Socket *>& sockets);
#ifdef PLATFORM_WIN32
	void               GetWaitEvents(Vector<Event *>& events);
#endif
	One<HttpServerRequest>   GetRequest();

	Time               GetStartTime() const           { return start_time; }
	double             GetHitCount() const            { return hit_count; }
	double             GetResponseTime() const        { return total_response_msec; }
	double             GetAvgTime() const;
	double             GetAvgLagTime() const;
	double             GetElapsedTime() const; // msec

	String             GetDefaultHeader() const       { return default_header; }
	void               DefaultHeader(String h)        { default_header = h; }

	int                GetMaxPostSize() const         { return max_post_size; }
	void               MaxPostSize(int size)          { max_post_size = size; }

	int                GetMaxRequestTime() const      { return max_request_time; }
	void               MaxRequestTime(int time)       { max_request_time = time; }

	bool               IsShowQuery() const            { return show_headers; }
	void               ShowQuery(bool sh = true)      { show_headers = sh; }

	void               AddRequest(int duration_msec);
	void               ReadPostData(Socket& socket, HttpQuery& query);

	void               AddWrite(Socket socket, String data);
	bool               DelayedWrite();
	bool               IsDelayedWrite() const;

public:
	Callback2<String, int> WhenLog;

private:
	bool               Accept();

private:
	Socket             socket;
	Socket             connection;
#ifdef PLATFORM_WIN32
	SocketEvent        sock_event;
	SocketEvent        conn_event;
#endif
	class SocketWrite
	{
	public:
		SocketWrite(Socket socket, String data, int ticks = 0);

	public:
		Socket      socket;
#ifdef PLATFORM_WIN32
		SocketEvent sock_event;
#endif
		String      data;
		int         done;
		int         ticks;
	};
	Array<SocketWrite> delayed_writes;

	int                server_port;
	dword              ipaddr;
	String             default_header;
	String             logfile;
	int                max_log_size;
	enum { DEFAULT_MAX_POST_SIZE = 1000000 };
	int                max_post_size;
	enum { DEFAULT_MAX_REQUEST_TIME = 60000 };
	int                max_request_time;
	bool               show_headers;

	int                request_time;
	int                request_state;
	enum { RS_FIRST, RS_SAPI, RS_GET_HEADERS, RS_POST_HEADERS, RS_POST_DATA };
	String             first_line;
	String             sapi_request;
	int                sapi_length;
	int                request_version;
	String             header_line;
	int                headers_length;
	int                post_length;
	String             post_data;
	HttpQuery          request_query;

	Time               start_time;
	double             hit_count;
	double             total_response_msec;

	enum { TRAILER = 10 };
	dword              trailing_times[TRAILER];
	dword              trailing_sum;
	int                trailing_count;
};

inline bool   HttpServerRequest::IsLogging() const { return server.IsLogging(); }

inline String GetHttpPath(HttpQuery query)  { return query.GetString("$$PATH"); }
inline String GetHttpQuery(HttpQuery query) { return query.GetString("$$QUERY"); }
String        GetHttpURI(HttpQuery query);
void          GetHttpPostData(HttpQuery& query, String data);
String        GetHttpQueryDump(HttpQuery query);
String        GetHttpErrorPage(HttpQuery query, String err, bool show_query);
