#ifndef __Html_htmld__
#define __Html_htmld__


namespace Upp {

String      ToHtmlD(const char *s);

class HtmlTagD;

class HtmlsD : Moveable<HtmlsD> {
	String text;

public:
	bool   IsNullInstance() const { return text.IsEmpty(); }

	HtmlsD& Text(const char *s);
	HtmlsD& Quote(const char *text);

	HtmlsD& Color(class Color color);
	HtmlsD& Percent(double d);

	HtmlsD& Color(const char *name, class Color color);
	HtmlsD& Percent(const char *name, double d);

	HtmlsD& Attr(const char *attr);
	HtmlsD& Attr(const char *attr, const char *s);
	HtmlsD& Attr(const char *attr, String s);
	HtmlsD& Attr(const char *attr, int i);
	HtmlsD& Attr(const char *attr, double d);
	HtmlsD& Attr(const char *attr, Date date);
	HtmlsD& Attr(const char *attr, Value v);

	HtmlsD& Attp(const char *attr, double val_or_percent);
	HtmlsD& Atth(const char *attr, const char *href);

	HtmlsD& Tag(const char *tag);
	HtmlsD& Gat();

	HtmlsD& Cat(String s)              { text.Cat(s); return *this; }
	HtmlsD& Cat(const char *s)         { text.Cat(s); return *this; }
	HtmlsD& Cat(char c)                { text.Cat(c); return *this; }
	HtmlsD& Cat(const char *s, int n)  { text.Cat(s, n); return *this; }
	HtmlsD& Cat(const HtmlsD& s)       { return Cat(~s); }
	HtmlsD& Cat(const HtmlTagD& tag);

	HtmlsD& Nbsp();
	HtmlsD& Br();
	HtmlsD& Hr();
	HtmlsD& Lf();
	
	String operator~() const           { return text; }

	HtmlsD(const char *s) : text(s) {}
	HtmlsD(const String& s) : text(s) {}
	HtmlsD() {}
	HtmlsD(const Nuller&) {}
};

class HtmlTagD : Moveable<HtmlTagD> {
protected:
	HtmlsD  tag;
	String end;

public:
	HtmlTagD&      Text(const char *s)                        { tag.Text(s); return *this; }
	HtmlTagD&      Quote(const char *s)                       { tag.Quote(s); return *this; }

	HtmlTagD&      Percent(double d)                          { tag.Percent(d); return *this; }

	HtmlTagD&      Color(const char *name, class Color color) { tag.Color(name, color); return *this; }
	HtmlTagD&      Percent(const char *name, double d)        { tag.Percent(name, d); return *this; }

	HtmlTagD&      Attr(const char *attr)                     { tag.Attr(attr); return *this; }
	HtmlTagD&      Attr(const char *attr, const char *v)      { tag.Attr(attr, v); return *this; }
	HtmlTagD&      Attr(const char *attr, String v)           { tag.Attr(attr, v); return *this; }
	HtmlTagD&      Attr(const char *attr, int v)              { tag.Attr(attr, v); return *this; }
	HtmlTagD&      Attr(const char *attr, double v)           { tag.Attr(attr, v); return *this; }
	HtmlTagD&      Attr(const char *attr, Date v)             { tag.Attr(attr, v); return *this; }
	HtmlTagD&      Attr(const char *attr, const Value& v)     { tag.Attr(attr, v); return *this; }

	HtmlTagD&      Attp(const char *attr, double vp)          { tag.Attp(attr, vp); return *this; }
	HtmlTagD&      Atth(const char *attr, const char *href)   { tag.Atth(attr, href); return *this; }

	HtmlTagD&      Type(const char *s);

	HtmlTagD&      Name(const char *s);
	HtmlTagD&      Name(String id);

	HtmlTagD&      Data(const char *v);
	HtmlTagD&      Data(String v);
	HtmlTagD&      Data(double d);
	HtmlTagD&      Data(Value v);

	HtmlTagD&      ReadOnly();
	HtmlTagD&      Multiple();
	HtmlTagD&      Selected();

	HtmlTagD&      Align(Alignment align);
	HtmlTagD&      Left();
	HtmlTagD&      Right();
	HtmlTagD&      Center();

	HtmlTagD&      VAlign(Alignment align);
	HtmlTagD&      Top();
	HtmlTagD&      VCenter();
	HtmlTagD&      Bottom();

	HtmlTagD&      HotKey(char c);

	HtmlTagD&      Size(int c);
	HtmlTagD&      Size(int cx, int cy);

	HtmlTagD&      Color(class Color color);
	HtmlTagD&      BorderColor(class Color color);
	HtmlTagD&      BgColor(class Color color);

	HtmlTagD&      Face(const char *s);
	HtmlTagD&      Border(int i);
	HtmlTagD&      CellSpacing(int i);
	HtmlTagD&      CellPadding(int i);

	HtmlTagD&      Width(double d);
	HtmlTagD&      Height(double d);

	HtmlTagD&      RowSpan(int i);
	HtmlTagD&      ColSpan(int i);

	HtmlTagD&      Class(const char *s);
	HtmlTagD&      DataFld(const char *s);
	HtmlTagD&      DataSrc(const char *s);
	HtmlTagD&      Href(const char *s);
	HtmlTagD&      HtmlId(const char *s);
	HtmlTagD&      Lang(const char *s);
	HtmlTagD&      Language(const char *s);
	HtmlTagD&      JavaScript();
	HtmlTagD&      VbScript();
	HtmlTagD&      Methods(const char *s);
	HtmlTagD&      Rel(const char *s);
	HtmlTagD&      Rev(const char *s);
	HtmlTagD&      Style(const char *s);
	HtmlTagD&      TabIndex(int i);
	HtmlTagD&      Target(const char *s);
	HtmlTagD&      TargetBlank();
	HtmlTagD&      TargetParent();
	HtmlTagD&      TargetSelf();
	HtmlTagD&      TargetTop();
	HtmlTagD&      Title(const char *s);
	HtmlTagD&      Urn(const char *s);
	HtmlTagD&      Alt(const char *s);
	HtmlTagD&      Coords(const char *s);
	HtmlTagD&      NoHref();
	HtmlTagD&      Shape(const char *s);
	HtmlTagD&      ShapeCirc();
	HtmlTagD&      ShapePoly();
	HtmlTagD&      ShapeRect();
	HtmlTagD&      Balance(double v);
	HtmlTagD&      Loop(int i);
	HtmlTagD&      Volume(double v);
	HtmlTagD&      Src(const char *s);
	HtmlTagD&      Alink(class Color c);
	HtmlTagD&      Vlink(class Color c);
	HtmlTagD&      Link(class Color c);
	HtmlTagD&      Background(const char *s);

	HtmlTagD&      OnClick(const char *js);

	HtmlsD         GetTag() const;
	HtmlsD         GetEndTag() const;

	void          Combine(const HtmlTagD& tag);

	HtmlsD         ApplyTo(String s) const;
	HtmlsD         ApplyTo(const char *s) const;

	HtmlTagD&      PairTag(const char *s);
	HtmlTagD&      SingleTag(const char *s);

	HtmlTagD       operator()() const                         { return *this; }
	HtmlsD         operator~() const                          { return ApplyTo(Null); }
	operator       HtmlsD() const                             { return ApplyTo(Null); }
	String         ToString() const                           { return ~ApplyTo(Null); }

	const HtmlsD&  Tag() const                                { return tag; }
	HtmlsD&        Tag()                                      { return tag; }

	const String& End() const                                { return end; }
	String&       End()                                      { return end; }

	bool          IsEmpty() const                            { return IsNull(tag); }

	HtmlTagD(const char *s);
	HtmlTagD() {}
};

HtmlTagD HtmlSingleTagD(const char *s);
HtmlTagD HtmlInputD(const char *type, const char *name = 0);
HtmlTagD HtmlEditD(String name);
HtmlTagD HtmlEditD(String name, int size, int maxlength = 256,
                 const char *defaultValue = 0, bool password = false);

HtmlTagD HtmlHiddenD(String name, Value val);
HtmlTagD HtmlHiddenD(const char *name, Value val);
HtmlTagD HtmlHiddenD(Value val);

HtmlTagD HtmlSubmitD(const char *text);

HtmlTagD HtmlSelectD(String name);
HtmlsD   HtmlOptionD(Value val, const char *text, bool selected = false);

HtmlTagD HtmlTableD();
HtmlTagD HtmlRowD();
HtmlTagD HtmlCellD();

HtmlTagD HtmlHeaderD(const char *title, String css = Null, const char *other = NULL);

//HtmlTagD HtmlFontD(Font font);

HtmlTagD HtmlLinkD(const char *link);

HtmlsD   HtmlBlockD(HtmlsD html, double width,
				              double left, double top = 0, double right = 0, double bottom = 0,
				              Alignment align = ALIGN_LEFT);

inline HtmlsD& operator<<(HtmlsD& s, const HtmlTagD& tag) { s.Cat(tag); return s; }
inline HtmlsD& operator<<(HtmlsD& s, const HtmlsD& v)     { s.Cat(v); return s; }
inline HtmlsD& operator<<(HtmlsD& s, String v)           { s.Cat(v); return s; }
inline HtmlsD& operator<<(HtmlsD& s, const char *v)      { s.Cat(v); return s; }

inline HtmlsD& operator+=(HtmlsD& s, const HtmlTagD& tag) { s.Cat(tag); return s; }

HtmlsD operator+(const HtmlTagD& tag1, const HtmlTagD& tag2);
HtmlsD operator+(const String& s, const HtmlTagD& tag);
HtmlsD operator+(const HtmlTagD& tag, const String& s);
HtmlsD operator+(const char *s, const HtmlTagD& tag);
HtmlsD operator+(const HtmlTagD& tag, const char *s);

HtmlsD operator+(const HtmlsD& s, const HtmlTagD& tag);
HtmlsD operator+(const HtmlTagD& tag, const HtmlsD& s);
HtmlsD operator+(const HtmlsD& a, const char *b);
HtmlsD operator+(const char *a, const HtmlsD& b);
HtmlsD operator+(const HtmlsD& a, const String& b);
HtmlsD operator+(const String& a, const HtmlsD& b);
HtmlsD operator+(const HtmlsD& a, const HtmlsD& b);

HtmlTagD operator/(const HtmlTagD& t1, const HtmlTagD& t2);
HtmlTagD& operator/=(HtmlTagD& tag, const HtmlTagD& s);

inline HtmlsD operator/(const HtmlTagD& tag, const HtmlsD& s)   { return tag.ApplyTo(~s); }
inline HtmlsD operator/(const HtmlTagD& tag, String s)          { return tag.ApplyTo(s); }
inline HtmlsD operator/(const HtmlTagD& tag, const char *s)     { return tag.ApplyTo(s); }

//////////////////////////////////////////////////////////////////////
// from htmlutil

HtmlTagD        operator % (const HtmlTagD& t1, const HtmlTagD& t2);
HtmlsD          operator % (const HtmlTagD& tag, const char *s);
inline HtmlsD   operator % (const HtmlTagD& tag, String s)     { return tag % ~s; }

//////////////////////////////////////////////////////////////////////

inline HtmlTagD HtmlBoldD()    { return HtmlTagD("B"); }
inline HtmlTagD HtmlItalicD()  { return HtmlTagD("I"); }
inline HtmlTagD HtmlBigD()     { return HtmlTagD("BIG"); }

//////////////////////////////////////////////////////////////////////

HtmlTagD        HtmlFontColorD(Color c);
HtmlTagD        HtmlFontSizeD(int s);

//////////////////////////////////////////////////////////////////////

inline HtmlsD   ToHtmlD(String s)   { return ToHtmlD(~s); }
HtmlTagD        HtmlLinkD(const char *link, const char *target);
HtmlTagD        HtmlImgD(String src, String alt = "");

HtmlsD          HtmlTextAreaD(String id, Size size, const char *defaultValue = 0);

HtmlTagD        HtmlButtonD(String id, const char *text);
HtmlTagD        HtmlWarnD(Color color = LtRed);
HtmlTagD        HtmlPackedTableD();
HtmlTagD        HtmlFormD(String action, bool multipart = false, String method = "POST");

HtmlsD          HtmlCheckD(String name, bool on);
HtmlsD          HtmlUploadD(String name);

HtmlTagD        HtmlTCellD();
HtmlTagD        HtmlRowCellD();
HtmlTagD        HtmlRowTCellD();

HtmlsD          HtmlMenuD(bool active, const char *text, const char *url, int wd = 0);
HtmlTagD        HtmlDialogD(int width = -100);
HtmlsD          HtmlHeadD(String title, const char *charset = "windows-1250");
HtmlsD          HtmlPageD(String title, const HtmlsD& body, Color bgcolor = White, const char *charset = "windows-1250");
HtmlsD          HtmlTitlePageD(String title, const HtmlsD& body, Color bgcolor = White, const char *charset = "windows-1250");

HtmlTagD HtmlLineD();
HtmlTagD HtmlParaD();
HtmlTagD HtmlCourierD(int size);
HtmlTagD HtmlArialD(int size);
HtmlTagD HtmlRomanD(int size);
HtmlTagD HtmlCourierD();
HtmlTagD HtmlArialD();
HtmlTagD HtmlRomanD();
HtmlTagD HtmlFntSizeD(int size);

}

#endif