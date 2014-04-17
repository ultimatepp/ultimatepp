#include "POP3.h"

#define LLOG(x) // DLOG(x)

String QDecode(const String& s) 
{
	String r, begin, end, chs, enc, txt;
	// q-encoded text format (RFC 2047):
	// =?(charset)?(encoding)?(encoded text)?=
	if(!SplitTo(s, '?', begin, chs, enc, txt, end))
		return s;
	if(begin != "=" || end != "=")
		return s;
	if(ToUpper(enc) == "B")
		r = Base64Decode(txt);	
	else
	if(ToUpper(enc) == "Q") {
		txt.Replace(" ", "_");		
		r = QPDecode(txt);
		r.Replace("_", " ");
	}
	else
		return s;
	int charset = CharsetByName(chs);
	return charset >= 0 ? ToCharset(CHARSET_DEFAULT, r, charset, '?') : r;
}

String DecodeHeaderValue(const String& s)
{
	String r, p, q;
	int pos = 0, part = 0, length = s.GetLength();
	bool isq = false;
	while(pos < length) {
		if(s[pos] == '=' && s[pos + 1] == '?') {
			// Process q-encoded text.
			q.Cat(s.Mid(pos, 2));
			pos += 2;	
			while(pos < length) {
				if(s[pos] == '?' && s[pos + 1] == '=' && part == 2) {
					q.Cat(s.Mid(pos, 2));
					r.Cat(QDecode(q));
					q.Clear();
					pos++;
					isq = true;
					part = 0;
					break;
				}
				else
				if(s[pos] == '?' && part < 2)
					part++;
				q.Cat(s[pos]);
				pos++;
				if(pos == length && !q.IsEmpty())
					r.Cat(q);
			}
		}
		else {
			// Process plain text
			while(pos < length) {
				if(s[pos] == '=' && s[pos + 1] == '?') {
					pos--;
					break;
				}
				p.Cat(s[pos], 1);
				pos++;				
				if(pos == length) 
					isq = false;
			}
			bool haschar = false;
			for(int i = 0; i < p.GetLength(); i++) 
				if(!IsSpace(p[i])) {
				   haschar = true;
				   break;
				}
			if((isq && haschar) || !isq)
				r.Cat(p);
			p.Clear();
		}
		pos++;
	}
	return r;	
}

bool InetMessage::ReadHeader(VectorMap<String, String>& hdr, Stream& ss)
{
	for(;;) {
		if(ss.IsEof())
			return false;
		String s = ss.GetLine();
		if(s.IsEmpty())
			return true;
		if(s[0] == ' ' || s[0] == '\t') {
			if(hdr.GetCount())
				hdr.Top().Cat(DecodeHeaderValue(TrimLeft(s.Mid(1))));
		}
		else {
			int q = s.Find(':');
			if(q >= 0)
				hdr.Add(ToLower(s.Mid(0, q)), DecodeHeaderValue(TrimLeft(s.Mid(q + 1))));
		}
	}
}

bool InetMessage::ReadHeader(const String& s)
{
	LLOG("ReadHeader " << s.GetCount());
	part.Clear();
	StringStream ss(s);
	return ReadHeader(part.Add().header, ss);
}

bool InetMessage::ReadPart(Stream& ss, int parent, int level)
{
	LLOG("ReadPart parent: " << parent << ", level: " << level);

	if(level > 5 || part.GetCount() > 200) // Sanity limit
		return false;

	int newparent = part.GetCount();

	Part& p = part.Add();
	p.parent = parent;

	if(!ReadHeader(p.header, ss))
		return false;

	MIMEHeader h(p.header.Get("content-type", String()));
	LLOG("content-type: " << h);
	if(!ToLower(~h).StartsWith("multipart")) {
		p.body = LoadStream(ss);
		return true;
	}

	String boundary = "--" + h["boundary"];
	String end_boundary = boundary + "--";

	LLOG("boundary " << boundary);
	
	for(;;) {
		String ln = ss.GetLine();
		if(ln == boundary)
			break;
		if(ss.IsEof())
			return false;
	}
	bool end = false;
	while(!end) {
		String body;
		bool next = false;
		for(;;) {
			if(ss.IsEof())
				return false;
			String ln = ss.GetLine();
			if(ln == boundary)
				break;
			if(ln == end_boundary) {
				end = true;
				break;
			}
			if(next)
				body << "\r\n";
			body << ln;
			next = true;
		}
		StringStream nss(body);
		ReadPart(nss, newparent, level + 1);
	}
	return true;
}

bool InetMessage::Read(const String& s)
{
	part.Clear();
	StringStream ss(s);
	return ReadPart(ss, Null, 0);
}

String InetMessage::Part::Decode() const
{
	String r = decode(ToLower(header.Get("content-transfer-encoding", "")),
	                  "quoted-printable", QPDecode(body),
	                  "base64", Base64Decode(body),
	                  body);
	int cs = CharsetByName(MIMEHeader(ToLower(header.Get("content-type", Null)))["charset"]);
	if(cs >= 0)
		r = ToCharset(CHARSET_DEFAULT, r, cs, '?');
	return r;
}

bool MIMEHeader::Parse(const char *s)
{
	value.Clear();
	param.Clear();
	CParser p(s);
	try {
		CParser p(s);
		const char *b = p.GetPtr();
		while(!p.IsEof() && !p.IsChar(';'))
			p.SkipTerm();
		value = TrimBoth(String(b, p.GetPtr()));
		if(*value == '\'' && *value.Last() == '\'' || // mime type can be quoted...
		   *value == '\"' && *value.Last() == '\"')
			value = TrimBoth(value.Mid(1, value.GetCount() - 2));
		while(!p.IsEof()) {
			if(p.Char(';') && p.IsId()) {
				const char *b = p.GetPtr();
				while(!p.IsEof() && !p.IsChar(';') && !p.IsChar('='))
					p.SkipTerm();
				String id = ToLower(TrimBoth(String(b, p.GetPtr())));
				String val;
				if(p.Char('='))
					if(p.IsChar('\''))
						val = p.ReadString('\'');
					else
					if(p.IsString())
						val = p.ReadString();
					else {
						const char *b = p.GetPtr();
						while(!p.IsEof() && !p.IsChar(';'))
							p.SkipTerm();
						val = TrimBoth(String(b, p.GetPtr()));
					}
				param.Add(id, val);
			}
			else
				p.SkipTerm();
		}
	}
	catch(CParser::Error) {}
	return value.GetCount();
}

String MIMEHeader::ToString() const
{
	String r = value;
	for(int i = 0; i < param.GetCount(); i++)
		r << "; " << param.GetKey(i) << '=' << AsCString(param[i]);
	return r;
}

Index<String> ParseMessageIDs(const String& s)
{
	Index<String> ref;
	int q = 0;
	for(;;) {
		q = s.Find('<', q);
		if(q < 0)
			break;
		int w = s.Find('>', q);
		if(w < 0)
			break;
		ref.FindAdd(s.Mid(q + 1, w - q - 1));
		q = w;
	}
	return ref;
}

String FormatMessageIDs(const Index<String>& id)
{
	String r;
	for(int i = 0; i < id.GetCount(); i++)
		if(id[i].GetCount()) {
			if(r.GetCount())
				r << ' ';
			r << '<' << id[i] << '>';
		}
	return r;
}
