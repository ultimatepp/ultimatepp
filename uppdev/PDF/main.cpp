#include "PDF.h"

#include <PdfDraw/PdfDraw.h>

GUI_APP_MAIN
{
	PdfDraw pdf;
//	pdf.DrawRect(100, 100, 500, 500, Blue);
	FontInfo fi = ScreenInfo().GetFontInfo(Roman(100));
	pdf.DrawRect(100, 100, 100, fi.GetHeight(), LtGray);
	pdf.DrawRect(100, 100 + fi.GetAscent(), 100, 1, Blue);
	pdf.DrawText(100, 100, "Ahoj", Roman(100), Black);
	pdf.DrawRect(1000, 1000, 100, 100, LtGray);
	pdf.DrawText(1000, 1000, 300, "Angle!", Roman(100), Black);
	SaveFile("d:\\pdf.pdf", pdf.Finish());
}
