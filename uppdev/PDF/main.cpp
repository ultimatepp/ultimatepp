#include <PdfDraw/PdfDraw.h>

using namespace Upp;

#define IMAGECLASS AppImg
#define IMAGEFILE <PDF/app.iml>
#include <Draw/iml.h>

CONSOLE_APP_MAIN
{
	PdfDraw pdf;
#if 0
	pdf.DrawText(100, 100, "Ahoj", Courier(100).Bold(), Black);
#else
	int cy = 0;
	for(int face = Font::SERIF; face < min(9, Font::GetFaceCount()); face++)
//	int face = 6;
		for(int strikeout = 0; strikeout < 2; strikeout++)
			for(int underline = 0; underline < 2; underline++)
				for(int italic = 0; italic < 2; italic++)
					for(int bold = 0; bold < 2; bold++) {
						Font fnt(face, 100);
						fnt.Bold(bold).Italic(italic).Underline(underline).Strikeout(strikeout);
						Cout() << fnt << "\n";
						LOG(face << ' ' << fnt << ", TTF: " << fnt.IsTrueType());
						if(1 || fnt.IsTrueType()) {
							pdf.DrawText(100, cy, AsString(fnt), fnt, Black);
							pdf.DrawText(2000, cy, 200, AsString(fnt), fnt, Black);
							cy += fnt.GetLineHeight();
							if(cy > 6000) {
								pdf.EndPage();
								pdf.StartPage();
								cy = 0;
							}
						}
					}
#endif
	SaveFile(GetHomeDirFile("pdf.pdf"), pdf.Finish());
}
