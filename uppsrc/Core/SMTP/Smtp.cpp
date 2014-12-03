#include "SMTP.h"

NAMESPACE_UPP

namespace Ini {
	INI_BOOL(Smtp_Trace, false, "Activates HTTP requests tracing")
	INI_BOOL(Smtp_TraceBody, false, "Activates HTTP requests body tracing")
	INI_BOOL(Smtp_CompressLog, false, "Activates log compression (removes long hex/encode64-like data)")
};

#define LLOG(x)      do { if(Ini::Smtp_Trace) if(Ini::Smtp_CompressLog) RLOG(CompressLog(String().Cat() << x)); else RLOG(x); } while(0)
#define LLOGB(x)      do { if(Ini::Smtp_TraceBody) if(Ini::Smtp_CompressLog) RLOG(CompressLog(String().Cat() << x)); else RLOG(x); } while(0)

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

void Smtp::CheckFail()
{
	if(msecs(start_time) > request_timeout)
		throw Exc(t_("Communication Failure: Timeout."));
	if(IsError())
		throw Exc("Connection error: " + GetErrorDesc());
}

void Smtp::SendData(const String &s, bool trace_bytes_only)
{
	if (trace_bytes_only)
	    LLOG("SMTP send body: " << s.GetCount() << " bytes");
	else
	    LLOG("SMTP send: " << s);
	const char *p = s.Begin(), *e = s.End();
	while(p != e) {
		CheckFail();
		int amount = Put(p, int(e - p));
		p += amount;
	}
}

String Smtp::SendRecv(const String& s, bool trace_bytes_only)
{
	SendData(s, trace_bytes_only);
	String reply;
	for(;;) {
		CheckFail();
		int c = Get();
		if(c >= 0) {
			if(c == '\n') {
				LLOG("Reply: " << reply);
				return reply;
			}
			reply.Cat(c);
		}
	}
}

void Smtp::SendRecvOK(const String& s, bool trace_bytes_only)
{
	String ans = SendRecv(s, trace_bytes_only);
	if(ans[0] != '2' || ans[1] != '5' || ans[2] != '0')
		throw Exc(ans);
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
		if(!IsNull(subject))
			msg << "Subject: " << Encode(subject) << "\r\n";
		if(!IsNull(reply_to))
			msg << "Reply-To: " << FormatAddr(reply_to, reply_to_name) << "\r\n";
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
		if(multi || alter)
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
	start_time = msecs();

	String ipaddr;

	try {
		if(IsNull(host))
			throw Exc(t_("Host not set."));

		if(to.IsEmpty())
			throw Exc(t_("Recipient not set."));

		if(!Connect(host, Nvl(port, ssl ? 465 : 25)))
			throw Exc(Format("Cannot open socket %s:%d: %s", host, port, GetErrorDesc()));

		GlobalTimeout(request_timeout);

		String ans;
		
		if(ssl)
			if(!StartSSL())
				throw Exc("Unable to start SSL");

		// receive initial message & send hello
		SendRecv(Null);
		String org;
		int pos = sender.Find('@');
		if(pos >= 0) {
			int start = ++pos, len = sender.GetLength();
			while(pos < len && sender[pos] != '>')
				pos++;
			org = sender.Mid(start, pos - start);
		}
		else
			org << TcpSocket::GetHostName();

		SendRecvOK("HELO " + org + "\r\n");
		if(!IsNull(auth_user)) {
			String ans = SendRecv("AUTH LOGIN\r\n");
			while(ans[0] != '2')
				if(ans[0] == '3' && ans[1] == '3' && ans[2] == '4' && ans[3] == ' ') {
					String param = Base64Decode(ans.GetIter(4), ans.End());
					if(param == "Username:")
						ans = SendRecv(Base64Encode(auth_user) + "\r\n");
					else if(param == "Password:")
						ans = SendRecv(Base64Encode(auth_pwd) + "\r\n");
					else
						throw Exc(ans);
				}
				else
					throw Exc(ans);
		}
		SendRecvOK("MAIL FROM:<" + sender + ">\r\n");
		for(int i = 0; i < to.GetCount(); i++)
			SendRecv("RCPT TO:<" + to[i] + ">\r\n");
		ans = SendRecv("DATA\r\n");

		if(memcmp(ans, "354", 3))
			throw Exc(ans);

		String msg = msg_;
		if(msg.GetCount() == 0)
			msg = GetMessage(true);

		SendRecvOK(msg + ".\r\n", true);

		SendRecv("QUIT\r\n");
		return true;
	}
	catch(Exc e) {
		error = e;
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
	ssl = false;
	New();
}

END_UPP_NAMESPACE
