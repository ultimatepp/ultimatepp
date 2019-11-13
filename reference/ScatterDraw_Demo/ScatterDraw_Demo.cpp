#include <ScatterDraw/ScatterDraw.h>

using namespace Upp;

#include <plugin/png/png.h>


CONSOLE_APP_MAIN
{
	Cout() << "\nScatterDraw_Demo";
	Cout() << "\nScatter painted to Image and saved to file";
	
	String fileName = AppendFileName(GetDesktopFolder(), "Scatter");
	
	ScatterDraw scatter;
	Vector<Pointf> s1;
	double x[] = {20, 30, 40, 50, 60};
	double y[] = {34, 45, 49, 44, 56};
	
	scatter.SerializeData(true);
	
	bool loaded = false;
	if (FileExists(fileName + ".xml")) {
		LoadFromXMLFile(scatter, fileName + ".xml");
		loaded = true;
	} else if (FileExists(fileName + ".json")) {
		LoadFromJsonFile(scatter, fileName + ".json");
		loaded = true;
	} else if (FileExists(fileName + ".bin")) {
		LoadFromFile(scatter, fileName + ".bin");
		loaded = true;
	}
	
	if (loaded) {
		Cout() << "\nRead scatter from file";
		fileName = fileName + "+";
	} else {
		Cout() << "\nPreparing scatter";
		
		s1 << Pointf(10,14) << Pointf(20,25) << Pointf(30,29) << Pointf(40,24) << Pointf(50,36);
		
		scatter.AddSeries(s1).Legend("Series 1").NoMark().Units("ºC", "seg");
		
		scatter.AddSeries(x, y, 5).Units("ºC", "seg").Legend("Series 2").Stroke(5, Null)
			   .Dash(LINE_DASHED).MarkStyle<SquareMarkPlot>().MarkWidth(12);
		
		scatter.SetXYMin(10, 0).SetRange(50, 100).SetMinUnits(20, 0).SetMajorUnits(10, 20);
		//scatter.ZoomToFit();
		
		scatter.SetTitle("ScatterDraw_Demo").SetTitleFont(SansSerif(14).Bold());
		scatter.SetLabelY("Temperature").SetLabelX("Time").SetLabelsFont(SansSerif(12));
		scatter.SetPlotAreaLeftMargin(40).SetPlotAreaRightMargin(30).SetPlotAreaTopMargin(40).SetPlotAreaBottomMargin(50);
		scatter.SetSize(Size(1000, 500));
		scatter.SetMode(ScatterDraw::MD_ANTIALIASED);
		scatter.SetLegendAnchor(ScatterDraw::RIGHT_TOP);
	}
	PNGEncoder().SaveFile(fileName + ".png", scatter.GetImage());
	StoreAsJsonFile(scatter, fileName + ".json", true);
	StoreAsXMLFile(scatter, "Scatter", fileName + ".xml");
	StoreToFile(scatter, fileName + ".bin");
	
	Cout() << "\nSaved '" << fileName << "'";
	Cout() << "\nPress key to end";
	ReadStdIn();
}
