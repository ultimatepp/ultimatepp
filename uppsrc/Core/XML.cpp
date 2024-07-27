#include "Core.h"

namespace Upp {

#define LLOG(x)  // LOG(x)

static inline void sDeXmlChar(StringBuffer& result, char chr, byte charset, bool escapelf)
{
	/**/ if(chr == '<')  result.Cat("&lt;");
	else if(chr == '>')  result.Cat("&gt;");
	else if(chr == '&')  result.Cat("&amp;");
	else if(chr == '\'') result.Cat("&apos;");
	else if(chr == '\"') result.Cat("&quot;");
	else if((byte)chr < ' ' && (escapelf || chr != '\n' && chr != '\t' && chr != '\r'))
		result.Cat(Format("&#x%02x;", (byte)chr));
	else if(!(chr & 0x80) || charset == CHARSET_UTF8) result.Cat(chr);
	else result.Cat(ToUtf8(ToUnicode(chr, charset)));
}

String DeXml(const char *s, byte charset, bool escapelf)
{
	if(charset == CHARSET_DEFAULT)
		charset = GetDefaultCharset();
	StringBuffer result;
	for(; *s; s++)
		sDeXmlChar(result, *s, charset, escapelf);
	return String(result);
}

String DeXml(const char *s, const char *end, byte charset, bool escapelf)
{
	if(charset == CHARSET_DEFAULT)
		charset = GetDefaultCharset();
	StringBuffer result;
	for(; s < end; s++)
		sDeXmlChar(result, *s, charset, escapelf);
	return String(result);
}

String DeXml(const String& s, byte charset, bool escapelf)
{
	return DeXml(~s, s.End(), charset, escapelf);
}

String XmlHeader(const char *encoding, const char *version, const char *standalone)
{
	StringBuffer r;
	r << "<?xml version=\"" << version << "\"";
	if(encoding)
		r << " encoding=\"" << encoding << "\"";
	if(standalone)
		r << " standalone=\"" << standalone << "\"";
	r << " ?>\r\n";
	return String(r);
}

String XmlPI(const char *text)
{
	StringBuffer r;
	r << "<?" << text << "?>\r\n";
	return String(r);
}

String XmlDecl(const char *text)
{
	StringBuffer r;
	r << "<!" << text << ">\r\n";
	return String(r);
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
	out << "<!-- " << text << " -->\r\n";
	return String(out);
}

String  XmlTag::operator()()
{
	return tag + "/>";
}

XmlTag& XmlTag::Tag(const char *s)
{
	tag.Clear();
	end.Clear();
	tag << '<' << s;
	end << "</" << s << '>';
	return *this;
}

String  XmlTag::operator()(const char *text)
{
	StringBuffer r;
	r << tag << ">";
	const char *s = text;
	bool wastag = true;
	bool wasslash = true;
	bool first = true;
	while(*s) {
		const char *b = s;
		while(*s == ' ' || *s == '\t')
			s++;
		if(s[0] == '<') {
			if(first)
				r << "\r\n";
			if(wastag && (wasslash || s[1] != '/'))
				r.Cat('\t');
		}
		else
		if(first) {
			r << text << end;
			return String(r);
		}
		first = false;
		wasslash = false;
		char last = 0;
		while(*s && *s != '\n' && *s != '\r') {
			if(*s == '<')
				wasslash = s[1] == '/';
			if(*s == '/' && s[1] == '>')
				wasslash = true;
			last = *s++;
		}
		wastag = last == '>';
		if(*s == '\r')
			s++;
		if(*s == '\n')
			s++;
		r.Cat(b, s);
	}
	if(!first)
		r << "\r\n";
	r << end;
	return String(r);
}

String  XmlTag::Text(const char *text, byte charset)
{
	StringBuffer r;
	return r << tag << '>' << DeXml(text, charset) << end;
}

String XmlTag::PreservedText(const char *text, byte charset)
{
	StringBuffer r;
	return r << tag << " xml:spaces=\"preserve\">" << DeXml(text, charset) << end;
}

XmlTag& XmlTag::operator()(const char *attr, const char *val)
{
	tag << ' ' << attr << "=\"" << DeXml(val, CHARSET_DEFAULT, true) << "\"";
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

XmlTag& XmlTag::operator()(const char *attr, float q)
{
	return operator()(attr, AsString(q));
}

force_inline
String XmlParser::Convert(StringBuffer& b)
{
	if(acharset == scharset)
		return b;
	return ToCharset(acharset, b, scharset);
}

void XmlParser::Ent(StringBuffer& out)
{
	LoadMore();
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
	const char *b = t;
	while(*t && *t != ';')
		t++;
	if(*t == ';') {
		int q = entity.Find(String(b, t));
		if(q >= 0) {
			out.Cat(entity[q]);
			term = t + 1;
			return;
		}
	}
	if(!relaxed)
		throw XmlError("Unknown entity");
	out.Cat('&');
}

inline static bool IsXmlNameChar(int c)
{
	return IsAlNum(c) || c == '.' || c == '-' || c == '_' || c == ':';
}

void XmlParser::LoadMore0()
{ // WARNING: Invalidates pointers to buffer
	if(in && !in->IsEof()) {
		int pos = int(term - begin);
		if(len - pos < MCHARS) {
			LLOG("LoadMore0 " << pos << ", " << len);
			begincolumn = GetColumn0();
			len -= pos;
			memmove(buffer, term, len);
			term = begin = buffer;
			len += in->Get(~buffer + len, CHUNK);
			buffer[len] = '\0';
		}
	}
}

bool XmlParser::More()
{ // WARNING: Invalidates pointers to buffer
	begincolumn = GetColumn();
	if(!in || in->IsEof())
		return false;
	LLOG("More " << (int)CHUNK);
	len = in->Get(buffer, CHUNK);
	buffer[len] = '\0';
	term = begin = buffer;
	return len;
}

void XmlParser::SkipWhites()
{
	while(HasMore() && (byte)*term <= ' ') {
		if(*term == '\n')
			line++;
		term++;
	}
	LoadMore();
}

void XmlParser::ReadAttr(StringBuffer& attrval, int c)
{
	term++;
	while(HasMore() && *term != c)
		if(*term == '&')
			Ent(attrval);
		else
			attrval.Cat(*term++);
	if(*term == c)
		term++;
}

void XmlParser::Next()
{
	nattr.Clear();
	nattr1 = nattrval1 = Null;
	if(empty_tag) {
		empty_tag = false;
		type = XML_END;
		LLOG("XML_END (empty tag) " << tagtext);
		return;
	}

	type = Null;
	StringBuffer raw_text;
	for(;;) {
		if(!HasMore()) {
			type = XML_EOF;
			LLOG("XML_EOF");
			return;
		}
		LoadMore();
		if(*term == '<') {
			if(term[1] == '!' && term[2] == '[' &&
			   term[3] == 'C' && term[4] == 'D' && term[5] == 'A' && term[6] == 'T' && term[7] == 'A' &&
			   term[8] == '[') { // ![CDATA[
				term += 9;
				LLOG("CDATA");
				for(;;) {
					if(!HasMore())
						throw XmlError("Unterminated CDATA");
					LoadMore();
					if(term[0] == ']' && term[1] == ']' && term[2] == '>') { // ]]>
						term += 3;
						break;
					}
					if(*term == '\n')
						line++;
					raw_text.Cat(*term++);
				}
				type = XML_TEXT;
				continue;
			}
			else
				break;
		}
		if(*term == '\n')
			line++;
		if(*term == '&') {
			Ent(raw_text);
			type = XML_TEXT;
		}
		else {
			if((byte)*term > ' ')
				type = XML_TEXT;
			raw_text.Cat(*term++);
		}
	}
	cdata = Convert(raw_text);

	if(cdata.GetCount() && (npreserve || preserveall))
		type = XML_TEXT;

	if(type == XML_TEXT)
		return;

	term++;
	LoadMore();
	if(*term == '!') {
		tagtext.Clear();
		type = XML_DECL;
		term++;
		if(term[0] == '-' && term[1] == '-') {
			type = XML_COMMENT;
			term += 2;
			for(;;) {
				LoadMore();
				if(term[0] == '-' && term[1] == '-' && term[2] == '>')
					break;
				if(!HasMore())
					throw XmlError("Unterminated comment");
				if(*term == '\n')
					line++;
				tagtext.Cat(*term++);
			}
			term += 3;
			return;
		}
		bool intdt = false;
		for(;;) {
			LoadMore();
			if (*term == '[')
				intdt = true;
			if(*term == '>' && intdt == false) {
				term++;
				break;
			}
			if(intdt == true && term[0] == ']' && term[1] == '>') {
				tagtext.Cat(*term++);
				term++;
				break;
			}
			if(!HasMore())
				throw XmlError("Unterminated declaration");
			if(*term == '\n')
				line++;
			tagtext.Cat(*term++);
		}
		LLOG("XML_DECL " << tagtext);
	}
	else
	if(*term == '?') {
		tagtext.Clear();
		type = XML_PI;
		term++;
		for(;;) {
			LoadMore();
			if(term[0] == '?' && term[1] == '>') {
				term += 2;
				LLOG("XML_PI " << tagtext);
				if(!tagtext.StartsWith("xml "))
					return;
				int q = tagtext.Find("encoding");
				if(q < 0)
					return;
				q = tagtext.Find('\"', q);
				if(q < 0)
					return;
				q++;
				int w = tagtext.Find('\"', q);
				if(w < 0)
					return;
				q = CharsetByName(tagtext.Mid(q, w - q));
				if(q)
					scharset = q;
				return;
			}
			if(!HasMore())
				throw XmlError("Unterminated processing info");
			if(*term == '\n')
				line++;
			tagtext.Cat(*term++);
		}
	}
	else
	if(*term == '/') {
		type = XML_END;
		term++;
		const char *t = term;
		while(IsXmlNameChar(*term))
			term++;
		tagtext = String(t, term);
		LLOG("XML_END " << tagtext);
		if(*term != '>')
			throw XmlError("Unterminated end-tag");
		term++;
	}
	else {
		type = XML_TAG;
		const char *t = term;
		while(IsXmlNameChar(*term))
			term++;
		tagtext = String(t, term);
		LLOG("XML_TAG " << tagtext);
		for(;;) {
			SkipWhites();
			if(*term == '>') {
				term++;
				break;
			}
			if(term[0] == '/' && term[1] == '>') {
				cdata.Clear();
				empty_tag = true;
				term += 2;
				break;
			}
			if(!HasMore())
				throw XmlError("Unterminated tag");
			LoadMore();
			const char *t = term++;
			while((byte)*term > ' ' && *term != '=' && *term != '>')
				term++;
			String attr(t, term);
			SkipWhites();
			if(*term == '=') {
				term++;
				SkipWhites();
				StringBuffer attrval;
				if(*term == '\"')
					ReadAttr(attrval, '\"');
				else
				if(*term == '\'')
					ReadAttr(attrval, '\'');
				else
					while(HasMore() && (byte)*term > ' ' && *term != '>' && *term != '/')
						if(*term == '&')
							Ent(attrval);
						else
							attrval.Cat(*term++);
				if(attr == "xml:space" && attrval.GetLength() == 8 && !memcmp(~attrval, "preserve", 8))
					npreserve = true;
				String aval = Convert(attrval);
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

void XmlParser::RegisterEntity(const String& id, const String& text)
{
	entity.Add(id, text);
}

bool   XmlParser::IsEof()
{
	return type == XML_EOF;
}

bool   XmlParser::IsTag()
{
	return type == XML_TAG;
}

String XmlParser::ReadTag(bool next)
{
	if(type != XML_TAG)
		throw XmlError("Expected tag");
	LLOG("ReadTag " << tagtext);
	String h = tagtext;
	if(next) {
		if(!raw)
			stack.Add(Nesting(h, npreserve));
		attr = pick(nattr);
		attr1 = nattr1;
		attrval1 = nattrval1;
		Next();
	}
	return h;
}

bool  XmlParser::Tag(const char *tag)
{
	if(IsTag() && tagtext == tag) {
		LLOG("Tag " << tagtext);
		if(!raw)
			stack.Add(Nesting(tagtext, npreserve));
		attr = pick(nattr);
		attr1 = nattr1;
		attrval1 = nattrval1;
		Next();
		return true;
	}
	return false;
}

bool  XmlParser::Tag(const String& tag)
{
	if(IsTag() && tagtext == tag) {
		LLOG("Tag " << tagtext);
		if(!raw)
			stack.Add(Nesting(tagtext, npreserve));
		attr = pick(nattr);
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
		throw XmlError(String().Cat() << '\'' << tag << "\' tag expected");
}

void  XmlParser::PassTag(const String& tag)
{
	if(!Tag(tag))
		throw XmlError(String().Cat() << '\'' << tag << "\' tag expected");
}

String XmlParser::ReadEnd(bool next)
{
	if(type != XML_END)
		throw XmlError("Expected end-tag");
	String x = tagtext;
	if(next)
		Next();
	return x;
}

bool  XmlParser::IsEnd()
{
	return type == XML_END;
}

bool  XmlParser::End()
{
	if(IsEof()) {
		if(relaxed)
			return true;
		throw XmlError("Unexpected end of file");
	}
	if(IsEnd()) {
		LLOG("EndTag " << tagtext);
		if(!raw) {
			if(stack.IsEmpty())
				throw XmlError(Format("Unexpected end-tag: </%s>", tagtext));
			if(stack.Top().tag != tagtext && !relaxed) {
				LLOG("Tag/end-tag mismatch: <" << stack.Top().tag << "> </" << tagtext << ">");
				throw XmlError(Format("Tag/end-tag mismatch: <%s> </%s>", stack.Top().tag, tagtext));
			}
			stack.Drop();
		}
		npreserve = (!stack.IsEmpty() && stack.Top().preserve_blanks);
		Next();
		return true;
	}
	return false;
}

bool XmlParser::End(const char *tag)
{
	if(IsEnd() && tagtext == tag) {
		Next();
		return true;
	}
	return false;
}

bool XmlParser::End(const String& tag)
{
	if(IsEnd() && tagtext == tag) {
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

void XmlParser::PassEnd(const char *tag)
{
	if(!End(tag))
		throw XmlError(String().Cat() << "Expected \'" << tag << "\' end-tag");
}

bool  XmlParser::TagE(const char *tag)
{
	if(Tag(tag)) {
		SkipEnd();
		return true;
	}
	return false;
}

void  XmlParser::PassTagE(const char *tag)
{
	PassTag(tag);
	SkipEnd();
}

bool XmlParser::TagElseSkip(const char *tag)
{
	if(Tag(tag))
		return true;
	Skip();
	return false;
}

bool XmlParser::LoopTag(const char *tag)
{
	while(!End()) {
		if(Tag(tag))
			return true;
		Skip();
	}
	return false;
}

VectorMap<String, String> XmlParser::PickAttrs()
{
	if(!IsNull(attr1))
		pick(const_cast<VectorMap<String, String>&>(attr).Insert(0, attr1, attrval1));
	return pick(attr);
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

float XmlParser::Float(const char *id, float def) const
{
	if(id == attr1) return ScanFloat(attrval1);
	int q = attr.Find(id);
	return q < 0 ? def : ScanFloat(attr[q]);
}

bool  XmlParser::IsText()
{
	if(npreserve || preserveall)
		return cdata.GetCount();
	const char *e = cdata.End();
	for(const char *s = cdata.Begin(); s < e; s++)
		if((byte)*s > ' ')
			return true;
	return false;
}

String XmlParser::ReadText()
{
	String h = cdata;
	cdata.Clear();
	if(type == XML_TEXT)
		Next();
	return h;
}

String XmlParser::ReadTextE()
{
	StringBuffer out;
	for(;;) {
		String t = ReadText();
		if(!IsNull(t))
			out.Cat(t);
		else if(IsEnd()) {
			PassEnd();
			return String(out);
		}
		else
			Skip();
	}
}

bool   XmlParser::IsDecl()
{
	return type == XML_DECL;
}

String XmlParser::ReadDecl(bool next)
{
	if(!IsDecl())
		throw XmlError("Declaration expected");
	String h = tagtext;
	if(next)
		Next();
	return h;
}

bool   XmlParser::IsPI()
{
	return type == XML_PI;
}

String XmlParser::ReadPI(bool next)
{
	if(!IsPI())
		throw XmlError("Processing info expected");
	String h = tagtext;
	if(next)
		Next();
	return h;
}

bool   XmlParser::IsComment()
{
	return type == XML_COMMENT;
}

String XmlParser::ReadComment(bool next)
{
	if(!IsComment())
		throw XmlError("Comment expected");
	String h = tagtext;
	if(next)
		Next();
	return h;
}

void XmlParser::Skip()
{
	if(IsEof())
		throw XmlError("Unexpected end of file");
	if(cdata.GetCount() && type != XML_TEXT)
		cdata.Clear();
	else
	if(IsTag() && !raw) {
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

int XmlParser::GetColumn0() const
{
	const char *s = term;
	int n = 0;
	while(s > begin && *(s - 1) != '\n') {
		s--;
		n++;
	}
	return n + begincolumn;
}

void XmlParser::Init()
{
	RegisterEntity("lt", "<");
	RegisterEntity("gt", ">");
	RegisterEntity("amp", "&");
	RegisterEntity("apos", "\'");
	RegisterEntity("quot", "\"");
	relaxed = false;
	empty_tag = false;
	npreserve = false;
	line = 1;
	preserveall = false;
	begincolumn = 0;
	in = NULL;
	len = 0;
	raw = false;
	acharset = GetDefaultCharset();
	scharset = CHARSET_UTF8;
}

XmlParser::XmlParser(const char *s)
{
	Init();
	begin = term = s;
	len = INT_MAX;
	try { // Need to catch first error as construction is usually outside try-catch client block
		Next();
	}
	catch(XmlError) {
		begin = term = s;
		len = INT_MAX;
	}
}

XmlParser::XmlParser(Stream& in_)
{
	Init();
	buffer.Alloc(CHUNK + MCHARS + 1);
	begin = term = "";
	in = &in_;
	try {
		Next();
	}
	catch(XmlError) {
		begin = term = "";
	}
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
	static XmlNode h;
	return h;
}

const XmlNode& XmlNode::operator[](const char *tag) const
{
	int q = FindTag(tag);
	return q < 0 ? Void() : node[q];
}

void XmlNode::Remove(int i)
{
	node.Remove(i);
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

bool XmlNode::HasTags() const
{
	for(int i = 0; i < GetCount(); i++)
		if(node[i].IsTag())
			return true;
	return false;
}

int  XmlNode::AttrInt(const char *id, int def) const
{
	String x = Attr(id);
	CParser p(x);
	return p.IsInt() ? p.ReadInt() : def;
}

XmlNode& XmlNode::SetAttr(const char *id, const String& text)
{
	if(!attr)
		attr.Create();
	attr->GetAdd(id) = text;
	return *this;
}

void XmlNode::SetAttrs(VectorMap<String, String>&& a)
{
	if(a.GetCount() == 0)
		attr.Clear();
	else {
		if(!attr)
			attr.Create();
		*attr = pick(a);
	}
}

XmlNode& XmlNode::SetAttr(const char *id, int i)
{
	SetAttr(id, AsString(i));
	return *this;
}

void XmlNode::Shrink()
{
	if(attr) {
		if(attr->GetCount() == 0)
			attr.Clear();
		else
			attr->Shrink();
	}
	node.Shrink();
}

XmlNode::XmlNode(const XmlNode& n, int)
{
	type = n.type;
	text = n.text;
	node = clone(n.node);
	if(n.attr) {
		attr.Create();
		*attr = clone(*n.attr);
	}
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

static XmlNode sReadXmlNode(XmlParser& p, ParseXmlFilter *filter, dword style)
{
	XmlNode m;
	if(p.IsTag()) {
		String tag = p.ReadTag();
		if(!filter || filter->DoTag(tag)) {
			m.CreateTag(tag);
			m.SetAttrs(p.PickAttrs());
			while(!p.End())
				if(!Ignore(p, style)) {
					XmlNode n = sReadXmlNode(p, filter, style);
					if(n.GetType() != XML_DOC) // tag was ignored
						m.Add() = pick(n);
				}
			if(filter)
				filter->EndTag();
		}
		else
			p.SkipEnd();
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
	if(p.IsText()) {
		m.CreateText(p.ReadText());
		m.Shrink();
		return m;
	}
	p.ReadText(); // skip empty text
	return m;
}

void ParseXmlFilter::EndTag() {}

XmlNode ParseXML(XmlParser& p, dword style, ParseXmlFilter *filter)
{
	XmlNode r;
	while(!p.IsEof())
		if(!Ignore(p, style)) {
			XmlNode n = sReadXmlNode(p, filter, style);
			if(n.GetType() != XML_DOC) // tag was ignored
				r.Add() = pick(n);
			else {
				if(p.IsRelaxed())
					p.Skip();
				else
					throw XmlError("Unexpected text");
			}
		}
	return r;
}

XmlNode ParseXML(XmlParser& p, dword style)
{
	return ParseXML(p, style, NULL);
}

XmlNode ParseXML(const char *s, dword style)
{
	XmlParser p(s);
	return ParseXML(p, style);
}

XmlNode ParseXML(Stream& in, dword style)
{
	XmlParser p(in);
	return ParseXML(p, style);
}

XmlNode ParseXMLFile(const char *path, dword style)
{
	FileIn in(path);
	if(!in)
		throw XmlError("Unable to open intput file!");
	return ParseXML(in, style);
}

XmlNode ParseXML(XmlParser& p, ParseXmlFilter& filter, dword style)
{
	return ParseXML(p, style, &filter);
}

XmlNode ParseXML(const char *s, ParseXmlFilter& filter, dword style)
{
	XmlParser p(s);
	return ParseXML(p, filter, style);
}

XmlNode ParseXML(Stream& in, ParseXmlFilter& filter, dword style)
{
	XmlParser p(in);
	return ParseXML(p, filter, style);
}

XmlNode ParseXMLFile(const char *path, ParseXmlFilter& filter, dword style)
{
	FileIn in(path);
	if(!in)
		throw XmlError("Unable to open intput file!");
	return ParseXML(in, filter, style);
}

bool ShouldPreserve(const String& s)
{
	if(*s == ' ' || *s == '\t' || *s == '\n')
		return true;
	const char *l = s.Last();
	if(*l == ' ' || *l == '\t' || *l == '\n')
		return true;
	l = s.End();
	for(const char *x = s; x < l; x++)
		if(*x == '\t' || *x == '\n' || *x == ' ' && x[1] == ' ')
			return true;
	return false;
}

static void sAsXML(Stream& out, const XmlNode& node, dword style, const String& indent)
{
	if(style & XML_HEADER) {
		String h = XmlHeader();
		if(!(style & XML_PRETTY))
			h.Trim(h.GetCount() - 2);
		out << h;
	}
	if(style & XML_DOCTYPE)
		for(int i = 0; i < node.GetCount(); i++) {
			const XmlNode& m = node.Node(i);
			if(m.GetType() == XML_TAG) {
				String h = XmlDocType(m.GetText());
				h.Trim(h.GetCount() - 2);
				out << h;
				break;
			}
		}
	style &= ~(XML_HEADER|XML_DOCTYPE);
	switch(node.GetType()) {
	case XML_PI:
		out << indent << "<?" << node.GetText() << "?>\r\n";
		break;
	case XML_DECL:
		out << indent << "<!" << node.GetText() << ">\r\n";
		break;
	case XML_COMMENT:
		out << indent << "<!--" << node.GetText() << "-->\r\n";
		break;
	case XML_DOC:
		for(int i = 0; i < node.GetCount(); i++)
			sAsXML(out, node.Node(i), style, indent);
		break;
	case XML_TEXT:
		out << DeXml(node.GetText(), CHARSET_DEFAULT, style & XML_ESCAPELF);
		break;
	case XML_TAG:
		XmlTag tag(node.GetText());
		for(int i = 0; i < node.GetAttrCount(); i++)
			tag(node.AttrId(i), node.Attr(i));
		if(node.GetCount()) {
			out << indent << tag.GetBegin();
			String indent2 = (style & XML_PRETTY) && node[0].IsTag() && node[node.GetCount() - 1].IsTag() ?
			                 indent + '\t' : String();
			for(int i = 0; i < node.GetCount(); i++)
				sAsXML(out, node.Node(i), style, indent2);
			if(indent2.GetCount())
				out << indent;
			out << tag.GetEnd();
		}
		else
			out << indent << tag();
	}
}

void AsXML(Stream& out, const XmlNode& n, dword style)
{
	sAsXML(out, n, style, style & XML_PRETTY ? "\r\n" : "");
}

String AsXML(const XmlNode& n, dword style)
{
	StringStream ss;
	AsXML(ss, n, style);
	return ss.GetResult();
}

bool AsXMLFile(const char *path, const XmlNode& n, dword style)
{
	FileOut out(path);
	if(!out)
		return false;
	AsXML(out, n, style);
	out.Close();
	return !out.IsError();
}

bool IgnoreXmlPaths::DoTag(const String& id)
{
	String new_path;
	if(path.GetCount())
		new_path = path.Top();
	new_path << '/' << id;
	if(list.Find(new_path) >= 0)
		return false;
	path.Add(new_path);
	return true;
}

void IgnoreXmlPaths::EndTag()
{
	path.Drop();
}

IgnoreXmlPaths::IgnoreXmlPaths(const char *s)
{
	list = Split(s, ';');
}

}