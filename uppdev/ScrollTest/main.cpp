#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGECLASS ImageImg
#define IMAGEFILE <ScrollTest/image.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS ImageImg
#define IMAGEFILE <ScrollTest/image.iml>
#include <Draw/iml_source.h>

Image h;

void InitSb()
{
	ScrollBar::Style& s = ScrollBar::StyleDefault().Write();
	s = s.Standard();
	Image uparrow = CtrlsImg::kUA();
	Size sz = ImageImg::Get(ImageImg::I_BU0).GetSize();
	s.barsize = sz.cx;
	s.arrowsize = sz.cy;

	for(int i = 0; i < 4; i++) {
		Image up = ImageImg::Get(ImageImg::I_BU0 + i);
		s.up.look[i] = ChLookWith(up, uparrow);
		Image vupper = ImageImg::Get(ImageImg::I_TU0 + i);
		s.vupper[i] = vupper;
		Image thumb = ImageImg::Get(ImageImg::I_TH0 + i);
		Image thumbhandle = ImageImg::THH();
		s.vthumb[i] = ChLookWith(thumb, thumbhandle);
		Image vlower = s.vlower[i] = MirrorVert(vupper);
		Image down = MirrorVert(up);
		Image downarrow = MirrorVert(uparrow);
		s.down.look[i] = ChLookWith(down, downarrow);
		s.left.look[i] = ChLookWith(RotateClockwise(down), RotateClockwise(downarrow));
		s.hlower[i] = RotateClockwise(vlower);
		s.hthumb[i] = ChLookWith(RotateClockwise(thumb), RotateClockwise(thumbhandle));
		s.hupper[i] = RotateClockwise(vupper);
		s.right.look[i] = ChLookWith(RotateClockwise(up), RotateClockwise(uparrow));
	}
}

struct Test : TopWindow {
	virtual void Paint(Draw& w)
	{
		w.DrawRect(GetSize(), White());
		ChPaint(w, Rect(10, 10, 100, 20), h);
	}
	
};

struct MyApp : TopWindow {
	virtual void Activate() { Ctrl::ReSkin(); }
	
	ArrayCtrl x;
	
	MyApp() {
		Sizeable();
		x.HeaderObject().Absolute();
		x.AddColumn("", 2000);
		for(int i = 0; i < 1000; i++)
			x.Add(i);
		Add(x.SizePos());		
	}
	
};

GUI_APP_MAIN
{
//	InitSb();
//	Test().Run();

	MyApp().Run();
}
