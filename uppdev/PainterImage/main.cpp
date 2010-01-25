#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

namespace Upp {
Image DownScale(const Image& img, int nx, int ny);
};

struct App : TopWindow {
	void Paint(Draw& w) {
		Size sz = GetSize();
		ImageDraw idw(1200, 600);
		idw.DrawRect(0, 0, 1200, 600, White);
		idw.DrawText(0, 0, "(Hello)", Serif(400));
		

#if 0		
		Image m = idw;
		Size isz = m.GetSize();
		m = DownScale(m, isz.cx / sz.cx, isz.cy / sz.cy);
		w.DrawRect(sz, White());
		w.DrawImage(0, 0, m);
#endif
		
		DrawPainter sw(w, GetSize());
		sw.Clear(White());
		sw.DrawImage(0, 0, sz.cx, sz.cy, idw);
	}
};

GUI_APP_MAIN
{
	ImageDraw idw(1200, 600);
	idw.DrawRect(0, 0, 1200, 600, White);
	idw.DrawText(0, 0, "(Hello)", Serif(400));
	
	DrawingDraw dw(350, 250);
	dw.DrawImage(0, 0, 350, 250, idw); 
	
	QtfRichObject pict(CreateDrawingObject(dw.GetResult(), Size(350, 250), Size(350, 250)));
	
//	/**/	PNGEncoder png;	
//	/**/	png.SaveFile("c:\\kk.png", pict.obj.ToImage(Size(350, 250)));
	
	PromptOK(String("[A5 Rendering problem: ") + pict.ToString());

	App().Sizeable().Run();
}
