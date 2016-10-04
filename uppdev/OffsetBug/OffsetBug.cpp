#include <Core/Core.h>

#include "SvgDraw.h"
#include <PdfDraw/PdfDraw.h>

using namespace UPP;

void DoPainting(Draw& w) 
{
	w.DrawEllipse(0, 0, 100, 30, WhiteGray(), 1, Cyan);
	w.DrawText(0, 0, "Hello world", Roman(30).Bold());
}

void Paint(Draw& w) 
{
	DoPainting(w);
	
	w.Offset(30, 50);
	DoPainting(w);
	w.End();
	
	w.Offset(20, 100);
	w.Clip(5, 5, 40, 20);
	DoPainting(w);
	w.End();
	w.End();
	
	w.Clipoff(10, 150, 60, 20);
	DoPainting(w);
	w.End();
}

CONSOLE_APP_MAIN
{
	SvgDraw svgDraw1(200, 200);
	Paint(svgDraw1);
	SaveFile(ConfigFile("svg_direct_drawing.svg"), svgDraw1.Finish());
	
	PdfDraw pdfDraw1(200, 200);
	Paint(pdfDraw1);
	SaveFile(ConfigFile("pdf_direct_drawing.pdf"), pdfDraw1.Finish());
	
	DrawingDraw d(200, 200);
	Paint(d);
	Drawing d2 = d;
	
	SvgDraw svgDraw2(200, 200);
	svgDraw2.DrawDrawing(0, 0, 200, 200, d2);
	SaveFile(ConfigFile("svg_DrawingDraw.svg"), svgDraw2.Finish());
	
	PdfDraw pdfDraw2(200, 200);
	pdfDraw2.DrawDrawing(0, 0, 200, 200, d2);
	SaveFile(ConfigFile("pdf_DrawingDraw.pdf"), pdfDraw2.Finish());
}
