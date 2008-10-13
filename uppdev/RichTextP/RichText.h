#ifndef RICHTEXT_H
#define RICHTEXT_H

#include <CtrlCore/CtrlCore.h>
#include <plugin/png/png.h>

NAMESPACE_UPP

struct Zoom {
	int m, d;

	int operator*(int x) const   { return d ? iscale(x, m, d) : 0; }
	int operator&(int x) const   { int q = d ? iscale(x, m, d) : 0; return x > 0 ? max(q, 1) : q; }

	double AsDouble() const      { return (double)m / d; }

	Zoom()                       { m = d = 1; }
	Zoom(const Nuller&)          { m = d = 0; }
	Zoom(int _m, int _d)         { m = _m, d = _d; }

	void Serialize(Stream& s)    { s % m % d; }

	bool operator==(Zoom a)      { return m == a.m && d == a.d; }
	bool operator!=(Zoom a)      { return m != a.m || d != a.d; }

	friend int operator/(int x, Zoom z)  { return z.m ? iscale(x, z.d, z.m) : 0; }
};

inline bool IsNull(Zoom z) { return (z.m | z.d) == 0; }

inline int operator*(int x, Zoom m)
{
	return m * x;
}

inline void operator*=(int& i, Zoom m)
{
	i = m * i;
}

inline void operator*=(Rect& r, Zoom m)
{
	r.left *= m;
	r.right *= m;
	r.top *= m;
	r.bottom *= m;
}

inline Size operator*(Zoom m, Size sz)
{
	return Size(m * sz.cx, m * sz.cy);
}

inline Size operator/(Size sz, Zoom m)
{
	return Size(sz.cx / m, sz.cy / m);
}

struct PageY : Moveable<PageY, RelOps<PageY> > {
	int page;
	int y;

	PageY(int page, int y) : page(page), y(y) {}
	PageY() { page = y = 0; }

#ifdef _DEBUG
	String ToString() const    { return AsString(page) + ":" + AsString(y); }
#endif
};

inline bool operator<(PageY a, PageY b)
{
	return a.page < b.page ? true : a.page == b.page ? a.y < b.y : false;
}

inline bool operator==(PageY a, PageY b)
{
	return a.page == b.page && a.y == b.y;
}

inline PageY operator+(PageY a, int b)
{
	return PageY(a.page, a.y + b);
}

struct PageRect : public Rect {
	int  page;

	operator int() const                 { return page; }
	operator PageY() const               { return PageY(page, top); }
	PageRect& operator=(const Rect& r)   { (Rect&)(*this) = r; page = 0; return *this; }
	PageRect(const Rect& r)              { (Rect&)(*this) = r; page = 0; }
	PageRect()                           { Clear(); page = 0; }
};

struct PageDraw {
	virtual Draw& Info() = 0;
	virtual Draw& Page(int i) = 0;

	operator Draw&()       { return Info(); }

	virtual ~PageDraw() {}
};

class RichObject;
class Bar;

struct RichObjectType : Moveable<RichObjectType> {
	virtual String GetTypeName(const Value& v) const = 0;
	virtual String GetCreateName() const;
	virtual Size   GetDefaultSize(const Value& data, Size maxsize) const;
	virtual Size   GetPhysicalSize(const Value& data) const;
	virtual Size   GetPixelSize(const Value& data) const;
	virtual void   Paint(const Value& data, Draw& w, Size sz) const;
	virtual Image  ToImage(const Value& data, Size sz) const;
	virtual Value  Read(const String& s) const;
	virtual String Write(const Value& v) const;
	virtual void   Menu(Bar& bar, RichObject& ex) const;
	virtual void   DefaultAction(RichObject& ex) const;
	virtual String GetLink(const Value& data, Point pt, Size sz) const;

	virtual bool   Accept(PasteClip& clip);
	virtual Value  Read(PasteClip& clip);
	virtual String GetClipFmts() const;
	virtual String GetClip(const Value& data, const String& fmt) const;

	RichObjectType();
	virtual ~RichObjectType();
};

class RichObject : Moveable<RichObject> {
	Value                 data;
	int                   ydelta;
	Size                  size;
	Size                  physical_size;
	Size                  pixel_size;
	bool                  keepratio;
	const RichObjectType *type;
	int64                 serial;
	String                type_name;

	static VectorMap<String, RichObjectType *>& Map();

	void                  NewSerial();

public:
	static void   Register(const char *name, RichObjectType *type) init_;
	static int    GetTypeCount()                 { return Map().GetCount(); }
	static int    FindType(const String& name)   { return Map().Find(name); }
	static RichObjectType& GetType(int i)        { return *Map()[i]; }
	static String GetTypeName(int i)             { return Map().GetKey(i); }

	void   SetSize(int cx, int cy)               { size = Size(cx, cy); NewSerial(); }
	void   SetSize(Size sz)                      { size = sz; NewSerial(); }
	Size   GetSize() const                       { return size; }
	void   Paint(Draw& w, Size sz) const;
	Image  ToImage(Size sz) const;
	Size   GetPhysicalSize() const               { return physical_size; }
	Size   GetPixelSize() const                  { return pixel_size; }
	Size   GetDefaultSize(Size maxsize) const    { return type ? type->GetDefaultSize(data, maxsize) : physical_size; }

	void   Set(RichObjectType *type, const Value& data, Size maxsize = Size(3967, 3967));
	bool   Set(const String& type_name, const Value& data, Size maxsize = Size(3967, 3967));

	String GetTypeName() const;
	Value  GetData() const                       { return data; }
	String GetLink(Point pt, Size sz) const      { return type ? type->GetLink(data, pt, sz) : String(); }

	const RichObjectType& GetType() const;

	bool   Read(const String& type, const String& data, Size sz);
	String Write() const                         { return type ? type->Write(data) : (String)data; }

	void   KeepRatio(bool b)                     { keepratio = b; }
	bool   IsKeepRatio() const                   { return keepratio; }

	void   SetYDelta(int yd)                     { ydelta = yd; }
	int    GetYDelta() const                     { return ydelta; }

	void   Menu(Bar& bar)                        { if(type) type->Menu(bar, *this); }
	void   DefaultAction()                       { if(type) type->DefaultAction(*this); }

	operator bool() const                        { return !IsNull(data); }

	void   Clear();

	int64  GetSerialId() const                   { return serial; }
	
	void   InitSize(int cx, int cy);

	RichObject();
	RichObject(RichObjectType *type, const Value& data, Size maxsize = Size(3967, 3967));
	RichObject(const String& type, const Value& data, Size maxsize = Size(3967, 3967));
};

RichObject CreateDrawingObject(const Drawing& dwg, Size dot_size, Size size);
RichObject CreateDrawingObject(const Drawing& dwg, int cx = 0, int cy = 0);
RichObject CreatePNGObject(const Image& img, Size dot_size, Size size);
RichObject CreatePNGObject(const Image& img, Size dot_size, Size size);
RichObject CreateImageObject(const Image& img, int cx = 0, int cy = 0);
RichObject CreateRawImageObject(const String& s, int cx = 0, int cy = 0);

struct RichPara;

enum {
	RICHHOT_LM = -1,
	RICHHOT_RM = -2,
};

struct RichHotPos {
	int table;
	int column;
	int delta;
	int left, cx;
	int textleft, textcx;

	RichHotPos()       { table = 0; column = Null; }
};

struct RichValPos : Moveable<RichValPos> {
	PageY   py;
	int     pos;
	WString data;
};

struct PaintInfo {
	Zoom    zoom;
	int     sell, selh;
	int     tablesel;
	Rect    cells;
	PageY   top;
	PageY   bottom;
	Color   hyperlink;
	Color   indexentry;
	bool    usecache;
	bool    sizetracking;
	Color   showcodes;
	Bits  (*spellingchecker)(const RichPara& para);
	int     highlightpara;
	Color   highlight;
	bool    coloroverride;

	PaintInfo();
};

int LineZoom(Zoom z, int a);

class RichTable;

#include "Para.h"

struct RichPos {
	int              tabtextparti;
	int              tabtextpartcount;
	int              tabposintabtext;
	int              tabtextlen;

	int              table;
	Size             tabsize;
	Point            cell;

	int              tablen;
	int              posintab;

	int              celllen;
	int              posincell;

	int              parai;
	int              partcount;
	int              posinpara;
	int              paralen;

	int              level;
	int              parenttab;

	RichPara::Format format;
	int              chr;
	RichObject       object;
	Id               field;
	String           fieldparam;
	RichPara::CharFormat fieldformat;

#ifdef _DEBUG
	String ToString() const;
#endif

	RichPos();
};

inline bool InSameTxt(const RichPos& a, const RichPos& b)
{
	return a.table == b.table && (a.table == 0 || a.cell == b.cell);
}

struct RichCaret : PageRect {
	int  lineascent;
	int  caretascent;
	int  caretdescent;
	int  objectcy;
	int  objectyd;
	int  line;
	Rect textpage;

	RichCaret()         { lineascent = caretascent = caretdescent = 0; }
};

struct RichStyle {
	RichPara::Format     format;
	String               name;
	Uuid                 next;

	static Uuid              GetDefaultId();
	static const RichStyle&  GetDefault();

	RichStyle()          { next = GetDefaultId(); }
};

typedef ArrayMap<Uuid, RichStyle> RichStyles;

const RichStyle& GetStyle(const RichStyles& s, const Uuid& id);
int   FindStyleWithName(const RichStyles& style, const String& name);

struct RichContext {
	const RichStyles& styles;
	Rect              page;
	PageY             py;

	void              Page() { py.page++; py.y = page.top; }

	RichContext(const RichStyles& styles) : styles(styles) {}
};

struct RichCellPos;

#include "Txt.h"
#include "Table.h"
#include "Text.h"

struct RichCellPos {
	int               pos;

	int               textlen;

	Size              tabsize;
	int               tabpos;
	int               tablen;
	int               cellpos;
	int               celllen;
	int               level;
	RichCell::Format  format;
	RichTable::Format tableformat;

#ifdef _DEBUG
	String ToString() const;
#endif
};

String DeQtf(const char *s);
String DeQtfLf(const char *s);

struct QtfRichObject {
	RichObject obj;

public:
	String ToString() const;

	QtfRichObject() {}
	QtfRichObject(const RichObject& o);
};

String AsQTF(const RichObject& obj);

RichText ParseQTF(const char *qtf, bool scolors = false, byte accesskey = 0);

RichText AsRichText(const wchar *s, const RichPara::Format& f = RichPara::Format());

enum
{
	QTF_BODY = 1,
	QTF_ALL_STYLES = 2,
	QTF_NOSTYLES = 4,
	QTF_CRLF = 8,
	QTF_NOCHARSET = 16,
	QTF_NOLANG = 32,
};

String   AsQTF(const RichText& doc, byte charset = CHARSET_UTF8,
               dword options = QTF_BODY|QTF_ALL_STYLES|QTF_CRLF);

inline String StylesAsQTF(const RichText& doc, byte charset = CHARSET_UTF8)
{ return AsQTF(doc, charset, QTF_ALL_STYLES|QTF_CRLF); }

inline String BodyAsQTF(const RichText& doc, byte charset = CHARSET_UTF8)
{ return AsQTF(doc, charset, QTF_BODY|QTF_CRLF); }

enum
{
	ROUNDOFF          = 1 << 20,
	MAX_FONTS         = 10000,
	MAX_DOTS          = 600 * 100,
	MAX_DOT_HEIGHT    = 1200,
	MAX_POINT_HEIGHT  = MAX_DOT_HEIGHT * 3 / 25,
};

inline int DotTwips (int dots) { return (dots * 12 +  5 * ROUNDOFF +  2) /  5 - ROUNDOFF; }
inline int TwipDots (int twp)  { return (twp  *  5 + 12 * ROUNDOFF +  6) / 12 - ROUNDOFF; }
inline int DotPoints(int dots) { return (dots *  3 + 25 * ROUNDOFF + 12) / 25 - ROUNDOFF; }
inline int PointDots(int pts)  { return (pts  * 25 +  3 * ROUNDOFF +  1) /  3 - ROUNDOFF; }
inline int TwipDotSize(int twp)  { return IsNull(twp) ? 0 : minmax<int>(TwipDots(twp), 0, MAX_DOTS); }
inline int PointDotHeight(int p) { return (minmax<int>(Nvl(p, 0), 0, MAX_POINT_HEIGHT) * 25 + 5) / 6; }

void       EncodeRTF(Stream& stream, const RichText& richtext, byte charset);
String     EncodeRTF(const RichText& richtext, byte charset);
String     EncodeRTF(const RichText& richtext);
RichText   ParseRTF(const char *rtf);

Zoom  GetRichTextStdScreenZoom();

const Display& QTFDisplay();
const Display& QTFDisplayVCenter();

String EncodeHtml(const RichText& text, Index<String>& css,
                  const VectorMap<String, String>& links,
                  const VectorMap<String, String>& labels,
                  const String& path, const String& base = Null, Zoom z = Zoom(8, 40),
                  const VectorMap<String, String>& escape = VectorMap<String, String>(),
                  int imtolerance = 0);
String AsCss(Index<String>& ss);

inline //BW - no labels
String EncodeHtml(const RichText& text, Index<String>& css,
                  const VectorMap<String, String>& links,
                  const String& path, const String& base = Null, Zoom z = Zoom(8, 40)) {
	return EncodeHtml(text, css, links, VectorMap<String, String>(), path, base, z);
}

struct SimplePageDraw : PageDraw {
	Draw& w;

	virtual Draw& Info();
	virtual Draw& Page(int);

	SimplePageDraw(Draw& w) : w(w) {}
	virtual ~SimplePageDraw() {}
};

struct PrintPageDraw : PageDraw {
	int     page;
	Draw&   w;
	NilDraw nw;

	Draw& Page(int _page)     { return page == _page ? w : (Draw&)nw; }
	Draw& Info()              { return w; }
	void  SetPage(int _page)  { page = _page; }

	PrintPageDraw(Draw& w) : w(w) {}
	virtual ~PrintPageDraw() {}
};

END_UPP_NAMESPACE

#endif
