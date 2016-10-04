#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct DockWindow : TopWindow {
	Point      p0;
	Rect       rect0;
	ToolButton close;

	virtual void Paint(Draw& w);
	virtual void LeftDown(Point p, dword keyflags);
	virtual void MouseMove(Point p, dword keyflags);
	virtual void LeftUp(Point p, dword keyflags);
	virtual void Layout();
	
	void DoClose();

	typedef DockWindow CLASSNAME;

	DockWindow();
};

void DockWindow::Layout()
{
	close.RightPos(2, 16).TopPos(2, 16);
}

void DockWindow::Paint(Draw& w)
{
	Rect r = GetSize();

	DrawFrame(w, r, SColorShadow());
	r.Deflate(1);
	DrawFrame(w, r, SColorLight());
	r.Deflate(1);

	int fcy = GetStdFontCy();
	int titlecy = max(fcy, 16) + 4;
	w.DrawRect(r.left, r.top, r.GetWidth(), titlecy, Blend(SColorFace(), SColorShadow()));
	DrawTextEllipsis(w, r.left + fcy / 4, r.top + (titlecy - fcy) / 2,
	                 r.GetWidth() - fcy / 2 - 16, GetTitle(), "...", StdFont(), SColorText());
}

void DockWindow::LeftDown(Point p, dword keyflags)
{
	p0 = GetMousePos();
	rect0 = GetRect();
	SetCapture();
}

void DockWindow::MouseMove(Point p, dword keyflags)
{
	if(HasCapture())
		SetRect(rect0.Offseted(GetMousePos() - p0));
}

void DockWindow::LeftUp(Point p, dword keyflags)
{
}

void DockWindow::DoClose()
{
	WhenClose();
}

DockWindow::DockWindow()
{
	FrameLess();
	Add(close);
	close.Image(CtrlImg::cross());
	close <<= THISBACK(DoClose);
}

GUI_APP_MAIN
{
	DockWindow test;
	test.SetRect(100, 100, 50, 30);
	test.Title("Just a title fun Just a title fun Just a title fun Just a title fun ").Run();
}
