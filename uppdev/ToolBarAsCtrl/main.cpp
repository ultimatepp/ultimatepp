#include "ToolBarAsCtrl.h"

#define IMAGECLASS ToolBarImg
#define IMAGEFILE <ToolBarAsCtrl/ToolBar.iml>
#include <Draw/iml.h>

void ToolBarAsCtrl::MyBar(Bar& tb)
{
	tb.Add("Test",ToolBarImg::heart(),THISBACK(DoNothing));
	tb.Add(h);
}

ToolBarAsCtrl::ToolBarAsCtrl()
{
	CtrlLayout(*this, "Window title");
	tb.SetRect(10,10,200,50);
	Add(tb);
	tb.Set(THISBACK(MyBar));
	//in 3 seconds you will see the button
//	SetTimeCallback(3000,THISBACK(ShowToolBarButtons));
}

void ToolBarAsCtrl::DoNothing() {
	//just to have a callback function
};

void ToolBarAsCtrl::ShowToolBarButtons() {
	tb.RefreshLayout();
};

void ToolBarAsCtrl::Layout() {
	//doesn't work in Layout!
	ShowToolBarButtons();
//	tb.Transparent();
};

GUI_APP_MAIN
{
	GUI_PopUpEffect_Write(GUIEFFECT_NONE);
	GUI_ToolTipDelay_Write(10);
	ToolBarAsCtrl().Run();
}
