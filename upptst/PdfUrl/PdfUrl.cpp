#include <PdfDraw/PdfDraw.h>
#include <RichText/RichText.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	String p = GetHomeDirFile("pdf.pdf");
	SaveFile(p, Pdf(ParseQTF("This is some [^http://www.ultimatepp.org/forums^ LINK]!!!")));
	LaunchWebBrowser(p);
}
