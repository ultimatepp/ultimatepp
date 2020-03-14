#include "ScatterCtrl_Demo.h"

#define IMAGECLASS Symbol
#define IMAGEFILE <ScatterCtrl_Demo/symbol.iml>
#include <Draw/iml.h>

class MyPlot : public SeriesPlot {
private:
	template <class T>
	void DoPaint(T& w, const double &scale, Vector<Pointf> &p, const double opacity, 
				double thick, const Color &color, String pattern, const Color &background, 
				const Color &fillColor, double y0) const {
		Vector<Pointf> t;
		t.SetCount(3);
		for (int i = 0; i < p.GetCount(); ++i) {
			t[0].x = p[i].x + 80;
			t[0].y = y0;
			t[1] = p[i];
			t[2].x = p[i].x - 80;
			t[2].y = y0;
			if (!IsNull(fillColor)) 
				FillPolylineOpa(w, t, scale, opacity, background, fillColor);
			DrawPolylineOpa(w, t, scale, 1, thick, color, pattern, background);		
		}
	}
	
public:
	void Paint(Draw& w, Vector<Pointf> &p, const double &scale, const double opacity, 
				double thick, const Color &color, String pattern, const Color &background, 
				const Color &fillColor, double , double , double y0, double , 
				bool ) const {
		DoPaint(w, scale, p, opacity, fround(thick), color, pattern, background, fillColor, y0);		
	}
	void Paint(Painter& w, Vector<Pointf> &p, const double &scale, const double opacity, 
				double thick, const Color &color, String pattern, const Color &background, 
				const Color &fillColor, double , double , double y0, double , 
				bool ) const {
		DoPaint(w, scale, p, opacity, fround(thick), color, pattern, background, fillColor, y0);		
	}
};

class MyMark : public MarkPlot {
private:
	template <class T>
	void DoPaint(T& w, const double& scale, const Point& cp, const double& , const Color& ) const {
		Size bugSize = Symbol::bug().GetSize()*scale;
		w.DrawImage(cp.x - bugSize.cx/2, cp.y - bugSize.cy/2, bugSize.cx, bugSize.cy, Symbol::bug());
	}

public:
	void Paint(Draw &p, const double& scale, const Point& cp, const double& size, const Color& markColor, 
				const double&, const Color&) const {
		DoPaint(p, scale, cp, size, markColor);
	}
	void Paint(Painter &p, const double& scale, const Point& cp, const double& size, const Color& markColor, 
				const double&, const Color&) const {
		DoPaint(p, scale, cp, size, markColor);
	}
};

void Tab10_User::Init()
{
	CtrlLayout(*this);	
	SizePos();
	
	sy[0] = 18; sy[1] = 29; sy[2] = 23; sy[3] = 25; sy[4] = 20;
	sx[0] = 10; sx[1] = 20; sx[2] = 30; sx[3] = 40; sx[4] = 50;
	scatter.AddSeries(sx, sy, 5).Legend("Series").PlotStyle<MyPlot>().MarkStyle<MyMark>()
		   .Stroke(3, LtGreen()).Fill(LtGray());
	
	scatter.SetRange(60, 50).SetMajorUnits(10, 10);
	scatter.ShowInfo().ShowContextMenu().ShowPropertiesDlg().ShowProcessDlg().Responsive();
}

ScatterDemo *Construct10()
{
	static Tab10_User tab;
	return &tab;
}

INITBLOCK {
	RegisterExample("User graph", Construct10, __FILE__);

	SeriesPlot::Register<MyPlot>("My plot");
	MarkPlot::Register<MyMark>("My mark");
}