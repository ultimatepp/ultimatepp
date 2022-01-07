#include <CtrlLib/CtrlLib.h>
#include <PdfDraw/PdfDraw.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	PdfDraw pdf;
	int cy = 0;
	pdf.DrawImage(5000, 20, 100, 100, CtrlImg::exclamation());
//	for(int face = Font::SERIF; face < Font::GetFaceCount(); face++)
	int face = 19;
	{
		Font fnt(face, 100);
		Cout() << fnt << "\n";
		LOG(face << ' ' << fnt << ", TTF: " << fnt.IsTrueType());
		String txt = AsString(fnt) + " 訓民正音 (훈민정음) 😜 🤪 ";
		pdf.DrawText(0, cy, txt, fnt, Black);
		pdf.DrawText(3000, cy, AsString(fnt) << ' ' << face, StdFont(100), Black);
		cy += fnt.GetLineHeight();
		if(cy > 6000) {
			pdf.EndPage();
			pdf.StartPage();
			cy = 0;
		}
	}
	String p = GetHomeDirFile("pdf.pdf");
	SaveFile(p, pdf.Finish());
	LaunchWebBrowser(p);
}
