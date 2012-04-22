#include "Web.h"

NAMESPACE_UPP

#define LLOG(x)  // RLOG(x)

#define SLOWWRITE 4 // KB/s, comment out to turn off

HttpServerRequest::HttpServerRequest(HttpServer& server, pick_ Socket& _socket, HttpQuery query_)
: server(server), socket(_socket), query(query_)
, request_ticks(GetTickCount())
{
	socket.Linger(1000);
#ifdef PLATFORM_WIN32
	event.Read(socket);
#endif
	String auth = query.GetString("$$AUTHORIZATION");
	if(!IsNull(auth)) {
		String dec = UrlDecode(auth);
		if(dec.GetLength() >= 6 && !MemICmp(dec, "basic ", 6)) {
			const char *p = dec.GetIter(6);
			while(*p == ' ')
				p++;
			String b6d = Base64Decode(p);
			const char *un = b6d, *unp = un;
			while(*unp && *unp != ':')
				unp++;
			query.Set("$$AUTH_TYPE", "BASIC");
			query.Set("$$AUTH_USERNAME", String(un, unp));
			if(*unp == ':')
				unp++;
			query.Set("$$AUTH_PASSWORD", String(unp));
		}
		else if(dec.GetLength() >= 7 && !MemICmp(dec, "digest ", 7)) {
			query.Set("$$AUTH_TYPE", "DIGEST");
			const char *p = dec.GetIter(7);
			while(*p)
				if((byte)*p <= ' ' || *p == ',')
					p++;
				else {
					const char *b = p;
					while(*p && *p != '=')
						p++;
					const char *e = p;
					while(e > b && (byte)e[-1] <= ' ')
						e--;
					String key(b, e);
					String value;
					if(*p == '=') {
						p++;
						if(*p == '\"') {
							b = ++p;
							while(*p && *p != '\"')
								if(*p != '\\' || *++p)
									value.Cat(*p++);
							if(*p == '\"')
								p++;
						}
						else {
							while((byte)*p >= ' ' && *p != ',')
								value.Cat(*p++);
						}
					}
					query.Set("$$AUTH_" + key, value);
				}
		}
	}

	LLOG("HttpServerRequest:\n" << query);
}

void HttpServerRequest::LogTime(const char *s, int level)
{
	server.LogTime(NFormat("(ID:%d)%s", (int)(uintptr_t)this, s), level);
}

int HttpServerRequest::GetDuration() const
{
	return GetTickCount() - request_ticks;
}

void HttpServerRequest::Write(String header, String body)
{
	Write(header, body, 200, "OK");
}

void HttpServerRequest::Write(String header, String body, int result_code, String result_text)
{
	int duration = GetDuration();
	if(IsNull(header))
		header = query.GetString("$$DEFAULT_HEADER");

	LogTime(NFormat("HttpServerRequest::Write(%d): done in %d msecs: %d bytes\n%s",
		result_code, duration, body.GetLength(), header), 2);

	server.AddRequest(GetTickCount() - request_ticks);

	String out;
	int httpver = query.GetInt("$$HTTP_VERSION");
	if(httpver >= 1000) { // send status line & headers
		out << NFormat("HTTP/%d.%d %d %s\r\n", httpver / 1000, httpver % 1000, result_code, result_text)
		<< header
		<< "Content-Length: " << body.GetLength() << "\r\n"
		"\r\n";
	}
	if(query.GetString("$$METHOD") != "HEAD")
		out.Cat(body);
//	out.Cat('\0', 8192);
	ASSERT(socket.IsOpen());
	server.AddWrite(socket, out);
	LogTime(NFormat("HttpServerRequest::Write(%d): %d bytes added to delayed write list",
		result_code, out.GetLength()), 2);
}

void HttpServerRequest::Redirect(String url)
{
	Htmls body = NFormat(t_("If you're not redirected automatically please use %s."), HtmlLink(url) / t_("this link"));
	body = HtmlPage(t_("Redirection to another web address"), body);
	String header;
	header << "Location: " << url << "\r\n";
	Write(header, body, 302, "Object Moved");
/*
	String out;
	if(query.GetInt("$$HTTP_VERSION") >= 1000) { // send status line & headers
		out <<
		"HTTP/1.0 302 Object Moved\r\n"
		<< "Location: " << url << "\r\n"
		<< "Content-Length: " << body.GetLength() << "\r\n"
		"\r\n";
	}
	out.Cat(body);
	server.AddWrite(socket, out);
	LogTime(NFormat("HttpServerRequest::Redirect() added to delayed write list: %s", url), 2);
*/
}

void HttpServerRequest::Error(String err)
{
	LogTime(NFormat("error after %d msecs: %s", GetDuration(), err), 0);
	Htmls body = GetHttpErrorPage(query, err, server.IsShowQuery());
	Write(HttpContentType(HttpTextHtml()), body);
}

//////////////////////////////////////////////////////////////////////
// HttpServer::

HttpServer::SocketWrite::SocketWrite(Socket socket_, String data_, int ticks_)
: socket(socket_), data(data_), done(0), ticks(ticks_)
{
#ifdef PLATFORM_WIN32
	sock_event.Write(socket);
#endif
}

HttpServer::HttpServer()
{
	show_headers = true;
	server_port = 0;
	default_header = HttpContentType(HttpTextHtml());
	start_time = GetSysTime();
	hit_count = 0;
	total_response_msec = 0;
	trailing_sum = 0;
	trailing_count = 0;
	max_post_size = DEFAULT_MAX_POST_SIZE;
	max_request_time = DEFAULT_MAX_REQUEST_TIME;
}

void HttpServer::Logging(const char *log, int mls)
{
	max_log_size = mls;
	logfile = log;
	if(logfile.IsEmpty())
		logfile = ConfigFile("requests.txt");
}

void HttpServer::Log(const char *s, int level)
{
	enum { MAX_LOG_SIZE = 500000 };
	WhenLog(s, level);
	if(logfile.IsEmpty())
		return;
//	VppLog() << s;
	FileStream fs;
	if(!fs.Open(logfile, FileStream::APPEND) && !fs.Open(logfile, FileStream::CREATE))
		return;
	fs.Put(s);
	int len = (int)fs.GetSize();
	fs.Close();
	if(len < max_log_size)
		return;
	String bak = ForceExt(logfile, "") + ".old" + GetFileExt(logfile);
	FileDelete(bak);
	FileMove(logfile, bak);
/*
#if defined(PLATFORM_WIN32)
	DeleteFile(bak);
	MoveFile(logfile, bak);
#elif defined(PLATFORM_POSIX)
	unlink(bak);
	rename(logfile, bak);
#else
	#error Unsupported platform
#endif
*/
	if(fs.Open(logfile, FileStream::APPEND)) {
		fs << "[" << Format(GetSysTime()) << "] old '" << logfile << "' backed up (" << len << " bytes)\n";
		fs.Close();
	}
}

void HttpServer::LogTime(const char *s, int level)
{
	String str;
	Log(str << '[' << Format(GetSysTime()) << " @ " << int(msecs() % 10000u) << "]: " << s << '\n', level);
}

bool HttpServer::Open(int port, int listen_count)
{
	LogTime(NFormat("HttpServer::Open(port = %d)", port), 1);
	if(!ServerSocket(socket, server_port = port, true, listen_count))
		return false;
#ifdef PLATFORM_WIN32
	sock_event.Accept(socket);
#endif
	return true;
}

bool HttpServer::Reopen()
{
	Close();
	return Open(server_port);
}

void HttpServer::Close()
{
	LogTime("HttpServer::Close()", 1);
	socket.Close();
}

bool HttpServer::DelayedWrite()
{
	static const int MAX_SEND_CHUNK = 65536;
//	if(!delayed_writes.IsEmpty())
//		LogTime("HttpServer::DelayedWrite, #writes = " + FormatInt(delayed_writes.GetCount()), 2);
	for(int i = delayed_writes.GetCount(); --i >= 0;) {
		SocketWrite& sw = delayed_writes[i];
		if(IsNull(sw.data)) {
			sw.socket.ReadCount(1, 0);
			bool rm = sw.socket.IsEof();
			if(!rm && sw.socket.IsError()) {
				LogTime("HttpServer::DelayedWrite: shutdown error: " + sw.socket.GetErrorText(), 0);
				rm = true;
			}
			if(!rm && msecs(sw.ticks) >= max_request_time) {
				LogTime("HttpServer::DelayedWrite: timed out waiting for socket shutdown", 0);
				rm = true;
			}
			if(rm) {
				sw.socket.Close();
				delayed_writes.Remove(i);
			}
			continue;
		}
//		int part = sw.data.GetLength() - sw.done; Mirek:unused
		int count = max(sw.socket.WriteWait(sw.data.Begin() + sw.done,
			min(sw.data.GetLength() - sw.done, MAX_SEND_CHUNK), 0), 0);
		if(sw.socket.IsError()) {
			if(sw.socket.GetErrorCode() == SOCKERR(ENOBUFS)) {
				sw.socket.ClearError();
				continue;
			}
			LogTime(NFormat("HttpServer::DelayedWrite(): %s", Socket::GetErrorText()), 0);
			delayed_writes.Remove(i);
			continue;
		}
		if(count > 0)
			LogTime(NFormat("HttpServer::DelayedWrite(): %d bytes written on %d", count, sw.socket.GetNumber()), 2);
		if((sw.done += count) >= sw.data.GetLength()) {
			LogTime(NFormat("HttpServer::DelayedWrite(): finished %d (%d left)",
				sw.socket.GetNumber(), delayed_writes.GetCount() - 1), 2);
			sw.ticks = msecs();
			sw.data = Null;
			sw.socket.StopWrite();
		}
		else if(msecs(sw.ticks) >= max_request_time) {
			LogTime(NFormat("HttpServer::DelayedWrite(): timed out after sending %d out of %d bytes",
				sw.done, sw.data.GetLength()), 0);
//			sw.socket.StopWrite();
			sw.socket.Close();
			delayed_writes.Remove(i);
		}
	}
	return !delayed_writes.IsEmpty();
}

HttpServer *HttpServer::Wait(const Vector<HttpServer *>& list, int msec)
{
	Vector<Socket *> read, write;
	for(int i = 0; i < list.GetCount(); i++) {
		list[i]->GetReadSockets(read);
		list[i]->GetWriteSockets(write);
	}
	if(msec > 0) {
		LLOG(String() << "HttpServer::Wait(" << list.GetCount() << " servers for " << msec
			<< " msecs) -> Socket::Wait(#read = " << read.GetCount() << ", #write = " << write.GetCount() << ")");
		if(read.IsEmpty() && write.IsEmpty() || !Socket::Wait(read, write, msec))
			return NULL;
	}
	for(int i = 0; i < list.GetCount(); i++)
		if(list[i]->Accept())
			return list[i];
	return NULL;
}

bool HttpServer::Wait(int msec)
{
//	LogTime(NFormat("wait(%d msec)", msec), 2);
	Vector<HttpServer *> list;
	list.SetCount(1);
	list[0] = this;
	bool ok = !!Wait(list, msec);
//	LogTime(NFormat("//wait -> %d", ok), 2);
	return ok;
}

void HttpServer::GetReadSockets(Vector<Socket *>& sockets)
{
	if(socket.IsOpen())
		sockets.Add(&socket);
	if(connection.IsOpen())
		sockets.Add(&connection);
}

void HttpServer::GetWriteSockets(Vector<Socket *>& sockets)
{
	for(int i = 0; i < delayed_writes.GetCount(); i++)
		sockets.Add(&delayed_writes[i].socket);
}

#ifdef PLATFORM_WIN32
void HttpServer::GetWaitEvents(Vector<Event *>& events)
{
	if(socket.IsOpen())
		events.Add(&sock_event);
	if(connection.IsOpen())
		events.Add(&conn_event);
	for(int i = 0; i < delayed_writes.GetCount(); i++)
		events.Add(&delayed_writes[i].sock_event);
}
#endif

bool HttpServer::Accept()
{
	if(connection.IsOpen())
		return true;
	if(socket.Accept(connection, &ipaddr, true, 0)) {
		connection.NoBlock();
#ifdef PLATFORM_WIN32
		conn_event.Read(connection);
#endif
		first_line = Null;
		sapi_request = Null;
		post_data = Null;
		headers_length = 0;
		request_version = 0;
		post_length = 0;
		request_time = GetTickCount();
		request_state = RS_FIRST;
		request_query = HttpQuery();
		LogTime(NFormat("HttpServer::Accept: accepted socket %d from %d.%d.%d.%d", connection.GetNumber(),
				int(ipaddr >> 24) & 0xff, int(ipaddr >> 16) & 0xff, int(ipaddr >> 8) & 0xff, int(ipaddr >> 0) & 0xff), 2);
		return true;
	}
	if(socket.IsError()) {
		LogTime(NFormat("HttpServer::Accept: error %s", Socket::GetErrorText()), 0);
		return false;
	}
	return true;
}

#define FOURCHAR(a,b,c,d) ((int(a)) + (int(b) * 0x100) + (int(c) * 0x10000) + (int(d) * 0x1000000))

One<HttpServerRequest> HttpServer::GetRequest()
{
	Socket conn = connection;
	connection.Clear();

	if(!conn.IsOpen())
		return NULL;
	if(conn.IsError()) {
		LogTime(NFormat("HttpServer::GetRequest: error on socket %d: %s", conn.GetNumber(), Socket::GetErrorText()), 2);
		return NULL;
	}
	int rtm = GetTickCount() - request_time;
	if(rtm >= max_request_time) {
		LogTime("request time elapsed, request trashed", 1);
		return NULL;
	}
	String data = conn.Read(0, max_post_size);
	if(data.IsVoid()) {
		LogTime(NFormat("Socket %d closed by client.", conn.GetNumber()), 1);
		return NULL;
	}
	if(conn.IsError()) {
		LogTime(NFormat("Request transmission error, request trashed (after %d bytes)", data.GetLength()), 1);
		return NULL;
	}
	LogTime(NFormat("%d bytes read: %s", data.GetLength(), data), 2);
	const char *p = data.Begin(), *e = data.End();
	while(p < e) {
		const char *b = p;
		switch(request_state) {
		case RS_FIRST:
			if(first_line.GetLength() < 4) {
				int left = min<int>((int)(uintptr_t)(e - p), 4 - first_line.GetLength());
				first_line.Cat(p, left);
				p += left;
				break;
			} {
				int four = Peek32le(first_line);
				if(four == FOURCHAR('S', 'A', 'P', 'I')) {
					if(first_line.GetLength() < 8) {
						int left = min<int>((int)(uintptr_t)(e - p), 8 - first_line.GetLength());
						first_line.Cat(p, left);
						p += left;
						break;
					}
					sapi_length = Peek32le(first_line.GetIter(4));
					if(sapi_length < 0 || sapi_length > max_post_size) {
						LogTime(NFormat("Invalid SAPI request length (%d), request trashed", sapi_length), 1);
						conn.Close();
						return NULL;
					}
					LogTime("SAPI request detected", 2);
					request_state = RS_SAPI;
					break;
				}
				if(four == FOURCHAR('S', 'T', 'A', 'T')) {
					String out;
					String cmdline;
#ifdef PLATFORM_WIN32
					cmdline = GetCommandLine();
#endif
					out << "READY " << cmdline << "\r\n";
					conn.Write(out);
//					conn.StopWrite();
					conn.Close();
					return NULL;
				}
				if(four != FOURCHAR('P', 'O', 'S', 'T') && four != FOURCHAR('G', 'E', 'T', ' ')
				&& four != FOURCHAR('H', 'E', 'A', 'D')) {
					LogTime(NFormat("Invalid HTTP request type (%s), request trashed", first_line), 1);
					return NULL;
				}
				while(p < e && *p != '\n')
					p++;
				if(first_line.GetLength() + (int)(p - b) > max_post_size) {
					LogTime("HTTP request length limit reached, request trashed", 1);
					return NULL;
				}
				first_line.Cat(b, (int)(uintptr_t)(p - b));
				if(p >= e)
					break;
				p++;

				const char *r = first_line;
				while(*r && *r != ' ' && *r != '\n')
					r++;
				if(*r == ' ') {
					r++;
					const char *s = r;
					while(*r && *r != ' ' && *r != '?')
						r++;
					request_query.Set("$$PATH", UrlDecode(String(s, r)));
					const char *e = r, *t = r;
					while(e > s && e[-1] != '/')
						if(*--e == '.')
							t = e;
					request_query.Set("$$TITLE", UrlDecode(String(e, t)));
					if(e > s)
						e--;
					t = e;
					while(e > s && e[-1] != '/')
						e--;
					request_query.Set("$$DIR", UrlDecode(String(e, t)));
					if(s < r && *s == '/')
						s++;
					t = s;
					while(s < r && *s != '/')
						s++;
					if(s < r)
						request_query.Set("$$ROOT", UrlDecode(String(t, s)));
					if(*r == '?') {
						s = ++r;
						while(*r && *r != ' ' && *r != '\n')
							r++;
						String raw_query(s, r);
						request_query.Set("$$QUERY", raw_query);
						request_query.Set(HttpQuery(raw_query));
					}
					if(!memcmp(r, " HTTP/", 6) && IsDigit(r[6])) { // HTTP version
						int upper = stou(r + 6, &r);
						int lower = 0;
						if(*r == '.' && IsDigit(*++r))
							lower = stou(r, &r);
						request_version = 1000 * upper + lower;
						request_query.SetInt("$$HTTP_VERSION", request_version);
					}
				}
				request_query.Set("$$IPADDR", FormatIP(ipaddr));
				request_query.Set("$$DEFAULT_HEADER", default_header);
				request_query.Set("$$METHOD", four == FOURCHAR('P', 'O', 'S', 'T') ? "POST"
					: four == FOURCHAR('H', 'E', 'A', 'D') ? "HEAD" : "GET");
				if(request_version >= 1000)
					request_state = (four == FOURCHAR('P', 'O', 'S', 'T') ? RS_POST_HEADERS : RS_GET_HEADERS);
				else { // simple HTTP request without headers
					One<HttpServerRequest> req = new HttpServerRequest(*this, conn, request_query);
					req->LogTime(GetHttpURI(request_query), 1);
					return req;
				}
			}
			break;

		case RS_SAPI:
			if(sapi_request.GetLength() < sapi_length) {
				int add = min<int>((int)(uintptr_t)(e - p), sapi_length - sapi_request.GetLength());
				LogTime(NFormat("SAPI request length = %d, collected %d, adding %d",
					sapi_length, sapi_request.GetLength(), add), 2);
				sapi_request.Cat(p, add);
				p += add;
			}
			if(sapi_request.GetLength() >= sapi_length) {
				LogTime("Streaming SAPI request", 2);
				StringStream strm(sapi_request);
				strm % request_query;
				if(strm.IsError()) {
					LOG("Invalid SAPI request data, request trashed");
					return NULL;
				}
				request_query.Set("$$DEFAULT_HEADER", default_header);
				One<HttpServerRequest> req = new HttpServerRequest(*this, conn, request_query);
				req->LogTime(GetHttpURI(request_query), 1);
				return req;
			}
			break;

		case RS_GET_HEADERS:
		case RS_POST_HEADERS:
			while(p < e && *p != '\n')
				p++;
			if(headers_length + header_line.GetLength() + (int)(p - b) > max_post_size) {
				LogTime("Header line too long, request trashed", 1);
				return NULL;
			}
			header_line.Cat(b, (int)(uintptr_t)(p - b));
			LOG("b - b = " << (p - b) << ", header_line = " << header_line);
			if(p >= e)
				break;
			p++;

			{ // read header item
				const char *r = header_line;
				while(*r && (byte)*r <= ' ')
					r++;
				if(*r == 0) { // end of headers
					if(request_state == RS_POST_HEADERS) {
						post_length = request_query.GetInt("$$CONTENT_LENGTH");
						if(!IsNull(post_length) && post_length > max_post_size) {
							LogTime(NFormat("HTTP POST length limit exceeded (%d), request trashed", post_length), 1);
							return NULL;
						}
						request_state = RS_POST_DATA;
						break;
					}
					else {
						One<HttpServerRequest> req = new HttpServerRequest(*this, conn, request_query);
						req->LogTime(GetHttpURI(request_query), 1);
						return req;
					}
				}
				String var = "$$";
				for(; *r && *r != ':'; r++)
					if(IsAlNum(*r))
						var.Cat(ToUpper(*r));
					else if(*r == '-' && !var.IsEmpty() && *var.Last() != '_')
						var.Cat('_');
				if(var.GetLength() > 2 && *r == ':') {
					while(*++r == ' ')
						;
					const char *s = r;
					r = header_line.End();
					while(r > s && (byte)r[-1] <= ' ')
						r--;
					String v = request_query.GetString(var);
					if(!v.IsEmpty())
						v.Cat(", ");
					v.Cat(s, (int)(uintptr_t)(r - s));
					request_query.Set(var, v);
					headers_length += var.GetLength() + v.GetLength();
				}
				header_line = Null;
				break;
			}
			break;

		case RS_POST_DATA:
			if(post_length == 0) {
				while(p < e && *p != '\n')
					p++;
				if(post_data.GetLength() + (int)(p - b) > max_post_size) {
					LogTime("HTTP POST length exceeded, request trashed", 1);
					return NULL;
				}
				post_data.Cat(b, (int)(uintptr_t)(p - b));
				if(p >= e)
					break;
				p++;
			}
			else if(post_data.GetLength() < post_length) {
				int add = min<int>(post_length - post_data.GetLength(), (int)(uintptr_t)(e - p));
				post_data.Cat(p, add);
				p += add;
				if(post_data.GetLength() < post_length)
					break;
			}
			{
				String content = request_query.GetString("$$CONTENT_TYPE");
				static const char utag[] = "application/x-www-form-urlencoded";
				static const char mtag[] = "multipart/";
				request_query.Set("$$POSTDATA", post_data);
				if(!socket.IsError()) {
					if(!CompareNoCase(content, utag))
						request_query.SetURL(post_data);
					else if(!strnicmp(content, mtag, 10))
						GetHttpPostData(request_query, post_data);
				}
				One<HttpServerRequest> req = new HttpServerRequest(*this, conn, request_query);
				req->LogTime(GetHttpURI(request_query), 1);
				return req;
			}
			break;

		default:
			NEVER();
			break;
		}
	}

	connection = conn;
	return NULL;

/*
	bool post = false;
	HttpQuery query;
	LOG((int)GetTickCount() << " HttpServer::GetRequest->PeekCount(4) on socket " << (SOCKET)sconn);
	String request = sconn.PeekCount(4, 100);
	LOG((int)GetTickCount() << " HttpServer::GetRequest on socket " << (SOCKET)sconn);
	if(request.GetLength() == 4)
		if(!memcmp(request, "GET ", 4))
			post = false;
		else if(!memcmp(request, "POST", 4))
			post = true;
		else if(!memcmp(request, "SAPI", 4)) {
			request = sconn.ReadCount(8);
			int len = PeekIL(request.Begin() + 4);
			if(len < 0 || len > max_post_size)
				return NULL;
			StringStream strm(sconn.ReadCount(len));
			if(sconn.IsError()) {
				LogTime("HttpServer::ReadRequest: ISAPI request error: " + sconn.GetError());
				return NULL;
			}
			strm % query;
			if(strm.IsError())
				return NULL;
			query.Set(WID__DEFAULT_HEADER, default_header);
			One<HttpServerRequest> req = new HttpServerRequest(*this, sconn, query);
			req->LogTime(GetHttpURI(query));
			return req;
		}
		else if(!memcmp(request, "STAT", 4)) {
			String out;
			String cmdline;
#ifdef PLATFORM_WIN32
			cmdline = GetCommandLine();
#endif
			out << "READY " << cmdline << "\r\n";
			sconn.Write(out);
			sconn.StopWrite();
			sconn.Close();
			return NULL;
		}
		else
			return NULL;
	int http_version = 9;
	request = sconn.ReadUntil('\n');
	const char *p = request;
	while(*p && *p != ' ' && *p != '\n')
		p++;
	if(*p == ' ') {
		p++;
		const char *b = p;
		while(*p && *p != ' ' && *p != '?')
			p++;
		query.Set(WID__PATH, String(b, p));
		if(*p == '?') {
			b = ++p;
			while(*p && *p != ' ' && *p != '\n')
				p++;
			String raw_query(b, p);
			query.Set(WID__QUERY, raw_query);
			query.Set(HttpQuery(raw_query));
		}
		if(!memcmp(p, " HTTP/", 6) && IsDigit(p[6])) { // HTTP version
			int upper = stou(p + 6, &p);
			int lower = 0;
			if(*p == '.' && IsDigit(*++p))
				lower = stou(p, &p);
			http_version = 1000 * upper + lower;
			query.SetInt(WID__HTTP_VERSION, http_version);
		}
	}
	query.Set(WID__DEFAULT_HEADER, default_header);
	query.Set(WID__METHOD, post ? "POST" : "GET");
	if(post && http_version < 1000)
		return NULL; // must be at least HTTP/1.0 to handle POST
	if(http_version >= 1000) { // read headers
		while(!sconn.IsError()) {
			request = sconn.ReadUntil('\n');
			p = request;
			while(*p && (byte)*p <= ' ')
				p++;
			if(*p == 0)
				break;
			String var = "$$";
			for(; *p && *p != ':'; p++)
				if(IsAlNum(*p))
					var.Cat(ToUpper(*p));
				else if(*p == '-' && !var.IsEmpty() && *var.Last() != '_')
					var.Cat('_');
			if(var.GetLength() > 2 && *p == ':') {
				while(*++p == ' ')
					;
				const char *b = p;
				p = request.End();
				while((byte)p[-1] <= ' ')
					p--;
				String s = query(var);
				if(!s.IsEmpty())
					s.Cat(", ");
				s.Cat(b, p - b);
				query.Set(var, s);
			}
		}
	}

	if(post)
		ReadPostData(sconn, query);
	if(sconn.IsError()) {
		LogTime(String() << "HttpServer::GetRequest: " << sconn.GetError());
		return NULL;
	}
	One<HttpServerRequest> req = new HttpServerRequest(*this, sconn, query);
	req->LogTime(GetHttpURI(query));
	return req;
*/
}

void HttpServer::ReadPostData(Socket& socket, HttpQuery& query)
{
	int length = query.GetInt("$$CONTENT_LENGTH");
	String buffer;
	if(length > 0)
		buffer = socket.ReadCount(min(length, max_post_size));
	else // length unknown, read just 1 line (works for www-form-urlencoded)
		buffer = socket.ReadUntil('\n', Null, max_post_size);
	String content = query.GetString("$$CONTENT_TYPE");
	static const String mtag = "multipart/";
	if(!socket.IsError() && strnicmp(content, mtag, mtag.GetLength())) { // assume single-part data
		query.SetURL(buffer);
		return;
	}
	GetHttpPostData(query, buffer);
}

void HttpServer::AddWrite(Socket socket, String data)
{
	if(data.GetLength() > 0)
		delayed_writes.Add(new SocketWrite(socket, data, msecs()));
}

bool HttpServer::IsDelayedWrite() const
{
	return !delayed_writes.IsEmpty();
}

double HttpServer::GetElapsedTime() const
{
	return (double)((GetSysTime() - start_time) * 1000);
}

double HttpServer::GetAvgTime() const
{
	return hit_count ? total_response_msec / hit_count : 0;
}

double HttpServer::GetAvgLagTime() const
{
	if(trailing_count == 0)
		return 0;
	return (double)trailing_sum / trailing_count;
}

void HttpServer::AddRequest(int response_msec)
{
	hit_count++;
	total_response_msec += response_msec;
	if(trailing_count >= TRAILER) {
		trailing_sum -= trailing_times[0];
		Copy(trailing_times, trailing_times + 1, trailing_times + TRAILER);
//		memmove(&trailing_times[0], &trailing_times[1], (TRAILER - 1) * sizeof(*trailing_times));
		trailing_count--;
	}
	trailing_sum += (trailing_times[trailing_count++] = response_msec);
}

String GetHttpURI(HttpQuery query)
{
	String out = query.GetString("$$PATH");
	if(!query.IsEmpty("$$QUERY"))
		out << '?' << query.GetString("$$QUERY");
	return out;
}

void GetHttpPostData(HttpQuery& query, String buffer)
{
	query.Set("$$POSTDATA", buffer);

	const char *p = buffer;
	while(p[0] != '-' || p[1] != '-') {
		while(*p != '\n')
			if(*p++ == 0)
				return; // end of file, boundary not found
		p++;
	}
	String delimiter;
 { // read multipart delimiter
		const char *b = (p += 2);
		while(*p && *p++ != '\n')
			;
		const char *e = p;
		while(e > b && (byte)e[-1] <= ' ')
			e--;
		delimiter = String(b, e);
	}
//	DUMP(delimiter);

	int delta = 4 + delimiter.GetLength();
	const char *e = buffer.End();
	if(e - p < delta)
		return;
	e -= delta;
	while(p < e) { // read individual parts
		String filename, content_type, name;
		while(!MemICmp(p, "content-", 8)) { // parse content specifiers
			p += 8;
			if(!MemICmp(p, "disposition:", 12)) {
				p += 12;
				while(*p && *p != '\n')
					if((byte)*p <= ' ')
						p++;
					else { // fetch key-value pair
						const char *kp = p;
						while(*p && *p != '\n' && *p != '=' && *p != ';')
							p++;
						const char *ke = p;
						String value;
						if(*p == '=') {
							const char *b = ++p;
							if(*p == '\"') { // quoted value
								b++;
								while(*++p && *p != '\n' && *p != '\"')
									;
								value = String(b, p);
								if(*p == '\"')
									p++;
							}
							else {
								while(*p && *p != '\n' && *p != ';')
									p++;
								value = String(b, p);
							}
						}
						if(ke - kp == 4 && !MemICmp(kp, "name", 4))
							name = value;
						else if(ke - kp == 8 && !MemICmp(kp, "filename", 8))
							filename = value;
						if(*p == ';')
							p++;
					}
			}
			else if(!MemICmp(p, "type:", 5)) {
				p += 5;
				while(*p && *p != '\n' && (byte)*p <= ' ')
					p++;
				const char *b = p;
				while(*p && *p != '\n')
					p++;
				const char *e = p;
				while(e > b && (byte)e[-1] <= ' ')
					e--;
				content_type = String(b, e);
			}
				;
			while(*p && *p++ != '\n')
				;
		}
		if(*p++ != '\r' || *p++ != '\n')
			return;
		const char *b = p;
		while(p < e) {
			p = (const char *)memchr(p, '\r', e - p);
			if(!p)
				return;
			if(p[0] == '\r' && p[1] == '\n' && p[2] == '-' && p[3] == '-'
				&& !memcmp(p + 4, delimiter, delimiter.GetLength()))
				break;
			p++;
		}
//		DUMP(name);
//		DUMP(content_type);
//		DUMP(filename);
//		DUMP(String(b, p));
		if(!name.IsEmpty()) { // add variables
			if(!filename.IsEmpty())
				query.Set("$$FILENAME$$" + name, filename);
			if(!content_type.IsEmpty())
				query.Set("$$CONTENT_TYPE$$" + name, content_type);
			query.Set(name, String(b, p));
		}
		p += delta;
		while(*p && *p++ != '\n')
			;
	}
}

static HtmlTag HtmlSmallFont()
{
	return HtmlFontSize(-2);
}

String GetHttpQueryDump(HttpQuery query)
{
	Htmls table;
	for(int i = 0; i < query.GetCount(); i++) {
		Htmls row;
		row << HtmlCell().VAlign(ALIGN_TOP) / HtmlSmallFont() / HtmlBold() / (ToHtml(query.GetKey(i)) + "&nbsp;") << '\n';
		String value = query.GetValue(i);
		if(value.GetLength() >= 1000) {
			value.Trim(1000);
			value << NFormat(t_("... (total length: %d bytes)"), value.GetLength());
		}
		row << HtmlCell().VAlign(ALIGN_TOP) / HtmlSmallFont() / ToHtml(value);
		table << HtmlRow() % row;
	}
	return HtmlTable().Border(0).CellSpacing(0).CellPadding(0) % table;
}

String GetHttpErrorPage(HttpQuery query, String err, bool show_query)
{
	Htmls body;
	body << t_("The web server is unable to satisfy your request:\n<p>\n");
	String request = query.GetString("$$PATH");
	if(!query.IsEmpty("$$QUERY"))
		request << '?' << query.GetString("$$QUERY");
	body << HtmlTag("TT") / ToHtml(request)
	<< "\n<p>\n"
	<< HtmlBold() / t_("Reason:") << err
	<< "\n<p>\n";
	if(show_query)
		body
		<< HtmlSmallFont() / HtmlBold() / t_("Detailed query data:")
		<< "\n<br>\n"
		<< GetHttpQueryDump(query);

	return HtmlTitlePage(t_("Web server error"), body);
}

END_UPP_NAMESPACE
