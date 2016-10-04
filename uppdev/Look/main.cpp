#include <CtrlLib/CtrlLib.h>

#define IMAGEFILE "look.iml"
#include <Draw/iml.h>

Image ColorAndScale(const Image& img, Color c, Size sz)
{
	Image m(img.GetSize());
	{
		ImageDraw w(m);
		w.DrawRect(img.GetSize(), c);
		w.DrawImage(0, 0, img);
	}

	PixelArray dest(sz.cx, sz.cy, -3);
	PixelCopyAntiAlias(dest, sz, ImageToPixelArray(m, ScreenInfo(), -3), m.GetSize());
	return PixelArrayToImage(dest);
}

struct Look : TopWindow {
	void LeftDown(Point, dword) { Refresh(); }
	void LeftUp(Point, dword) { Refresh(); }
	void Paint(Draw& w);
};

struct XPButtonPaint {
	Color ohigh, oshade;
	Color frame;
	Color ihigh, ishade;
	Color mark;

	Color topleft, topright, bottomleft, bottomright;

	Rect  r;

	void PaintButtonRecct(Draw& w, Color left, Color top, Color right, Color bottom);
};


void Look::Paint(Draw& w)
{
	w.DrawRect(GetSize(), SColorFace);
	int y = 0;
	int click = GetMouseLeft();
	for(int i = 40; i >= 8; i--) {
		w.DrawRect(0, y + i + 1, 2000, 1, SGray);
		w.DrawText(0, y, AsString(i));
		int x = 50;
		Image img[] = { Dino(), Other(), TheIDE() };
		Color c[] = { SLtGray(), SWhiteGray(), SGray(), SBlue() };
		for(int q = 0; q < __countof(img); q++)
			for(int j = 0; j < 4; j++) {
				w.DrawImage(x + click, y + click, ColorAndScale(img[q], c[j], Size(i - click, i - click)));
				x += 40;
			}
		y += i + 2;
	}
}

GUI_APP_MAIN
{

	BOOL menu_fade;
	BOOL menu_eff;
	SystemParametersInfo(SPI_GETMENUFADE, 0, &menu_fade, 0);
	SystemParametersInfo(SPI_GETMENUANIMATION, 0, &menu_eff, 0);

	DUMP(menu_eff);
	DUMP(menu_fade);

	DropList x;
	x <<= "A";
	x.Add("A");
	x.Disable();
	Look l;
	l.Sizeable().Zoomable().BackPaint();
	l.Open();
//	l.SetAlpha(230);
	l.Add(x.LeftPos(0, 200).TopPos(0, Ctrl::STDSIZE));
	l.Run();
}
