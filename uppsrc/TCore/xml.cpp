#include          <webmap_i/webmap.h>
#pragma hdrstop

#include "xml.h"

String ToXml(const char *s)
{
	String result;
	for(; *s; s++)
		/**/ if(*s == '<')  result.Cat("&lt;");
		else if(*s == '>')  result.Cat("&gt;");
		else if(*s == '&')  result.Cat("&amp;");
		else if(*s == '\'') result.Cat("&apos;");
		else if(*s == '\"') result.Cat("&quot;");
		else if(!(*s & 0x80)) result.Cat(*s);
		else result.Cat(ToUtf8(ToUnicode(*s, CHARSET_DEFAULT)));
//		else result.Cat(NFormat("&#x%04x;", ToUnicode(*s, CHARSET_DEFAULT)));
	return result;
}

Xmls& Xmls::Attr(const char *a)                  { Cat(' '); Cat(a); return *this; }
Xmls& Xmls::Attr(const char *a, const char *s)   { Attr(a); Cat('='); return Quote(s); }
Xmls& Xmls::Attr(const char *a, String s)        { return Attr(a, ~s); }
Xmls& Xmls::Attr(const char *a, int i)           { return Attr(a, ::FormatInt(i)); }
Xmls& Xmls::Attr(const char *a, double d)        { return Attr(a, ::FormatDouble(d)); }
Xmls& Xmls::Attr(const char *a, Date d)          { return Attr(a, ::Format(d)); }
Xmls& Xmls::Attr(const char *a, Value v)         { return Attr(a, StdFormat(v)); }

Xmls& Xmls::Cat(const XmlTag& tag)               { return Cat(Xmls(tag)); }

XmlTag::XmlTag(const char *t)
{
	tag = "<";
	tag.Cat(t);
	end = "</";
	end.Cat(t);
	end.Cat('>');
}

void  XmlTag::Combine(const XmlTag& tag2, bool linebreak)
{
	if(tag2.IsEmpty())
		return;
	if(!tag.IsEmpty()) {
		String e = end;
		end = tag2.end;
		tag.Cat('>');
		if(linebreak) {
			tag.Cat('\n');
			end.Cat('\n');
		}
		tag.Cat(tag2.tag);
		end.Cat(e);
	}
	else {
		tag = tag2.tag;
		end = tag2.end;
	}
}

Xmls XmlTag::ApplyTo(const char *s, bool lf) const
{
	Xmls h;
	h.Cat(tag);
	if(!s || !*s) {
		const char *e = end;
		while(*e && *e != '>')
			e++;
		h.Cat('/');
		h.Cat(e, end.End() - e);
	}
	else {
		h.Cat('>');
		if(lf)
			h.Cat('\n');
		h.Cat(s);
		h.Cat(end);
		if(lf)
			h.Cat('\n');
	}
	return h;
}

Xmls Xml10(const char *encoding)
{
	Xmls out = "<?xml";
	out.Attr("version", "1.0");
	out.Attr("encoding", encoding);
	out.Cat("?>\n");
	return out;
}

Xmls XmlComment(const char *text)
{
	Xmls out = "<!-- ";
	out.Cat(text);
	out.Cat(" -->\n");
	return out;
}
