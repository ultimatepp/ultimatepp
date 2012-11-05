#include <PdfDraw/PdfDraw.h>

using namespace Upp;

#define IMAGECLASS AppImg
#define IMAGEFILE <PDF/app.iml>
#include <Draw/iml.h>

CONSOLE_APP_MAIN
{
	PdfDraw pdf;
	pdf.DrawRect(400, 400, 500, 500, Blue);
	pdf.DrawImage(100, 500, 200, 200, AppImg::test());
	pdf.DrawImage(300, 500, 200, 200, AppImg::testbw());
	pdf.DrawImage(500, 500, 200, 200, AppImg::testg());
	pdf.DrawText(100, 100, "Ahoj", Roman(100), Black);
	pdf.DrawText(100, 200, "Ahoj", Arial(100), Black);
	pdf.DrawText(300, 100, "Ahoj", Roman(150).Italic(), Black);
	pdf.DrawText(300, 200, "Ahoj", Arial(150).Italic(), Black);
	
	int cy = 1000;
	for(int bold = 0; bold < 2; bold++)
		for(int italic = 0; italic < 2; italic++) {
			pdf.DrawText(500, 1000, "Ahoj", Roman(100).Bold(), Black);
			pdf.DrawText(500, 1200, "Ahoj", Arial(100).Bold(), Black);
			pdf.DrawText(500, 1300, "Ahoj", Courier(100).Bold(), Black);
		}


	SaveFile(GetHomeDirFile("pdf.pdf"), pdf.Finish());
}
