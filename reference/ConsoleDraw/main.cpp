#include <Painter/Painter.h>
#include <RichText/RichText.h>
#include <PdfDraw/PdfDraw.h>
#include <plugin/png/png.h>

using namespace Upp;

void DoDraw(Draw& w)
{
	w.DrawRect(0, 0, 600, 200, White());
	w.DrawText(0, 0, "Hello world!", Arial(100), Black());
}

CONSOLE_APP_MAIN {
	{
		ImagePainter w(600, 100);
		DoDraw(w);
		PNGEncoder().SaveFile(GetHomeDirFile("hello.png"), w);
	}
	{
		RichText txt = ParseQTF("[A4 Just a [*/ little] test!");
		ImagePainter w(1000, 200);
		w.DrawRect(0, 0, 1000, 200, White());
		txt.Paint(w, 0, 0, 1000);
		PNGEncoder().SaveFile(GetHomeDirFile("richtext.png"), w);	
		
		PdfDraw pdf;
		txt.Paint(pdf, 0, 0, 1000);
		SaveFile(GetHomeDirFile("richtext.pdf"), pdf.Finish());
	}
}
