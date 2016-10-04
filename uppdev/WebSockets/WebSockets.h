#ifndef _WebSockets_WebSockets_h_
#define _WebSockets_WebSockets_h_

#include <Core/Core.h>

using namespace Upp;

class WebSocket : public TcpSocket {
	int64 ReadLen(int n);

	int    opcode;
	String data;
	int64  maxlen;

	bool Handshake();

public:
	enum {
		ERROR_NOHEADER = TcpSocket::ERROR_LAST, ERROR_NOKEY, ERROR_DATA, ERROR_SEND, ERROR_LEN_LIMIT
	};
	enum {
		FIN = 0x80,
		CONTINUE = 0x0,
		TEXT = 0x1,
		BINARY = 0x2,
		CLOSE = 0x8,
		PING = 0x9,
		PONG = 0xa,
	};

	bool   WebAccept(TcpSocket& server);

	bool   RecieveRaw();
	String Recieve();
	
	bool   IsFin()           { return opcode & FIN; }
	int    GetOpCode() const { return opcode & 15; }
	bool   IsText() const    { return GetOpCode() == TEXT; }
	bool   IsBinary() const  { return GetOpCode() == BINARY; }
	bool   IsClosed() const  { return GetOpCode() == CLOSE; }
	String GetData() const   { return data; }

	bool   SendRaw(int hdr, const void *data, int64 len);

	bool   SendText(const void *data, int64 len, bool fin = true)   { return SendRaw((fin ? 0x80 : 0)|TEXT, data, len); }
	bool   SendText(const String& data, bool fin = true)            { return SendText(~data, data.GetCount(), fin); }

	bool   SendBinary(const void *data, int64 len, bool fin = true) { return SendRaw((fin ? 0x80 : 0)|BINARY, data, len); }
	bool   SendBinary(const String& data, bool fin = true)          { return SendBinary(~data, data.GetCount(), fin); }

	void   Reset();
	
	WebSocket& MaxLen(int64 maxlen_)                                { maxlen = maxlen_; return *this; }
	
	WebSocket()                                                     { Reset(); }
};

#endif
