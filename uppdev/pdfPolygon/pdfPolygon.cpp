#include <PdfDraw/PdfDraw.h>

CONSOLE_APP_MAIN
{
	PdfDraw pdf;
	pdf.StartPage();
	pdf.DrawRect(300,300,500,400,Gray);

	Vector<Point> v;
	v<<Point(300,1000)<<Point(1000,1000)<<Point(1000,2000)<<Point(300,2000)<<Point(300,1000);
	pdf.DrawPolygon(v,Blue,10,Black);//doesn't work
	pdf.DrawPolyline(v,10);
	pdf.EndPage();

	SaveFile("e:\\PolygonTest.pdf", pdf.Finish());

}
