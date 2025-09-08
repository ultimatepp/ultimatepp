#ifndef RICHTEXT_H
#define RICHTEXT_H

#include <Draw/Draw.h>
#include <Painter/Painter.h>
#include <plugin/png/png.h>

namespace Upp {
	
#define IMAGECLASS RichTextImg
#define IMAGEFILE <RichText/RichText.iml>
#include <Draw/iml_header.h>

INITIALIZE(RichImage);

class  PasteClip;
struct RichPara;
class  RichTable;
class  RichTxt;

inline Color VoidColor() { return Color::Special(223); } // Used for mixed/no change in Get/SetCellFormat

struct Zoom {
	int m, d;

	int operator*(int x) const   { return d ? iscale(x, m, d) : 0; }
	int operator&(int x) const   { int q = d ? iscale(x, m, d) : 0; return x > 0 ? max(q, 1) : q; }

	double AsDouble() const      { return (double)m / d; }
	Zoom   Reciprocal() const    { return Zoom(d, m); }

	Zoom()                       { m = d = 1; }
	Zoom(const Nuller&)          { m = d = 0; }
	Zoom(int _m, int _d)         { m = _m, d = _d; }

	void Serialize(Stream& s)    { s % m % d; }

	bool operator==(Zoom a)      { return m == a.m && d == a.d; }
	bool operator!=(Zoom a)      { return m != a.m || d != a.d; }
	
	String ToString() const      { return String() << m << "/" << d; }

	friend int operator/(int x, Zoom z)  { return z.m ? iscale(x, z.d, z.m) : 0; }
};

#include "Diagram.h"

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

struct PageY : Moveable<PageY>, RelOps<PageY> {
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

struct RichTextLayoutTracer {
	virtual void  Paragraph(const Rect& page, PageY y, const RichPara& para);
	virtual void  EndParagraph(PageY y);
	virtual void  Table(const Rect& page, PageY y, const RichTable& table);
	virtual void  EndTable(PageY y);
	virtual void  TableRow(const Rect& page, PageY y, int i, const RichTable& table);
	virtual void  EndTableRow(PageY y);
	virtual void  TableCell(const Rect& page, PageY y, int i, int j, const RichTable& table, PageY ny);
	virtual void  EndTableCell(PageY y);
};

struct PageDraw {
	virtual Draw& Page(int i) = 0;
	
	RichTextLayoutTracer *tracer;
	
	PageDraw() { tracer = NULL; }

	virtual ~PageDraw() {}
};

class RichObject;
class Bar;

struct RichObjectPaintInfo {
	Color ink = Black();
	bool  dark = false;
	void *context = nullptr;
};

struct RichObjectType {
	virtual String GetTypeName(const Value& v) const = 0;
	virtual String GetCreateName() const;
	virtual Value  Read(const String& s) const;
	virtual String Write(const Value& v) const;
	virtual bool   IsText() const;

	virtual bool   Accept(PasteClip& clip);
	virtual Value  Read(PasteClip& clip);
	virtual String GetClipFmts() const;
	virtual String GetClip(const Value& data, const String& fmt) const;

	virtual Size   GetDefaultSize(const Value& data, Size maxsize, void *context) const;
	virtual Size   GetPhysicalSize(const Value& data, void *context) const;
	virtual Size   GetPixelSize(const Value& data, void *context) const;
	virtual void   Menu(Bar& bar, RichObject& ex, void *context) const;
	virtual void   DefaultAction(RichObject& ex, void *context) const;
	virtual String GetLink(const Value& data, Point pt, Size sz, void *context) const;

	virtual void   Paint(const Value& data, Draw& w, Size sz, const RichObjectPaintInfo& pi) const;
	virtual Image  ToImage(int64 serial_id, const Value& data, Size sz, const RichObjectPaintInfo& pi) const;
	
	Size           StdDefaultSize(const Value& data, Size maxsize, void *context) const;

	RichObjectType();
	virtual ~RichObjectType();
	
protected:
	virtual Size   GetDefaultSize(const Value& data, Size maxsize) const;
	virtual Size   GetPhysicalSize(const Value& data) const;
	virtual Size   GetPixelSize(const Value& data) const;
	virtual void   Menu(Bar& bar, RichObject& ex) const;
	virtual void   DefaultAction(RichObject& ex) const;
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
	void                  AdjustPhysicalSize();

public:
	static void   Register(const char *name, RichObjectType *type) init_;
	static int    GetTypeCount()                 { return Map().GetCount(); }
	static int    FindType(const String& name)   { return Map().Find(name); }
	static RichObjectType& GetType(int i)        { return *Map()[i]; }
	static String GetTypeName(int i)             { return Map().GetKey(i); }
	
	void   SetSize(int cx, int cy)               { size = Size(cx, cy); }
	void   SetSize(Size sz)                      { SetSize(sz.cx, sz.cy); }
	Size   GetSize() const                       { return size; }
	void   Paint(Draw& w, Size sz, const RichObjectPaintInfo& pi = RichObjectPaintInfo()) const;
	Image  ToImage(Size sz, const RichObjectPaintInfo& pi = RichObjectPaintInfo()) const;
	Size   GetPhysicalSize() const               { return physical_size; }
	Size   GetPixelSize() const                  { return pixel_size; }
	Size   GetDefaultSize(Size maxsize, void *context = NULL) const { return type ? type->GetDefaultSize(data, maxsize, context) : physical_size; }

	void   Set(RichObjectType *type, const Value& data, Size maxsize = Size(3967, 3967), void *context = NULL);
	bool   Set(const String& type_name, const Value& data, Size maxsize = Size(3967, 3967), void *context = NULL);
	void   SetData(const Value& v);

	String GetTypeName() const;
	Value  GetData() const                       { return data; }
	String GetLink(Point pt, Size sz, void *context = NULL) const { return type ? type->GetLink(data, pt, sz, context) : String(); }

	const RichObjectType& GetType() const;

	bool   Read(const String& type, const String& data, Size sz, void *context = NULL);
	String Write() const                         { return type ? type->Write(data) : (String)data; }
	bool   IsText() const                        { return type ? type->IsText() : false; }

	void   KeepRatio(bool b)                     { keepratio = b; }
	bool   IsKeepRatio() const                   { return keepratio; }

	void   SetYDelta(int yd)                     { ydelta = yd; }
	int    GetYDelta() const                     { return ydelta; }

	void   Menu(Bar& bar, void *context = NULL)  { if(type) type->Menu(bar, *this, context); }
	void   DefaultAction(void *context = NULL)   { if(type) type->DefaultAction(*this, context); }

	operator bool() const                        { return !IsNull(data); }

	void   Clear();

	int64  GetSerialId() const                   { return serial; }
	
	void   InitSize(int cx, int cy, void *context = NULL);

	RichObject();
	RichObject(RichObjectType *type, const Value& data, Size maxsize = Size(3967, 3967));
	RichObject(const String& type, const Value& data, Size maxsize = Size(3967, 3967));
};

RichObject CreateDrawingObject(const Drawing& dwg, Size dot_size, Size size);
RichObject CreateDrawingObject(const Drawing& dwg, int cx = 0, int cy = 0);
RichObject CreatePaintingObject(const Painting& dwg, Size dot_size, Size size);
RichObject CreatePaintingObject(const Painting& dwg, int cx = 0, int cy = 0);
RichObject CreatePNGObject(const Image& img, Size dot_size, Size size);
RichObject CreatePNGObject(const Image& img, int cx = 0, int cy = 0);
RichObject CreateRawImageObject(const String& s, int cx = 0, int cy = 0);

enum {
	RICHHOT_LM = -1,
	RICHHOT_RM = -2,
};

struct RichHotPos { // used for resizing table dimensions by mouse
	int table;
	int column; // can be RICHHOT_LM or RICHHOT_RM or columns index
	int delta;
	int left, cx;
	int textleft, textcx;

	RichHotPos()       { table = 0; column = Null; left = cx = 0; }
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
	Color   textcolor; // not Null overrides text color, makes paper transparent, useful for QTFDisplay
	bool    indexentrybg;
	bool    usecache;
	bool    sizetracking;
	bool    mono_glyphs;
	Color   showcodes;
	Bits  (*spellingchecker)(const RichPara& para);
	int     highlightpara;
	Gate<const String&> WhenHighlight;
	Color   highlight;
	bool    darktheme;
	void   *context;
	bool    showlabels;
	bool    shrink_oversized_objects;
	void  (*DrawSelection)(Draw& w, int x, int y, int cx, int cy);
	
	Color   ResolveInk(Color ink) const;
	Color   ResolvePaper(Color paper) const;

	PaintInfo();
};

int LineZoom(Zoom z, int a);

class RichTable;
class RichText;
struct RichStyle;
struct RichContext;

typedef ArrayMap<Uuid, RichStyle> RichStyles;

#include "Para.h"

struct RichContext {
	const RichText   *text;
	const RichStyles *styles;
	RichText         *header, *footer;
	int               header_cy, footer_cy; // next page header/footer size
	int               current_header_cy, current_footer_cy; // current header/footer size
	Rect              page;
	PageY             py;
	RichPara::Number  number;

	void              HeaderFooter(RichText *header, RichText *footer_qtf);
	void              AdjustPage();
	void              Page();
	void              Set(PageY p0, const Rect& first_page, const Rect& next_page, PageY p);

	RichContext(const RichStyles& styles, const RichText *text);
	RichContext() {}
};

struct RichPos {
	int              tabtextparti;
	int              tabtextpartcount;
	int              tabposintabtext;
	int              tabtextlen;

	int              table; // current level table index (unique in text) or zero if not in table
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

const RichStyle& GetStyle(const RichStyles& s, const Uuid& id);
int   FindStyleWithName(const RichStyles& style, const String& name);

class RichText;

struct RichCellPos;

enum {
	QTF_BODY = 1,
	QTF_ALL_STYLES = 2,
	QTF_NOSTYLES = 4,
	QTF_CRLF = 8,
	QTF_NOCHARSET = 16,
	QTF_NOLANG = 32,
};

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

String QtfFormat(Color c);

struct QtfRichObject {
	RichObject obj;

public:
	String ToString() const;

	QtfRichObject() {}
	QtfRichObject(const RichObject& o);
};

RichText AsRichText(const RichObject& obj);
String   AsQTF(const RichObject& obj);

bool ParseQTF(RichText& txt, const char *qtf, int accesskey = 0, void *context = NULL);

RichText ParseQTF(const char *qtf, int accesskey = 0, void *context = NULL);

RichText AsRichText(const wchar *s, const RichPara::Format& f = RichPara::Format());

String   AsQTF(const RichText& doc, byte charset = CHARSET_UTF8,
               dword options = QTF_BODY|QTF_ALL_STYLES|QTF_CRLF);

inline String StylesAsQTF(const RichText& doc, byte charset = CHARSET_UTF8)
{ return AsQTF(doc, charset, QTF_ALL_STYLES|QTF_CRLF); }

inline String BodyAsQTF(const RichText& doc, byte charset = CHARSET_UTF8)
{ return AsQTF(doc, charset, QTF_BODY|QTF_CRLF); }


void SetQTF(One<RichText>& txt, const String& qtf);

int GetRichTextScreenStdFontHeight();

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

void  SetRichTextStdScreenZoom(int m, int d);
Zoom  GetRichTextStdScreenZoom();

const Display& QTFDisplay();
const Display& QTFDisplayVCenter();

class HtmlObjectSaver
{
public:
	virtual ~HtmlObjectSaver() {}
	
	virtual String GetHtml(const RichObject& object)                     { return Null; }
	virtual String GetHtml(const RichObject& object, const String& link) { return GetHtml(object); }
};

String EncodeHtml(const RichText& text, Index<String>& css,
                  const VectorMap<String, String>& links,
                  const VectorMap<String, String>& labels,
                  const String& path, const String& base = Null, Zoom z = Zoom(8, 40),
                  const VectorMap<String, String>& escape = VectorMap<String, String>(),
                  int imtolerance = 0);
String EncodeHtml(const RichText& text, Index<String>& css,
                  const VectorMap<String, String>& links,
                  const VectorMap<String, String>& labels,
                  HtmlObjectSaver& object_saver, Zoom z = Zoom(8, 40),
                  const VectorMap<String, String>& escape = VectorMap<String, String>());
String AsCss(Index<String>& ss);

String MakeHtml(const char *title, const String& css, const String& body);

inline //BW - no labels
String EncodeHtml(const RichText& text, Index<String>& css,
                  const VectorMap<String, String>& links,
                  const String& path, const String& base = Null, Zoom z = Zoom(8, 40)) {
	return EncodeHtml(text, css, links, VectorMap<String, String>(), path, base, z);
}

RichText ParseTrivialHtml(const char *html, Font base_font = Serif(int(15 * 600 / 96.0)));

struct SimplePageDraw : PageDraw {
	Draw& w;

	virtual Draw& Page(int);

	SimplePageDraw(Draw& w) : w(w) {}
	virtual ~SimplePageDraw() {}
};

struct PrintPageDraw : PageDraw {
	int     page;
	Draw&   w;
	NilDraw nw;

	Draw& Page(int _page)     { return page == _page ? w : (Draw&)nw; }
	void  SetPage(int _page)  { page = _page; }

	PrintPageDraw(Draw& w) : w(w) {}
	virtual ~PrintPageDraw() {}
};

Array<Drawing> RenderPages(const RichText& txt, Size pagesize = Size(3968, 6074));

String Pdf(const RichText& txt, Size pagesize = Size(3968, 6074), int margin = 200,
           bool pdfa = false, const PdfSignatureInfo *sign = NULL);


}

#endif
