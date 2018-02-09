#include <CtrlLib/CtrlLib.h>
#include <Report/Report.h>
#include <PdfDraw/PdfDraw.h>

using namespace Upp;

GUI_APP_MAIN
{
	PdfDraw w;

	DrawJPEG(w, 100, 100, 2000, 1333, LoadDataFile("hermes.jpg"));
	
	String pdf = GetHomeDirFile("test.pdf");
	SaveFile(pdf, w.Finish());

	LaunchWebBrowser(pdf);
}
