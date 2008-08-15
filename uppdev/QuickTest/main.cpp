#include "QuickTest.h"
#include <ide/QuickTabs/QuickTabs.h>

#define IMAGECLASS Img
#define IMAGEFILE <ide/QuickTabs/QuickTabs.iml>
#include <Draw/iml_source.h>

void QuickTest::Paint(Draw& w)
{
	Size sz = GetSize();
	w.DrawRect(sz, Black());
	w.DrawImage(100, 100, Img::CR2);
	w.DrawImage(120, 100, Img::CR1);
}

QuickTest::QuickTest()
{
}

GUI_APP_MAIN
{
	QuickTest().Run();
}
