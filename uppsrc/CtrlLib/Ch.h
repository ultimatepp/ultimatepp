void ChStdSkin();
void ChClassicSkin();
void ChHostSkin();

enum {
	CORNER_TOP_LEFT = 0x01,
	CORNER_TOP_RIGHT = 0x02,
	CORNER_BOTTOM_LEFT = 0x04,
	CORNER_BOTTOM_RIGHT = 0x08,
};

void  RoundedRect(Painter& w, double x, double y, double cx, double cy, double rx, double ry, dword corners);
void  RoundedRect(Painter& w, Rectf r, double rx, double ry, dword corner);
Image MakeButton(int radius, const Image& face, int border_width, Color border_color, dword corner);
