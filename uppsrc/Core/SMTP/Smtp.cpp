#include "SMTP.h"

namespace Upp {

namespace Ini {
	INI_BOOL(Smtp_Trace, false, "Activates HTTP requests tracing")
	INI_BOOL(Smtp_TraceBody, false, "Activates HTTP requests body tracing")
	INI_BOOL(Smtp_CompressLog, false, "Activates log compression (removes long hex/encode64-like data)")
};

#define LLOG(x)      do { if(Ini::Smtp_Trace) { if(Ini::Smtp_CompressLog) RLOG(CompressLog(String().Cat() << x)); else RLOG(x); } } while(0)
#define LLOGB(x)      do { if(Ini::Smtp_TraceBody) { if(Ini::Smtp_CompressLog) RLOG(CompressLog(String().Cat() << x)); else RLOG(x); } } while(0)

void Smtp::Trace(bool b)
{
    Ini::Smtp_Trace = b;
}

void Smtp::TraceBody(bool b)
{
    Ini::Smtp_TraceBody = b;
}

static String GetDelimiter(const char *b, const char *e, String init)
{
	static const char delimiters[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdef"
		"ghijklmnopqrstuvxyz()+/:?0123456"
		"789abcdefghijklmnopqrstuvwxyzABC"
		"DEFGHIJKLMNOPQRSTUVWXYZ012345678";

	String out = init;
	if(b == e)
		return out;
	if(IsNull(out))
		out.Cat(delimiters[*b++ & 0x7F]);
	int l = out.GetLength();
	for(; b != e; b++)
	{
		b = (const char *)memchr(b, *out, e - b);
		if(!b || e - b < l)
			return out;
		if(!memcmp(b, out, l))
		{
			if(e - b == l)
				return out + '/';
			out.Cat(delimiters[b[l] & 0x7F]);
		}
	}
	return out;
}

static String GetDelimiter(String s, String init)
{
	return GetDelimiter(s.Begin(), s.End(), init);
}

String Smtp::GetDomainName()
{
	String org;
	auto pos = sender.Find('@');
	if(pos >= 0) {
		auto start = ++pos, len = sender.GetLength();
		while(pos < len && sender[pos] != '>')
			pos++;
		org = sender.Mid(start, pos - start);
	}
	else org << TcpSocket::GetHostName();
	return org;
}

ValueMap Smtp::GetExtensions()
{
	ValueMap features;
	if(!smtp_msg.IsEmpty()) {
		StringStream ss(smtp_msg);
		ss.GetLine();
		while(!ss.IsEof()) {
			// Get smtp service extensions.
			auto e = NormalizeSpaces(TrimBoth(ss.GetLine().Mid(4)));
			Vector<String> v = Split(e, ' ');
			features.GetAdd(ToLower(v[0]));
			for(auto i = 1; i < v.GetCount(); i++)
				features(v[0]) << v[i];
		}
	}
	return pick(features);
}

int Smtp::GetSmtpCode(const String& s)
{
	if(s.IsVoid() || s.GetLength() < 3 || !IsDigit(s[0]) || !IsDigit(s[1]) || !IsDigit(s[2]))
		return -1;
	return StrInt(s.Mid(0, 3));
}

void Smtp::SetSender()
{
	// Specify the e-mail address of the sender.
	SendRecvOK("MAIL FROM:<" + sender + ">");
}

void Smtp::SetRecipients()
{
	// A single e-mail can be sent to multiple recipients.
	for(auto& rcp : to)
		SendRecv("RCPT TO:<" + rcp + ">");
}

void Smtp::SendRecv(const String& s)
{
	// We need to check the control connection.
	if(!IsOpen())
		throw Exc("Socket is not open.");

	// Send request.
	if(!s.IsEmpty())
		SendData(s + "\r\n");

	// Receive response.
	// Response can be "multiline".
	smtp_msg = GetLine();
	smtp_code = GetSmtpCode(smtp_msg);
	if(smtp_code == -1)
		throw Exc("Recv failed. " << GetErrorDesc());
	LLOG("<< " << smtp_msg);
	smtp_msg.Cat('\n');
	if(smtp_msg[3] && smtp_msg[3] == '-') {
		for(;;) {
			auto line = GetLine();
			if(line.IsVoid()) {
				throw Exc("Recv failed: " << GetErrorDesc());
			}
			auto end_code = GetSmtpCode(line);
			LLOG("<< " << line);
			smtp_msg.Cat(line);
			smtp_msg.Cat('\n');
			if(smtp_code == end_code && line[3] && line[3] == ' ')
				break;
		}
	}
}

void Smtp::SendRecvOK(const String& s)
{
	SendRecv(s);
	if(!ReplyIsSuccess())
		throw Exc(smtp_msg);
}

bool Smtp::SendHello()
{
	auto org = GetDomainName();
	// Try EHLO command first.
	// EHLO command may return available service extensions.
	SendRecv("EHLO " << org);
	if(!ReplyIsSuccess()) {
		// Fall back to original client greeting if EHLO fails.
		SendRecvOK("HELO " << org);
		return false;
	}
	return true;
}

void Smtp::StartTls()
{
	SendRecv("STARTTLS");
	if(!ReplyIsSuccess() || !StartSSL())
		throw Exc("Unable to init TLS session.");
	LLOG("++ STARTTLS successful.");
}

void Smtp::Authenticate()
{
	if(!IsNull(auth_user)) {
		SendRecv("AUTH LOGIN");
		while(ReplyIsPending()){
			auto param = Base64Decode(smtp_msg.GetIter(4), smtp_msg.End());
			if(param == "Username:")
				SendRecv(Base64Encode(auth_user));
			else
			if(param == "Password:")
				SendRecv(Base64Encode(auth_pwd));
		}
		if(!ReplyIsSuccess())
			throw Exc(smtp_msg);
	}
}

void Smtp::Quit()
{
	SendRecv("QUIT");
}

void Smtp::SendMail(const String& msg_)
{
	SetSender();
	SetRecipients();

	// Now send the actual e-mail.
	SendRecv("DATA");
	if(ReplyIsPending()) {
		String msg = msg_;
		if(msg.IsEmpty())
			msg = GetMessage(true);
		
		SendRecv(msg + ".");
		if(ReplyIsSuccess())
			return;
	}
	throw Exc(smtp_msg);
}

void Smtp::SendData(const String &s)
{
	if(Ini::Smtp_TraceBody)
		LLOG(">> " << s);
	else
		LLOG(">> [Smtp send body: " << s.GetCharCount() << " bytes]");
	if(!PutAll(s))
		throw Exc("Send failed. " << GetErrorDesc());
}


//////////////////////////////////////////////////////////////////////
// Smtp::

static const char default_mime[] = "application/octet-stream";

String Smtp::Encode(const String& text)
{
	String txt = ToCharset(CHARSET_UTF8, text);
	String r = "=?UTF-8?Q?";
	for(const char *s = txt; *s; s++) {
		if((byte)*s < ' ' || (byte)*s > 127 || *s == '=' || *s == '?' || *s == ' ')
			r << '=' << FormatIntHexUpper((byte)*s, 2);
		else
			r.Cat(*s);
	}
	r << "?=";
	return r;
}

Smtp& Smtp::To(const String& t, const String& name, AS a)
{
	to.Add(t);
	to_name.Add(name);
	as.Add(a);
	return *this;
}

Smtp& Smtp::Subject(const String& s)
{
	subject = s;
	return *this;
}

Smtp& Smtp::ReplyTo(const String& r, const String& name)
{
	reply_to = r;
	reply_to_name = name;
	return *this;
}

Smtp& Smtp::From(const String& f, const String& name, const String& s)
{
	from = f;
	from_name = name;
	sender = Nvl(s, f);
	return *this;
}

Smtp& Smtp::AttachFile(const char *filename, const char *mime)
{
	Attachment& attach = attachments.Add();
	attach.name = GetFileNamePos(filename);
	attach.mime = (mime ? mime : default_mime);
	attach.file = filename;
	return *this;
}

Smtp& Smtp::Attach(const char *name, const String& data, const char *mime)
{
	Attachment& attach = attachments.Add();
	attach.name = name;
	attach.mime = (mime ? mime : default_mime);
	attach.data = data;
	return *this;
}

String Smtp::FormatAddr(const String& addr, const String& name)
{
	String r;
	if(name.GetCount())
		r << "\"" << Encode(name) << "\" ";
	r << '<' << addr << '>';
	return r;
}

String Smtp::GetMessageID()
{
	int q = sender.Find('@');
	return message_id + (q >= 0 ? sender.Mid(q) : "@unknown_host.org");
}

bool Smtp::NoAdd(const char *id)
{
	return !(add_header.StartsWith(id + String(":")) || add_header.Find(String("\n") + id + ":") >= 0);
}

String Smtp::GetMessage(bool chunks)
{
	String delimiter = "?";
	for(int i = 0; i < body.GetCount(); i++)
		delimiter = GetDelimiter(body[i], delimiter);
	bool alter = body.GetCount() > 1;
	bool multi = !attachments.IsEmpty();

	String msg;
	if(!no_header) { // generate message header
		if (sender != from) msg << "Sender: " << sender << "\r\n";
		msg << "From: " << FormatAddr(from, from_name) << "\r\n";
		static const AS as_list[] = { TO, CC, BCC };
		static const char *as_name[] = { "To", "CC", "BCC" };
		for(int a = 0; a < __countof(as_list); a++)
		{
			int pos = 0;
			for(int i = 0; i < as.GetCount(); i++)
				if(as[i] == as_list[a])
				{
					if(pos && pos + to[i].GetLength() >= 70)
					{
						msg << "\r\n     ";
						pos = 5;
					}
					else if(pos)
					{
						msg << ", ";
						pos += 2;
					}
					else
					{
						msg << as_name[a] << ": ";
						pos = (int)strlen(as_name[a]) + 2;
					}
					msg << FormatAddr(to[i], to_name[i]);
				}
			if(pos)
				msg << "\r\n";
		}
		if(!IsNull(subject) && NoAdd("Subject"))
			msg << "Subject: " << Encode(subject) << "\r\n";
		if(!IsNull(reply_to) && NoAdd("Reply-To"))
			msg << "Reply-To: " << FormatAddr(reply_to, reply_to_name) << "\r\n";
		if(NoAdd("Message-ID"))
			msg << "Message-ID: <" << GetMessageID() << ">\r\n";
		if(!IsNull(time_sent)) {
			static const char *dayofweek[] =
			{ "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
			static const char *month[] =
			{ "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
			msg << "Date: "
				<< dayofweek[DayOfWeek(time_sent)] << ", "
				<< (int)time_sent.day << ' ' << month[time_sent.month - 1] << ' ' << (int)time_sent.year
				<< ' ' << Sprintf("%2d:%02d:%02d " + GetTimeZoneText(),
				                  time_sent.hour, time_sent.minute, time_sent.second)
				<< "\r\n";
		}
		if((multi || alter) && NoAdd("Content-Type"))
			msg << "Content-Type: multipart/" << (alter ? "alternative" : "mixed")
				<< "; boundary=\"" << delimiter << "\"\r\n"
				"\r\n";
		msg << add_header;
	}

	for(int i = 0; i < body.GetCount(); i++) {
		String t = body[i], m = mime[i];
		if(!no_header) {
			if(multi || alter)
				msg << "--" << delimiter << "\r\n";
			if(IsNull(m))
				m << "text/plain; charset=\"" << MIMECharsetName(CHARSET_DEFAULT) << "\"";
			msg << "Content-Type: " << m << "\r\n"
			"Content-Transfer-Encoding: quoted-printable\r\n";
		}
		if(!no_header_sep)
			msg << "\r\n";
		bool begin = true;
		for(const char *p = t.Begin(), *e = t.End(); p != e; p++)
			if(*p >= 33 && *p <= 126 && *p != '=' && (*p != '.' || !begin)) {
				msg.Cat(*p);
				begin = false;
			}
			else if(*p == '.' && begin) {
				msg.Cat("..");
				begin = false;
			}
			else if(*p == ' ' && p + 1 != e && p[1] != '\r' && p[1] != '\n') {
				msg.Cat(' ');
				begin = false;
			}
			else if(*p == '\r')
				;
			else if(*p == '\n') {
				msg.Cat("\r\n");
				begin = true;
			}
			else {
				static const char hex[] = "0123456789ABCDEF";
				msg.Cat('=');
				msg.Cat(hex[(*p >> 4) & 15]);
				msg.Cat(hex[*p & 15]);
				begin = false;
			}

		if(!begin)
			msg.Cat("\r\n");
	}
	for(int i = 0; i < attachments.GetCount(); i++) {
		const Attachment& a = attachments[i];
		One<Stream> source;
		if(a.file.GetCount()) {
			FileIn& in = source.Create<FileIn>();
			if(!in.Open(a.file))
				throw Exc("Unable to open attachment file " + a.file);
		}
		else
			source.Create<StringStream>().Open(a.data);
		msg << "--" << delimiter << "\r\n"
			"Content-Type: " << a.mime << "; name=\"" << a.name << "\"\r\n"
			"Content-Transfer-Encoding: base64\r\n"
			"Content-Disposition: attachment; filename=\"" << a.name << "\"\r\n"
			"\r\n";

		char buffer[54];
		for(int c; (c = source -> Get(buffer, sizeof(buffer))) != 0;)
		{
			msg.Cat(Base64Encode(buffer, buffer + c));
			msg.Cat('\r');
			msg.Cat('\n');
			if(msg.GetLength() >= 65536 && chunks) {
				SendData(msg);
				msg = Null;
			}
		}
	}
	if(multi || alter)
		msg << "--" << delimiter << "--\r\n";
	LLOGB("Msg:");
	LLOGB(msg);

	return msg;
}

bool Smtp::Send(const String& msg_)
{
	smtp_code = 0;
	smtp_msg.Clear();

	try {
		if(IsNull(host))
			throw Exc(t_("Host not set."));

		if(to.IsEmpty())
			throw Exc(t_("Recipient not set."));

		if(!Connect(host, Nvl(port, starttls ? 587 : (ssl ? 465 : 25))))
			throw Exc(Format("Cannot open socket %s:%d: %s", host, port, GetErrorDesc()));

		GlobalTimeout(request_timeout);

		if(ssl)
			if(!StartSSL())
				throw Exc("Unable to start SSL");

		// Receive initial message.
		SendRecv(Null);

		// Send HELO/EHLO command and query smtp service extensions.
		if(SendHello()) {
			auto ext = GetExtensions();
			if(!ext.IsEmpty()) {
				// Check services.
				if(starttls) {
					if(ext.Find("starttls") < 0)
						throw Exc("STARTTLS is not supported by the server.");
					StartTls();
					SendHello();
				}
			}
		}

		// Everything is fine. Let us login now.
		Authenticate();

		// Send mail.
		SendMail(msg_);

		// Close connection.
		Quit();
		return true;
	}
	catch(Exc e) {
		error = e;
		LLOG("-- " << error);
		return false;
	}
}

Smtp& Smtp::New() {
	to.Clear();
	to_name.Clear();
	as.Clear();
	body.Clear();
	mime.Clear();
	error.Clear();
	add_header.Clear();
	from.Clear();
	sender.Clear();
	message_id = AsString(Uuid::Create());
	return *this;
}

Smtp::Smtp()
{
	port = Null;
	no_header = no_header_sep = false;
	time_sent = GetSysTime();
	request_timeout = 120000;
	smtp_code = 0;
	ssl = false;
	starttls = false;
	New();
}

}
