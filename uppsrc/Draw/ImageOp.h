Image CreateImage(Size sz, const RGBA& rgba);
Image CreateImage(Size sz, Color color);
Image SetColorKeepAlpha(const Image& img, Color c);

void  SetHotSpots(Image& m, Point hotspot, Point hotspot2 = Point(0, 0));
Image WithHotSpots(const Image& m, Point hotspot, Point hotspot2 = Point(0, 0));
Image WithHotSpots(const Image& m, int x1, int y1, int x2, int y2);
Image WithHotSpot(const Image& m, int x1, int y1);

void  ScanOpaque(Image& m);
void DstSrcOp(ImageBuffer& dest, Point p, const Image& src, const Rect& srect,
                           void (*op)(RGBA *t, const RGBA *s, int n), bool co = false);

void Copy(ImageBuffer& dest, Point p, const Image& src, const Rect& srect, bool co = false);

void Over(ImageBuffer& dest, Point p, const Image& src, const Rect& srect, bool co = false);
void Over(Image& dest, Point p, const Image& _src, const Rect& srect, bool co = false);
void Over(Image& dest, const Image& _src, bool co = false);

void Fill(ImageBuffer& dest, const Rect& rect, RGBA color);

void  Copy(Image& dest, Point p, const Image& src, const Rect& srect, bool co = false);
Image GetOver(const Image& dest, const Image& src);
void  Fill(Image& dest, const Rect& rect, RGBA color);

Image Copy(const Image& src, const Rect& srect);

void  OverStraightOpaque(ImageBuffer& dest, Point p, const Image& src, const Rect& srect, bool co = false);
void  OverStraightOpaque(Image& dest, Point p, const Image& _src, const Rect& srect, bool co = false);

void  Crop(RasterEncoder& tgt, Raster& img, const Rect& rc);
Image Crop(const Image& img, const Rect& rc);
Image Crop(const Image& img, int x, int y, int cx, int cy);

Image AddMargins(const Image& img, int left, int top, int right, int bottom, RGBA color = RGBAZero());

Rect  FindBounds(const Image& m, RGBA bg = RGBAZero());
Image AutoCrop(const Image& m, RGBA bg = RGBAZero());
void  AutoCrop(Image *m, int count, RGBA bg = RGBAZero());

void  ClampHotSpots(Image& m);

Image ColorMask(const Image& src, Color transparent);

void  CanvasSize(RasterEncoder& tgt, Raster& img, int cx, int cy);
Image CanvasSize(const Image& img, int cx, int cy);

Image AssignAlpha(const Image& img, const Image& new_alpha);

Image Grayscale(const Image& img);
Image Grayscale(const Image& img, int amount);
Image Contrast(const Image& img, int amount = 256);

Image HorzFadeOut(int cx, int cy, Color color);
Image HorzFadeOut(Size sz, Color color);

void DrawRasterData(Draw& w, int x, int y, int cx, int cy, const String& data);

class RescaleImage {
	Raster       *src;
	Size          tsz;
	Vector<dword> horz;
	Vector<dword> vert;
	void        (*row_proc)(dword *dest, const RGBA *src, const dword *map);
	Size          size;
	int           cx4;
	int           count;
	int           segment;
	int           entry;
	int           step;
	int           segspan;
	bool          bigseg;
	Buffer<dword> row_buffers;
	int           first;
	int           full;
	const dword  *offsets;
	int           offset;
	int           y;

	struct Ln {
		RasterLine line;
		int        ii;
	};

	Ln            cache[4];
	int           cii;
	const RGBA   *GetLine(int i);

public:
	void Create(Size sz, Raster& src, const Rect& src_rc);
	void Get(RGBA *line);

	Image CoRescale(Size sz, const Image& img, const Rect& src_rc);
};

bool  Rescale(RasterEncoder& tgt, Size sz, Raster& src, const Rect& src_rc,
              Gate<int, int> progress = Null);
Image Rescale(const Image& src, Size sz, const Rect& src_rc, Gate<int, int> progress = Null);
Image Rescale(const Image& src, Size sz, Gate<int, int> progress = Null);
Image Rescale(const Image& src, int cx, int cy, Gate<int, int> progress = Null);

struct ImageFilter9 {
	virtual RGBA operator()(const RGBA **mx) = 0;
	virtual ~ImageFilter9() {}
};

Image Filter(const Image& img, ImageFilter9& filter);
void  Filter(RasterEncoder& target, Raster& src, ImageFilter9& filter);

Image Etched(const Image& img);
Image Sharpen(const Image& img, int amount = 100);
Image Dither(const Image& m, int dival = 394);
Image GaussianBlur(const Image& img, int radius, bool co = false);

Image RotateClockwise(const Image& img);
Image RotateAntiClockwise(const Image& img);
Image Rotate180(const Image& orig);
Image MirrorHorz(const Image& img);
Image MirrorVert(const Image& img);
Image Rotate(const Image& m, int angle);
Image Transpose(const Image& img);
Image Transverse(const Image& img);

Image FlipImage(const Image& m, int mode);

Image Colorize(const Image& img, Color color, int alpha = 100);
Image Equalight(const Image& img, int thold = 10);

Image DarkTheme(const Image& img);

//Chameleon support
int   Diff(RGBA a, RGBA b);
Image Unglyph(const Image& m, Color& c, double& factor);
Image Unglyph(const Image& m, Color& c);
Image Unglyph(const Image& m);
Image VertBlend(Image img1, Image img2, int y0, int y1);
Image HorzBlend(Image img1, Image img2, int x0, int x1);
Image HorzSymm(Image src);

bool   IsSingleColor(const Image& m, const Rect& rect);

Image  RecreateAlpha(const Image& overwhite, const Image& overblack);
int    ImageMargin(const Image& m, int p, int dist);
int    ImageMarginV(const Image& _m, int p, int dist);
Rect   GetImageMargins(const Image& m, RGBA margin_color);

struct ChPartMaker {
	Image image;
	Color border;
	Color bg;

	bool  t, b, l, r;
	byte  tl, tr, bl, br;

	void ResetShape();
	Image Make() const;

	ChPartMaker(const Image& m);
};

// Image cache

struct ImageMaker {
	virtual String Key() const = 0;
	virtual Image  Make() const = 0;
	virtual ~ImageMaker() {}
};

void  SysImageRealized(const Image& img); // SystemDraw realized Image handle in GUI
void  SysImageReleased(const Image& img); // SystemDraw dropped Image handle

Image MakeImage(const ImageMaker& m);
Image MakeImage(const Image& image, Image (*make)(const Image& image));

void SweepMkImageCache(); // deprecated, use AdjustValueCache();
void SetMakeImageCacheMax(int m); // deprecated, use SetupValueCache
void SetMakeImageCacheSize(int m); // deprecated, use SetupValueCache

Image MakeImagePaintOnly(const ImageMaker& m);

Image RescaleFilter(const Image& img, Size sz, const Rect& sr,
                    double (*kernel_fn)(double x), int kernel_width, Gate<int, int> progress,
                    bool co);
Image RescaleFilter(const Image& img, Size sz,
                    double (*kernel_fn)(double x), int kernel_width, Gate<int, int> progress,
                    bool co);
Image RescaleFilter(const Image& img, int cx, int cy,
                    double (*kernel_fn)(double x), int kernel_width, Gate<int, int> progress,
                    bool co);

enum {
	FILTER_NEAREST = 0,
	FILTER_BILINEAR = 1,
	FILTER_BSPLINE = 2,
	FILTER_COSTELLO = 3, // (name misspelled)
	FILTER_COSTELLA = 3,
	FILTER_BICUBIC_MITCHELL = 4,
	FILTER_BICUBIC_CATMULLROM = 5,
	FILTER_LANCZOS2 = 6,
	FILTER_LANCZOS3 = 7,
	FILTER_LANCZOS4 = 8,
	FILTER_LANCZOS5 = 9,
};

Tuple2<double (*)(double), int> GetImageFilterFunction(int filter);

struct ImageFilterKernel {
	int        a;
	int        n;
	int        shift;
	int        ashift;
	int        kernel_size;
	const int *kernel;
	double     mul;

	int Get(int x, int dx) const { return kernel[clamp(((x << shift) - dx) * a / n + ashift, 0, kernel_size)]; }

	void Init(double (*kfn)(double x), int a, int src_sz, int tgt_sz);
	void Init(int filter, int src_sz, int tgt_sz);

	ImageFilterKernel() {}
	ImageFilterKernel(double (*kfn)(double x), int a, int src_sz, int tgt_sz);
};

Image RescaleFilter(const Image& img, Size sz, const Rect& sr, int filter, Gate<int, int> progress = Null, bool co = false);
Image RescaleFilter(const Image& img, Size sz, int filter, Gate<int, int> progress = Null);
Image RescaleFilter(const Image& img, int cx, int cy, int filter, Gate<int, int> progress = Null);

Image CoRescaleFilter(const Image& img, Size sz, const Rect& sr, int filter, Gate<int, int> progress = Null);
Image CoRescaleFilter(const Image& img, Size sz, int filter, Gate<int, int> progress = Null);
Image CoRescaleFilter(const Image& img, int cx, int cy, int filter, Gate<int, int> progress = Null);

Image CachedRescale(const Image& m, Size sz, const Rect& src, int filter = Null);
Image CachedRescale(const Image& m, Size sz, int filter = Null);
Image CachedRescalePaintOnly(const Image& m, Size sz, const Rect& src, int filter = Null);
Image CachedRescalePaintOnly(const Image& m, Size sz, int filter = Null);

Image CachedSetColorKeepAlpha(const Image& img, Color color);
Image CachedSetColorKeepAlphaPaintOnly(const Image& img, Color color);

Image Magnify(const Image& img, const Rect& src, int nx, int ny, bool co);
Image Magnify(const Image& img, int nx, int ny, bool co = false);
Image Minify(const Image& img, int nx, int ny, bool co = false);
Image MinifyCached(const Image& img, int nx, int ny, bool co = false);

Image  DownSample3x(const Image& src, bool co = false);
Image  DownSample2x(const Image& src, bool co = false);

Image Upscale2x(const Image& src);
Image Downscale2x(const Image& src);

void SetUHDMode(bool b = true);
bool IsUHDMode();
void SyncUHDMode();

// Image DPI(const Image& m);
Image DPI(const Image& img, int expected);

inline int    DPI(int a)          { return IsUHDMode() ? 2 * a : a; }
inline double DPI(double a)       { return IsUHDMode() ? 2 * a : a; }
inline Size   DPI(Size sz)        { return IsUHDMode() ? 2 * sz : sz; }
inline Size   DPI(int cx, int cy) { return Size(DPI(cx), DPI(cy)); }

inline Image DPI(const Image& a, const Image& b) { return IsUHDMode() ? b : a; }

struct RGBAV {
	dword r, g, b, a;

	void Set(dword v) { r = g = b = a = v; }
	void Clear()      { Set(0); }
	void Put(dword weight, const RGBA& src) {
		r += weight * src.r;
		g += weight * src.g;
		b += weight * src.b;
		a += weight * src.a;
	}
	void Put(const RGBA& src) {
		r += src.r;
		g += src.g;
		b += src.b;
		a += src.a;
	}
	RGBA Get(int div) const {
		RGBA c;
		c.r = byte(r / div);
		c.g = byte(g / div);
		c.b = byte(b / div);
		c.a = byte(a / div);
		return c;
	}
};

// Obsolete, replace with RescaleFilter!
Image RescaleBicubic(const Image& src, Size sz, const Rect& src_rc, Gate<int, int> progress = Null);
Image RescaleBicubic(const Image& img, Size sz, Gate<int, int> progress = Null);
Image RescaleBicubic(const Image& img, int cx, int cy, Gate<int, int> progress = Null);