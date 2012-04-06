#include "Core.h"

NAMESPACE_UPP

bool HttpRequest_Trace__;

#define LLOG(x)      do { if(HttpRequest_Trace__) RLOG(x); } while(0)
	
#ifdef _DEBUG
_DBG_
// #define ENDZIP
#endif

void HttpRequest::Trace(bool b)
{
	HttpRequest_Trace__ = b;
}

void HttpRequest::Init()
{
	port = 0;
	proxy_port = 0;
	max_header_size = 1000000;
	max_content_size = 10000000;
	max_redirects = 5;
	max_retries = 3;
	force_digest = false;
	std_headers = true;
	hasurlvar = false;
	method = METHOD_GET;
	phase = START;
	redirect_count = 0;
	retry_count = 0;
	gzip = false;
	WhenContent = callback(this, &HttpRequest::ContentOut);
	chunk = 4096;
	timeout = 120000;
}

HttpRequest::HttpRequest()
{
	Init();
}

HttpRequest::HttpRequest(const char *url)
{
	Init();
	Url(url);
}

HttpRequest& HttpRequest::Url(const char *u)
{
	const char *t = u;
	while(*t && *t != '?')
		if(*t++ == '/' && *t == '/') {
			u = ++t;
			break;
		}
	t = u;
	while(*u && *u != ':' && *u != '/' && *u != '?')
		u++;
	if(*u == '?' && u[1])
		hasurlvar = true;
	host = String(t, u);
	port = 0;
	if(*u == ':')
		port = ScanInt(u + 1, &u);
	path = u;
	int q = path.Find('#');
	if(q >= 0)
		path.Trim(q);
	return *this;
}

HttpRequest& HttpRequest::Proxy(const char *p)
{
	const char *t = p;
	while(*p && *p != ':')
		p++;
	proxy_host = String(t, p);
	proxy_port = 80;
	if(*p++ == ':' && IsDigit(*p))
		proxy_port = ScanInt(p);
	return *this;
}

HttpRequest& HttpRequest::Post(const char *id, const String& data)
{
	POST();
	if(postdata.GetCount())
		postdata << '&';
	postdata << id << '=' << UrlEncode(data);
	return *this;
}

HttpRequest& HttpRequest::UrlVar(const char *id, const String& data)
{
	int c = *path.Last();
	if(hasurlvar && c != '&')
		path << '&';
	if(!hasurlvar && c != '?')
		path << '?';
	path << id << '=' << UrlEncode(data);
	hasurlvar = true;
	return *this;
}

String HttpRequest::CalculateDigest(const String& authenticate) const
{
	const char *p = authenticate;
	String realm, qop, nonce, opaque;
	while(*p) {
		if(!IsAlNum(*p)) {
			p++;
			continue;
		}
		else {
			const char *b = p;
			while(IsAlNum(*p))
				p++;
			String var = ToLower(String(b, p));
			String value;
			while(*p && (byte)*p <= ' ')
				p++;
			if(*p == '=') {
				p++;
				while(*p && (byte)*p <= ' ')
					p++;
				if(*p == '\"') {
					p++;
					while(*p && *p != '\"')
						if(*p != '\\' || *++p)
							value.Cat(*p++);
					if(*p == '\"')
						p++;
				}
				else {
					b = p;
					while(*p && *p != ',' && (byte)*p > ' ')
						p++;
					value = String(b, p);
				}
			}
			if(var == "realm")
				realm = value;
			else if(var == "qop")
				qop = value;
			else if(var == "nonce")
				nonce = value;
			else if(var == "opaque")
				opaque = value;
		}
	}
	String hv1, hv2;
	hv1 << username << ':' << realm << ':' << password;
	String ha1 = MD5String(hv1);
	hv2 << (method == METHOD_GET ? "GET" : method == METHOD_PUT ? "PUT" : method == METHOD_POST ? "POST" : "READ")
	<< ':' << path;
	String ha2 = MD5String(hv2);
	int nc = 1;
	String cnonce = FormatIntHex(Random(), 8);
	String hv;
	hv << ha1
	   << ':' << nonce
	   << ':' << FormatIntHex(nc, 8)
	   << ':' << cnonce
	   << ':' << qop << ':' << ha2;
	String ha = MD5String(hv);
	String auth;
	auth << "username=" << AsCString(username)
	     << ", realm=" << AsCString(realm)
	     << ", nonce=" << AsCString(nonce)
	     << ", uri=" << AsCString(path)
	     << ", qop=" << AsCString(qop)
	     << ", nc=" << AsCString(FormatIntHex(nc, 8))
	     << ", cnonce=" << cnonce
	     << ", response=" << AsCString(ha);
	if(!IsNull(opaque))
		auth << ", opaque=" << AsCString(opaque);
	return auth;
}

HttpRequest& HttpRequest::Header(const char *id, const String& data)
{
	request_headers << id << ": " << data << "\r\n";
	return *this;
}

void HttpRequest::HttpError(const char *s)
{
	if(IsError())
		return;
	error = NFormat(t_("%s:%d: ") + String(s), host, port);
	LLOG("HTTP ERROR: " << error);
	Close();
}

void HttpRequest::StartPhase(int s)
{
	LLOG("Starting status " << s << ' ' << host);
	phase = s;
	data.Clear();
}

bool HttpRequest::Do()
{
	int c1, c2;
	switch(phase) {
	case START:
		retry_count = 0;
		redirect_count = 0;
		start_time = msecs();
		StartRequest();
		break;
	case REQUEST:
		if(SendingData())
			break;
		StartPhase(HEADER);
		break;
	case HEADER:
		if(ReadingHeader())
			break;
		StartBody();
		break;
	case BODY:
		if(ReadingBody())
			break;
		Finish();
		break;
	case CHUNK_HEADER:
		ReadingChunkHeader();
		break;
	case CHUNK_BODY:
		if(ReadingBody())
			break;
		c1 = Get();
		c2 = Get();
		if(c1 != '\r' || c2 != '\n')
			HttpError("missing ending CRLF in chunked transfer");
		StartPhase(CHUNK_HEADER);
		break;
	case TRAILER:
		if(ReadingHeader())
			break;
		header.Parse(data);
		Finish();
		break;
	case FINISHED:
	case FAILED:
		return false;
	default:
		NEVER();
	}

	if(phase != FAILED)
		if(IsSocketError() || IsError())
			phase = FAILED;
		else
		if(msecs() - start_time >= timeout) {
			HttpError("connection timed out");
			phase = FAILED;
		}
		else
		if(IsAbort()) {
			HttpError("connection was aborted");
			phase = FAILED;
		}
	
	if(phase == FAILED) {
		if(retry_count++ < max_retries) {
			LLOG("HTTP retry on error " << GetErrorDesc());
			StartRequest();
		}
	}
	return phase != FINISHED && phase != FAILED;
}

void HttpRequest::Finish()
{
	if(gzip) {
	#ifdef ENDZIP
		body = GZDecompress(body);
		if(body.IsVoid()) {
			HttpError("gzip decompress at finish error");
			phase = FAILED;
			return;
		}
	#else
		z.End();
		if(z.IsError()) {
			HttpError("gzip format error (finish)");
			phase = FAILED;
			return;
		}
	#endif
	}
	Close();
	if(status_code == 401 && !IsNull(username)) {
		String authenticate = header["www-authenticate"];
		if(authenticate.GetCount() && redirect_count++ < max_redirects) {
			LLOG("HTTP auth digest");
			Digest(CalculateDigest(authenticate));
			StartRequest();
			return;
		}
	}
	if(status_code >= 300 && status_code < 400) {
		String url = GetRedirectUrl();
		if(url.GetCount() && redirect_count++ < max_redirects) {
			LLOG("HTTP redirect " << url);
			Url(url);
			StartRequest();
			retry_count = 0;
			return;
		}
	}
	phase = FINISHED;

//	if(retry_count < 2)
//		HttpError("Checking retry");
}

void HttpRequest::ReadingChunkHeader()
{
	for(;;) {
		int c = Get();
		if(c < 0)
			break;
		else
		if(c == '\n') {
			int n = ScanInt(~data, NULL, 16);
			LLOG("HTTP Chunk header: 0x" << data << " = " << n);
			if(IsNull(n)) {
				HttpError("invalid chunk header");
				break;
			}
			if(n == 0) {
				StartPhase(TRAILER);
				break;
			}
			count += n;
			StartPhase(CHUNK_BODY);
			break;
		}
		if(c != '\r')
			data.Cat(c);
	}
}

String HttpRequest::GetRedirectUrl()
{
	String redirect_url = TrimLeft(header["location"]);
	int q = redirect_url.Find('?');
	int p = path.Find('?');
	if(p >= 0 && q < 0)
		redirect_url.Cat(path.Mid(p));
	return redirect_url;
}

int   HttpRequest::GetContentLength()
{
	return Nvl(ScanInt(header["content-length"]), -1);
}

void HttpRequest::StartBody()
{
	LLOG("HTTP Header received: ");
	LLOG(data);
	header.Clear();
	if(!header.Parse(data)) {
		HttpError("invalid HTTP header");
		return;
	}
	
	if(!header.Response(protocol, status_code, reason_phrase)) {
		HttpError("invalid HTTP response");
		return;
	}
	
	LLOG("HTTP status code: " << status_code);

	count = GetContentLength();
	
	if(count > 0)
		body.Reserve(count);

	if(method == METHOD_HEAD)
		phase = FINISHED;
	else
	if(header["transfer-encoding"] == "chunked") {
		count = 0;
		StartPhase(CHUNK_HEADER);
	}
	else
		StartPhase(BODY);
	body.Clear();
	bodylen = 0;
	gzip = GetHeader("content-encoding") == "gzip";
	if(gzip) {
		gzip = true;
		z.WhenOut = callback(this, &HttpRequest::Out);
		z.ChunkSize(chunk).GZip().Decompress();
	}
}

void HttpRequest::ContentOut(const void *ptr, dword size)
{
	body.Cat((const char *)ptr, size);
}

void HttpRequest::Out(const void *ptr, dword size)
{
	LLOG("HTTP Out " << size);
	if(z.IsError())
		HttpError("gzip format error");
	int64 l = bodylen + size;
	if(l > max_content_size) {
		HttpError("content length exceeded " + AsString(max_content_size));
		phase = FAILED;
		return;
	}
	WhenContent(ptr, size);
	bodylen += size;
}

bool HttpRequest::ReadingBody()
{
	LLOG("HTTP reading data " << count);
	int n = chunk;
	if(count >= 0)
		n = min(n, count);
	String s = Get(n);
	if(s.GetCount() == 0)
		return !IsEof() && count;
#ifndef ENDZIP
	if(gzip)
		z.Put(~s, s.GetCount());
	else
#endif
		Out(~s, s.GetCount());
	if(count >= 0) {
		count -= s.GetCount();
		return !IsEof() && count > 0;
	}
	return !IsEof();
}

void HttpRequest::StartRequest()
{
	Close();
	ClearError();
	gzip = false;
	z.Clear();

	bool use_proxy = !IsNull(proxy_host);

	int p = use_proxy ? proxy_port : port;
	if(!Connect(use_proxy ? proxy_host : host, p ? p : DEFAULT_HTTP_PORT))
		return;

	StartPhase(REQUEST);
	count = 0;
	String ctype = contenttype;
	if((method == METHOD_POST || method == METHOD_PUT) && IsNull(ctype))
		ctype = "application/x-www-form-urlencoded";
	switch(method) {
	case METHOD_GET:  data << "GET "; break;
	case METHOD_POST: data << "POST "; break;
	case METHOD_PUT: data << "PUT "; break;
	case METHOD_HEAD: data << "HEAD "; break;
	default: NEVER(); // invalid method
	}
	String host_port = host;
	if(port)
		host_port << ':' << port;
	String url;
	url << "http://" << host_port << Nvl(path, "/");
	if(use_proxy)
		data << url;
	else
		data << Nvl(path, "/");
	data << " HTTP/1.1\r\n";
	if(std_headers) {
		data << "URL: " << url << "\r\n"
		     << "Host: " << host_port << "\r\n"
		     << "Connection: close\r\n"
		     << "Accept: " << Nvl(accept, "*/*") << "\r\n"
		     << "Accept-Encoding: gzip\r\n"
		     << "Agent: " << Nvl(agent, "Ultimate++ HTTP client") << "\r\n";
		if(postdata.GetCount())
			data << "Content-Length: " << postdata.GetCount() << "\r\n";
		if(ctype.GetCount())
			data << "Content-Type: " << ctype << "\r\n";
	}
	if(use_proxy && !IsNull(proxy_username))
		 data << "Proxy-Authorization: Basic " << Base64Encode(proxy_username + ':' + proxy_password) << "\r\n";
	if(!IsNull(digest))
		data << "Authorization: Digest " << digest << "\r\n";
	else
	if(!force_digest && (!IsNull(username) || !IsNull(password)))
		data << "Authorization: Basic " << Base64Encode(username + ":" + password) << "\r\n";
	data << request_headers << "\r\n" << postdata; // !!! POST PHASE !!!
	LLOG("HTTP REQUEST " << host << ":" << port);
	LLOG("HTTP request:\n" << data);
}

bool HttpRequest::SendingData()
{
	for(;;) {
		int n = min(2048, data.GetLength() - count);
		n = Put(~data + count, n);
		if(n == 0)
			break;
		count += n;
	}
	return count < data.GetLength();
}

bool HttpRequest::ReadingHeader()
{
	for(;;) {
		int c = Get();
		if(c < 0)
			return !IsEof();
		else
			data.Cat(c);
		if(data.GetCount() > 3) {
			const char *h = data.Last();
			if(h[0] == '\n' && (h[-1] == '\r' && h[-2] == '\n' || h[-1] == '\n'))
				return false;
		}
		if(data.GetCount() > max_header_size) {
			HttpError("HTTP header exceeded " + AsString(max_header_size));
			return true;
		}
	}
}

String HttpRequest::Execute()
{
	while(Do());
	return IsSuccess() ? GetContent() : String::GetVoid();
}

END_UPP_NAMESPACE
