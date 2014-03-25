#include "POP3.h"

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
				hdr.Top().Cat(TrimLeft(s.Mid(1)));
		}
		else {
			int q = s.Find(':');
			if(q >= 0)
				hdr.Add(ToLower(s.Mid(0, q)), TrimLeft(s.Mid(q + 1)));
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
		part.Add().body = LoadStream(ss);
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
			p.body << ln << "\r\n";
		}
	}
}
