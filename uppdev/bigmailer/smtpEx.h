#ifndef _test_smtp_smtpEx_h_
#define _test_smtp_smtpEx_h_
#include <Web/Web.h>
class SmtpMailEx
{
public:
	enum AS { TO, CC, BCC };

	SmtpMailEx();

	SmtpMailEx&      Host(String h)                 { host = h; return *this; }
	SmtpMailEx&      Port(int p)                    { port = p; return *this; }
	SmtpMailEx&      From(String f)                 { from = f; return *this; }
	SmtpMailEx&      To(String t, AS a = TO)        { to.Add(t); as.Add(a); return *this; }
	SmtpMailEx&      Text(String t, const char *m = 0) { text = t; mime = m; return *this; }
	SmtpMailEx&      NoHeader()                     { no_header = true; return *this; }
	SmtpMailEx&      NoHeaderSep()                  { no_header_sep = true; return *this; }
	SmtpMailEx&      ReplyTo(String r)              { reply_to = r; return *this; }
	SmtpMailEx&      TimeSent(Time t)               { time_sent = t; return *this; }
	SmtpMailEx&      Subject(String s)              { subject = s; return *this; }
	SmtpMailEx&      Transcript(bool t = true)      { transcript = t; return *this; }
	SmtpMailEx&      AttachFile(const char *filename, const char *mime = 0);
	SmtpMailEx&      Attach(const char *name, String data, const char *mime = 0);
	SmtpMailEx&      User( const String& u )        { user = u ; return *this ; }
	SmtpMailEx&      Password( const String& p )    { pass = p ; return *this ; }


	bool           Send();

	String         GetError() const          { return error; }
	String         GetTranscript() const     { return transcript_text; }


private:

	bool authenticate( Socket& s ) ;
	String user, pass;
	String serverGreeting ;

	struct Attachment
	{
		String name; // mail name
		String file; // source path (dynamic attachments only)
		String mime; // content type (application/octet-stream by default)
		String data;
	};

	String         host;
	int            port; // default = 25
	String         from;
	Vector<String> to;
	Vector<char>   as;
	String         text;
	String         mime; // default: text/plain; charset=windows-1250
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

#endif
