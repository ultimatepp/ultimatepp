#include "FTGLDemo.h"

FTGLDemo::FTGLDemo()
{
	Add(ftglCtrl.SizePos());
}


GUI_APP_MAIN
{
	FTGLDemo ftglDemo;
	ftglDemo.Sizeable().Zoomable().MinimizeBox();
	ftglDemo.Run();
}
