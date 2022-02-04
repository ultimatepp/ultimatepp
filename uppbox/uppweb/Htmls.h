String      ToHtml(const char *s);

class HtmlTag;

class Htmls : public String, Moveable<Htmls> {
public:
	bool   IsNullInstance() const { return IsEmpty(); }

	Htmls& Text(const char *s);
	Htmls& Quote(const char *text);

	Htmls& Color(class Color color);
	Htmls& Percent(double d);

	Htmls& Color(const char *name, class Color color);
	Htmls& Percent(const char *name, double d);

	Htmls& Attr(const char *attr);
	Htmls& Attr(const char *attr, const char *s);
	Htmls& Attr(const char *attr, String s);
	Htmls& Attr(const char *attr, int i);
	Htmls& Attr(const char *attr, double d);
	Htmls& Attr(const char *attr, Date date);
	Htmls& Attr(const char *attr, Value v);

	Htmls& Attp(const char *attr, double val_or_percent);
	Htmls& Atth(const char *attr, const char *href);

	Htmls& Tag(const char *tag);
	Htmls& Gat();

	Htmls& Cat(const HtmlTag& tag);
	Htmls& Cat(String s)              { String::Cat(s); return *this; }
	Htmls& Cat(const char *s)         { String::Cat(s); return *this; }
	Htmls& Cat(char c)                { String::Cat(c); return *this; }
	Htmls& Cat(const char *s, int n)  { String::Cat(s, n); return *this; }

	Htmls& Nbsp();
	Htmls& Br();
	Htmls& Hr();
	Htmls& Lf();

	Htmls(const char *s) : String(s) {}
	Htmls(const String& s) : String(s) {}
	Htmls() {}
	Htmls(const Nuller&) {}
};

class HtmlTag : Moveable<HtmlTag> {
protected:
	Htmls  tag;
	String end;

public:
	HtmlTag&      Text(const char *s)                        { tag.Text(s); return *this; }
	HtmlTag&      Quote(const char *s)                       { tag.Quote(s); return *this; }

	HtmlTag&      Percent(double d)                          { tag.Percent(d); return *this; }

	HtmlTag&      Color(const char *name, class Color color) { tag.Color(name, color); return *this; }
	HtmlTag&      Percent(const char *name, double d)        { tag.Percent(name, d); return *this; }

	HtmlTag&      Attr(const char *attr)                     { tag.Attr(attr); return *this; }
	HtmlTag&      Attr(const char *attr, const char *v)      { tag.Attr(attr, v); return *this; }
	HtmlTag&      Attr(const char *attr, String v)           { tag.Attr(attr, v); return *this; }
	HtmlTag&      Attr(const char *attr, int v)              { tag.Attr(attr, v); return *this; }
	HtmlTag&      Attr(const char *attr, double v)           { tag.Attr(attr, v); return *this; }
	HtmlTag&      Attr(const char *attr, Date v)             { tag.Attr(attr, v); return *this; }
	HtmlTag&      Attr(const char *attr, const Value& v)     { tag.Attr(attr, v); return *this; }

	HtmlTag&      Attp(const char *attr, double vp)          { tag.Attp(attr, vp); return *this; }
	HtmlTag&      Atth(const char *attr, const char *href)   { tag.Atth(attr, href); return *this; }

	HtmlTag&      Type(const char *s);

	HtmlTag&      Name(const char *s);
	HtmlTag&      Name(String id);

	HtmlTag&      Data(const char *v);
	HtmlTag&      Data(String v);
	HtmlTag&      Data(double d);
	HtmlTag&      Data(Value v);

	HtmlTag&      ReadOnly();
	HtmlTag&      Multiple();
	HtmlTag&      Selected();

	HtmlTag&      Align(Alignment align);
	HtmlTag&      Left();
	HtmlTag&      Right();
	HtmlTag&      Center();

	HtmlTag&      VAlign(Alignment align);
	HtmlTag&      Top();
	HtmlTag&      VCenter();
	HtmlTag&      Bottom();

	HtmlTag&      HotKey(char c);

	HtmlTag&      Size(int c);
	HtmlTag&      Size(int cx, int cy);

	HtmlTag&      Color(class Color color);
	HtmlTag&      BorderColor(class Color color);
	HtmlTag&      BgColor(class Color color);

	HtmlTag&      Face(const char *s);
	HtmlTag&      Border(int i);
	HtmlTag&      CellSpacing(int i);
	HtmlTag&      CellPadding(int i);

	HtmlTag&      Width(double d);
	HtmlTag&      Height(double d);

	HtmlTag&      RowSpan(int i);
	HtmlTag&      ColSpan(int i);

	HtmlTag&      Class(const char *s);
	HtmlTag&      DataFld(const char *s);
	HtmlTag&      DataSrc(const char *s);
	HtmlTag&      Href(const char *s);
	HtmlTag&      HtmlId(const char *s);
	HtmlTag&      Lang(const char *s);
	HtmlTag&      Language(const char *s);
	HtmlTag&      JavaScript();
	HtmlTag&      VbScript();
	HtmlTag&      Methods(const char *s);
	HtmlTag&      Rel(const char *s);
	HtmlTag&      Rev(const char *s);
	HtmlTag&      Style(const char *s);
	HtmlTag&      TabIndex(int i);
	HtmlTag&      Target(const char *s);
	HtmlTag&      TargetBlank();
	HtmlTag&      TargetParent();
	HtmlTag&      TargetSelf();
	HtmlTag&      TargetTop();
	HtmlTag&      Title(const char *s);
	HtmlTag&      Urn(const char *s);
	HtmlTag&      Alt(const char *s);
	HtmlTag&      Coords(const char *s);
	HtmlTag&      NoHref();
	HtmlTag&      Shape(const char *s);
	HtmlTag&      ShapeCirc();
	HtmlTag&      ShapePoly();
	HtmlTag&      ShapeRect();
	HtmlTag&      Balance(double v);
	HtmlTag&      Loop(int i);
	HtmlTag&      Volume(double v);
	HtmlTag&      Src(const char *s);
	HtmlTag&      Alink(class Color c);
	HtmlTag&      Vlink(class Color c);
	HtmlTag&      Link(class Color c);
	HtmlTag&      Background(const char *s);

	HtmlTag&      OnClick(const char *js);

	Htmls         GetTag() const;
	Htmls         GetEndTag() const;

	void          Combine(const HtmlTag& tag);

	Htmls         ApplyTo(String s) const;
	Htmls         ApplyTo(const char *s) const;

	HtmlTag&      PairTag(const char *s);
	HtmlTag&      SingleTag(const char *s);

	HtmlTag       operator()() const                         { return *this; }
	Htmls         operator~() const                          { return ApplyTo(Null); }
	operator      Htmls() const                              { return ApplyTo(Null); }
	String        ToString() const                           { return ApplyTo(Null); }

	const Htmls&  Tag() const                                { return tag; }
	Htmls&        Tag()                                      { return tag; }

	const String& End() const                                { return end; }
	String&       End()                                      { return end; }

	bool          IsEmpty() const                            { return tag.IsEmpty(); }

	HtmlTag(const char *s);
	HtmlTag() {}
};

class HtmlHeader {
public:
	HtmlTag Create();
	
	HtmlHeader& Title(const String& title)             { this->title = title; return *this; }
	HtmlHeader& Description(const String& description) { this->description = description; return *this; }
	HtmlHeader& Css(const String& css)                 { this->css = css; return *this; }
	HtmlHeader& Other(const String& other)             { this->other = other; return *this; }
	
private:
	String title;
	String description;
	String css;
	String other;
};

HtmlTag HtmlSingleTag(const char *s);
HtmlTag HtmlInput(const char *type, const char *name = 0);
HtmlTag HtmlEdit(String name);
HtmlTag HtmlEdit(String name, int size, int maxlength = 256,
                 const char *defaultValue = 0, bool password = false);

HtmlTag HtmlHidden(String name, Value val);
HtmlTag HtmlHidden(const char *name, Value val);
HtmlTag HtmlHidden(Value val);

HtmlTag HtmlSubmit(const char *text);

HtmlTag HtmlSelect(String name);
Htmls   HtmlOption(Value val, const char *text, bool selected = false);

HtmlTag HtmlTable();
HtmlTag HtmlRow();
HtmlTag HtmlCell();

HtmlTag HtmlLink(const char *link);

Htmls   HtmlBlock(Htmls html, double width,
				              double left, double top = 0, double right = 0, double bottom = 0,
				              Alignment align = ALIGN_LEFT);

inline Htmls& operator<<(Htmls& s, const HtmlTag& tag) { s.Cat(tag); return s; }
inline Htmls& operator<<(Htmls& s, const Htmls& v)     { s.Cat(v); return s; }
inline Htmls& operator<<(Htmls& s, String v)           { s.Cat(v); return s; }
inline Htmls& operator<<(Htmls& s, const char *v)      { s.Cat(v); return s; }

inline Htmls& operator+=(Htmls& s, const HtmlTag& tag) { s.Cat(tag); return s; }

Htmls operator+(const HtmlTag& tag1, const HtmlTag& tag2);
/*
Htmls operator+(const String& s, const HtmlTag& tag);
Htmls operator+(const HtmlTag& tag, const String& s);
*/
Htmls operator+(const char *s, const HtmlTag& tag);
Htmls operator+(const HtmlTag& tag, const char *s);
/*
Htmls operator+(const Htmls& s, const HtmlTag& tag);
Htmls operator+(const HtmlTag& tag, const Htmls& s);
*/

HtmlTag operator/(const HtmlTag& t1, const HtmlTag& t2);
HtmlTag& operator/=(HtmlTag& tag, const HtmlTag& s);

inline Htmls operator/(const HtmlTag& tag, String s)         { return tag.ApplyTo(s); }
inline Htmls operator/(const HtmlTag& tag, const char *s)    { return tag.ApplyTo(s); }

//////////////////////////////////////////////////////////////////////
// from htmlutil

HtmlTag        operator % (const HtmlTag& t1, const HtmlTag& t2);
Htmls          operator % (const HtmlTag& tag, const char *s);
inline Htmls   operator % (const HtmlTag& tag, String s)     { return tag % ~s; }

//////////////////////////////////////////////////////////////////////

inline HtmlTag HtmlBold()    { return HtmlTag("B"); }
inline HtmlTag HtmlItalic()  { return HtmlTag("I"); }
inline HtmlTag HtmlBig()     { return HtmlTag("BIG"); }

//////////////////////////////////////////////////////////////////////

HtmlTag        HtmlFontColor(Color c);
HtmlTag        HtmlFontSize(int s);

//////////////////////////////////////////////////////////////////////

inline Htmls   ToHtml(String s)   { return ToHtml(~s); }
HtmlTag        HtmlLink(const char *link, const char *target);
HtmlTag        HtmlImg(String src, String alt = "");

Htmls          HtmlTextArea(String id, Size size, const char *defaultValue = 0);

HtmlTag        HtmlButton(String id, const char *text);
HtmlTag        HtmlWarn(Color color = LtRed);
HtmlTag        HtmlPackedTable();
HtmlTag        HtmlForm(String action, bool multipart = false, String method = "POST");

Htmls          HtmlCheck(String name, bool on);
Htmls          HtmlUpload(String name);

HtmlTag        HtmlTCell();
HtmlTag        HtmlRowCell();
HtmlTag        HtmlRowTCell();

Htmls          HtmlMenu(bool active, const char *text, const char *url, int wd = 0);
HtmlTag        HtmlDialog(int width = -100);
Htmls          HtmlHead(String title, const char *charset = "windows-1250");
Htmls          HtmlPage(String title, const Htmls& body, Color bgcolor = White, const char *charset = "windows-1250");
Htmls          HtmlTitlePage(String title, const Htmls& body, Color bgcolor = White, const char *charset = "windows-1250");

HtmlTag HtmlLine();
HtmlTag HtmlPara();
HtmlTag HtmlCourier(int size);
HtmlTag HtmlArial(int size);
HtmlTag HtmlRoman(int size);
HtmlTag HtmlCourier();
HtmlTag HtmlArial();
HtmlTag HtmlRoman();
HtmlTag HtmlFntSize(int size);
