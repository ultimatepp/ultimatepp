#include "POP3.h"

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
	part.Clear();
	StringStream ss(s);
	return ReadHeader(part.Add().header, ss);
}

bool InetMessage::ReadPart(Stream& ss, int parent, int level)
{
	if(level > 5 || part.GetCount() > 200) // Sanity limit
		return false;

	int newparent = part.GetCount();

	Part& p = part.Add();
	p.parent = parent;

	if(!ReadHeader(p.header, ss))
		return false;

	String content_type = p.header.Get("content-type", String());
	if(!ToLower(content_type).StartsWith("multipart")) {
		p.body = LoadStream(ss);
		return true;
	}

	String boundary;
	int q = content_type.Find("boundary=");
	if(q < 0)
		return false;
	q += strlen("boundary=");
	int qq = content_type.Find(";", q);
	if(qq >= 0)
		boundary = content_type.Mid(q, qq);
	else
		boundary = content_type.Mid(q);

	if(*boundary == '\"')
		boundary = boundary.Mid(1);
	if(*boundary.Last() == '\"')
		boundary.Trim(boundary.GetCount() - 1);

	boundary = "--" + boundary;
	String end_boundary = boundary + "--";
	
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
			body << ln << "\r\n";
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
	String r = decode(ToLower(header.Get("content-transfer-encoding", "quoted-printable")),
	                  "quoted-printable", QPDecode(body), "base64", Base64Decode(body), body);
	String content_type = ToLower(header.Get("content-type", Null));
	int q = content_type.Find("charset=");
	if(q >= 0) {
		q += strlen("charset=");
		int qq = content_type.Find(';', q);
		String charset = qq >= 0 ? content_type.Mid(q, qq + 1) : content_type.Mid(q);
		int cs = CharsetByName(charset);
		if(cs >= 0)
			r = ToCharset(CHARSET_DEFAULT, r, cs, '?');
	}
	return r;
}
