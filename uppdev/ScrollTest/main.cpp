#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGECLASS ImageImg
#define IMAGEFILE <ScrollTest/image.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS ImageImg
#define IMAGEFILE <ScrollTest/image.iml>
#include <Draw/iml_source.h>

void InitSb()
{
	ScrollBar::Style& s = ScrollBar::StyleDefault().Write();
	s = s.Standard();
	for(int i = 0; i < 4; i++) {
		s.vupper[i] = ImageImg::Get(ImageImg::I_TU0 + i);
		s.vlower[i] = MirrorVert(s.vupper[i]);
		s.vthumb[i] = ImageImg::Get(ImageImg::I_TH0 + i);
	}
}

GUI_APP_MAIN
{
	InitSb();
	TopWindow app;
	app.Sizeable();
	ArrayCtrl x;
	x.AddColumn("");
	for(int i = 0; i < 30; i++)
		x.Add(i);
	app.Add(x.SizePos());
	app.Run();
}
