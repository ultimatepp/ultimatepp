#include <CtrlLib/CtrlLib.h>
#include "GlDraw.h"
#include <GLCtrl/GLCtrl.h>

using namespace Upp;

struct OpenGLExample : GLCtrl {
	Point point;

	virtual void GLPaint() {
		DLOG("GLPaint");
		Size sz = GetSize();
	
		GLDraw w;
		
		Rect r = Rect(GetSize()).Deflated(20, 20);
		
		r = GetSize();
		
		w.InitGL(r);

		w.DrawRect(sz, LtRed());
		DrawFatFrame(w, r, LtBlue(), 2);

		DrawFatFrame(w, r.Deflated(5), LtBlue(), 2);
		
		w.DrawRect(100, 100, 2, 2, Blue());
		
		w.DrawImage(200, 100, CtrlImg::exclamation());
		w.DrawImage(300, 100, CtrlImg::reporticon());		
		w.DrawImage(350, 100, CtrlImg::reporticon(), Blue());
		w.DrawImage(400, 100, CtrlImg::swap_color_cursor());
		
		for(int i = 0; i < 16; i++) {
			w.DrawImage(50 + 20 * i, 180, CtrlImg::checkers());
			w.DrawImage(50 + 20 * i, 200, CtrlImg::checkers(), Rect(0, 0, i, 16));
			w.DrawImage(50 + 20 * i, 220, CtrlImg::checkers(), Rect(0, 0, 16, i));
			w.DrawImage(50 + 20 * i, 240, CtrlImg::checkers(), Rect(0, 0, i, i));
			w.DrawImage(50 + 20 * i, 260, CtrlImg::checkers(), RectC(i, i, 4, 3));
		}
		
		w.DrawRect(50, 50, 20, 20, InvertColor());
		w.DrawRect(55, 55, 8, 8, InvertColor());
		
		GLDraw::ClearCache();
	}

	virtual void MouseMove(Point p, dword) {
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
