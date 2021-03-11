#include "PdfDraw.h"

namespace Upp {

String Pdf(const Array<Drawing>& report, Size sz, int margin, bool pdfa,
           const PdfSignatureInfo *sign)
{
	PdfDraw w(sz + 2 * margin, pdfa);
	for(int i = 0; i < report.GetCount(); i++) {
		w.StartPage();
		w.DrawDrawing(margin, margin, sz.cx, sz.cy, report[i]);
		w.EndPage();
	}
	return w.Finish(sign);
}

INITIALIZER(PdfDraw)
{
	SetDrawingToPdfFn(Pdf, DrawJPEG);
}

}
