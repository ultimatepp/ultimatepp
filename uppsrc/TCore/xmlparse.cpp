#include "TCore.h"
#pragma hdrstop

NAMESPACE_UPP

String XMLTag::ToString() const
{
	String out;
	out << '<' << name;
	for(int i = 0; i < attrib.GetCount(); i++)
		out << ' ' << attrib.GetKey(i) << '=' << attrib[i];
	out << '>';
	return out;
}

String XMLTagTree::ToString() const
{
	String out;
	Format(out, 0);
	return out;
}

void XMLTagTree::Format(String& out, int indent) const
{
	out.Cat('\t', indent);
	out << XMLTag::ToString() << '\n';
	if(!text.IsEmpty()) {
		out.Cat('\t', indent);
		out << text << '\n';
	}
	for(int i = 0; i < subtags.GetCount(); i++)
		subtags[i].Format(out, indent + 1);
	out.Cat('\t', indent);
	out << "</" << name << ">\n";
}

const XMLTagTree& XMLTagTree::Tag(String name) const
{
	static XMLTagTree empty;
	return subtags.Get(name, empty);
}

static VectorMap<String, String>& StdCharRefMap()
{
	static VectorMap<String, String> crm;
	if(!crm.IsEmpty())
		return crm;
	static const char *crm_map[][2] = {
		{ "lt",   "<"  },
		{ "gt",   ">"  },
		{ "amp",  "&"  },
		{ "apos", "\'" },
		{ "quot", "\"" },
	};
	for(int i = 0; i < __countof(crm_map); i++)
		crm.Add(crm_map[i][0], crm_map[i][1]);
	return crm;
}

static int NameCharFilter(int c)
{
	return IsAlNum(c) || c == '.' || c == '-' || c == '_' || c == ':' ? c : 0;
}

void XMLParser::Open(Stream& stream, int line_)
{
	Close();
	InitOpen(stream, line_);
}

void XMLParser::Open(String string, int line_)
{
	Close();
	stringinput = new StringStream(string);
	InitOpen(*stringinput, line_);
}

void XMLParser::InitOpen(Stream& strm, int line)
{
	input = &strm;
	line_number = line - 1;
	current_entity = FIRST;
	charset = CHARSET_UTF8;
	honor_spaces = false;
	FetchLine();
	Next();
}

void XMLParser::Close()
{
	input = NULL;
	stringinput.Clear();
	tag_stack.Clear();
	space_stack.Clear();
}

XMLTagTree XMLParser::Read()
{
	XMLTagTree output;
	Read(output);
	return output;
}

void XMLParser::Read(XMLTagTree& tree)
{
	RTIMING("XMLParser::Read");
	for(;; Next())
		switch(Entity()) {
			case TEXT: {
				tree.text.Cat(Text());
				break;
			}
			case TAG: {
				const XMLTag& tag = Tag();
				XMLTagTree& subtree = tree.subtags.Add(tag.name);
				*(XMLTag*)&subtree <<= tag;
				Next();
				Read(subtree);
				break;
			}
			default:
				return;
		}
}

bool XMLParser::FetchLine()
{
	RTIMING("XMLParser::FetchLine");
	line_data = input->GetLine();
	line_ptr = line_data;
	line_number++;
	return !line_data.IsEmpty() || !input->IsEof();
}

bool XMLParser::SkipWhite()
{
	RTIMING("XMLParser::SkipWhite");
	while((byte)*line_ptr <= ' ') {
		if(*line_ptr == '\0') {
			if(!FetchLine())
				return false;
		}
		else if(honor_spaces)
			return true;
		else
			line_ptr++;
	}
	return true;
}

bool XMLParser::InLevel(int count)
{
	if(current_entity == ISEOF || GetCount() < count || current_entity == ENDTAG && GetCount() == count)
		return false;
	return true;
}

bool XMLParser::NextLevel(int count)
{
	RTIMING("XMLParser::NextLevel");
	return Next() != ISEOF && InLevel(count);
}

void XMLParser::SkipLevel(int count)
{
	RTIMING("XMLParser::SkipLevel");
	while(NextLevel(count))
		;
}

XMLParser::ENTITY XMLParser::Next()
{
	RTIMING("XMLParser::Next");
	if(current_entity == TAG && loner_tag)
		return current_entity = ENDTAG;
	if(current_entity == ENDTAG && !tag_stack.IsEmpty()) {
		tag_stack.Drop();
		space_stack.Drop();
		honor_spaces = (!space_stack.IsEmpty() && space_stack.Top());
	}
	current_text = Null;
	while(SkipWhite()) {
		if(*line_ptr == '<') {
			RTIMING("XMLParser::Next / '<'");
			char loner = '/';
			loner_tag = false;
			bool endtag = false;
			bool excl = false;
			XMLTag tag;
			const char *tagname = ++line_ptr;
			if(*line_ptr == '!') {
				excl = true;
				loner_tag = true;
				loner = '!';
				if(*++line_ptr == '-' && line_ptr[1] == '-') { // comment
					line_ptr += 2;
					while(line_ptr[0] != '-' || line_ptr[1] != '-' || line_ptr[2] != '>')
						if(*line_ptr++ == 0 && !FetchLine())
							return current_entity = ISEOF;
					line_ptr += 3;
					continue;
				}
			}
			else if(*line_ptr == '?') {
				loner_tag = true;
				loner = '?';
				line_ptr++;
			}
			else if(*line_ptr == '/') {
				endtag = true;
				tagname = line_ptr++;
			}
			while(NameCharFilter(*line_ptr))
				line_ptr++;
			tag.name = String(tagname, line_ptr);
			while(SkipWhite() && *line_ptr != '>' && !(excl && *line_ptr == '[') && !(*line_ptr == loner && line_ptr[1] == '>')) {
				const char *attrname = line_ptr;
				while((byte)*++line_ptr > ' ' && *line_ptr != '='
				&& *line_ptr != '>' && !(excl && *line_ptr == '[') && !(*line_ptr == loner && line_ptr[1] == '>'))
					;
				String attr(attrname, line_ptr);
				WString value;
				SkipWhite();
				if(*line_ptr == '=') {
					line_ptr++;
					SkipWhite();
					StringBuffer attrval;
					if(*line_ptr == '\"') {
						line_ptr++;
						while(*line_ptr && *line_ptr != '\"')
							if(*line_ptr == '&')
								attrval.Cat(GetCharRef());
							else
								attrval.Cat(*line_ptr++);
						if(*line_ptr == '\"')
							line_ptr++;
					}
					else {
						while((byte)*line_ptr > ' ' && *line_ptr != '>'
						&& !(excl && *line_ptr == '[') && !(*line_ptr == loner && line_ptr[1] == '>'))
							if(*line_ptr == '&')
								attrval.Cat(GetCharRef());
							else
								attrval.Cat(*line_ptr++);
					}
					value = FromUtf8(~attrval, attrval.GetLength());
				}
				tag.attrib.Add(attr, value);
			}
			if(!endtag) {
				space_stack.Add() = honor_spaces;
				static const WString preserve = "preserve";
				if(tag.Attrib("xml:space") == preserve)
					honor_spaces = true;
				tag_stack.Add() = tag;
			}
			if(excl && *line_ptr == '[') {
				loner_tag = false;
				line_ptr++;
			}
			else {
				if(*line_ptr && *line_ptr != '>') {
					loner_tag = true;
					line_ptr++;
				}
				if(*line_ptr == '>')
					line_ptr++;
			}
			if(tag_stack.GetCount() == 1 && tag.name == "?xml") {
				String encoding = ToLower(tag_stack.Top().Attrib("encoding").ToString());
				/**/ if(encoding == "iso-8859-1")   charset = CHARSET_ISO8859_1;
				else if(encoding == "iso-8859-2")   charset = CHARSET_ISO8859_2;
				else if(encoding == "windows-1250") charset = CHARSET_WIN1250;
				else if(encoding == "windows-1251") charset = CHARSET_WIN1251;
				else if(encoding == "windows-1252") charset = CHARSET_WIN1252;
				else if(encoding == "utf-8")        charset = CHARSET_UTF8;
			}
			return current_entity = (endtag ? ENDTAG : TAG);
		}
		else { // text
			RTIMING("XMLParser::Next / text");
			char closer = 0;
			if(!tag_stack.IsEmpty() && *tag_stack.Top().name == '!')
				closer = ']';
			StringBuffer raw_text;
			while(*line_ptr != '<' && *line_ptr != closer) {
				if(*line_ptr == '&')
					raw_text.Cat(GetCharRef());
				else if(*line_ptr)
					raw_text.Cat(*line_ptr++);
				else if(!FetchLine())
					break;
			}
			if(!honor_spaces) {
				const char *p = raw_text.Begin(), *e = raw_text.End();
				while(e > p && (byte)e[-1] <= ' ')
					e--;
				raw_text.SetLength(e - p);
			}
			if(raw_text.GetLength() > 0) {
				current_text = FromUtf8(~raw_text, raw_text.GetLength());
				return current_entity = TEXT;
			}
			if(closer && *line_ptr == closer) {
				line_ptr++;
				if(*line_ptr == '>')
					line_ptr++;
				return current_entity = ENDTAG;
			}
		}
	}
	return current_entity = ISEOF;
}

String XMLParser::GetCharRef()
{
	RTIMING("XMLParser::GetCharRef");
	String out;
	int outconst = 0;
	if(*++line_ptr == '#') {
		if(*++line_ptr == 'X' || *line_ptr == 'x') {
			for(byte c; (c = ctoi(*++line_ptr)) < 16; outconst = 16 * outconst + c)
				;
		}
		else {
			while(IsDigit(*line_ptr))
				outconst = 10 * outconst + *line_ptr++ - '0';
		}
		out = ToUtf8(outconst);
	}
	else {
		const char *begin = line_ptr;
		while(*++line_ptr && *line_ptr != ';')
			;
		out = StdCharRefMap().Get(String(begin, line_ptr), Null);
	}
	if(*line_ptr == ';')
		line_ptr++;
	return out;
}

void XMLParser::SelfTest()
{
	for(; !IsEof(); Next())
		switch(Entity()) {
			case TAG: {
				const XMLTag& tag = Tag();
				String path;
				for(int i = 0; i < GetCount(); i++)
					path << (i ? "/" : "") << tag_stack[i].name;
				RLOG("TAG[" << GetCount() << "]: " << path);
				for(int i = 0; i < tag.attrib.GetCount(); i++)
					RLOG("\tATTRIB[" << tag.attrib.GetKey(i) << "]=" << tag.attrib[i]);
				break;
			}
			case ENDTAG: {
				String path;
				for(int i = 0; i < GetCount(); i++)
					path << (i ? "/" : "") << tag_stack[i].name;
				RLOG("END[" << GetCount() << "]: " << path);
				break;
			}
			case TEXT: {
				RLOG("TXT: " << Text());
				break;
			}
			default: {
				RLOG("invalid entity type: " << (int)Entity());
				break;
			}
		}
	RLOG("[EOF]");
}

XMLTagTree XMLReadTagTree(Stream& stream, int line)
{
	XMLParser parser(stream, line);
	return parser.Read();
}

XMLTagTree XMLReadTagTree(String string, int line)
{
	XMLParser parser(string, line);
	return parser.Read();
}

String ToXml(const char *s, byte charset)
{
	if(charset == CHARSET_DEFAULT)
		charset = GetDefaultCharset();
	StringBuffer result;
	for(; *s; s++)
		/**/ if(*s == '<')  result.Cat("&lt;");
		else if(*s == '>')  result.Cat("&gt;");
		else if(*s == '&')  result.Cat("&amp;");
		else if(*s == '\'') result.Cat("&apos;");
		else if(*s == '\"') result.Cat("&quot;");
		else if((byte)*s < ' ' && *s != '\n') result.Cat(NFormat("&#x%02x;", (byte)*s));
		else if(!(*s & 0x80) || charset == CHARSET_UTF8) result.Cat(*s);
		else result.Cat(ToUtf8(ToUnicode(*s, charset)));
	return result;
}

Xmls& Xmls::Attr(const char *a)                  { Cat(' '); Cat(a); return *this; }
Xmls& Xmls::Attr(const char *a, const char *s)   { Attr(a); Cat('='); return Quote(s); }
Xmls& Xmls::Attr(const char *a, String s)        { return Attr(a, ~s); }
Xmls& Xmls::Attr(const char *a, int i)           { return Attr(a, UPP::FormatInt(i)); }
Xmls& Xmls::Attr(const char *a, double d)        { return Attr(a, UPP::FormatDouble(d)); }
Xmls& Xmls::Attr(const char *a, Date d)          { return Attr(a, UPP::Format(d)); }
Xmls& Xmls::Attr(const char *a, Value v)         { return Attr(a, StdFormat(v)); }

Xmls& Xmls::Cat(const XmlsTag& tag)               { return Cat(Xmls(tag)); }

XmlsTag::XmlsTag(const char *t)
{
	tag = "<";
	tag.Cat(t);
	end = "</";
	end.Cat(t);
	end.Cat('>');
}

void  XmlsTag::Combine(const XmlsTag& tag2, bool linebreak)
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

Xmls XmlsTag::ApplyTo(const char *s, bool lf) const
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
		if(lf)
			h.Cat('\n');
		h.Cat(end);
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

Xmls XmlsComment(const char *text)
{
	Xmls out = "<!-- ";
	out.Cat(text);
	out.Cat(" -->\n");
	return out;
}

END_UPP_NAMESPACE
