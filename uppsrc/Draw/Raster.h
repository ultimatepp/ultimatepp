enum {
	RASTER_1,
	RASTER_2,
	RASTER_4,
	RASTER_8,
	RASTER_8ALPHA,
	RASTER_16,
	RASTER_24,
	RASTER_32,
	RASTER_32ALPHA,
	RASTER_32PREMULTIPLIED,

	RASTER_MSBFIRST = 0x80,

	RASTER_MAP_R = 32,
	RASTER_SHIFT_R = 3,
	RASTER_MAP_G = 64,
	RASTER_SHIFT_G = 2,
	RASTER_MAP_B = 16,
	RASTER_SHIFT_B = 4,

	RASTER_MAP_MAX = 64
};

struct PaletteCv {
	Buffer<byte> cv;

	byte *At(int r, int b)           { return cv + (r << 10) + (b << 6); }
	byte  Get(const RGBA& b) const   { return cv[(int(b.r >> RASTER_SHIFT_R) << 10) +
	                                             (int(b.g >> RASTER_SHIFT_G)) +
	                                             (int(b.b >> RASTER_SHIFT_B) << 6)]; }
	PaletteCv()                      { cv.Alloc(RASTER_MAP_R * RASTER_MAP_G * RASTER_MAP_B); }
};

class RasterFormat {
	byte  type;
	dword rmask, gmask, bmask;
	byte  rpos, gpos, bpos, apos;

	static void TailBits(RGBA *t, const byte *src, int cx, byte andm, byte shift, const RGBA *palette);
	static void TailBitsMSB1st(RGBA *t, const byte *src, int cx, byte shift1, byte andm, byte shift, const RGBA *palette);

public:
	void Set1lf()                    { type = RASTER_1; }
	void Set1mf()                    { type = RASTER_1|RASTER_MSBFIRST; }
	void Set2lf()                    { type = RASTER_2; }
	void Set2mf()                    { type = RASTER_2|RASTER_MSBFIRST; }
	void Set4lf()                    { type = RASTER_4; }
	void Set4mf()                    { type = RASTER_4|RASTER_MSBFIRST; }
	void Set8()                      { type = RASTER_8; }
	void Set8A()                     { type = RASTER_8ALPHA; }
	void Set16le(dword rmask, dword gmask, dword bmask);
	void Set16be(dword rmask, dword gmask, dword bmask);
	void Set24le(dword rmask, dword gmask, dword bmask);
	void Set24be(dword rmask, dword gmask, dword bmask);
	void Set32le(dword rmask, dword gmask, dword bmask, dword amask = 0);
	void Set32be(dword rmask, dword gmask, dword bmask, dword amask = 0);
	void Set32leStraight(dword rmask, dword gmask, dword bmask, dword amask);
	void Set32beStraight(dword rmask, dword gmask, dword bmask, dword amask);
	void SetRGBA();
	void SetRGBAStraight();

	int  GetType() const             { return type; }
	int  IsRGBA() const;
	int  GetByteCount(int cx) const;
	int  GetBpp() const;
	bool HasAlpha() const;
	int  GetColorCount() const;
	int  GetPaletteCount() const;

	void Read(RGBA *t, const byte *s, int cx, const RGBA *palette) const;
	void Write(byte *t, const RGBA *s, int cx, const PaletteCv *palcv) const;
};

enum FlipMode {
	FLIP_NONE                 = 0,
	FLIP_MIRROR_HORZ          = 1,
	FLIP_ROTATE_180           = 2,
	FLIP_MIRROR_VERT          = 3,
	FLIP_TRANSPOSE            = 4,
	FLIP_ROTATE_CLOCKWISE     = 5,
	FLIP_TRANSVERSE           = 6,
	FLIP_ROTATE_ANTICLOCKWISE = 7,
};

class Raster {
public:
	class Line {
		mutable const RGBA *data;
		const byte         *fmtdata;
		Raster             *raster;
		mutable bool        free;
		bool                fmtfree;

		friend class       Raster;

		void               MakeRGBA() const;
		void Free()                             { if(free) delete[] data; if(fmtfree) delete[] fmtdata; }
		void Pick(Line&& b);

	public:
		const RGBA         *GetRGBA() const     { if(!data) MakeRGBA(); return data; }
		const byte         *GetRawData() const  { return fmtdata; }
		operator const RGBA *() const           { return GetRGBA(); }
		const RGBA *operator~() const           { return GetRGBA(); }

		Line(const RGBA *data, bool free)
			: data(data), fmtdata((byte *)data), raster(NULL), free(free), fmtfree(false) {}
		Line(const byte *fmtdata, Raster *raster, bool fmtfree)
			: data(NULL), fmtdata(fmtdata), raster(raster), free(false), fmtfree(fmtfree) {}
		Line(Line&& b)                          { Pick(pick(b)); }
		Line()                                  { data = NULL; fmtdata = NULL; raster = NULL; free = fmtfree = false; }
		~Line()                                 { Free(); }

		void operator=(Line&& b)                { Free(); Pick(pick(b)); }
	};

	struct Info {
		int   bpp;
		int   colors;
		Size  dots;
		Point hotspot;
		int   kind;
		int   orientation;
		
		Info();
	};

public:
	virtual void    SeekPage(int page);
	virtual int     GetActivePage() const;
	virtual int     GetPageCount();
	virtual int     GetPageAspect(int n);
	virtual int     GetPageDelay(int n);
	virtual Rect    GetPageRect(int n);
	virtual int		GetPageDisposal(int n);

	virtual bool    Create();
	virtual Size    GetSize() = 0;
	virtual Info    GetInfo();
	virtual Line    GetLine(int line) = 0;
	virtual bool    IsError();
	virtual int     GetPaletteCount();
	virtual const RGBA *GetPalette();
	virtual const RasterFormat *GetFormat();

	virtual Value   GetMetaData(String id);
	virtual void    EnumMetaData(Vector<String>& id_list);

	int    GetWidth()                              { return GetSize().cx; }
	int    GetHeight()                             { return GetSize().cy; }
	Line   operator[](int i)                       { return GetLine(i); }

	Image  GetImage(int x, int y, int cx, int cy, const Gate<int, int> progress = Null);
	Image  GetImage(const Gate<int, int> progress = Null);

	virtual ~Raster();
};

typedef Raster::Line RasterLine;
typedef Raster::Info RasterInfo;

void CreatePaletteCv(const RGBA *palette, int ncolors, PaletteCv& cv_pal);
void CreatePalette(Raster& raster, RGBA *palette, int ncolors);
void CreatePalette(Raster& raster, RGBA *palette, int ncolors, PaletteCv& cv);

class ImageRaster : public Raster {
	Image img;

public:
	virtual Size    GetSize();
	virtual Line    GetLine(int line);
	virtual Info    GetInfo();

	ImageRaster(const Image& img) : img(img) {}
};

class MemoryRaster : public Raster {
	RasterFormat    format;
	Info            info;
	Size            size;
	Vector< Buffer<byte> > lines;
	Vector<RGBA>    palette;

public:
	virtual Size               GetSize()         { return size; }
	virtual Info               GetInfo()         { return info; }
	virtual Line               GetLine(int line);
	virtual int                GetPaletteCount() { return palette.GetCount(); }
	virtual const RGBA         *GetPalette()     { return palette.Begin(); }
	virtual const RasterFormat *GetFormat()      { return &format; }

	void                       Load(Raster& raster);
	int                        GetLength() const;

	MemoryRaster();
	MemoryRaster(Raster& raster)                 { Load(raster); }
};

class StreamRaster : public Raster {
	Stream *s;
	bool    error;

	typedef StreamRaster *(*RasterFactory)();
	template <class T> static StreamRaster *FactoryFn() { return new T; }
	static void AddFormat(RasterFactory f);
	static Vector<void *>& Map();

public:
	Stream& GetStream()                 { return *s; }
	bool    Open(Stream& _s);
	bool    IsError();

	void    SetError()                  { error = true; }

	Image Load(Stream& s, const Gate<int, int> progress = Null);
	Image LoadFile(const char *fn, const Gate<int, int> progress = Null);
	Image LoadString(const String& s, const Gate<int, int> progress = Null);

	template <class T>
	static void Register()              { AddFormat(&StreamRaster::FactoryFn<T>); }

	static One<StreamRaster> OpenAny(Stream& s);
	static Image LoadAny(Stream& s, Gate<int, int> progress = Null);
	static Image LoadFileAny(const char *fn, Gate<int, int> progress = Null);
	static Image LoadStringAny(const String& s, Gate<int, int> progress = Null);

	StreamRaster()                      { error = true; }
};

class RasterEncoder {
	Size           size, dots;
	Point          hotspot;
	RGBA          *line;
	Buffer<byte>   scanline;
	int            line_bytes;
	Buffer<RGBA>   h;
	Buffer<RGBA>   palette;
	One<PaletteCv> palette_cv;

	RGBA          *Pal();

protected:
	RasterFormat   format;
	void           SetLine(RGBA *_line);

public:
	virtual int  GetPaletteCount();
	virtual void Start(Size sz) = 0;
	virtual void WriteLineRaw(const byte *data) = 0;

	void WriteLine();
	void WriteLine(const RGBA *s);

	const RasterFormat& GetFormat() const        { return format; }
	const RGBA         *GetPalette();
	const PaletteCv    *GetPaletteCv()           { return ~palette_cv; }

	operator RGBA *()                            { return line; }
	RGBA *operator~()                            { return line; }
	Size GetSize() const                         { return size; }
	int  GetWidth() const                        { return GetSize().cx; }
	int  GetHeight() const                       { return GetSize().cy; }

	void  SetHotSpot(Point p)                    { hotspot = p; }
	Point GetHotSpot() const                     { return hotspot; }

	void SetDots(Size _dots)                     { dots = _dots; }
	Size GetDots() const                         { return dots; }

	void SetPalette(const RGBA *palette);
	void SetPalette(Raster& pal_raster);

	void Create(Size sz);
	void Create(int cx, int cy)                  { Create(Size(cx, cy)); }

	void Create(Size sz, const RGBA *palette);
	void Create(int cx, int cy, const RGBA *palette);
	void Create(Size sz, Raster& pal_raster);
	void Create(int cx, int cy, Raster& pal_raster);

	RasterEncoder();
	virtual ~RasterEncoder();
};

class ImageEncoder : public RasterEncoder {
	int         ii;
	ImageBuffer ib;

public:
	virtual void Start(Size sz);
	virtual void WriteLineRaw(const byte *data);

	operator const RGBA *()             { return Image(ib); }
	operator Image()                    { return Image(ib); }
};

class StreamRasterEncoder : public RasterEncoder {
	Stream *s;

public:
	Stream& GetStream()                 { return *s; }
	void    SetStream(Stream& _s)       { s = &_s; }

	void    Save(Stream& s, Raster& raster);
	bool    SaveFile(const char *fn, Raster& raster);
	String  SaveString(Raster& raster);

	void    Save(Stream& s, const Image& img);
	bool    SaveFile(const char *fn, const Image& img);
	String  SaveString(const Image& img);
};
