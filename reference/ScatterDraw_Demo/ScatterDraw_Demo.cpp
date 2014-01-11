#include <ScatterDraw/ScatterDraw.h>

using namespace Upp;

#include <plugin/png/png.h>


CONSOLE_APP_MAIN
{
	Cout() << "\nScatterDraw_Demo";
	Cout() << "\n\nScatter painted to Image and saved to file";
	
	ScatterDraw scatter;
	Vector<Pointf> s1;
	s1 << Pointf(10,14) << Pointf(20,25) << Pointf(30,29) << Pointf(40,24) << Pointf(50,36);
	scatter.AddSeries(s1).Legend("Series 1").NoMark().Units("ºC", "seg");
	
	double x[] = {20, 30, 40, 50, 60};
	double y[] = {34, 45, 49, 44, 56};
	scatter.AddSeries(x, y, 5).Units("ºC", "seg").Legend("Series 2").Stroke(5, Null)
		   .Dash(LINE_DASHED).MarkStyle<SquareMarkPlot>().MarkWidth(12);
	
	scatter.FitToData();
	
	scatter.SetTitle("ScatterDraw_Demo").SetTitleFont(SansSerif(14).Bold());
	scatter.SetLabelY("Temperature").SetLabelX("Time").SetLabelsFont(SansSerif(12));
	scatter.SetPlotAreaLeftMargin(40).SetPlotAreaRightMargin(30).SetPlotAreaTopMargin(40).SetPlotAreaBottomMargin(50);
	scatter.SetSize(Size(1000, 500));
	scatter.SetMode(ScatterDraw::MD_ANTIALIASED);
	//scatter.SetLegendAnchor(ScatterDraw::LEGEND_ANCHOR_RIGHT_TOP);
	
	String fileName = AppendFileName(GetDesktopFolder(), "Scatter.png");
	PNGEncoder().SaveFile(fileName, scatter.GetImage());
	
	Cout() << "\n\nSaved '" << ToSystemCharset(fileName) << "'";
	Cout() << "\n\nPress key to end";
	getchar();	
}
