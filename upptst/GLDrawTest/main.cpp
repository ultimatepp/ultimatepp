#include <CtrlLib/CtrlLib.h>
#include <GLCtrl/GLCtrl.h>
#include <GLDraw/GLDraw.h>

using namespace Upp;

struct OpenGLExample : GLCtrl {
	Point point;

	virtual void GLPaint() {
		DLOG("GLPaint");
		Size sz = GetSize();
	
		DrawGL w(sz);
		
		Rect r = Rect(GetSize()).Deflated(20, 20);
		
		w.DrawRect(sz, White());
		return;
		DrawFatFrame(w, r, LtBlue(), 2);

		DrawFatFrame(w, r.Deflated(5), LtBlue(), 2);
		
		w.DrawRect(100, 100, 2, 2, Blue());
		
		DDUMP(CtrlImg::exclamation());
	
		w.DrawImage(200, 100, CtrlImg::exclamation());
		w.DrawImage(300, 100, CtrlImg::reporticon());		
		w.DrawImage(400, 100, CtrlImg::reporticon(), Blue());
		w.DrawImage(500, 100, CtrlImg::swap_color_cursor());
		
		for(int i = 0; i < 16; i++) {
			w.DrawImage(50 + 20 * i, 180, CtrlImg::checkers());
			w.DrawImage(50 + 20 * i, 200, CtrlImg::checkers(), Rect(0, 0, i, 16));
			w.DrawImage(50 + 20 * i, 220, CtrlImg::checkers(), Rect(0, 0, 16, i));
			w.DrawImage(50 + 20 * i, 240, CtrlImg::checkers(), Rect(0, 0, i, i));
			w.DrawImage(50 + 20 * i, 260, CtrlImg::checkers(), RectC(i, i, 4, 3));
		}
		
		w.DrawRect(50, 50, 20, 20, InvertColor());
		w.DrawRect(55, 55, 8, 8, InvertColor());

	//	w.DrawRect(0, 0, sz.cx, sz.cy, SWhite);
		w.DrawRect(10, 10, 30, 30, SRed);
		w.DrawLine(45, 45, 80, 120, 4, Blue);
		w.DrawLine(80, 90, 400, 0, PEN_DASHDOT);
		w.DrawEllipse(200, 200, 50, 100, Green);
		w.DrawImage(200, 10, CtrlImg::HandCursor());
		const char *text = "This text is centered";
		Size tsz = GetTextSize(text, Arial(25).Bold());
		w.DrawText((sz.cx - tsz.cx) / 2, (sz.cy - tsz.cy) / 2, text, Arial(27).Bold(), SBlue);
		w.Clipoff(200, 50, 95, 100);
		w.DrawText(0, 80, "CLIPPED", Roman(25));
		w.End();

		RichText txt = ParseQTF(LoadFile(GetDataFile("test.qtf")));
		DDUMP(txt.GetLength());
		if(1) {
			RTIMING("SystemDraw");
			DLOG("---------------------------------------");
			txt.Paint(Zoom(2, 10), w, 20, 20, 500);
		}
	
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
