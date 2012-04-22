#ifndef _smtp_smtp_h
#define _smtp_smtp_h

#include <Core/Core.h>

NAMESPACE_UPP

class Smtp : public TcpSocket {
	struct Attachment
	{
		String name; // mail name
		String file; // source path (dynamic attachments only)
		String mime; // content type (application/octet-stream by default)
		String data;
	};

	String         host;
	int            port; // default = 25
	bool           ssl;
	String         auth_user;
	String         auth_pwd;
	String         from;
	String         from_name;
	Vector<String> to;
	Vector<String> to_name;
	Vector<char>   as;
	Vector<String> body;
	Vector<String> mime; // default: text/plain; charset=<default application charset>
	Array<Attachment> attachments;
	int            request_timeout;

	bool           no_header; // default = false
	bool           no_header_sep; // default = false
	Time           time_sent;
	String         reply_to;
	String         reply_to_name;
	String         subject;

	int            start_time;
	String         error;

	void   CheckFail();
	void   Send(const String &s);
	String SendRecv(const String& s);
	void   SendRecvOK(const String& s);

public:
	enum AS { TO, CC, BCC };

	Smtp&      RequestTimeout(int ms)                             { request_timeout = ms; return *this; }
	Smtp&      Host(const String& h)                              { host = h; return *this; }
	Smtp&      Port(int p)                                        { port = p; return *this; }
	Smtp&      SSL(bool b = true)                                 { ssl = b; return *this; }
	Smtp&      Auth(const String& user, const String& pwd)        { auth_user = user; auth_pwd = pwd; return *this; }
	Smtp&      From(const String& email, const String& name = Null);
	Smtp&      To(const String& email, const String& name, AS a = TO);
	Smtp&      To(const String& email, AS a = TO)                     { return To(email, Null, a); }
	Smtp&      Cc(const String& email, const String& name = Null)     { return To(email, name, CC); }
	Smtp&      Bcc(const String& email, const String& name = Null)    { return To(email, name, BCC); }
	Smtp&      ReplyTo(const String& email, const String& name = Null);
	Smtp&      TimeSent(Time t)                                   { time_sent = t; return *this; }
	Smtp&      Subject(const String& s);
	Smtp&      Body(const String& s, const String& mime_ = Null)  { body.Add(s); mime.Add(mime_); return *this; }
	Smtp&      NoHeader()                                         { no_header = true; return *this; }
	Smtp&      NoHeaderSep()                                      { no_header_sep = true; return *this; }
	Smtp&      AttachFile(const char *filename, const char *mime = 0);
	Smtp&      Attach(const char *name, const String& data, const char *mime = 0);

	Smtp&      New();

	bool       Send();
	
	String     GetError() const                                   { return error; }

	Smtp();

	static void    Trace(bool b = true);

	static String  Encode(const String& text);
	static String  FormatAddr(const String& addr, const String& name);
};

END_UPP_NAMESPACE

#endif
