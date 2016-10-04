#include "VegaMain.h"


#define IMAGECLASS    VegaImg
#define IMAGEFILE <ForlanoVega/Vega.iml>
#include <Draw/iml_source.h>

//you can have it as standard.... and even do not change
VegaMain::VegaMain()
{
	CtrlLayout(*this, "Forlano Vega Main");
	spls.Vert();
	spls.SetPos(8000);
//	spls.Vert(tabs, consoleFrame);
	spls.Add(tabs);
	spls.Add(consoleFrame);
	rubbish = " ";

	SetBars();
	SetTabs();
	Init();
	BackPaint();
}

GUI_APP_MAIN
{
	VegaMain().Sizeable().Zoomable().Run();
}
