#ifndef _PDF_pdf_h_
#define _PDF_pdf_h_

#include <Draw/Draw.h>
#include <plugin/z/z.h>
#include <Painter/Painter.h>

namespace Upp {

INITIALIZE(PdfDraw);

class TTFReader {
	struct TTFStream {
		struct Fail {};

		virtual void Raw(void *ptr, int count) = 0;

		TTFStream& operator%(byte& q);
		TTFStream& operator%(int32& q);
		TTFStream& operator%(uint32& q);
		TTFStream& operator%(int16& q);
		TTFStream& operator%(uint16& q);

		virtual ~TTFStream() {}
	};

	struct TTFStreamIn : TTFStream {
		const char *beg;
		const char *s;
		const char *lim;

		virtual void Raw(void *ptr, int count);

		void   Seek(int offset);
		int    Get8();
		int    Get16();
		int    Get32();
		String Get(int n);

		virtual ~TTFStreamIn() {}
	};

	struct TTFStreamOut : TTFStream {
		String out;

		virtual void Raw(void *ptr, int count) { out.Cat((const char *)ptr, count); }

		void   Put8(int data)       { out.Cat(data); }
		void   Put16(int data)      { Put8(HIBYTE(data)); Put8(LOBYTE(data)); }
		void   Put32(int data)      { Put16(HIWORD(data)); Put16(LOWORD(data)); }
		void   Put(const String& s) { out.Cat(s); }

		operator String() const     { return out; }

		virtual ~TTFStreamOut() {}
	};

	Font font;
	String current_table;

	struct Table : Moveable<Table> {
		int offset;
		int length;
	};
	VectorMap<String, Table> table;
	
	VectorMap<wchar, int> glyph_map;

	struct GlyphInfo : Moveable<GlyphInfo> {
		int    offset;
		int    size;
		uint16 advanceWidth;
		int16  leftSideBearing;
	};

	Vector<GlyphInfo> glyphinfo;

	void   SetGlyph(wchar ch, word glyph);

	struct Fail {};

	static void Error() { throw Fail(); }

	int    Peek8(const char *s);
	int    Peek16(const char *s);
	int    Peek32(const char *s);
	int    Peek8(const char *s, int i);
	int    Peek16(const char *s, int i);
	int    Peek32(const char *s, int i);
	int    Read8(const char *&s);
	int    Read16(const char *&s);
	int    Read32(const char *&s);
	String Read(const char *&s, int n);

	void   Reset();

	const char *Seek(const char *tab, int& len);
	const char *Seek(const char *tab);
	void        Seek(const char *tab, TTFStreamIn& s);
	String      GetTable(const char *tab);

	enum {
		ARG_1_AND_2_ARE_WORDS     = (1<<0),
		ARGS_ARE_XY_VALUES        = (1<<1),
		ROUND_XY_TO_GRID          = (1<<2),
		WE_HAVE_A_SCALE           = (1<<3),
		MORE_COMPONENTS           = (1<<5),
		WE_HAVE_AN_X_AND_Y_SCALE  = (1<<6),
		WE_HAVE_A_TWO_BY_TWO      = (1<<7),
		WE_HAVE_INSTRUCTIONS      = (1<<8),
		USE_MY_METRICS            = (1<<9),
	};

	struct NewTable {
		String name;
		String data;
		dword  checksum;
	};

	static void   Add(Array<NewTable>& t, const char *name, const String& data);
	static void   Set(NewTable& m, const String& data);
	static String CreateHeader(const Array<NewTable>& t);

	static inline int    Copy8(TTFStreamOut& out, TTFStreamIn& in)
		{ int q = in.Get8(); out.Put8(q); return q; }
	static inline int    Copy16(TTFStreamOut& out, TTFStreamIn& in)
		{ int q = in.Get16(); out.Put16(q); return q; }
	static inline int    Copy32(TTFStreamOut& out, TTFStreamIn& in)
		{ int q = in.Get32(); out.Put32(q); return q; }
	static inline String Copy(TTFStreamOut& out, TTFStreamIn& in, int n)
		{ String q = in.Get(n); out.Put(q); return q; }

public:
	struct Head {
		dword   version;
		dword   fontRevision;
		uint32  checkSumAdjustment;
		uint32  magicNumber;
		uint16  flags;
		uint16  unitsPerEm;
		byte    created[8];
		byte    modified[8];
		int16   xMin;
		int16   yMin;
		int16   xMax;
		int16   yMax;
		uint16  macStyle;
		uint16  lowestRecPPEM;
		int16   fontDirectionHint;
		int16   indexToLocFormat;
		int16   glyphDataFormat;

		void Serialize(TTFStream& s);
	};

	struct Hhea {
		uint32  version;
		int16   ascent;
		int16   descent;
		int16   lineGap;
		uint16  advanceWidthMax;
		int16   minLeftSideBearing;
		int16   minRightSideBearing;
		int16   xMaxExtent;
		int16   caretSlopeRise;
		int16   caretSlopeRun;
		int16   caretOffset;
		int16   reserved1;
		int16   reserved2;
		int16   reserved3;
		int16   reserved4;
		int16   metricDataFormat;
		uint16  numOfLongHorMetrics;

		void Serialize(TTFStream& s);
	};

	struct Maxp {
		uint32  version;
		uint16  numGlyphs;
		uint16  maxPoints;
		uint16  maxContours;
		uint16  maxComponentPoints;
		uint16  maxComponentContours;
		uint16  maxZones;
		uint16  maxTwilightPoints;
		uint16  maxStorage;
		uint16  maxFunctionDefs;
		uint16  maxInstructionDefs;
		uint16  maxStackElements;
		uint16  maxSizeOfInstructions;
		uint16  maxComponentElements;
		uint16  maxComponentDepth;

		void Serialize(TTFStream& s);
	};

	struct Post {
		uint32  format;
		int32   italicAngle;
		int16   underlinePosition;
		int16   underlineThickness;
		uint16  isFixedPitch;
		uint16  reserved;
		uint32  minMemType42;
		uint32  maxMemType42;
		uint32  minMemType1;
		uint32  maxMemType1;

		void Serialize(TTFStream& s);
	};

	Head   head;
	Hhea   hhea;
	Maxp   maxp;
	Post   post;
	String ps_name;

	int    GetGlyph(wchar chr)               { return glyph_map.Get(chr, 0); }
	word   GetAdvanceWidth(wchar chr)        { int i = glyph_map.Get(chr, 0); return i < glyphinfo.GetCount() ? glyphinfo[GetGlyph(chr)].advanceWidth : 0; }

	String Subset(const Vector<wchar>& chars, int first = 0, bool os2 = false);
	bool   Open(const Font& fnt, bool symbol = false, bool justcheck = false);

	TTFReader();
};

struct PdfSignatureInfo {
	String pkey;
	String cert;

	String name;
	String location;
	String reason;
	String contact_info;

	Time   time;
	
	PdfSignatureInfo() { time = Null; }
};

class PdfDraw : public Draw {
public:
	virtual dword GetInfo() const;
	virtual Size  GetPageSize() const;

	virtual void StartPage();
	virtual void EndPage();

	virtual void BeginOp();
	virtual void EndOp();
	virtual void OffsetOp(Point p);
	virtual bool ClipOp(const Rect& r);
	virtual bool ClipoffOp(const Rect& r);
	virtual bool ExcludeClipOp(const Rect& r);
	virtual bool IntersectClipOp(const Rect& r);
	virtual bool IsPaintingOp(const Rect& r) const;

	virtual	void DrawRectOp(int x, int y, int cx, int cy, Color color);
	virtual void DrawImageOp(int x, int y, int cx, int cy, const Image& img, const Rect& src, Color color);
	virtual void DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color);
	virtual void DrawPolyPolylineOp(const Point *vertices, int vertex_count,
	                                const int *counts, int count_count,
	                                int width, Color color, Color doxor);
	virtual void DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count,
	                                   const int *subpolygon_counts, int scc,
	                                   const int *disjunct_polygon_counts, int dpcc,
	                                   Color color, int width, Color outline,
	                                   uint64 pattern, Color doxor);
	virtual void DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor);
	virtual void DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color);
	virtual void DrawTextOp(int x, int y, int angle, const wchar *text, Font font,
		                    Color ink, int n, const int *dx);

	virtual void Escape(const String& data);

private:
	struct CharPos : Moveable<CharPos>   { word fi, ci; };

	struct OutlineInfo : Moveable<OutlineInfo> {
		bool standard_ttf;
		bool sitalic;
		bool sbold;
	};
	
	struct UrlInfo {
		Rectf  rect;
		String url;
	};

	VectorMap<Font, OutlineInfo>                outline_info;
	VectorMap<Font, Vector<wchar>>              pdffont;
	VectorMap<Font, VectorMap<wchar, CharPos>>  fontchars;
	Index<uint64>                               patterns;
	VectorMap<Tuple2<int64, Rect>, Image>       images;
	Array<Array<UrlInfo>>                       page_url;
	String                                      data; // temporary escape data, e.g. JPEG
	Vector<String>                              jpeg;
	
	Vector<int>  offset;
	StringBuffer out;
	StringBuffer page;
	Size        pgsz;
	Color       rgcolor;
	Color       RGcolor;
	int         fontid;
	double      textht;
	double      linewidth;
	int         margin;
	uint64      patternid;
	bool        pdfa;
	bool        empty;
	String      url;
	Vector<Point> offset_stack;
	Point       current_offset;

	double Pt(double dot)               { return 0.12 * dot; }
	String Ptf(double dot)              { return FormatF(Pt(dot), 5); }

	int    Pos()                        { return offset.GetCount() + 1; }
	int    BeginObj();
	void   EndObj();
	int    PutStream(const String& data, const String& keys = Null, bool compress = true);

	void    PutRect(const Rect& rc);
	void    PutrgColor(Color rg, uint64 pattern = 0);
	void    PutRGColor(Color RG);
	void    PutFontHeight(int fi, double ht);
	void    PutLineWidth(int lw);

	CharPos GetCharPos(Font fnt, wchar chr);
	void    FlushText(int dx, int fi, int height, const String& txt);
	static String PdfColor(Color c);
	static String PdfString(const char *s);

	void PushOffset();
	void PopOffset();

	OutlineInfo GetOutlineInfo(Font fnt);

	void Init(int pagecx, int pagecy, int margin, bool pdfa);

	struct RGlyph : Moveable<RGlyph> {
		String data;
		Size   sz;
		int    x;
		int    color_image = -1;
	};
	
	struct CGlyph : Moveable<CGlyph> {
		Size   sz;
		int    x;
		int    image;
	};
	
	VectorMap<Tuple<Font, int>, CGlyph> color_glyph;

	int    PdfImage(const Image& img, const Rect& src);
	CGlyph ColorGlyph(Font fnt, int chr);
	RGlyph RasterGlyph(Font fnt, int chr);

public:
	String Finish(const PdfSignatureInfo *sign = NULL);
	void   Clear();
	bool   IsEmpty() const                                   { return empty; }
	
	PdfDraw(int pagecx, int pagecy, bool pdfa = false)       { Init(pagecx, pagecy, 0, pdfa); }
	PdfDraw(Size pgsz = Size(5100, 6600), bool pdfa = false) { Init(pgsz.cx, pgsz.cy, 0, pdfa); }
};

void DrawJPEG(Draw& w, int x, int y, int cx, int cy, const String& jpeg_data);

String Pdf(const Array<Drawing>& report, Size pagesize, int margin, bool pdfa = false,
           const PdfSignatureInfo *sign = NULL);

}

#endif
