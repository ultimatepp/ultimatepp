#include <ScatterDraw/ScatterDraw.h>

using namespace Upp;

#include <plugin/png/png.h>
#include <PdfDraw/PdfDraw.h>


CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	UppLog() << "\nScatterDraw_Demo";
	
	String fileName = GetExeDirFile("Scatter");
	
	ScatterDraw scatter;
	Vector<Pointf> s1 = {{10,14}, {20,65}, {30,29}, {40,32}, {50,12}};
	Vector<double> x = {20, 30, 40, 50, 60};
	Vector<double> y = {34, 85, 49, 72, 56};
	
	UppLog() << "\nPreparing scatter";
	
	scatter.AddSeries(s1).Legend("Series 1").NoPlot().Units("ºC", "seg");
	
	scatter.AddSeries(x, y).Units("ºC", "seg").Legend("Series 2").Stroke(5, Null)
		   .Dash(LINE_DASHED).MarkStyle<SquareMarkPlot>().MarkWidth(12);
	
	SplineEquation spline;
	VectorPointf s1data(s1);
	spline.Fit(s1data);
	scatter.AddSeries(spline).Legend("Series 1 Softened").NoMark().Dash(LINE_SOLID).Stroke(1.5);
	
	scatter.SetXYMin(10, 0).SetRange(50, 100).SetMinUnits(20, 0).SetMajorUnits(10, 20);		// Done by hand
	
	scatter.SetTitle("ScatterDraw_Demo").SetTitleFont(SansSerif(14).Bold());
	scatter.SetLabelY("Temperature").SetLabelX("Time").SetLabelsFont(SansSerif(12));
	scatter.SetPlotAreaLeftMargin(40).SetPlotAreaRightMargin(30).SetPlotAreaTopMargin(40).SetPlotAreaBottomMargin(50);
	scatter.SetSize(Size(1000, 500));
	scatter.SetMode(ScatterDraw::MD_ANTIALIASED);
	scatter.SetPlotAreaLeftMargin(70);
	scatter.SetLegendAnchor(ScatterDraw::RIGHT_TOP);
	
	PNGEncoder().SaveFile(fileName + ".png", scatter.GetImage());
	
	PdfDraw pdf(Size(1000, 500));
	scatter.SetSize(Size(1000, 500));
	scatter.SetDrawing(pdf, false);
	SaveFile(fileName + ".pdf", pdf.Finish());
		
	UppLog() << "\nSaved '" << fileName << "'";
	#ifdef flagDEBUG
	Cout() << "\nPress enter key to end";
	ReadStdIn();
	#endif
}
