#ifndef _TCore_xmlparse_h_
#define _TCore_xmlparse_h_

NAMESPACE_UPP

inline bool CheckXml(const char *s) { return *s == '<' && s[1] == '?' && s[2] == 'x' && s[3] == 'm' && s[4] == 'l'; }

struct XMLTag : DeepCopyOption<XMLTag> {
	XMLTag() {}
	XMLTag(const XMLTag& tag, int deep)
	: name(tag.name), attrib(tag.attrib, deep) {}

	String ToString() const;

	int     Find(String name) const         { return attrib.Find(name); }
	WString operator [] (int i) const       { return attrib[i]; }

	WString Attrib(String name) const       { return attrib.Get(name, Null); }
	int     AttribInt(String name) const    { return ScanInt(attrib.Get(name, Null)); }
	double  AttribDouble(String name) const { return ScanDouble(attrib.Get(name, Null)); }

	String name;
	VectorMap<String, WString> attrib;
};

struct XMLTagTree : XMLTag, DeepCopyOption<XMLTagTree> {
	XMLTagTree() {}
	XMLTagTree(const XMLTagTree& tree, int deep)
	: XMLTag(tree, deep), text(tree.text), subtags(tree.subtags, deep) {}

	String             ToString() const;
	const XMLTagTree&  Tag(String name) const;
	int                Find(String name) const      { return subtags.Find(name); }
	int                FindNext(int i) const        { return subtags.FindNext(i); }
	const XMLTagTree&  operator [] (int i) const    { return subtags[i]; }

	WString text;
	ArrayMap<String, XMLTagTree> subtags;

private:
	void Format(String& output, int indent) const;
};

class XMLParser {
public:
	XMLParser() {}
	XMLParser(Stream& stream, int line = 1) { Open(stream, line); }
	XMLParser(String string, int line = 1)  { Open(string, line); }

	void          Open(Stream& stream, int line = 1);
	void          Open(String string, int line = 1);
	void          Close();

	XMLTagTree    Read();

	enum ENTITY { FIRST = -1, ISEOF, TAG, ENDTAG, TEXT };
	ENTITY        Entity() const            { return current_entity; }
	bool          IsEof() const             { return current_entity == ISEOF; }
	bool          IsTag() const             { return current_entity == TAG; }
	bool          IsEndTag() const          { return current_entity == ENDTAG; }
	bool          IsText() const            { return current_entity == TEXT; }
	ENTITY        Next();
	bool          InLevel(int count);
	bool          NextLevel(int count);
	void          SkipLevel(int count);

	const XMLTag& Tag() const               { return tag_stack.Top(); }
	int           GetCount() const          { return tag_stack.GetCount(); }
	const XMLTag& operator [] (int i) const { return tag_stack[i]; }

	WString       Text() const              { return current_text; }
	int           GetLine() const           { return line_number; }

	void          SelfTest();

private:
	String        GetCharRef();
	bool          FetchLine();
	bool          SkipWhite();
	void          InitOpen(Stream& strm, int line);
	void          Read(XMLTagTree& tree);

private:
	Stream       *input;
	One<Stream>   stringinput;
	String        line_data;
	const char   *line_ptr;
	int           line_number;
	Array<XMLTag> tag_stack;
	Vector<bool>  space_stack;
	ENTITY        current_entity;
	WString       current_text;
	byte          charset;
	bool          loner_tag;
	bool          honor_spaces;
};

XMLTagTree XMLReadTagTree(Stream& stream, int line = 1);
XMLTagTree XMLReadTagTree(String string, int line = 1);

String        ToXml(const char *s, byte charset = CHARSET_DEFAULT);

class XmlsTag;

class Xmls : public String, Moveable<Xmls> {
public:
	Xmls(const char *s) : String(s) {}
	Xmls(const String& s) : String(s) {}
	Xmls(const Nuller& = Null) {}

	bool  IsNullInstance() const     { return IsEmpty(); }

	Xmls& Cat()                      { return *this; }

	Xmls& Text(const char *s)        { String::Cat(ToXml(s)); return *this; }
	Xmls& Quote(const char *text)    { Cat('\"'); Cat(ToXml(text)); Cat('\"'); return *this; }

	Xmls& Attr(const char *attr);
	Xmls& Attr(const char *attr, const char *s);
	Xmls& Attr(const char *attr, String s);
	Xmls& Attr(const char *attr, int i);
	Xmls& Attr(const char *attr, double d);
	Xmls& Attr(const char *attr, Date date);
	Xmls& Attr(const char *attr, Value v);

	Xmls& Cat(const XmlsTag& tag);
	Xmls& Cat(String s)              { String::Cat(s); return *this; }
	Xmls& Cat(const char *s)         { String::Cat(s); return *this; }
	Xmls& Cat(char c)                { String::Cat(c); return *this; }
	Xmls& Cat(const char *s, int n)  { String::Cat(s, n); return *this; }
};

class XmlsTag : Moveable<XmlsTag> {
public:
	XmlsTag(const char *s);
	XmlsTag() {}

	XmlsTag&       Text(const char *s)                        { tag.Text(s); return *this; }
	XmlsTag&       Quote(const char *s)                       { tag.Quote(s); return *this; }

	XmlsTag&       Attr(const char *attr)                     { tag.Attr(attr); return *this; }
	XmlsTag&       Attr(const char *attr, const char *v)      { tag.Attr(attr, v); return *this; }
	XmlsTag&       Attr(const char *attr, String v)           { tag.Attr(attr, v); return *this; }
	XmlsTag&       Attr(const char *attr, int v)              { tag.Attr(attr, v); return *this; }
	XmlsTag&       Attr(const char *attr, double v)           { tag.Attr(attr, v); return *this; }
	XmlsTag&       Attr(const char *attr, Date v)             { tag.Attr(attr, v); return *this; }
	XmlsTag&       Attr(const char *attr, Value v)            { tag.Attr(attr, v); return *this; }

	void          Combine(const XmlsTag& tag, bool linebreak);

	Xmls          ApplyTo(const char *s, bool lf = false) const;
	Xmls          ApplyTo(String s, bool lf = false) const   { return ApplyTo(~s, lf); }

	XmlsTag        operator()() const                         { return *this; }
	Xmls          operator~() const                          { return ApplyTo(""); }
	operator      Xmls() const                               { return ApplyTo(""); }
	String        ToString() const                           { return ApplyTo(""); }

	const Xmls&   Tag() const                                { return tag; }
	Xmls&         Tag()                                      { return tag; }

	const String& End() const                                { return end; }
	String&       End()                                      { return end; }

	bool          IsEmpty() const                            { return tag.IsEmpty(); }

protected:
	Xmls          tag;
	String        end;
};

Xmls           Xml10(const char *encoding);
inline Xmls    Xml10(byte charset = CHARSET_DEFAULT)                 { return Xml10(MIMECharsetName(charset)); }
Xmls           XmlsComment(const char *text);

inline Xmls&   operator << (Xmls& s, const XmlsTag& tag)             { return s.Cat(tag); }
inline Xmls&   operator << (Xmls& s, const Xmls& v)                  { return s.Cat(v); }
inline Xmls&   operator << (Xmls& s, String v)                       { return s.Cat(v); }
inline Xmls&   operator << (Xmls& s, const char *v)                  { return s.Cat(v); }

inline Xmls    operator + (const XmlsTag& tag1, const XmlsTag& tag2) { Xmls r(tag1); r << tag2; return r; }
inline Xmls    operator + (const String& s, const XmlsTag& tag)      { Xmls r(s); r << tag; return r; }
inline Xmls    operator + (const XmlsTag& tag, const String& s)      { Xmls r(tag); r << s; return r; }
inline Xmls    operator + (const char *s, const XmlsTag& tag)        { Xmls r(s); r << tag; return r; }
inline Xmls    operator + (const XmlsTag& tag, const char *s)        { Xmls r(tag); r << s; return r; }

inline XmlsTag  operator /  (const XmlsTag& t1, const XmlsTag& t2)   { XmlsTag r(t1); r.Combine(t2, false); return r; }
inline XmlsTag& operator /= (XmlsTag& tag, const XmlsTag& s)         { tag.Combine(s, false); return tag; }

inline Xmls    operator / (const XmlsTag& tag, String s)             { return tag.ApplyTo(s, false); }
inline Xmls    operator / (const XmlsTag& tag, const char *s)        { return tag.ApplyTo(s, false); }

inline XmlsTag  operator %  (const XmlsTag& t1, const XmlsTag& t2)   { XmlsTag r(t1); r.Combine(t2, true); return r; }
inline XmlsTag& operator %= (XmlsTag& tag, const XmlsTag& s)         { tag.Combine(s, true); return tag; }

inline Xmls    operator % (const XmlsTag& tag, String s)             { return tag.ApplyTo(s, true); }
inline Xmls    operator % (const XmlsTag& tag, const char *s)        { return tag.ApplyTo(s, true); }

END_UPP_NAMESPACE

#endif
