#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGECLASS ArrowImg
#define IMAGEFILE <PrintImage/Arrow.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS ArrowImg
#define IMAGEFILE <PrintImage/Arrow.iml>
#include <Draw/iml_source.h>

GUI_APP_MAIN
{
	PrinterJob pd("test");
	if(!pd.Execute())
		return;
	Draw& w = pd;
	Size sz = w.GetPageSize();
	w.StartPage();
	Size isz = CtrlImg::exclamation().GetSize();
	w.DrawImage(1000, 0, 500, 500, ArrowImg::Arrow());
	w.DrawImage(0, 0, 500, 500, CtrlImg::exclamation());
	w.EndPage();
}
