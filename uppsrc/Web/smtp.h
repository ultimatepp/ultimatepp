class SmtpMail
{
public:
	enum AS { TO, CC, BCC };

	SmtpMail();

	SmtpMail&      Host(const String& h)                              { host = h; return *this; }
	SmtpMail&      Port(int p)                                        { port = p; return *this; }
	SmtpMail&      From(const String& from, const String& name = Null);
	SmtpMail&      To(const String& t, const String& name, AS a = TO);
	SmtpMail&      To(const String& t, AS a = TO)                     { return To(t, Null, a); }
	SmtpMail&      Cc(const String& t, const String& name = Null)     { return To(t, name, CC); }
	SmtpMail&      Bcc(const String& t, const String& name = Null)    { return To(t, name, BCC); }
	SmtpMail&      ReplyTo(const String& r, const String& name = Null);
	SmtpMail&      Text(const String& t, const String& mime_ = Null)  { text.Add(t); mime.Add(mime_); return *this; }
	SmtpMail&      NoHeader()                                         { no_header = true; return *this; }
	SmtpMail&      NoHeaderSep()                                      { no_header_sep = true; return *this; }
	SmtpMail&      TimeSent(Time t)                                   { time_sent = t; return *this; }
	SmtpMail&      Subject(const String& s);
	SmtpMail&      Transcript(bool t = true)                          { transcript = t; return *this; }
	SmtpMail&      AttachFile(const char *filename, const char *mime = 0);
	SmtpMail&      Attach(const char *name, const String& data, const char *mime = 0);
	SmtpMail&      Auth(const String& user, const String& pwd)        { auth_user = user; auth_pwd = pwd; return *this; }
	SmtpMail&      AddHeader(const String& text)                      { add_header << text << "\r\n"; return *this; }
	SmtpMail&      AddHeader(const char *id, const String& txt)       { add_header << id << ": " << txt << "\r\n"; return *this; }

	SmtpMail&      New();

	bool           Send();
	
	String         GetError() const                                   { return error; }
	String         GetTranscript() const                              { return transcript_text; }

	static String  Encode(const String& text);
	static String  FormatAddr(const String& addr, const String& name);

private:
	struct Attachment
	{
		String name; // mail name
		String file; // source path (dynamic attachments only)
		String mime; // content type (application/octet-stream by default)
		String data;
	};

	String         host;
	int            port; // default = 25
	String         auth_user;
	String         auth_pwd;
	String         from;
	String         from_name;
	Vector<String> to;
	Vector<String> to_name;
	Vector<char>   as;
	Vector<String> text;
	Vector<String> mime; // default: text/plain; charset=<default application charset>
	bool           transcript; // default = false
	Array<Attachment> attachments;

	// header-related parts
	bool           no_header; // default = false
	bool           no_header_sep; // default = false
	Time           time_sent;
	String         reply_to;
	String         reply_to_name;
	String         subject;
	String         add_header;

	// state automaton
	String         error;
	String         transcript_text;
};
