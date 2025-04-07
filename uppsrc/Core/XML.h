String DeXml(const char *s, byte charset = CHARSET_DEFAULT, bool escapelf = false);
String DeXml(const char *s, const char *end, byte charset = CHARSET_DEFAULT, bool escapelf = false);
String DeXml(const String& s, byte charset = CHARSET_DEFAULT, bool escapelf = false);
String XmlPI(const char *text);
String XmlHeader(const char *encoding = "UTF-8", const char *version = "1.0", const char *standalone = NULL);
String XmlDecl(const char *text);
String XmlDocType(const char *text);
String XmlDoc(const char *name, const char *xmlbody);
String XmlComment(const char *text);

class XmlTag : Moveable<XmlTag> {
	String tag;
	String end;

public:
	XmlTag& Tag(const char *s);

	String  operator()();
	String  operator()(const char *text);
	String  operator()(const String& text)                        { return operator()(~text); }
	String  Text(const char *s, byte charset = CHARSET_DEFAULT);
	String  Text(const String& s, byte charset = CHARSET_DEFAULT) { return Text(~s, charset); }
	String  PreservedText(const char *s, byte charset = CHARSET_DEFAULT);
	String  PreservedText(const String& s, byte charset = CHARSET_DEFAULT) { return PreservedText(~s, charset); }

	String  GetBegin() const                                      { return tag + '>'; }
	String  GetEnd() const                                        { return end; }

	XmlTag& operator()(const char *attr, const char *val);
	XmlTag& operator()(const char *attr, int q);
	XmlTag& operator()(const char *attr, double q);
	XmlTag& operator()(const char *attr, float q);

	XmlTag() {}
	XmlTag(const char *tag)                                       { Tag(tag); }
};

enum { XML_DOC, XML_TAG, XML_END, XML_TEXT, XML_DECL, XML_PI, XML_COMMENT, XML_EOF };

struct XmlError : public Exc
{
	XmlError(const char *s) : Exc(s) {}
};

class XmlParser {
	enum {
#ifdef flagTEST_XML // .This is for testing purposes only to increase boundary condition frequency
		MCHARS = 128,
		CHUNK = 256
#else
		MCHARS = 256,
		CHUNK = 16384
#endif
	};

	struct Nesting {
		Nesting(String tag = Null, bool blanks = false) : tag(tag), preserve_blanks(blanks) {}
		String tag;
		bool   preserve_blanks;
	};

	VectorMap<String, String> entity;

	Stream                   *in;
	Buffer<char>              buffer;
	int                       len;
	int                       begincolumn;
	const char               *begin;
	const char               *term;

	String                    attr1, attrval1;
	VectorMap<String, String> attr;
	Array<Nesting>            stack;

	int                       type;
	String                    nattr1, nattrval1;
	VectorMap<String, String> nattr;
	String                    tagtext;
	String                    cdata;
	bool                      empty_tag;
	bool                      npreserve, preserveall;
	bool                      relaxed;
	bool                      raw;

	int                       line;

	byte                      acharset;
	byte                      scharset;

	void                      Init();
	void                      LoadMore0();
	void                      LoadMore()              { if(len - (term - begin) < MCHARS) LoadMore0(); }
	bool                      More();
	bool                      HasMore()               { return *term || More(); }
	void                      Ent(StringBuffer& out);
	void                      Next();
	void                      ReadAttr(StringBuffer& b, int c);
	String                    ReadTag(bool next);
	String                    ReadEnd(bool next);
	String                    ReadDecl(bool next);
	String                    ReadPI(bool next);
	String                    ReadComment(bool next);
	int                       GetColumn0() const;
	String                    Convert(StringBuffer& b);

public:
	void   SkipWhites();

	void   RegisterEntity(const String& id, const String& text);

	bool   IsEof();
	const char *GetPtr() const                                { return term; }

	bool   IsTag();
	String PeekTag()                                          { return ReadTag(false); }
	String ReadTag()                                          { return ReadTag(true); }
	bool   Tag(const char *tag);
	bool   Tag(const String& tag);
	void   PassTag(const char *tag);
	void   PassTag(const String& tag);
	bool   IsEnd();
	String PeekEnd()                                          { return ReadEnd(false); }
	String ReadEnd()                                          { return ReadEnd(true); }
	bool   End();
	bool   End(const char *tag);
	bool   End(const String& tag);
	void   PassEnd();
	void   PassEnd(const char *tag);
	bool   TagE(const char *tag);
	void   PassTagE(const char *tag);
	bool   TagElseSkip(const char *tag);
	bool   LoopTag(const char *tag);

	int    GetAttrCount() const                               { return attr.GetCount() + !IsNull(attr1); }
	String GetAttr(int i) const                               { return i ? attr.GetKey(i - 1) : attr1; }
	bool   IsAttr(const char *id) const                       { return attr1 == id || attr.Find(id) >= 0; }
	String operator[](int i) const                            { return i ? attr[i - 1] : attrval1; }
	String operator[](const char *id) const                   { return attr1 == id ? attrval1 : attr.Get(id, Null); }
	int    Int(const char *id, int def = Null) const;
	double Double(const char *id, double def = Null) const;
	float  Float(const char *id, float def = Null) const;

	bool   IsText();
	String PeekText()                                         { return cdata; }
	String ReadText();
	String ReadTextE();

	bool   IsDecl();
	String PeekDecl()                                         { return ReadDecl(false); }
	String ReadDecl()                                         { return ReadDecl(true); }

	bool   IsPI();
	String PeekPI()                                           { return ReadPI(false); }
	String ReadPI()                                           { return ReadPI(true); }

	bool   IsComment();
	String PeekComment()                                      { return ReadComment(false); }
	String ReadComment()                                      { return ReadComment(true); }

	void   Skip();
	void   SkipEnd();

	VectorMap<String, String> PickAttrs();

	int    GetLine() const                                    { return line; }
	int    GetColumn() const                                  { return GetColumn0() + 1; }

	void   Relaxed(bool b = true)                             { relaxed = b; }
	bool   IsRelaxed() const                                  { return relaxed; }
	void   PreserveAllWhiteSpaces(bool b = true)              { preserveall = b; }
	void   Raw(bool b = true)                                 { raw = b; }

	XmlParser(const char *s);
	XmlParser(Stream& in);
};

class XmlNode : Moveable<XmlNode>, DeepCopyOption<XmlNode> {
	int                              type;
	String                           text;
	Array<XmlNode>                   node;
	One< VectorMap<String, String> > attr;

public:
	static const XmlNode& Void();
	bool           IsVoid() const                             { return this == &Void(); }

	int            GetType() const                            { return type; }
	String         GetText() const                            { return text; }
	String         GetTag() const                             { return text; }
	bool           IsTag() const                              { return type == XML_TAG; }
	bool           IsTag(const char *tag) const               { return IsTag() && text == tag; }
	bool           IsText() const                             { return type == XML_TEXT; }

	void           Clear()                                    { text.Clear(); attr.Clear(); node.Clear(); type = -1; }
	void           CreateTag(const char *tag)                 { type = XML_TAG; text = tag; }
	void           CreateText(const String& txt)              { type = XML_TEXT; text = txt; }
	void           CreatePI(const String& pi)                 { type = XML_PI; text = pi; }
	void           CreateDecl(const String& decl)             { type = XML_DECL; text = decl; }
	void           CreateComment(const String& comment)       { type = XML_COMMENT; text = comment; }
	void           CreateDocument()                           { Clear(); }
	bool           IsEmpty() const                            { return type == XML_DOC && node.GetCount() == 0; }
	operator bool() const                                     { return !IsEmpty(); }

	int            GetCount() const                           { return node.GetCount(); }
	XmlNode&       At(int i)                                  { return node.At(i); }
	const XmlNode& Node(int i) const                          { return node[i]; }
	const XmlNode& operator[](int i) const                    { return i >= 0 && i < node.GetCount() ? node[i] : Void(); }
	const XmlNode& operator[](const char *tag) const;
	XmlNode&       Add()                                      { return node.Add(); }
	void           Remove(int i);
	void           AddText(const String& txt)                 { Add().CreateText(txt); }
	int            FindTag(const char *tag) const;
	XmlNode&       Add(const char *tag);
	XmlNode&       GetAdd(const char *tag);
	XmlNode&       operator()(const char *tag)                { return GetAdd(tag); }
	void           Remove(const char *tag);

	String         GatherText() const;
	String         operator~() const                          { return GatherText(); }
	bool           HasTags() const;

	int            GetAttrCount() const                       { return attr ? attr->GetCount() : 0; }
	String         AttrId(int i) const                        { return attr->GetKey(i); }
	String         Attr(int i) const                          { return (*attr)[i]; }
	String         Attr(const char *id) const                 { return attr ? attr->Get(id, Null) : String(); }
	XmlNode&       SetAttr(const char *id, const String& val);
	int            AttrInt(const char *id, int def = Null) const;
	XmlNode&       SetAttr(const char *id, int val);

	void           SetAttrs(VectorMap<String, String>&& a);

	void           Shrink();

	rval_default(XmlNode);

	XmlNode(const XmlNode& n, int);

	XmlNode()                                                 { type = XML_DOC; }

	typedef Array<XmlNode>::ConstIterator ConstIterator;
	ConstIterator          Begin() const                      { return node.Begin(); }
	ConstIterator          End() const                        { return node.End(); }

	typedef XmlNode        value_type;
	typedef ConstIterator  const_iterator;
	typedef const XmlNode& const_reference;
	typedef int            size_type;
	typedef int            difference_type;
	const_iterator         begin() const                      { return Begin(); }
	const_iterator         end() const                        { return End(); }
};

enum {
	XML_IGNORE_DECLS    = 0x01,
	XML_IGNORE_PIS      = 0x02,
	XML_IGNORE_COMMENTS = 0x04,
};

struct ParseXmlFilter {
	virtual bool DoTag(const String& tag) = 0;
	virtual void EndTag();
};

XmlNode ParseXML(XmlParser& p, dword style = XML_IGNORE_DECLS|XML_IGNORE_PIS|XML_IGNORE_COMMENTS);
XmlNode ParseXML(const char *s, dword style = XML_IGNORE_DECLS|XML_IGNORE_PIS|XML_IGNORE_COMMENTS);
XmlNode ParseXML(Stream& in, dword style = XML_IGNORE_DECLS|XML_IGNORE_PIS|XML_IGNORE_COMMENTS);
XmlNode ParseXMLFile(const char *path, dword style = XML_IGNORE_DECLS|XML_IGNORE_PIS|XML_IGNORE_COMMENTS);

XmlNode ParseXML(XmlParser& p, ParseXmlFilter& filter, dword style = XML_IGNORE_DECLS|XML_IGNORE_PIS|XML_IGNORE_COMMENTS);
XmlNode ParseXML(const char *s, ParseXmlFilter& filter, dword style = XML_IGNORE_DECLS|XML_IGNORE_PIS|XML_IGNORE_COMMENTS);
XmlNode ParseXML(Stream& in, ParseXmlFilter& filter, dword style = XML_IGNORE_DECLS|XML_IGNORE_PIS|XML_IGNORE_COMMENTS);
XmlNode ParseXMLFile(const char *path, ParseXmlFilter& filter, dword style = XML_IGNORE_DECLS|XML_IGNORE_PIS|XML_IGNORE_COMMENTS);

class IgnoreXmlPaths : public ParseXmlFilter {
public:
	virtual bool DoTag(const String& id);
	virtual void EndTag();

private:
	Index<String>  list;
	Vector<String> path;

public:
	IgnoreXmlPaths(const char *s);
};

enum {
	XML_HEADER   = 0x01,
	XML_DOCTYPE  = 0x02,
	XML_PRETTY   = 0x04,
	XML_ESCAPELF = 0x08,
};

void    AsXML(Stream& out, const XmlNode& n, dword style = XML_HEADER|XML_DOCTYPE|XML_PRETTY);
String  AsXML(const XmlNode& n, dword style = XML_HEADER|XML_DOCTYPE|XML_PRETTY);
bool    AsXMLFile(const char *path, const XmlNode& n, dword style = XML_HEADER|XML_DOCTYPE|XML_PRETTY);