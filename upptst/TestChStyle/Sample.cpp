#include "TestChStyle.h"

#define IMAGECLASS TestImg
#define IMAGEFILE <TestChStyle/Test.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS TestImg
#define IMAGEFILE <TestChStyle/Test.iml>
#include <Draw/iml_source.h>

void Sample::Paint(Draw& w)
{
	w.DrawRect(GetSize(), Cyan());
	
//	w.DrawImage(10, 10, CtrlImg::spinup());
//	ChPaint(w, 10, 10, 100, 20, CtrlsImg::EFE());
//	ChPaint(w, 101, 0, 17, 23, DropList::StyleDefault().rmiddle[0]);
#ifdef _DEBUG
	w.DrawImage(10, 10, (Image)GetChameleonSample());
	w.DrawImage(100, 10, Magnify((Image)GetChameleonSample(), 4, 4));
	
#endif

//	EditFieldFrame().FramePaint(w, RectC(10, 50, 100, 20));
}
