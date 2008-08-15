#include "StatusBarTest.h"

StatusBarTest::StatusBarTest()
{
	Title("StatusBar Test Application");
	Icon(IconImages::Logo());
	AddFrame(m_sb);
	//Zoomable();
	Maximize();
}

GUI_APP_MAIN
{
	StatusBarTest().Run();
}
