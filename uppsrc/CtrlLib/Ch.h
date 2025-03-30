void ChBaseSkin();

void ChHostSkin();
void ChClassicSkin();
void ChStdSkin();
void ChDarkSkin();
void ChGraySkin();
void ChFlatSkin();
void ChFlatDarkSkin();
void ChFlatGraySkin();

#ifdef PLATFORM_WIN32
void ChHostSkinLight();
void ChHostSkinDark();
#endif

Vector<Tuple<void (*)(), String>> GetAllChSkins();

enum {
	CORNER_TOP_LEFT = 0x01,
	CORNER_TOP_RIGHT = 0x02,
	CORNER_BOTTOM_LEFT = 0x04,
	CORNER_BOTTOM_RIGHT = 0x08,
};

void  RoundedRect(Painter& w, double x, double y, double cx, double cy, double rx, double ry, dword corners);
void  RoundedRect(Painter& w, Rectf r, double rx, double ry, dword corner);
Image MakeElement(Size sz, double radius, const Image& face, double border_width, Color border_color, Event<Painter&, const Rectf&> shape);
Image MakeButton(int radius, const Image& face, double border_width, Color border_color = Null, dword corner = 0xff);
Image MakeButton(int radius, Color face, double border_width, Color border_color = Null, dword corner = 0xff);

Image Hot3(const Image& m); // Adds resizing hotspots at 1/3
Image ChHot(const Image& m, int n = 2); // Adds resizing hotspots DPI(n)
Color AvgColor(const Image& m, const Rect& rr);
Color AvgColor(const Image& m, int margin = 0);
Color AvgColor(const Image& m, RGBA bg, const Rect& rr);
Color AvgColor(const Image& m, RGBA bg, int margin = 0);

Color GetInk(const Image& m); // the color that is most different from AvgColor
int   GetRoundness(const Image& m);
void  FixButton(Image& button); // fix button if it is too close to face color

Image WithRect(Image m, int x, int y, int cx, int cy, Color c);
Image WithLeftLine(const Image& m, Color c, int w = DPI(1));
Image WithRightLine(const Image& m, Color c, int w = DPI(1));
Image WithTopLine(const Image& m, Color c, int w = DPI(1));
Image WithBottomLine(const Image& m, Color c, int w = DPI(1));

Color FaceColor(int adj);

// Creates synthetic style based on SColors and basic button face, returns roundness
int ChSynthetic(Image *button100x100, Color *text, bool macos = false, int dpi = DPI(1));

void  RoundStyleArrows();

Image MakeRoundScrollbarThumb(int width, int margin, Color fill, int stroke = 0, Color pen = Gray());

void SyntheticTab(int i, int roundness, Color ink, int pen);
void SyntheticTab(int i, int roundness, Color ink);

// for diagnostics purposes
#ifdef _DEBUG
void  SetChameleonSample(const Value& m, bool once = true);
Value GetChameleonSample();
#endif
