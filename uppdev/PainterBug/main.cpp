#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

#define IMAGECLASS Images
#define IMAGEFILE <PainterBug/Pictures.iml>
#include <Draw/iml.h>

struct App : TopWindow
{
	App()
	{
		Sizeable();
		Zoomable();		
	}
	
	virtual void Paint(Draw& w)
	{
		Size sz = GetSize();
		w.DrawRect(sz, White);
		
		
		w.DrawText(10, 2, "GDI");
		w.DrawText(250, 2, "Painter");
		
		int cx = 200;
		int cy = 150;
		
		ChPaint(w, Rect(10, 20, 10 + cx, 20 + cy), Images::WindowBg());
		
		ImageBuffer ib(cx, cy);
		BufferPainter bp(ib);
		
		bp.Clear(RGBAZero());
		
		ChPaint(bp, Rect(0, 0, cx, cy), Images::WindowBg());
		
		w.DrawImage(250, 20, ib);
	}
};

GUI_APP_MAIN
{
	App().Run();
}
