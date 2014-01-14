#include "Core.h"

#define LLOG(x) // DLOG(x)

NAMESPACE_UPP

bool WebSocket::WebAccept(TcpSocket& socket_, HttpHeader& hdr)
{
	socket = &socket_;
	String key = hdr["sec-websocket-key"];
	if(IsNull(key))
		return false;
	
	byte sha1[20];
	SHA1(sha1, key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
	
	return socket->PutAll(
		"HTTP/1.1 101 Switching Protocols\r\n"
		"Upgrade: websocket\r\n"
		"Connection: Upgrade\r\n"
		"Sec-WebSocket-Accept: " + Base64Encode((char *)sha1, 20) + "\r\n\r\n"
	);
}

bool WebSocket::WebAccept(TcpSocket& socket)
{
	HttpHeader hdr;
	if(!hdr.Read(socket))
		return false;
	return WebAccept(socket, hdr);
}

int64 WebSocket::ReadLen(int n)
{
	int64 len = 0;
	while(n-- > 0)
		len = (len << 8) | (byte)socket->Get();
	return len;
}

bool WebSocket::RecieveRaw()
{
	if(IsError())
		return false;

	opcode = socket->Get();
	if(opcode < 0)
		return false;
	int64 len = socket->Get();
	bool mask = len & 128;
	len &= 127;
	if(len == 127)
		len = ReadLen(8);
	if(len == 126)
		len = ReadLen(2);

	byte key[4];
	if(mask)
		socket->Get(key, 4);

	if(IsError()) {
		socket->SetSockError("websocket recieve", ERROR_DATA, "Invalid data");
		return false;
	}

	if(len > maxlen || len < 0) {
		socket->SetSockError("websocket recieve", ERROR_LEN_LIMIT, "Frame limit exceeded, size " + AsString(len));
		return false;
	}

	StringBuffer frame((int)len); // TODO int64
	char *buffer = ~frame;
	if(!socket->GetAll(buffer, (int)len)) {
		socket->SetSockError("websocket recieve", ERROR_DATA, "Invalid data");
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
	LLOG("WebSocket::Recieve");
	for(;;) {
		if(!RecieveRaw()) {
			LLOG("WebSocket::Recieve failed");
			return String::GetVoid();
		}
		if(GetOpCode() == PING)
			SendRaw(PONG, ~data, data.GetLength());
		else {
			if(GetOpCode() == CLOSE)
				SendRaw(CLOSE, ~data, data.GetLength());
			break;
		}
	}
	LLOG("WebSocket::Recieve len: " << data.GetLength());
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

	LLOG("WebSocket::SendRaw hdr: " << hdr << ", len: " << len);
	
	if(IsError() || !socket->PutAll(~b, b.GetLength()) || !socket->PutAll(data, (int)len)) {
		socket->SetSockError("websocket send", ERROR_SEND, "Failed to send data");
		LLOG("WebSocket::SendRaw FAILED");
		return false;
	}
	
	LLOG("WebSocket::SendRaw OK");
	
	return true;
}

void WebSocket::Reset()
{
	opcode = 0;
	data.Clear();
	maxlen = 10 * 1024 * 1024;
	socket = NULL;
}

void WebSocket::Close()
{
	if(socket) {
		socket->Close();
		opcode = 0;
		data.Clear();
		socket = NULL;
	}
}


END_UPP_NAMESPACE
