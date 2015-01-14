#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	virtual void CancelMode();
	virtual void Paint(Draw& w);
	virtual void DragAndDrop(Point p, PasteClip& d);
	virtual void DragLeave();

	Image img;
	bool  dnd;
	
	MyApp() { dnd = false; }
};

void MyApp::Paint(Draw& w)
{
	Size sz = GetSize();
	w.DrawRect(sz, dnd ? SColorInfo() : SColorFace());
	w.DrawImage(0, 0, img);
}

void MyApp::DragAndDrop(Point p, PasteClip& d)
{
	if(AcceptImage(d))
		img = GetImage(d);
	dnd = d.IsAccepted();
	Refresh();
}

void MyApp::DragLeave()
{
	CancelMode();
}

void MyApp::CancelMode()
{
	dnd = false;
	Refresh();
}

GUI_APP_MAIN
{
	MyApp().Run();
}
