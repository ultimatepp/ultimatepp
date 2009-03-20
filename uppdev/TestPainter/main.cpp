#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

class ImageBuffer__ {
protected:
	ImageBuffer ib;
	
public:
	ImageBuffer__(Size sz) : ib(sz) {}
};

class ImagePainter : private ImageBuffer__, public BufferPainter {
public:
	ImagePainter(Size sz, int mode = MODE_ANTIALIASED) : ImageBuffer__(sz), BufferPainter(ImageBuffer__::ib, mode) {}
	
	Image GetResult() { return ImageBuffer__::ib; }
	operator Image()  { return GetResult(); }
};

class DrawPainter : public ImagePainter {
	Draw& w;

public:
	DrawPainter(Draw& w, int mode = MODE_ANTIALIASED);
	~DrawPainter();
};

DrawPainter::DrawPainter(Draw& w, int mode)
:	ImagePainter(w.GetPagePixels(), mode), w(w)
{}

DrawPainter::~DrawPainter()
{
	w.DrawImage(0, 0, *this);
}

struct App : TopWindow {
	void Paint(Draw& w) {
		DUMP(w.GetPagePixels());
		DUMP(GetSize());
		DrawPainter sw(w);
		sw.Clear(White());
		sw.Rectangle(0, 0, 100, 100).Stroke(2, Red());
		sw.Text(0, 0, "", Arial(20)).Fill(Blue());
	}
};

GUI_APP_MAIN
{
	App().Run();
}

