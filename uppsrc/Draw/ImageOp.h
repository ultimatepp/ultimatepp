Image CreateImage(Size sz, const RGBA& rgba);
Image CreateImage(Size sz, Color color);
Image SetColorKeepAlpha(const Image& img, Color c);

Image WithHotSpots(const Image& m, int x1, int y1, int x2, int y2);
Image WithHotSpot(const Image& m, int x1, int y1);

void Over(ImageBuffer& dest, Point p, const Image& src, const Rect& srect);
void Copy(ImageBuffer& dest, Point p, const Image& src, const Rect& srect);
void Fill(ImageBuffer& dest, const Rect& rect, RGBA color);

void  Copy(Image& dest, Point p, const Image& src, const Rect& srect);
void  Over(Image& dest, Point p, const Image& src, const Rect& srect);
void  Fill(Image& dest, const Rect& rect, RGBA color);

void  OverStraightOpaque(ImageBuffer& dest, Point p, const Image& src, const Rect& srect);
void  OverStraightOpaque(Image& dest, Point p, const Image& _src, const Rect& srect);

void  Crop(RasterEncoder& tgt, Raster& img, const Rect& rc);
Image Crop(const Image& img, const Rect& rc);
Image Crop(const Image& img, int x, int y, int cx, int cy);

Image AutoCrop(const Image& m, RGBA c);

Image ColorMask(const Image& src, Color transparent);

void  CanvasSize(RasterEncoder& tgt, Raster& img, int cx, int cy);
Image CanvasSize(const Image& img, int cx, int cy);

Image AssignAlpha(const Image& img, const Image& new_alpha);

Image Grayscale(const Image& img);
Image Grayscale(const Image& img, int amount);
Image Contrast(const Image& img, int amount = 256);

Image HorzFadeOut(int cx, int cy, Color color);
Image HorzFadeOut(Size sz, Color color);

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
};

void DrawRasterData(Draw& w, int x, int y, int cx, int cy, const String& data);

bool  Rescale(RasterEncoder& tgt, Size sz, Raster& src, const Rect& src_rc,
              Gate2<int, int> progress = false);
Image Rescale(const Image& src, Size sz, const Rect& src_rc, Gate2<int, int> progress = false);
Image Rescale(const Image& src, Size sz, Gate2<int, int> progress = false);
Image Rescale(const Image& src, int cx, int cy, Gate2<int, int> progress = false);

struct ImageFilter9 {
	virtual RGBA operator()(const RGBA **mx) = 0;
	virtual ~ImageFilter9() {}
};

Image Filter(const Image& img, ImageFilter9& filter);
void  Filter(RasterEncoder& target, Raster& src, ImageFilter9& filter);

Image Etched(const Image& img);
Image Sharpen(const Image& img, int amount = 100);
Image Dither(const Image& m, int dival = 394);

Image RotateClockwise(const Image& img);
Image RotateAntiClockwise(const Image& img);
Image Rotate180(const Image& orig);
Image MirrorHorz(const Image& img);
Image MirrorVert(const Image& img);
Image Rotate(const Image& m, int angle);

Image Colorize(const Image& img, Color color, int alpha = 100);
Image Equalight(const Image& img, int thold = 10);

//Chameleon support
int   Diff(RGBA a, RGBA b);
Image Unglyph(const Image& m, Color& c, double& factor);
Image Unglyph(const Image& m, Color& c);
Image Unglyph(const Image& m);
Image VertBlend(Image img1, Image img2, int y0, int y1);
Image HorzBlend(Image img1, Image img2, int x0, int x1);
Image HorzSymm(Image src);
Image AdjustForDarkBk(const Image& img);

enum {
	IMAGECONTENT_VERTDUP = 1,
	IMAGECONTENT_HORZDUP = 2,
	IMAGECONTENT_OPAQUEBODY = 4,
};

int    ClassifyContent(const Image& m, const Rect& rect);

Image  RecreateAlpha(const Image& overwhite, const Image& overblack);
int    ImageMargin(const Image& m, int p, int dist);
int    ImageMarginV(const Image& _m, int p, int dist);

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

Image Magnify(const Image& img, int nx, int ny);

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

void  SweepMkImageCache();

void  ClearMakeImageCache();
void  SetMakeImageCacheSize(int m);
void  SetMakeImageCacheMax(int m);

Image MakeImagePaintOnly(const ImageMaker& m);

Image RescaleFilter(const Image& img, Size sz, const Rect& sr,
                    double (*kernel_fn)(double x), int kernel_width, Gate2<int, int> progress);
Image RescaleFilter(const Image& img, Size sz,
                    double (*kernel_fn)(double x), int kernel_width, Gate2<int, int> progress);
Image RescaleFilter(const Image& img, int cx, int cy,
                    double (*kernel_fn)(double x), int kernel_width, Gate2<int, int> progress);

enum {
	FILTER_NEAREST = 0,
	FILTER_BILINEAR = 1,
	FILTER_BSPLINE = 2,
	FILTER_COSTELLO = 3,
	FILTER_BICUBIC_MITCHELL = 4,
	FILTER_BICUBIC_CATMULLROM = 5,
	FILTER_LANCZOS2 = 6,
	FILTER_LANCZOS3 = 7,
	FILTER_LANCZOS4 = 8,
	FILTER_LANCZOS5 = 9,
};

Image RescaleFilter(const Image& img, Size sz, const Rect& sr, int filter, Gate2<int, int> progress = false);
Image RescaleFilter(const Image& img, Size sz, int filter, Gate2<int, int> progress = false);
Image RescaleFilter(const Image& img, int cx, int cy, int filter, Gate2<int, int> progress = false);

Image CachedRescale(const Image& m, Size sz, const Rect& src, int filter = Null);
Image CachedRescale(const Image& m, Size sz, int filter = Null);
Image CachedRescalePaintOnly(const Image& m, Size sz, const Rect& src, int filter = Null);
Image CachedRescalePaintOnly(const Image& m, Size sz, int filter = Null);

Image CachedSetColorKeepAlpha(const Image& img, Color color);
Image CachedSetColorKeepAlphaPaintOnly(const Image& img, Color color);

// Obsolete, replace with RescaleFilter!
Image RescaleBicubic(const Image& src, Size sz, const Rect& src_rc, Gate2<int, int> progress = false);
Image RescaleBicubic(const Image& img, Size sz, Gate2<int, int> progress = false);
Image RescaleBicubic(const Image& img, int cx, int cy, Gate2<int, int> progress = false);

void SetUHDMode(bool b = true);
bool IsUHDMode();

Image DPI(const Image& m);
Image DPI(const Image& img, int expected);

inline int   DPI(int a)   { return IsUHDMode() ? 2 * a : a; }
inline Size  DPI(Size sz) { return IsUHDMode() ? 2 * sz : sz; }
