#include <CtrlLib/CtrlLib.h>
#include <RichEdit/RichEdit.h>

using namespace Upp;

struct App : public Ctrl {
	EditString x, y;
	ArrayCtrl a, b;
	DropList dl;
	LineEdit editor;
	
	StaticRect popup;
	StaticRect popup2;
	TopWindow  win;
	
	void Paint(Draw& w)
	{
		Size sz = GetSize();
		w.DrawRect(0, 0, sz.cx, sz.cy, SWhite);
		w.DrawRect(10, 10, 30, 30, SRed);
		w.DrawLine(45, 45, 80, 120, 4, Blue);
		w.DrawLine(80, 90, 400, 0, PEN_DASHDOT);
		w.DrawEllipse(200, 200, 50, 100, Green);
		w.DrawImage(200, 10, CtrlImg::HandCursor());
		const char *text = "This text is centered";
		Size tsz = GetTextSize(text, Arial(25).Bold());
		w.DrawText((sz.cx - tsz.cx) / 2, (sz.cy - tsz.cy) / 2, text, Arial(27).Bold(), SBlue);
		w.DrawRect(80, 480, 100, 100, Blue());
		w.Clipoff(200, 50, 95, 100);
		w.DrawText(0, 80, "CLIPPED", Roman(25));
		w.End();
	/*	
		DrawDragRect((SystemDraw&)w,
					 Rect(0, 0, 0, 0),
					 Rect(23, 165, 672, 301),
					 Size(1163, 513),
					 1,
					 Black(),
					 MAKEQWORD(Random(), Random()));*/
	}
	
	void LeftDown(Point p, dword)
	{
		popup.SetRect(p.x, p.y, 100, 100);
		popup.SetForeground();
	}

	void RightDown(Point p, dword)
	{
		win.SetRect(p.x, p.y, 300, 100);
		win.SetForeground();
	}
	
	void InitArray(ArrayCtrl& a)
	{
		a.AddColumn("first");
		a.AddColumn("second");
		for(int i = 0; i < 100; i++)
			a.Add(i, FormatIntRoman(i));
	}

	App()
	{
		x <<= "Hello world!";
		Add(x.LeftPos(100, 100).TopPos(500, 20));
		Add(a.LeftPos(300, 150).TopPos(10, 300));
		Add(dl.LeftPos(10, 300).TopPos(10, 30));
		InitArray(a);
		InitArray(b);
		popup.SetFrame(BlackFrame());
		popup.Add(b.HSizePos(10, 200).VSizePos(10, 10));
		b.SetFrame(ViewFrame());
		popup.Add(y.HSizePos(210, 10).TopPos(10, 30));
		popup.SetRect(800, 100, 100, 100);
		
		popup2.SetRect(500, 50, 300, 400);
		popup2.SetFrame(BlackFrame());
		popup2.Color(Magenta());
		
		win.Title("Title of the window !");
		
		Add(editor.LeftPos(500, 300).TopPos(100, 300));
		editor <<= LoadFile("U:/scorpio.txt");;
		
	//	Add(popup);
		
		for(int i = 0; i < 100; i++)
			dl.Add(i);
//		Sizeable();
	}
};

#define EDITOR 1

GUI_APP_MAIN
{
#if EDITOR
	RichEditWithToolBar app;
	app.SetQTF(LoadFile(ConfigFile("test.qtf")));
#else	
	App app;
	app.popup.PopUp();
//	app.popup2.PopUp(&app.popup);
//	app.win.Open();
#endif

	ChStdSkin();

#if defined(GUI_FB) || defined(GUI_WINGL)
	Ctrl::SetDesktop(app);
	app.SetFocus();
//	Ctrl::SetRenderingMode(MODE_NOAA);
//	Ctrl::SetRenderingMode(MODE_SUBPIXEL);
#else
	//make app a TopWindow, that EventLoop can handle
	//not needed it app is a TopWindow itself
	TopWindow top;
	top.Add(app.SizePos());
	top.Open();
#endif
	Ctrl::EventLoop();
#if EDITOR
	SaveFile(ConfigFile("test.qtf"), app.GetQTF());
#endif
}
