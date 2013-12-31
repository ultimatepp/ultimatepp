#include "WebSockets.h"

bool WebSocket::Handshake()
{
	HttpHeader hdr;
	if(!hdr.Read(*this)) {
		SetSockError("websocket handshake", ERROR_NOHEADER, "Failed to read HTTP header");
		return false;
	}
	String key = hdr["sec-websocket-key"];
	if(IsNull(key)) {
		SetSockError("websocket handshake", ERROR_NOKEY, "Missing sec-websocket-key");
		return false;
	}
	
	byte sha1[20];
	SHA1(sha1, key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
	
	DLOG(
		"HTTP/1.1 101 Switching Protocols\r\n"
		"Upgrade: websocket\r\n"
		"Connection: Upgrade\r\n"
		"Sec-WebSocket-Accept: " + Base64Encode((char *)sha1, 20) + "\r\n\r\n"
	);
	
	return PutAll(
		"HTTP/1.1 101 Switching Protocols\r\n"
		"Upgrade: websocket\r\n"
		"Connection: Upgrade\r\n"
		"Sec-WebSocket-Accept: " + Base64Encode((char *)sha1, 20) + "\r\n\r\n"
	);
}

int64 WebSocket::ReadLen(int n)
{
	int64 len = 0;
	while(n-- > 0)
		len = (len << 8) | (byte)Get();
	return len;
}

bool WebSocket::RecieveRaw()
{
	if(IsError())
		return false;

	opcode = Get();
	int64 len = Get();
	bool mask = len & 128;
	len &= 127;
	if(len == 127)
		len = ReadLen(8);
	if(len == 126)
		len = ReadLen(2);

	byte key[4];
	if(mask)
		Get(key, 4);

	if(IsError()) {
		SetSockError("websocket recieve", ERROR_DATA, "Invalid data");
		return false;
	}

	if(len > maxlen) {
		SetSockError("websocket recieve", ERROR_LEN_LIMIT, "Frame limit exceeded, size " + AsString(len));
		return false;
	}

	StringBuffer frame((int)len); // TODO int64
	char *buffer = ~frame;
	if(!GetAll(buffer, (int)len)) {
		SetSockError("websocket recieve", ERROR_DATA, "Invalid data");
		return false;
	}
	
	if(mask)
		for(int i = 0; i < len; i++)
			buffer[i] ^= key[i & 3];

	data = frame;
	return true;
}

String WebSocket::Recieve()
{
	for(;;) {
		if(!RecieveRaw())
			return String::GetVoid();
		if(GetOpCode() == PING)
			SendRaw(PONG, ~data, data.GetLength());
		else
		if(GetOpCode() == CLOSE)
			SendRaw(CLOSE, ~data, data.GetLength());
		else
			break;
	}
	return data;
}

bool WebSocket::SendRaw(int hdr, const void *data, int64 len)
{
	if(IsError())
		return false;
	
	ASSERT(len < INT_MAX); // temporary, todo
	String b;
	b.Cat(hdr);
	if(len > 65535) {
		b.Cat(127);
		b.Cat(byte(len >> 56));
		b.Cat(byte(len >> 48));
		b.Cat(byte(len >> 40));
		b.Cat(byte(len >> 32));
		b.Cat(byte(len >> 24));
		b.Cat(byte(len >> 16));
		b.Cat(byte(len >> 8));
		b.Cat(byte(len));
	}
	else
	if(len > 125) {
		b.Cat(126);
		b.Cat(byte(len >> 8));
		b.Cat(byte(len));
	}
	else
		b.Cat((int)len);
	
	if(IsError() || !PutAll(~b, b.GetLength()) || !PutAll(data, (int)len)) {
		SetSockError("websocket send", ERROR_SEND, "Failed to send data");
		return false;
	}
	
	return true;
}

void WebSocket::Reset()
{
	opcode = 0;
	data.Clear();
	maxlen = 10 * 1024 * 1024;
}

