#include <CtrlLib/CtrlLib.h>
#include <GLDraw/GLDraw.h>
#include <GLCtrl/GLCtrl.h>

using namespace Upp;

struct OpenGLExample : GLCtrl {
	Point point;

	void GLPaint() override {
		Size sz = GetSize();
	
		GLDraw w;
		
		w.Init(sz);

		w.DrawRect(0, 0, sz.cx, sz.cy, SWhite);
		w.DrawRect(10, 10, 30, 30, SRed);
		w.DrawLine(45, 45, 80, 120, 4, Blue);
		w.DrawLine(80, 90, 400, 0, PEN_DASHDOT);
		w.DrawImage(200, 10, CtrlImg::reporticon());
		w.DrawImage(280, 10, CtrlImg::reporticon(), Red());
		const char *text = "This text is centered";
		Size tsz = GetTextSize(text, Arial(25).Bold());
		w.DrawText((sz.cx - tsz.cx) / 2, (sz.cy - tsz.cy) / 2, text, Arial(27).Bold(), SBlue);
		w.Clipoff(200, 50, 95, 100);
		w.DrawText(0, 80, "CLIPPED", Roman(25));
		w.End();
		
		for(int angle = 0; angle < 360; angle += 30)
			w.DrawText(sz.cx - sz.cx / 3, sz.cy - sz.cy / 3, angle * 10, "x   Text with angle " + AsString(angle));
	}

	void MouseMove(Point p, dword) override {
		point = p;
		Refresh();
	}
};

GUI_APP_MAIN
{
	Ctrl::GlobalBackPaint();
	TopWindow win;
	OpenGLExample gl;
	gl.SetFrame(InsetFrame());
	win.Add(gl.HSizePos(10, 10).VSizePos(10, 10));
	win.Sizeable().Zoomable();
	win.Open();
	win.Run();
}
