#include <PdfDraw/PdfDraw.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	PdfDraw pdf;
	int cy = 0;
	for(int face = Font::SERIF; face < Font::GetFaceCount(); face++) {
		Font fnt(face, 100);
		Cout() << fnt << "\n";
		LOG(face << ' ' << fnt << ", TTF: " << fnt.IsTrueType());
		if(1 || fnt.IsTrueType()) {
			pdf.DrawText(0, cy, AsString(fnt), fnt, Black);
			pdf.DrawText(200, cy, AsString(fnt), StdFont(100), Black);
			cy += fnt.GetLineHeight();
			if(cy > 6000) {
				pdf.EndPage();
				pdf.StartPage();
				cy = 0;
			}
		}
	}
	String p = GetHomeDirFile("pdf.pdf");
	SaveFile(p, pdf.Finish());
	LaunchWebBrowser(p);
}
