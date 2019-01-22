#include <PlotLib/PlotLib.h>
#include <plugin/png/png.h>
using namespace Upp;

#define IMAGECLASS Symbol
#define IMAGEFILE <PlotLibExample/symbol.iml>
#include <Draw/iml.h>

// If you wish, you can define virtualy anything as symbol, just use the WithPlotSymbol
// template, which will setup almost everything for you and override the PaintOp method
class Bug: public WithPlotSymbol<Bug>{
public:
	virtual void PaintOp(Painter& sw)const{
		sw.Rectangle(0,0,s.cx,s.cy).Fill(Symbol::bug(),0,0,s.cx,0);
	}
	Bug(){s=Sizef(11,11);};
};


CONSOLE_APP_MAIN {
	Plot plot;
	// Register your PlotSymbol. This is not mandatory,
	// it only makes the symbol appear in PlotStyleDlg
	PlotSymbol::Register<Bug>("Ladybug");
	
	// You can create your data directly in plot ...
	plot.data.Add();
	plot.data[0].SetColor(LtRed()).SetTitle("Butterfly Curve");
	for(double t = -12*M_PI; t <= 12*M_PI; t+=0.02){
		double r=(exp(cos(t))-2*cos(4*t)+pow(sin(t/12),5));
		plot.data[0].AddXY(r*sin(t),r*cos(t));
	}
	
	// ... or take your data from elsewhere and copy them in plot. Supported types are
	//     Vector<Pointf>, VectorMap, and Vector of reasonable formats (i.e. convertable
	//     to double) + the same containers in Array flavor
	VectorMap<double,double> v;
	for(double x = -4; x <= 4; x+=0.05){
		v.Add(x,4*exp(-0.25*x*x)*sin(10*x));
	}
	plot.data.Add(v);
	plot.data[1].SetDash("1.5").SetTitle("4*exp(-x^2/4)*sin(10*x)").SetThickness(1.5)
	            .SetSymbol("Cross",Size(9,9),Black());
	v.Clear();
	
	// random Ladybugs! :-)
	PlotData dataset;
	for(double x = -4; x <= 4; x+=0.5){
		dataset.AddXY(x,int(Random(7))-3);
	}
	plot.data.Add(dataset);
	plot.data[2].SetColor(Green()).SetTitle("Totally random")
	              .SetThickness(2).SetSymbol("Ladybug");

	// Plot can be setup any time before painting
	plot.SetMode(PLOT_AA).SetLimits(-4.0,4.0,-4.0,4.0).SetPlotSize(640,480);

	// When it's all prepared, just call GetImage to get the plot and use the returned image
	// in whatever way you want, e.g. save it into PNG file:
	PNGEncoder().SaveFile(GetHomeDirFile("plot.png"), plot.GetImage());
	
	// If you want a legend, just create one ;-) It is a separate picture, 
	// if you want it combined with the plot, it's up to you...
	PNGEncoder().SaveFile(GetHomeDirFile("legend.png"), 
	                      Legend(plot).SetBackground(White()).GetLegend());
}
