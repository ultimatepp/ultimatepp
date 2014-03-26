#include "POP3.h"

String DecodeHeaderValue(const String& s)
{
	String decodedstring;
	
	for(int begin = 0, end = 0, pos = 0; pos <= s.GetCharCount(); pos++) {
		pos = s.Find("=?", begin);
		if(pos < begin || pos > begin) {
			if(pos == -1) {
				if(begin >= 0)
					pos  = s.GetCharCount();
				else
					break;
			}
			end = pos;
			for(int c = begin; c < end; c++) {
				if(!IsSpace(s[c])) {
					decodedstring << s.Mid(begin, end - begin);
					break;
				}
			}
			begin = end;
		}
		else
		if(begin == pos) {
			String decodebuffer;
			 begin = pos += 2;
			 for(int n = 0; pos <= s.GetCharCount(); pos++) {
			     // Quoted-Printable Word Format:
			     // (Character set) ? (Encoding) ? (Word)
			     if(n < 2) {
			         if(s[pos] == '?') {
			             decodebuffer.Cat(" ", 1);
			             n++;
			         }
			         else
			             decodebuffer.Cat(s[pos], 1);
			     }
			     else {
			         // Find ending token.
			         if((end = s.Find("?=", pos)) == -1) 
			             return decodedstring = t_("<Parser Error: Cannot Find ending token.>");
			         else {
							// Check for spaces within the qp word. Some older mail clients do 
							// not encode ' ' spaces to '_'. This behaviour is against the rules.
							// We have to fix their fault.
							for(pos; pos < end; pos++) {
								if(IsSpace(s[pos]))
									decodebuffer.Cat("_");
								else
									decodebuffer.Cat(s[pos], 1);
							}
							pos = end += 2;
							begin = pos;
							Buffer<char> chs(32), enc(2), txt;
							txt.Alloc(decodebuffer.GetLength());
							sscanf(decodebuffer, "%s %s %s",
						       &chs[0],
						       &enc[0],
						       &txt[0]);
							int charset = CharsetByName(chs);
							// Now we are ready to decode the quoted-printable text.
							// QP text should be either base64 or "Q" encoded.
							if(String(enc) == "B") {
								decodedstring << ToUnicode(Base64Decode(String(txt)), charset);
								break;
							}
							else if(String(enc) == "Q") {
								String qtext(txt);
								for(int i = 0; i < qtext.GetCharCount(); i++) {
									if(qtext[i] == '=') {
										String ascii;
										unsigned int chr1 = ctoi(qtext[++i]);
										unsigned int chr2 = ctoi(qtext[++i]); 
										ascii.Cat(16 * chr1 + chr2);
										decodedstring << ToUnicode(ascii, charset);																		
									}
									else
									if(qtext[i] == '_') {
										decodedstring.Cat(" ");
									}
									else
										decodedstring.Cat(qtext[i], 1);		
								}
									
							}
						break;								
			        }
			    }			     
			}
		}
	}
	
	return decodedstring;
}

bool InetMessage::ReadHeader(VectorMap<String, String>& hdr, StringStream& ss)
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
	header.Clear();
	StringStream ss(s);
	return ReadHeader(header, ss);
}

bool InetMessage::Read(const String& s)
{
	part.Clear();
	header.Clear();
	StringStream ss(s);
	if(!ReadHeader(header, ss))
		return false;

	String type = header.Get("content-type");
	if(!type.StartsWith("multipart")) {
		part.Add().body = DecodeHeaderValue(LoadStream(ss));
		return true;
	}
		
	String boundary;
	int q = type.Find("boundary=");
	if(q < 0)
		return false;
	q += strlen("boundary=");
	int qq = type.Find(";", q);
	if(qq >= 0)
		boundary = type.Mid(q, qq);
	else
		boundary = type.Mid(q);

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
	for(;;) {
		Part& p = part.Add();
		if(!ReadHeader(p.header, ss))
			return false;
		for(;;) {
			if(ss.IsEof())
				return false;
			String ln = ss.GetLine();
			if(ln == boundary)
				break;
			if(ln == end_boundary)
				return true;
			p.body << DecodeHeaderValue(ln) << "\r\n";
		}
	}
}

String InetMessage::GetHeader(int parti, const char *id) const
{
	return Nvl(GetPartHeader(parti, id), header.Get(id, Null));
}

String InetMessage::GetPartBody(int i) const
{
	String encoding = ToLower(Nvl(GetHeader(i, "Content-Transfer-Encoding"), "quoted-printable"));
	String body = part[i].body;
	body = decode(encoding, "quoted-printable", QPDecode(body), "base64", Base64Decode(body), body);
	String content_type = ToLower(GetHeader(i, "content-type"));
	int q = content_type.Find("charset=");
	if(q >= 0) {
		q += strlen("charset=");
		int qq = content_type.Find(';', q);
		String charset = qq >= 0 ? content_type.Mid(q, qq + 1) : content_type.Mid(q);
		int cs = CharsetByName(charset);
		if(cs >= 0)
			body = ToCharset(CHARSET_DEFAULT, body, cs, '?');
	}
	return body;
}
