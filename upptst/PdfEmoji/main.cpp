#include <CtrlLib/CtrlLib.h>
#include <PdfDraw/PdfDraw.h>
#include <RichText/RichText.h>

using namespace Upp;

GUI_APP_MAIN
{
	String p = GetHomeDirFile("pdf.pdf");
	SaveFile(p, Pdf(ParseQTF("Some emojis: 🤣 😇 🙂 🙃 😉 😌 😍 🥰 😘 😗 😙 😚 😋 😛 😝 😜 🤪")));
	LaunchWebBrowser(p);
}
