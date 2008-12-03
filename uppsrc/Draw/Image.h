#define NEWIMAGE

enum ImageKind {
	IMAGE_UNKNOWN,
	IMAGE_EMPTY,
	IMAGE_ALPHA,
	IMAGE_MASK,
	IMAGE_OPAQUE,
};

inline bool operator==(const RGBA& a, const RGBA& b)
{
	return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

inline bool operator!=(const RGBA& a, const RGBA& b)
{
	return !(a == b);
}

inline RGBA RGBAZero() { RGBA c; c.r = c.g = c.b = c.a = 0; return c; }

void Fill(RGBA *t, const RGBA& src, int n);
void FillColor(RGBA *t, const RGBA& src, int n);

void Copy(RGBA *t, const RGBA *s, int n);

int  Premultiply(RGBA *t, const RGBA *s, int len);
int  Unmultiply(RGBA *t, const RGBA *s, int len);

void AlphaBlend(RGBA *t, const RGBA *s, int len);
void AlphaBlend(RGBA *t, const RGBA *s, int len, Color color);

void AlphaBlendOpaque(RGBA *t, const RGBA *s, int len, Color color);
void AlphaBlendOpaque(RGBA *t, const RGBA *s, int len);

void AlphaBlendStraight(RGBA *b, const RGBA *f, int len);
void AlphaBlendStraight(RGBA *b, const RGBA *f, int len, Color color);
void AlphaBlendStraightOpaque(RGBA *t, const RGBA *s, int len);
void AlphaBlendStraightOpaque(RGBA *t, const RGBA *s, int len, int alpha);

int  GetChMaskPos32(dword mask);
void AlphaBlendOverBgST(RGBA *b, RGBA bg, int len);

const byte *UnpackRLE(RGBA *t, const byte *src, int len);
String      PackRLE(const RGBA *s, int len);

inline int  Grayscale(int r, int g, int b) { return (77 * r + 151 * g + 28 * b) >> 8; }
inline int  Grayscale(const RGBA& c)       { return Grayscale(c.r, c.g, c.b); }
inline byte Saturate255(int x)             { return byte(~(x >> 24) & (x | (-(x >> 8) >> 24)) & 0xff); }

class  Image;
class  ImageDraw;

class ImageBuffer : NoCopy {
	mutable int  kind;
	Size         size;
	Buffer<RGBA> pixels;
	Point        hotspot;
	Point        spot2;
	Size         dots;

	void         Set(Image& img);
	void         DeepCopy(const ImageBuffer& img);

	RGBA*        Line(int i) const      { ASSERT(i >= 0 && i < size.cy); return (RGBA *)~pixels + i * size.cx; }
	friend void  DropPixels___(ImageBuffer& b) { b.pixels.Clear(); }

	friend class Image;

public:
	void  SetKind(int k)                { kind = k; }
	int   GetKind() const               { return kind; }
	int   ScanKind() const;
	int   GetScanKind() const           { return kind == IMAGE_UNKNOWN ? ScanKind() : kind; }

	void  SetHotSpot(Point p)           { hotspot = p; }
	Point GetHotSpot() const            { return hotspot; }

	void  Set2ndSpot(Point p)           { spot2 = p; }
	Point Get2ndSpot() const            { return spot2; }

	void  SetDots(Size sz)              { dots = sz; }
	Size  GetDots() const               { return dots; }

	Size  GetSize() const               { return size; }
	int   GetWidth() const              { return size.cx; }
	int   GetHeight() const             { return size.cy; }
	int   GetLength() const             { return size.cx * size.cy; }

	RGBA *operator[](int i)             { return Line(i); }
	const RGBA *operator[](int i) const { return Line(i); }
	RGBA *operator~()                   { return pixels; }
	operator RGBA*()                    { return pixels; }
	const RGBA *operator~() const       { return pixels; }
	operator const RGBA*() const        { return pixels; }

	void  Create(int cx, int cy);
	void  Create(Size sz)               { Create(sz.cx, sz.cy); }
	bool  IsEmpty() const               { return (size.cx | size.cy) == 0; }
	void  Clear()                       { Create(0, 0); }

	void  operator=(Image& img);
	void  operator=(ImageBuffer& img);

	ImageBuffer()                       { Create(0, 0); }
	ImageBuffer(int cx, int cy)         { Create(cx, cy); }
	ImageBuffer(Size sz)                { Create(sz.cx, sz.cy); }
	ImageBuffer(Image& img);
	ImageBuffer(ImageDraw& w);
	ImageBuffer(ImageBuffer& b);
};

void Premultiply(ImageBuffer& b);
void Unmultiply(ImageBuffer& b);

void SetSurface(Draw& w, int x, int y, int cx, int cy, const RGBA *pixels);

class Image : public AssignValueTypeNo< Image, 150, Moveable<Image> > {
private:
	struct Data : Link<Data> {
		Atomic refcount;
		int64  serial;
		int    paintcount;

		static Link<Data>            ResData[1];
		static int                   ResCount;

		void   Retain()  { AtomicInc(refcount); }
		void   Release() { if(AtomicDec(refcount) == 0) delete this; }

#ifdef PLATFORM_WIN32
		LPCSTR      cursor_cheat;
		HBITMAP     hbmp;
		HBITMAP     hmask;
		HBITMAP     himg;
		RGBA       *section;

		void CreateHBMP(HDC dc, const RGBA *data);
		int  GetResCount() const { return !!hbmp + !!hmask + !!himg; }
#endif

#ifdef PLATFORM_X11
		int         cursor_cheat;
		XPicture    picture;
		XPicture    picture8;
		int  GetResCount() const { return !!picture; }
#endif

		ImageBuffer buffer;
		bool        paintonly;

		void        SysInit();
		void        SysRelease();
		int         GetKind();
		void        Paint(Draw& w, int x, int y, const Rect& src, Color c);
		void        PaintOnlyShrink();

		Data(ImageBuffer& b);
		~Data();
	};

	Data *data;

	static Link<Image::Data>     ResData[1];
	static int                   ResCount;

	void Set(ImageBuffer& b);

	friend class ImageBuffer;
	friend struct Data;

	friend class Draw;

	void PaintImage(Draw& w, int x, int y, const Rect& src, Color c) const;

	friend void SetPaintOnly___(Image& m);
	friend void DrawImageBandRLE(Draw& w, int x, int y, const Image& m, int minp);

#ifdef PLATFORM_WIN32
#ifndef PLATFORM_WINCE
	void         SetCursorCheat(LPCSTR id)   { data->cursor_cheat = id; }
	LPCSTR       GetCursorCheat() const      { return data ? data->cursor_cheat : NULL; }
	friend Image Win32IconCursor(LPCSTR id, int iconsize, bool cursor);
	friend HICON IconWin32(const Image& img, bool cursor);
#endif
#endif

#ifdef PLATFORM_X11
	void         SetCursorCheat(int id)      { data->cursor_cheat = id; }
	int          GetCursorCheat() const      { return data ? data->cursor_cheat : -1; }
	friend       Cursor X11Cursor(const Image&);
	friend       Image sX11Cursor__(int c);
#endif

public:
	const RGBA*    operator~() const;
	operator const RGBA*() const;
	const RGBA* operator[](int i) const;

	Size  GetSize() const;
	int   GetWidth() const                    { return GetSize().cx; }
	int   GetHeight() const                   { return GetSize().cy; }
	int   GetLength() const;
	Point GetHotSpot() const;
	Point Get2ndSpot() const;
	Size  GetDots() const;
	int   GetKindNoScan() const;
	int   GetKind() const;

	int64 GetSerialId() const                 { return data ? data->serial : 0; }
	bool  IsSame(const Image& img) const      { return GetSerialId() == img.GetSerialId(); }

	bool   operator==(const Image& img) const;
	bool   operator!=(const Image& img) const;
	dword  GetHashValue() const;
	String ToString() const;

	void  Serialize(Stream& s);
	void  Clear();

	Image& operator=(const Image& img);
	Image& operator=(ImageBuffer& img);

	bool IsNullInstance() const         { Size sz = GetSize(); return (sz.cx|sz.cy) == 0; }

	bool IsEmpty() const                { return IsNullInstance(); }
	operator Value() const              { return RichValue<Image>(*this); }

	Image()                             { data = NULL; }
	Image(const Nuller&)                { data = NULL; }
	Image(const Value& src);
	Image(const Image& img);
	Image(Image (*fn)());
	Image(ImageBuffer& b);
	~Image();


	static Image Arrow();
	static Image Wait();
	static Image IBeam();
	static Image No();
	static Image SizeAll();
	static Image SizeHorz();
	static Image SizeVert();
	static Image SizeTopLeft();
	static Image SizeTop();
	static Image SizeTopRight();
	static Image SizeLeft();
	static Image SizeRight();
	static Image SizeBottomLeft();
	static Image SizeBottom();
	static Image SizeBottomRight();
	static Image Cross();
	static Image Hand();

	// IML support ("private"), deprecated - legacy .iml
	struct Init {
		const char *const *scans;
		int16 scan_count;
		char info[24];
	};
	explicit Image(const Init& init);
};

Image Premultiply(const Image& img);
Image Unmultiply(const Image& img);

Vector<Image> UnpackImlData(const void *ptr, int len);
Vector<Image> UnpackImlData(const String& d);

class Iml {
	struct IImage : Moveable<IImage> {
		bool  loaded;
		Image image;

		IImage() { loaded = false; }
	};
	struct Data : Moveable<Data> {
		const char *data;
		int   len, count;
	};
	Vector<Data> data;
	VectorMap<String, IImage> map;
	const Image::Init *img_init;
	const char **name;
	bool  premultiply;

	void  Init(int n);

public:
	void   Enter();
	void   Leave();
	void   Reset();
	int    GetCount() const                  { return map.GetCount(); }
	String GetId(int i)                      { return map.GetKey(i); }
	Image  Get(int i);
	int    Find(const String& s) const       { return map.Find(s); }
	void   Set(int i, const Image& img);
	void   Premultiplied()                   { premultiply = false; }

#ifdef _DEBUG
	int    GetBinSize() const;
#endif

	Iml(const Image::Init *img_init, const char **name, int n);//Deprecated - legacy .iml
	void AddData(const byte *data, int len, int count);
};

void   Register(const char *imageclass, Iml& iml);

int    GetImlCount();
String GetImlName(int i);
Iml&   GetIml(int i);
int    FindIml(const char *name);
Image  GetImlImage(const char *name);
void   SetImlImage(const char *name, const Image& m);

String StoreImageAsString(const Image& img);
Image  LoadImageFromString(const String& s);
Size   GetImageStringSize(const String& src);
Size   GetImageStringDots(const String& src);

#include "Raster.h"
#include "ImageOp.h"

#ifdef PLATFORM_WIN32
#ifndef PLATFORM_WINCE

Image Win32Icon(LPCSTR id, int iconsize = 0);
Image Win32Icon(int id, int iconsize = 0);
Image Win32Cursor(LPCSTR id);
Image Win32Cursor(int id);
HICON IconWin32(const Image& img, bool cursor = false);
Image Win32DllIcon(const char *dll, int ii, bool large);

#endif
#endif

#ifdef PLATFORM_X11
Cursor X11Cursor(const Image& img);
#endif
