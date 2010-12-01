#include <CtrlLib/CtrlLib.h>
#include <PlotCtrl/PlotCtrl.h>
using namespace Upp;

#define IMAGECLASS Symbol
#define IMAGEFILE <PlotCtrlExample/symbol.iml>
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

class PlotWindow : public TopWindow{
public:
	typedef PlotWindow CLASSNAME;
	PlotCtrl plot;
	LegendCtrl legend;
	PlotWindow():legend(plot){
		Sizeable().Title("PlotCtrl Example").SetRect(200,200,640,480);
		plot.SetMode(PLOT_AA).SetLimits(-4,4,-4,4);
		Add(plot.HSizePos(0,0).VSizePos(0,0));
		legend.SetRect(60,10,1,1); // We adjust the size later, when we know how much space it needs
		legend.SetBackground(White());
		legend.WhenSync=THISBACK(ResizeLegend);
		Add(legend);
		// alternative styles are up to your imagination, e.g.:
//		plot.SetBackground(Color(0x444444,0)).SetFrameColor(Color(0x660000,0))
//		    .SetFontColor(White()).SetAxisColor(LtGreen());
//		legend.SetFrameColor(LtGreen()).SetBackground(Color(0x666666,0));
	}
	void ResizeLegend(){
		legend.SetSize(legend.GetSizeHint());
	}
	bool Key(dword keys, int count){
		if (keys==K_CTRL_A){
			plot.ZoomAll(true);
			return true;
		}
		if (keys==K_CTRL_L){
			if(legend.IsShown()){legend.Hide();}
			else{legend.Show();}
			return true;
		}
		if (keys==K_CTRL_O){
			PlotStyleDlg(plot).Run();
			return true;
		}
		return false;
	}
};

GUI_APP_MAIN{
	PlotWindow p;
	// Register your PlotSymbol. This is not mandatory,
	// it only makes the symbol appear in PlotStyleDlg
	PlotSymbol::Register<Bug>("Ladybug");
	
	// You can create your data directly in plot ...
	p.plot.data.Add();
	p.plot.data[0].SetColor(LtRed()).SetTitle("Butterfly Curve");
	for(double t = -12*M_PI; t <= 12*M_PI; t+=0.02){
		double r=(exp(cos(t))-2*cos(4*t)+pow(sin(t/12),5));
		p.plot.data[0].AddXY(r*sin(t),r*cos(t));
	}
	
	// ... or take your data from elsewhere and copy them in plot. Supported types are
	//     Vector<Pointf>, VectorMap, and Vector of reasonable formats (i.e. convertable
	//     to double) + the same containers in Array flavor
	VectorMap<double,double> v;
	for(double x = -4; x <= 4; x+=0.05){
		v.Add(x,4*exp(-0.25*x*x)*sin(10*x));
	}
	p.plot.data.Add(v);
	p.plot.data[1].SetDash("1.5").SetTitle("4*exp(-x^2/4)*sin(10*x)").SetThickness(1.5)
	              .SetSymbol("Cross",Size(9,9),Black());
	v.Clear();
	
	// random Ladybugs! :-)
	PlotData dataset;
	for(double x = -4; x <= 4; x+=0.5){
		dataset.AddXY(x,int(Random(7))-3);
	}
	p.plot.data.Add(dataset);
	p.plot.data[2].SetColor(Green()).SetTitle("Totally random")
	              .SetThickness(2).SetSymbol("Ladybug");

	// And now, just start the gui and watch what you painted...
	p.Run();
	
	// If you want to take a "screenshot", nothing is easier. Just call GetImage()
	// and you'll get current view (well, last view in this example, but you get the idea ;-) )
	PNGEncoder().SaveFile(GetHomeDirFile("plot.png"), p.plot.GetImage());
}
