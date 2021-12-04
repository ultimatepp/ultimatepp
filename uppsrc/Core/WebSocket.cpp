#include "Core.h"

namespace Upp {
	
static bool sTrace;

#define LLOG(x)  if(sTrace) RLOG((client ? "WS CLIENT " : "WS SERVER ") << x)

void WebSocket::Trace(bool b)
{
	sTrace = b;
}

String WebSocket::FormatBlock(const String& s)
{
	return AsCString(s.GetCount() < 500 ? s : s.Mid(0, 500), INT_MAX, NULL, ASCSTRING_OCTALHI);
}

WebSocket::WebSocket()
{
	Clear();

	static String request_headers_const =
	    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
		"Accept-Language: cs,en-US;q=0.7,en;q=0.3\r\n"
		"Sec-WebSocket-Version: 13\r\n"
		"Sec-WebSocket-Extensions: permessage-deflate\r\n"
		"Connection: keep-alive, Upgrade\r\n"
		"Pragma: no-cache\r\n"
		"Cache-Control: no-cache\r\n"
		"Upgrade: websocket\r\n";
    request_headers = request_headers_const;
}

void WebSocket::Clear()
{
	socket = &std_socket;
	opcode = 0;
	current_opcode = 0;
	data.Clear();
	data_pos = 0;
	in_queue.Clear();
	out_queue.Clear();
	out_at = 0;
	error.Clear();
	socket->Clear();
	close_sent = close_received = false;
	client = false;
}

void WebSocket::Error(const String& err)
{
	LLOG("ERROR: " << err);
	error = err;
}

bool WebSocket::Accept(TcpSocket& listen_socket)
{
	Clear();
	if(!socket->Accept(listen_socket)) {
		Error("Accept has failed");
		return false;
	}
	opcode = HTTP_REQUEST_HEADER;
	return true;
}

WebSocket& WebSocket::Header(const char *id, const String& data)
{
	request_headers << id << ": " << data << "\r\n";
	return *this;
}

bool WebSocket::Connect(const String& url)
{
	const char *u = url;
	bool ssl = memcmp(u, "wss", 3) == 0;
	const char *t = u;
	while(*t && *t != '?')
		if(*t++ == '/' && *t == '/') {
			u = ++t;
			break;
		}
	t = u;
	while(*u && *u != ':' && *u != '/' && *u != '?')
		u++;
	String host = String(t, u);
	int port = ssl ? 443 : 80;
	if(*u == ':')
		port = ScanInt(u + 1, &u);
	
	return Connect(url.StartsWith("wss:") ? "https:" + url.Mid(4)
	               : url.StartsWith("ws:") ? "http:" + url.Mid(3) : url,
	               host, ssl, port);
}

bool WebSocket::Connect(const String& uri_, const String& host_, bool ssl_, int port)
{
	Clear();
	
	client = true;
	redirect = 0;
	
	uri = uri_;
	host = host_;
	ssl = ssl_;
	
	if(socket->IsBlocking()) {
		if(!addrinfo.Execute(host, port)) {
			Error("Not found");
			return false;
		}
		LLOG("DNS resolved");
		StartConnect();
		while(opcode != READING_FRAME_HEADER) {
			Do0();
			if(IsError())
				return false;
		}
	}
	else {
		opcode = DNS;
		addrinfo.Start(host, port);
	}
	return true;
}

void WebSocket::SendRequest()
{
	LLOG("Sending connection request");
	String h;
	for(int i = 0; i < 16; i++)
		h.Cat(Random());
	Out( // needs to be the first thing to sent after the connection is established
	    "GET " + uri + " HTTP/1.1\r\n"
	    "Host: " + host + "\r\n" +
		"Sec-WebSocket-Key: " + Base64Encode(h) + "\r\n" +
	    request_headers +
	    "\r\n"
	);
	opcode = HTTP_RESPONSE_HEADER;
}

void WebSocket::StartConnect()
{
	if(!socket->Connect(addrinfo)) {
		Error("Connect has failed");
		return;
	}
	
	LLOG("Connect issued");
	
	if(IsBlocking()) {
		if(ssl) {
			socket->StartSSL();
			socket->SSLHandshake();
			LLOG("Blocking SSL handshake finished");
		}
		SendRequest();
		return;
	}
	
	if(ssl) {
		if(!socket->StartSSL()) {
			Error("Unable to start SSL handshake");
			return;
		}
		LLOG("Started SSL handshake");
		opcode = SSL_HANDSHAKE;
	}
	else
		SendRequest();
}

void WebSocket::Dns()
{
	if(addrinfo.InProgress())
		return;
	if(!addrinfo.GetResult()) {
		Error("DNS lookup failed");
		return;
	}
	LLOG("DNS resolved");
	StartConnect();
}

void WebSocket::SSLHandshake()
{
	if(socket->SSLHandshake())
		return;
	LLOG("SSL handshake finished");
	SendRequest();
}

bool WebSocket::ReadHttpHeader()
{
	for(;;) {
		int c = socket->Get();
		if(c < 0)
			return false;
		else
			data.Cat(c);
		if(data.GetCount() == 2 && data[0] == '\r' && data[1] == '\n') { // header is empty
			Error("Empty HTTP header");
			return false;
		}
		if(data.GetCount() >= 3) {
			const char *h = data.Last();
			if(h[0] == '\n' && h[-1] == '\r' && h[-2] == '\n') { // empty ending line after non-empty header
				LLOG("HTTP header received: " << FormatBlock(data));
				return true;
			}
		}
		if(data.GetCount() > 100000) {
			Error("HTTP header size exceeded");
			return false;
		}
	}
}

void WebSocket::RequestHeader()
{
	if(ReadHttpHeader()) {
		HttpHeader hdr;
		if(!hdr.Parse(data)) {
			Error("Invalid HTTP header");
			return;
		}
		String dummy;
		hdr.Request(dummy, uri, dummy);
		String key = hdr["sec-websocket-key"];
		if(IsNull(key)) {
			Error("Invalid HTTP header: missing sec-websocket-key");
			return;
		}
	
		byte sha1[20];
		SHA1(sha1, key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
		
		Out(
			"HTTP/1.1 101 Switching Protocols\r\n"
			"Upgrade: websocket\r\n"
			"Connection: Upgrade\r\n"
			"Sec-WebSocket-Accept: " + Base64Encode((char *)sha1, 20) + "\r\n\r\n"
		);
		
		LLOG("HTTP request header received, sending response");
		data.Clear();
		opcode = READING_FRAME_HEADER;
	}
}

void WebSocket::ResponseHeader()
{
	if(ReadHttpHeader()) {
		HttpHeader h;
		h.Parse(data);
		int code = h.GetCode();
		if(code >= 300 && code < 400) {
			int r = redirect + 1;
			if(r++ < 5) {
				Connect(h["location"]);
				redirect = r;
				return;
			}
		}
		if(ToLower(h["upgrade"]) != "websocket") {
			Error("Invalid server response HTTP header");
			return;
		}
		LLOG("HTTP response header received");
		opcode = READING_FRAME_HEADER;
		data.Clear();
	}
}

void WebSocket::FrameHeader()
{
	for(;;) {
		int c = socket->Get();
		if(c < 0)
			return;
		data.Cat(c);
		
		LLOG("Receiving frame header, current header len: " << data.GetCount());

		int ii = 0;
		bool ok = true;
		auto Get = [&]() -> byte {
			if(ii < data.GetCount())
				return data[ii++];
			ok = false;
			return 0;
		};
		auto GetLen = [&](int count) -> int64 {
			int64 len = 0;
			while(count--)
				len = (len << 8) | Get();
			return len;
		};
		int new_opcode = Get();
		length = Get();
		mask = length & 128;
		length &= 127;
		if(length == 127)
			length = GetLen(8);
		if(length == 126)
			length = GetLen(2);
		if(mask) {
			key[0] = Get();
			key[1] = Get();
			key[2] = Get();
			key[3] = Get();
		}

		if(ok) {
			LLOG("Frame header received, len: " << length << ", code " << new_opcode);
			opcode = new_opcode;
			data.Clear();
			data_pos = 0;
			return;
		}
	}
}

void WebSocket::Close(const String& msg, bool wait_reply)
{
	LLOG("Sending CLOSE");
	SendRaw(CLOSE|FIN, msg, MASK);
	close_sent = true;
	if(IsBlocking())
		while((wait_reply ? IsOpen() : out_queue.GetCount()) && !IsError() && socket->IsOpen())
			Do0();
}

void WebSocket::FrameData()
{
	Buffer<char> buffer(32768);
	while(data_pos < length) {
		int n = socket->Get(~buffer, (int)min(length - data_pos, (int64)32768));
		if(n == 0)
			return;
		if(mask)
			for(int i = 0; i < n; i++) // TODO: Optimize
				buffer[i] ^= key[(i + data_pos) & 3];
		data.Cat(~buffer, n); // TODO: Split long data
		data_pos += n;
		LLOG("Frame data chunk received, chunk len: " << n);
	}
	LLOG("Frame data received");
	int op = opcode & 15;
	switch(op) {
	case PING:
		LLOG("PING");
		SendRaw(PONG|FIN, data, MASK);
		break;
	case CLOSE:
		LLOG("CLOSE received");
		close_received = true;
		if(!close_sent)
			Close(data);
		socket->Close();
		break;
	default:
		Input& m = in_queue.AddTail();
		m.opcode = opcode;
		m.data = data;
		LLOG((m.opcode & TEXT ? "TEXT: " : "BINARY: ") << FormatBlock(data));
		LLOG("Input queue count is now " << in_queue.GetCount());
		break;
	}
	data.Clear();
	opcode = READING_FRAME_HEADER;
}

void WebSocket::Do0()
{
	int prev_opcode;
	do {
		prev_opcode = opcode;
		if(findarg(opcode, DNS, SSL_HANDSHAKE) < 0) {
			Output();
			if(socket->IsEof() && !(close_sent || close_received))
				Error("Socket has been closed unexpectedly");
		}
		if(IsError())
			return;
		switch(opcode) {
		case DNS:
			Dns();
			break;
		case SSL_HANDSHAKE:
			SSLHandshake();
			break;
		case HTTP_RESPONSE_HEADER:
			ResponseHeader();
			break;
		case HTTP_REQUEST_HEADER:
			RequestHeader();
			break;
		case READING_FRAME_HEADER:
			FrameHeader();
			break;
		default:
			FrameData();
			break;
		}
	}
	while(!IsBlocking() && opcode != prev_opcode);
}

void WebSocket::Do()
{
	ASSERT(!IsBlocking());
	Do0();
}

String WebSocket::Receive()
{
	current_opcode = 0;
	do {
		Do0();
		if(in_queue.GetCount()) {
			String s = in_queue.Head().data;
			current_opcode = in_queue.Head().opcode;
			in_queue.DropHead();
			return s;
		}
	}
	while(IsBlocking() && socket->IsOpen() && !IsError());
	return String::GetVoid();
}

void WebSocket::Out(const String& s)
{
	out_queue.AddTail(s);
	while((IsBlocking() || close_sent) && socket->IsOpen() && !IsError() && out_queue.GetCount())
		Output();
}

void WebSocket::Output()
{
	if(socket->IsOpen()) {
		while(out_queue.GetCount()) {
			const String& s = out_queue.Head();
			int n = socket->Put(~s + out_at, s.GetCount() - out_at);
			if(n == 0)
				break;
			LLOG("Sent " << n << " bytes: " << FormatBlock(s));
			out_at += n;
			if(out_at >= s.GetCount()) {
				out_at = 0;
				out_queue.DropHead();
				LLOG("Block sent complete, " << out_queue.GetCount() << " remaining blocks in queue");
			}
		}
	}
}

void WebSocket::SendRaw(int hdr, const String& data, dword mask)
{
	if(IsError())
		return;
	
	ASSERT(!close_sent);
	LLOG("Send " << data.GetCount() << " bytes, hdr: " << hdr);
	
	String header;
	header.Cat(hdr);
	int len = data.GetCount();
	if(len > 65535) {
		header.Cat(127 | mask);
		header.Cat(0);
		header.Cat(0);
		header.Cat(0);
		header.Cat(0);
		header.Cat(byte(len >> 24));
		header.Cat(byte(len >> 16));
		header.Cat(byte(len >> 8));
		header.Cat(byte(len));
	}
	else
	if(len > 125) {
		header.Cat(126 | mask);
		header.Cat(byte(len >> 8));
		header.Cat(byte(len));
	}
	else
		header.Cat((int)len | mask);

	if(mask) {
		byte Cle[4];
		for(int i = 0; i < 4; i++)
			header.Cat(Cle[i] = (byte)Random());

		Out(header);

		if(data.GetCount()) {
			StringBuffer buf(data.GetCount());
			int n = data.GetCount();
			for(int i = 0; i < n; i++)
				buf[i] = data[i] ^ Cle[i & 3];
			Out(buf);
		}
	}
	else {
		Out(header);
	
		if(data.GetCount() == 0)
			return;
		Out(data);
	}
}

bool WebSocket::WebAccept(TcpSocket& socket_, HttpHeader& hdr)
{
	socket = &socket_;
	String key = hdr["sec-websocket-key"];
	if(IsNull(key))
		return false;
	
	byte sha1[20];
	SHA1(sha1, key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
	
	Out(
		"HTTP/1.1 101 Switching Protocols\r\n"
		"Upgrade: websocket\r\n"
		"Connection: Upgrade\r\n"
		"Sec-WebSocket-Accept: " + Base64Encode((char *)sha1, 20) + "\r\n\r\n"
	);

	LLOG("HTTP request header received, sending response");
	data.Clear();
	opcode = READING_FRAME_HEADER;
	return true;
}

bool WebSocket::WebAccept(TcpSocket& socket)
{
	HttpHeader hdr;
	if(!hdr.Read(socket))
		return false;
	return WebAccept(socket, hdr);
}

}
