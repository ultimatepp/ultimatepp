class SmtpMail
{
public:
	enum AS { TO, CC, BCC };

	SmtpMail();

	SmtpMail&      Host(String h)                 { host = h; return *this; }
	SmtpMail&      Port(int p)                    { port = p; return *this; }
	SmtpMail&      From(String f)                 { from = f; return *this; }
	SmtpMail&      To(String t, AS a = TO)        { to.Add(t); as.Add(a); return *this; }
	SmtpMail&      Text(String t, String m = Null) { text.Add(t); mime.Add(m); return *this; }
	SmtpMail&      NoHeader()                     { no_header = true; return *this; }
	SmtpMail&      NoHeaderSep()                  { no_header_sep = true; return *this; }
	SmtpMail&      ReplyTo(String r)              { reply_to = r; return *this; }
	SmtpMail&      TimeSent(Time t)               { time_sent = t; return *this; }
	SmtpMail&      Subject(String s)              { subject = s; return *this; }
	SmtpMail&      Transcript(bool t = true)      { transcript = t; return *this; }
	SmtpMail&      AttachFile(const char *filename, const char *mime = 0);
	SmtpMail&      Attach(const char *name, String data, const char *mime = 0);
	SmtpMail&      Auth(String user, String pwd)  { auth_user = user; auth_pwd = pwd; return *this; }

	bool           Send();

	String         GetError() const          { return error; }
	String         GetTranscript() const     { return transcript_text; }

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
	Vector<String> to;
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
	String         subject;

	// state automaton
	String         error;
	String         transcript_text;
};
