#include "GrabYourScreen.h"

#define IMAGEFILE <GrabYourScreen/iml.iml>
#define IMAGECLASS Images
#include <Draw/iml.h>

GrabYourScreen::GrabYourScreen() {
	Icon(Images::Computer());
	LargeIcon(Images::Computer());
	Sizeable().Zoomable();//.TopMost(true, true);
}

void GrabYourScreen::Init() {
	Title("GrabYourScreen");
	
	Add(mainTab.SizePos());
	
	grabScreen.Init(*this);
	mainTab.Add(grabScreen.SizePos(), t_("Screen"));
	grabBook.Init();
	mainTab.Add(grabBook.SizePos(), t_("Book"));
	Rect rect = GetRect();
	SetRect(rect.left, rect.top, 500, 500);
}

void GrabYourScreen::Exit()  {
	Break();
}

GUI_APP_MAIN {
	try {
		GrabYourScreen program;
		program.Open();
		program.Init();
		program.Run();
	} catch(Exc error) {
		Exclamation(Format(t_("Error %s"), error));
	}
}
