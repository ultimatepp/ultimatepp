#include <PdfDraw/PdfDraw.h>
#include <RichText/RichText.h>

using namespace Upp;

#define IMAGECLASS AppImg
#define IMAGEFILE <PDF/app.iml>
#include <Draw/iml.h>

CONSOLE_APP_MAIN
{
	String p = GetHomeDirFile("pdf.pdf");
	SaveFile(p, Pdf(ParseQTF("Hello! This is some [^http://www.ultimatepp.org/forums^ LINK]!!!")
//	                , Size(3968, 6074), 0
				)
	);
	LaunchWebBrowser(p);
}
