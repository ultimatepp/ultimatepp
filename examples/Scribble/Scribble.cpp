#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <Scribble/Scribble.lay>
#include <CtrlCore/lay.h>

class Scribble : public TopWindow {
public:
	virtual void   Paint(Draw& w);
	virtual void   MouseMove(Point p, dword keyflags);
	virtual void   LeftDown(Point p, dword keyflags);
	virtual void   Layout();

protected:
	Vector< Vector<Point> > image;
	Size       imagesize;

	MenuBar    menu;
	ScrollBars scroll;

	void    Scroll();

	void    SetupSize();
	void    Exit();
	void    About();

	void    FileBar(Bar& bar);
	void    EditBar(Bar& bar);
	void    AboutBar(Bar& bar);
	void    MainBar(Bar& bar);

public:
	typedef Scribble CLASSNAME;

	Scribble();
};

void Scribble::Paint(Draw& w)
{
	Size sz = GetSize();
	Point pos = scroll;
	w.Clip(0, 0, imagesize.cx, imagesize.cy);
	w.DrawRect(0, 0, imagesize.cx, imagesize.cy, White);
	for(int i = 0; i < image.GetCount(); i++) {
		const Vector<Point>& stroke = image[i];
		Point p1 = stroke[0] - pos;
		for(int j = 1; j < stroke.GetCount(); j++) {
			Point p2 = stroke[j] - pos;
			w.DrawLine(p1, p2);
			p1 = p2;
		}
		w.DrawRect(p1.x, p1.y, 1, 1, Black);
	}
	w.End();

	w.DrawRect(imagesize.cx - pos.x, 0, sz.cx - pos.x, sz.cy, SLtGray);
	w.DrawRect(0, imagesize.cy - pos.y, imagesize.cx - pos.x, sz.cy - pos.y, SLtGray);
	w.DrawRect(-pos.x, -pos.y + imagesize.cy, imagesize.cx, 1, SBlack);
	w.DrawRect(-pos.x + imagesize.cx, -pos.y, 1, imagesize.cy, SBlack);
}

void Scribble::MouseMove(Point p, dword keyflags)
{
	if(HasCapture()) {
		image.Top().Add() = scroll + p;
		Refresh();
	}
}

void Scribble::LeftDown(Point p, dword keyflags)
{
	image.Add().Add() = scroll + p;
	SetCapture();
	Refresh();
}

void Scribble::SetupSize()
{
	WithSizeLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Drawing size");
	dlg.cx.Min(1);
	dlg.cy.Min(1);
	dlg.cx <<= imagesize.cx;
	dlg.cy <<= imagesize.cy;
	if(dlg.Run() != IDOK) return;
	imagesize = Size(~dlg.cx, ~dlg.cy);
	scroll.SetTotal(imagesize);
	Refresh();
}

void Scribble::Layout()
{
	scroll.SetPage(GetSize());
	scroll.SetTotal(imagesize);
}

void Scribble::Scroll()
{
	SetFocus();
	Refresh();
}

void Scribble::Exit()
{
	if(PromptOKCancel("Exit Scribble ?"))
		Close();
}

void Scribble::About()
{
	PromptOK("[6R/ Scribble3]&ULTIMATE`+`+ tutorial sample");
}

void Scribble::FileBar(Bar& bar)
{
	bar.Add("Exit", THISBACK(Exit));
}

void Scribble::EditBar(Bar& bar)
{
	bar.Add("Drawing size..", THISBACK(SetupSize));
}

void Scribble::AboutBar(Bar& bar)
{
	bar.Add("About..", THISBACK(About));
}

void Scribble::MainBar(Bar& bar)
{
	bar.Add("File", THISBACK(FileBar));
	bar.Add("Edit", THISBACK(EditBar));
	bar.Add("Help", THISBACK(AboutBar));
}

Scribble::Scribble()
{
	imagesize = Size(600, 400);
	Title("Scribble").Zoomable().Sizeable();
	AddFrame(menu);
	AddFrame(InsetFrame());
	AddFrame(scroll);
	menu.Set(THISBACK(MainBar));
	scroll.WhenScroll = THISBACK(Scroll);
	WhenClose = THISBACK(Exit);
	BackPaint();
}

GUI_APP_MAIN
{
	Scribble().Run();
}
