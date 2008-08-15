#include "GUISample1.h"

#define IMAGECLASS Imgs
#define IMAGEFILE <GUISample1/Icons.iml>
#include <Draw/iml.h>

GUISample1::GUISample1()
{
	CtrlLayout(*this, "Window title");
	MainTB.Set(THISBACK(InitToolbar));

	S1.Horz();
	
	F1.Color(Color(0x80, 0x80, 0x80));
	TB1.Set(THISBACK(InitToolbar));
	
	L1.SetMinSize(Size(100,0));
	L1.SetText("Uzdevumi");
	L1.SetInk(Color(0xff, 0xff, 0xff));
	F1.Add(L1);
	L1.HSizePos();
	L1.TopPos(0, 28);
	
	TB1.SetMinSize(Size(100,100));
	F1.Add(TB1);
	TB1.HSizePos();
	TB1.TopPos(28, 56);

	F1.Add(AC1);
	AC1.HSizePos();
	AC1.VSizePos(56, 0);

	//this adds only ~380px
	S1.SetMinPixels(0, 200);
	
	S1 << F1 << AC2 << AC3;
	
}

void GUISample1::ToolbarClicked()
{
	PromptOK("Click");
}

void GUISample1::InitToolbar(Bar& bar)
{
	bar.Separator();
	bar.Add(Imgs::Mimg1(), THISBACK(ToolbarClicked));
	bar.Separator();
	bar.Add(Imgs::Mimg1(), THISBACK(ToolbarClicked));
}

GUI_APP_MAIN
{
	GUISample1().Sizeable(true).Run();
}

