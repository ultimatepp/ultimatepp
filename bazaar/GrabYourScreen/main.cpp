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
	grabBook.Init();
	mainTab.Add(grabBook.SizePos(), "Book");
	grabScreen.Init(*this);
	mainTab.Add(grabScreen.SizePos(), "Screen");
}

void GrabYourScreen::Exit()  {
	Break();
}

GUI_APP_MAIN {
	GrabYourScreen program;
	program.Open();
	program.Init();
	program.Run();
}
