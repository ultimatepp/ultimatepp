#ifndef _POP3_POP3_h
#define _POP3_POP3_h

#include <Core/Core.h>

namespace Upp {

class Pop3 : public TcpSocket 
{
	String       proxy_host;
	int          proxy_port;
	String       proxy_username;
	String       proxy_password;

	String       host;
	String       user;
	String       pass;
	String       error;
	String       data;
	int          port;
	bool         ssl;
	bool         online;

	bool         GetListItems(ValueMap& list, dword type1, dword type2);
	String       GetTimeStamp();
	bool         Authenticate();
	bool         PutGet(const String& s, bool multiline = false, bool nolog = false);

public:
	Pop3&        Host(const String& h)                  { host = h; return *this; }
	Pop3&        Port(int p)                            { port = p; return *this; }
	Pop3&        User(const String& u, const String& p) { user = u; pass = p; return *this; }
	Pop3&        SSL(bool b = true)                     { ssl  = b; return *this; }
	
	Pop3&        Proxy(const String& host, int port)         { proxy_host = host; proxy_port = port; return *this; }
	Pop3&        Proxy(const char *p);
	Pop3&        ProxyAuth(const String& u, const String& p) { proxy_username = u; proxy_password = p; return *this; }

	int          GetMessageCount();
	String       GetMessage(int index);
	String       GetMessageHeader(int index);
	bool         RemoveMessage(int index);

	bool         GetMessageList(ValueMap& list);
	String       GetMessageUniqueId(int index);
	bool         GetMessageUniqueIds(ValueMap& uids);

	bool         Undo();
	bool         Noop();
	
	bool         Login();
	bool         Logout();
	
	bool         IsOnline() const                         { return online; }

	String       GetLastError()                           { return IsError() ? GetErrorDesc() : error; }
	static void  Trace(bool b = true);

	Pop3();
	~Pop3();
};

struct InetMessage {
	struct Part : Moveable<Part> {
		int                       parent;
		VectorMap<String, String> header;
		String                    body;
		
		String operator[](const char *id) const          { return header.Get(id, Null); }
		void   Set(const char *id, const String& value)  { header.GetAdd(id) = value; }
		String Decode() const;
		
		bool   IsMultipart(String& boundary, String& end_boundary) const;
	};

	Vector<Part> part;

	bool   Read(const String& msg);
	bool   ReadHeader(const String& msg);
	
	void   Clear()                                       { part.Clear(); }

	int         GetCount() const                         { return part.GetCount(); }
	const Part& operator[](int i) const                  { return part[i]; }
	String      operator[](const char *id) const         { return GetCount() ? part[0][id] : String(); }
	void        Set(const char *id, const String& value) { return part.At(0).Set(id, value); }
	
	String GetMessage() const;

private:
	bool   ReadHeader(VectorMap<String, String>& hdr, Stream& ss);
	bool   ReadPart(Stream& ss, int parent, int level);
	void   PutBody(int pi, String& r, int level) const;
};

struct MIMEHeader {
	String value;
	VectorMap<String, String> param;
	
	String operator[](const char *id) const { return param.Get(id, Null); }
	String operator~() const                { return value; }
	
	bool   Parse(const char *s);
	String ToString() const;
	
	MIMEHeader(const char *s)               { Parse(s); }
	MIMEHeader()                            {}
};

Index<String> ParseMessageIDs(const String& s);
String        FormatMessageIDs(const Index<String>& id);

}

#endif
