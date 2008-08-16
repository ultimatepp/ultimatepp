#include "Core.h"

NAMESPACE_UPP

#define LLOG(x) // LOG(x);

String DeXml(const char *s, byte charset)
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

String XmlHeader(const char *encoding, const char *version, const char *standalone)
{
	StringBuffer r;
	r << "<?xml version=\"" << version << "\" encoding=\"" << encoding << "\" standalone=\""
	  << standalone << "\" ?>\n";
	return r;
}

String XmlPI(const char *text)
{
	StringBuffer r;
	r << "<?" << text << "?>\n";
	return r;
}

String XmlDecl(const char *text)
{
	StringBuffer r;
	r << "<!" << text << ">\n";
	return r;
}

String XmlDocType(const char *text)
{
	return XmlDecl("DOCTYPE " + String(text));
}

String XmlDoc(const char *name, const char *xmlbody)
{
	return XmlHeader() + XmlDocType(name) + XmlTag(name)(xmlbody);
}

String XmlComment(const char *text)
{
	StringBuffer out;
	out << "<!-- " << text << " -->\n";
	return out;
}

XmlTag& XmlTag::Tag(const char *s)
{
	tag.Clear();
	end.Clear();
	tag << '<' << s;
	end << "</" << s << ">\n";
	return *this;
}

String  XmlTag::operator()()
{
	return tag + "/>\n";
}

String  XmlTag::operator()(const char* text)
{
	StringBuffer r;
	r << tag << '>';
	if(strchr(text, '\n')) {
		r << '\n';
		bool tab = true;
		while(*text) {
			if(tab)
				r << '\t';
			tab = *text == '\n';
			r.Cat(*text++);
		}
	}
	else
		r << text;
	r << end;
	return r;
}

String  XmlTag::Text(const char *text, byte charset)
{
	StringBuffer r;
	return r << tag << '>' << DeXml(text, charset) << end;
}

XmlTag& XmlTag::operator()(const char *attr, const char *val)
{
	tag << ' ' << attr << "=\"" << DeXml(val) << "\"";
	return *this;
}

XmlTag& XmlTag::operator()(const char *attr, int q)
{
	return operator()(attr, AsString(q));
}

XmlTag& XmlTag::operator()(const char *attr, double q)
{
	return operator()(attr, AsString(q));
}

void XmlParser::Ent(StringBuffer& out)
{
	int outconst = 0;
	const char *t = ++term;
	if(*t == '#') {
		if(*++t == 'X' || *t == 'x') {
			for(byte c; (c = ctoi(*++t)) < 16; outconst = 16 * outconst + c)
				;
		}
		else {
			while(IsDigit(*t))
				outconst = 10 * outconst + *t++ - '0';
		}
		out.Cat(ToUtf8(outconst));
		if(*t == ';')
			t++;
		term = t;
		return;
	}
	if(t[0] == 'l' && t[1] == 't' && t[2] == ';') {
		t += 3;
		out.Cat('<');
	}
	else
	if(t[0] == 'g' && t[1] == 't' && t[2] == ';') {
		t += 3;
		out.Cat('>');
	}
	else
	if(t[0] == 'a')
		if(t[1] == 'm' && t[2] == 'p' && t[3] == ';') {
			t += 4;
			out.Cat('&');
		}
		else
		if(t[1] == 'p' && t[2] == 'o' && t[3] == 's' && t[4] == ';') {
			t += 5;
			out.Cat('\'');
		}
		else
			out.Cat('&');
	else
	if(t[0] == 'q' && t[1] == 'u' && t[2] == 'o' && t[3] == 't' && t[4] == ';') {
		t += 5;
		out.Cat('\"');
	}
	else
		out.Cat('&');
	term = t;
}

inline static bool IsXmlNameChar(int c)
{
	return IsAlNum(c) || c == '.' || c == '-' || c == '_' || c == ':';
}

void XmlParser::SkipWhites()
{
	while(*term && (byte)*term <= ' ') {
		if(*term == '\n')
			line++;
		term++;
	}
}

void XmlParser::Next()
{
	nattr.Clear();
	nattr1 = nattrval1 = Null;
	if(empty_tag) {
		empty_tag = false;
		type = XML_END;
		return;
	}
	text.Clear();
	type = XML_EOF;
	if(!npreserve)
		SkipWhites();
	if(*term == '<') {
		term++;
		if(*term == '!') {
			type = XML_DECL;
			term++;
			if(term[0] == '-' && term[1] == '-') {
				type = XML_COMMENT;
				term += 2;
				for(;;) {
					if(term[0] == '-' && term[1] == '-' && term[2] == '>')
						break;
					if(*term == '\0') {
						throw XmlError("Unterminated comment");
						type = XML_EOF;
						return;
					}
					if(*term == '\n')
						line++;
					text.Cat(*term++);
				}
				term += 3;
				return;
			}
			bool intdt = false;
			for(;;) {
				if (*term == '[')
					intdt = true;
				if(*term == '>' && intdt == false) {
					term++;
					break;
				}
				if(intdt == true && term[0] == ']' && term[1] == '>') {
					text.Cat(*term++);
					term++;
					break;
				}
				if(*term == '\0')
					throw XmlError("Unterminated declaration");
				if(*term == '\n')
					line++;
				text.Cat(*term++);
			}
		}
		else
		if(*term == '?') {
			type = XML_PI;
			term++;
			for(;;) {
				if(term[0] == '?' && term[1] == '>') {
					term += 2;
					return;
				}
				if(*term == '\0')
					throw XmlError("Unterminated processing info");
				if(*term == '\n')
					line++;
				text.Cat(*term++);
			}
		}
		else
		if(*term == '/') {
			type = XML_END;
			term++;
			const char *t = term;
			while(IsXmlNameChar(*term))
				term++;
			text = String(t, term);
			if(*term != '>')
				throw XmlError("Unterminated end-tag");
			term++;
		}
		else {
			type = XML_TAG;
			const char *t = term;
			while(IsXmlNameChar(*term))
				term++;
			text = String(t, term);
			for(;;) {
				SkipWhites();
				if(*term == '>') {
					term++;
					break;
				}
				if(term[0] == '/' && term[1] == '>') {
					empty_tag = true;
					term += 2;
					break;
				}
				if(*term == '\0')
					throw XmlError("Unterminated tag");
				const char *t = term++;
				while((byte)*term > ' ' && *term != '=' && *term != '>')
					term++;
				String attr(t, term);
				SkipWhites();
				if(*term == '=') {
					term++;
					SkipWhites();
					StringBuffer attrval;
					if(*term == '\"') {
						term++;
						while(*term && *term != '\"')
							if(*term == '&')
								Ent(attrval);
							else {
								const char *e = term;
								while(*++e && *e != '&' && *e != '\"')
									;
								attrval.Cat(term, (int)(e - term));
								term = e;
							}
						if(*term == '\"')
							term++;
					}
					else {
						while((byte)*term > ' ' && *term != '>')
							if(*term == '&')
								Ent(attrval);
							else {
								const char *e = term;
								while((byte)*++e > ' ' && *e != '>' && *e != '&')
									;
								attrval.Cat(term,(int) (e - term));
								term = e;
							}
					}
					if(attr == "xml:space" && attrval.GetLength() == 8 && !memcmp(~attrval, "preserve", 8))
						npreserve = true;
					String aval = FromUtf8(~attrval, attrval.GetLength()).ToString();
					if(IsNull(nattr1)) {
						nattr1 = attr;
						nattrval1 = aval;
					}
					else
						nattr.Add(attr, aval);
				}
			}
		}
	}
	else if(*term == 0)
		type = XML_EOF;
	else {
		StringBuffer raw_text;
		while(*term != '<' && *term) {
			if(*term == '\n')
				line++;
			if(*term == '&')
				Ent(raw_text);
			else {
				const char *e = term;
				while(*++e && *e != '&' && *e != '<')
					;
				raw_text.Cat(term, (int)(e - term));
				term = e;
			}
		}
		const char *re = raw_text.End();
		if(!npreserve) {
			while(re > raw_text.Begin() && (byte)re[-1] <= ' ')
				re--;
		}
		text = FromUtf8(~raw_text, (int)(re - ~raw_text)).ToString();
		type = XML_TEXT;
	}
}

bool   XmlParser::IsEof()
{
	return type == XML_EOF;
}

bool   XmlParser::IsTag()
{
	return type == XML_TAG;
}

String XmlParser::ReadTag()
{
	if(type != XML_TAG)
		throw XmlError("Expected tag");
	LLOG("ReadTag " << text);
	String h = text;
	stack.Add(Nesting(h, npreserve));
	attr = nattr;
	attr1 = nattr1;
	attrval1 = nattrval1;
	Next();
	return h;
}

bool  XmlParser::Tag(const char *tag)
{
	if(IsTag() && text == tag) {
		LLOG("Tag " << text);
		stack.Add(Nesting(text, npreserve));
		attr = nattr;
		attr1 = nattr1;
		attrval1 = nattrval1;
		Next();
		return true;
	}
	return false;
}

bool  XmlParser::Tag(const String& tag)
{
	if(IsTag() && text == tag) {
		LLOG("Tag " << text);
		stack.Add(Nesting(text, npreserve));
		attr = nattr;
		attr1 = nattr1;
		attrval1 = nattrval1;
		Next();
		return true;
	}
	return false;
}

void  XmlParser::PassTag(const char *tag)
{
	if(!Tag(tag))
		throw XmlError(String().Cat() << '\'' << tag << "'\' tag expected");
}

void  XmlParser::PassTag(const String& tag)
{
	if(!Tag(tag))
		throw XmlError(String().Cat() << '\'' << tag << "'\' tag expected");
}

bool  XmlParser::IsEnd()
{
	return type == XML_END;
}

bool  XmlParser::End()
{
	if(IsEof())
		throw XmlError("Unexpected end of file");
	if(IsEnd()) {
		LLOG("EndTag " << text);
		if(stack.IsEmpty())
			throw XmlError(NFormat("Unexpected end-tag: </%s>", text));
		if(stack.Top().tag != text && !relaxed) {
			LLOG("Tag/end-tag mismatch: <" << stack.Top().tag << "> </" << text << ">");
			throw XmlError(NFormat("Tag/end-tag mismatch: <%s> </%s>", stack.Top().tag, text));
		}
		stack.Drop();
		npreserve = (!stack.IsEmpty() && stack.Top().preserve_blanks);
		Next();
		return true;
	}
	return false;
}

void  XmlParser::PassEnd()
{
	if(!End())
		throw XmlError(String().Cat() << "Expected \'" << (stack.GetCount() ? stack.Top().tag : String()) << "\' end-tag");
}

bool  XmlParser::TagE(const char *tag)
{
	if(Tag(tag)) {
		PassEnd();
		return true;
	}
	return false;
}

void  XmlParser::PassTagE(const char *tag)
{
	PassTag(tag);
	PassEnd();
}

VectorMap<String, String> XmlParser::PickAttrs() pick_
{
	if(!IsNull(attr1))
		const_cast<VectorMap<String, String>&>(attr).Insert(0, attr1, attrval1);
	return attr;
}

int   XmlParser::Int(const char *id, int def) const
{
	if(id == attr1) return ScanInt(attrval1);
	int q = attr.Find(id);
	return q < 0 ? def : ScanInt(attr[q]);
}

double XmlParser::Double(const char *id, double def) const
{
	if(id == attr1) return ScanDouble(attrval1);
	int q = attr.Find(id);
	return q < 0 ? def : ScanDouble(attr[q]);
}

bool  XmlParser::IsText()
{
	return type == XML_TEXT;
}

String XmlParser::ReadText()
{
	if(!IsText())
		throw XmlError("Character data expected");
	String h = text;
	Next();
	return h;
}

String XmlParser::ReadTextE()
{
	String out;
	while(!IsEnd())
		if(IsText())
			out.Cat(ReadText());
		else
			Skip();
	PassEnd();
	return out;
}

bool   XmlParser::IsDecl()
{
	return type == XML_DECL;
}

String XmlParser::ReadDecl()
{
	if(!IsDecl())
		throw XmlError("Declaration expected");
	String h = text;
	Next();
	return h;
}

bool   XmlParser::IsPI()
{
	return type == XML_PI;
}

String XmlParser::ReadPI()
{
	if(!IsPI())
		throw XmlError("Processing info expected");
	String h = text;
	Next();
	return h;
}

bool   XmlParser::IsComment()
{
	return type == XML_COMMENT;
}

String XmlParser::ReadComment()
{
	if(!IsComment())
		throw XmlError("Comment expected");
	String h = text;
	Next();
	return h;
}

void XmlParser::Skip()
{
	if(IsEof())
		throw XmlError("Unexpected end of file");
	if(IsTag()) {
		String n = ReadTag();
		while(!End()) {
			if(IsEof())
				throw XmlError("Unexpected end of file expected when skipping tag \'" + n + "\'");
			Skip();
		}
	}
	else
		Next();
}

void XmlParser::SkipEnd()
{
	while(!IsEnd()) Skip();
	PassEnd();
}

int XmlParser::GetColumn() const
{
	const char *s = term;
	int n = 1;
	while(s > begin && *(s - 1) != '\n') {
		s--;
		n++;
	}
	return n;
}

XmlParser::XmlParser(const char *s)
{
	relaxed = false;
	empty_tag = false;
	npreserve = false;
	begin = term = s;
	line = 1;
	Next();
}

int XmlNode::FindTag(const char *_tag) const
{
	String tag = _tag;
	for(int i = 0; i < node.GetCount(); i++)
		if(node[i].type == XML_TAG && node[i].text == tag)
			return i;
	return -1;
}

XmlNode& XmlNode::Add(const char *tag)
{
	XmlNode& m = node.Add();
	m.CreateTag(tag);
	return m;
}

XmlNode& XmlNode::GetAdd(const char *tag)
{
	int q = FindTag(tag);
	return q >= 0 ? node[q] : Add(tag);
}

const XmlNode& XmlNode::Void()
{
	static XmlNode *h;
	ONCELOCK {
		static XmlNode empty;
		h = &empty;
	}
	return *h;
}

const XmlNode& XmlNode::operator[](const char *tag) const
{
	int q = FindTag(tag);
	return q < 0 ? Void() : node[q];
}

void XmlNode::Remove(const char *tag)
{
	int q = FindTag(tag);
	if(q >= 0)
		node.Remove(q);
}

String XmlNode::GatherText() const
{
	String r;
	for(int i = 0; i < GetCount(); i++)
		if(node[i].IsText())
			r << node[i].GetText();
	return r;
}

int  XmlNode::AttrInt(const char *id, int def) const
{
	String x = Attr(id);
	CParser p(x);
	return p.IsInt() ? p.ReadInt() : def;
}

XmlNode& XmlNode::SetAttr(const char *id, int i)
{
	SetAttr(id, AsString(i));
	return *this;
}

XmlNode& XmlNode::SetAttr(const char *id, const String& text)
{
	attr.GetAdd(id) = text;
	return *this;
}

bool Ignore(XmlParser& p, dword style)
{
	if((XML_IGNORE_DECLS & style) && p.IsDecl() ||
	   (XML_IGNORE_PIS & style) && p.IsPI() ||
	   (XML_IGNORE_COMMENTS & style) && p.IsComment()) {
		p.Skip();
		return true;
	}
	return false;
}

static XmlNode sReadXmlNode(XmlParser& p, dword style)
{
	XmlNode m;
	if(p.IsTag()) {
		m.CreateTag(p.ReadTag());
		m.SetAttrsPick(p.PickAttrs());
		while(!p.End())
			if(!Ignore(p, style))
				m.Add() = sReadXmlNode(p, style);
		return m;
	}
	if(p.IsText()) {
		m.CreateText(p.ReadText());
		return m;
	}
	if(p.IsPI()) {
		m.CreatePI(p.ReadPI());
		return m;
	}
	if(p.IsDecl()) {
		m.CreateDecl(p.ReadDecl());
		return m;
	}
	if(p.IsComment()) {
		m.CreateComment(p.ReadComment());
		return m;
	}
	NEVER();
	return m;
}

XmlNode ParseXML(XmlParser& p, dword style)
{
	XmlNode r;
	while(!p.IsEof())
		if(!Ignore(p, style))
			r.Add() = sReadXmlNode(p, style);
	return r;
}

XmlNode ParseXML(const char *s, dword style)
{
	XmlParser p(s);
	return ParseXML(p, style);
}

String AsXML(const XmlNode& node, dword style)
{
	StringBuffer r;
	if(style & XML_HEADER)
		r << XmlHeader();
	if(style & XML_DOCTYPE)
		for(int i = 0; i < node.GetCount(); i++) {
			const XmlNode& m = node.Node(i);
			if(m.GetType() == XML_TAG) {
				r << XmlDocType(m.GetText());
				break;
			}
		}
	style &= ~(XML_HEADER|XML_DOCTYPE);
	switch(node.GetType()) {
	case XML_PI:
		r << "<?" << node.GetText() << "?>\n";
		break;
	case XML_DECL:
		r << "<!" << node.GetText() << ">\n";
		break;
	case XML_COMMENT:
		r << "<!--" << node.GetText() << "-->\n";
		break;
	case XML_DOC:
		for(int i = 0; i < node.GetCount(); i++)
			r << AsXML(node.Node(i), style);
		break;
	case XML_TEXT:
		r << DeXml(node.GetText());
		break;
	case XML_TAG:
		XmlTag tag(node.GetText());
		for(int i = 0; i < node.GetAttrCount(); i++)
			tag(node.AttrId(i), node.Attr(i));
		if(node.GetCount()) {
			StringBuffer body;
			for(int i = 0; i < node.GetCount(); i++)
				body << AsXML(node.Node(i), style);
			r << tag(~body);
		}
		else
			r << tag();
	}
	return r;
}

END_UPP_NAMESPACE
