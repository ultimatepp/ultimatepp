#ifndef _ScatterCtrl_Demo_ScatterCtrl_Demo_h
#define _ScatterCtrl_Demo_ScatterCtrl_Demo_h

#include <ScatterCtrl/ScatterCtrl.h>
#include <ScatterCtrl/PieCtrl.h>

using namespace Upp;

#define LAYOUTFILE <ScatterCtrl_Demo/ScatterCtrl_Demo.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS MyImages
#define IMAGEFILE <ScatterCtrl_Demo/ScatterCtrl_Demo.iml>
#include <Draw/iml_header.h>


struct ScatterDemo : StaticRect {
	virtual void Init() {};
	virtual ScatterCtrl &Scatter() = 0;
};
 
void RegisterExample(const char *name, ScatterDemo* (*ctrl)(), String fileName);

class Tab1_Basic : public WithTab1Basic<ScatterDemo> {
public:
	typedef Tab1_Basic CLASSNAME;
	
	virtual void Init();
	virtual ScatterCtrl &Scatter()	{return scatter;};

private:
	Vector<Pointf> sDemoRed1, sDemoRed2,sDemoGreen;
	Vector<Pointf> s1;
	double s2y[5];
	double s3x[5], s3y[5];
	Vector<double> s4x, s4y;
	VectorMap<double, double> s5;
};

class Tab2_Functions : public WithTab2Functions<ScatterDemo> {
public:
	typedef Tab2_Functions CLASSNAME;
	
	void Init();
	virtual ScatterCtrl &Scatter()	{return scatter;};
	
	class MySource : public FuncSource {
	public:
		void Init(double data) {
			this->data = data;
			function = [=](double x) {return Calc(x);};
		}
		
		double Calc(double x);
		
	private:
		double data;
	};
	MySource source;
};

class Tab3_ParametricFunctions : public WithTab3ParametricFunctions<ScatterDemo> {
public:
	typedef Tab3_ParametricFunctions CLASSNAME;
	
	void Init();
	virtual ScatterCtrl &Scatter()	{return scatter;};
};

class Tab4_Formatting : public WithTab4Formatting<ScatterDemo> {
public:
	typedef Tab4_Formatting CLASSNAME;
	
	void Init();
	virtual ScatterCtrl &Scatter()	{return scatter;};
	
private:
	Vector<Pointf> s1, s2;
};

class Tab5_Dynamic : public WithTab5Dynamic<ScatterDemo> {
public:
	typedef Tab5_Dynamic CLASSNAME;
	
	void Init();
	void Animate();
	void Start();
	void Stop();
	void Reset();
	void PgDown();
	void PgUp();
	void Plus();
	void Minus();
	virtual ScatterCtrl &Scatter()	{return scatter;};
	
private:
	double t;
	Vector<Pointf> s1, s2;
};

class Tab6_Logarithmic : public WithTab6Logarithmic<ScatterDemo> {
public:
	typedef Tab6_Logarithmic CLASSNAME;
	
	void Init();
	void FormatX(String& s, int i, double d);	
	virtual ScatterCtrl &Scatter()	{return scatter;};

private:
	Vector<Pointf> s1;
};

class Tab7_Operations : public WithTab7Operations<ScatterDemo> {
public:
	typedef Tab7_Operations CLASSNAME;
	
	void Init();		
	void AddSeries();
	void RemoveFirstSeries();
	void RemoveLastSeries();
	void RemoveAllSeries();
	virtual ScatterCtrl &Scatter()	{return scatter;};

private:
	Array<Vector<Pointf> > series;
};

class Tab8_Secondary : public WithTab8Secondary<ScatterDemo> {
public:
	typedef Tab8_Secondary CLASSNAME;

	void Init();
	void MyFormat(String& s, int i, double d)
	{
		s = Format("%Mon", fround(d)); 
		if (!s.IsEmpty())
			s << "\n2011";
	}
	void MyFormatDelta(String& s, int i, double d)
	{
		s = FormatDouble(d, 1) + " " + t_("months");
	}
	virtual ScatterCtrl &Scatter()	{return scatter;};

private:
	Vector<Pointf> s1, s2, s3;
};

class Tab9_Big : public WithTab9Big<ScatterDemo> {
public:
	typedef Tab9_Big CLASSNAME;
	
	void Init();
	void OnFastView()		{scatter.SetFastViewX(fastView);};
	void OnSequentialX()	{scatter.SetSequentialXAll(sequentialX);};
	virtual ScatterCtrl &Scatter()	{return scatter;};

private:
	Vector<Pointf> s1,s2;
};

class Tab10_User : public WithTab10User<ScatterDemo> {
public:
	typedef Tab10_User CLASSNAME;
	
	void Init();
	virtual ScatterCtrl &Scatter()	{return scatter;};

private:
	double sx[5], sy[5];
};

class Tab11_Trend : public WithTab11Trend<ScatterDemo> {
public:
	typedef Tab11_Trend CLASSNAME;

	Tab11_Trend() {};	
	void Init();
	void OnSeries();
	virtual ScatterCtrl &Scatter()	{return scatter;};

private:
	Vector<double> s1x, s1y;
	Vector<double> s2x, s2y;
	LinearEquation linear;
	PolynomialEquation poly2, poly4;
	FourierEquation fourier;
	ExponentialEquation exponential;
	Rational1Equation rational1;
	SinEquation sin;
};

class Tab12_Linked : public WithTab12Linked<ScatterDemo> {
public:
	typedef Tab12_Linked CLASSNAME;
	
	virtual void Init();
	virtual ScatterCtrl &Scatter()	{return scatter1;};

private:
	Vector<Pointf> s1, s2, s3;
	void OnLink();
};

class TabUserEquation : public WithTabUserEquation<ScatterDemo> {
public:
	typedef TabUserEquation CLASSNAME;
	
	virtual void Init();
	void OnUpdate();
	virtual ScatterCtrl &Scatter()	{return scatter;};

private:
	UserEquation userEquation;
};

class TabRangePlot : public WithTabRangePlot<ScatterDemo> {
public:
	typedef TabRangePlot CLASSNAME;
	
	void Init();
	virtual ScatterCtrl &Scatter()	{return scatter;};
	
private:
	Vector<Vector <double> > s1, s2;
	Vector<int> idsRange;
	Vector<Pointf> sdata, smin, smax;
	VectorPointf dmin, dmax;
	DataRange range;
};

class TabStackedPlot : public WithTabStackedPlot<ScatterDemo> {
public:
	typedef TabStackedPlot CLASSNAME;
	
	void Init();
	virtual ScatterCtrl &Scatter()	{return scatter;};
	
	void OnPlot();
	
private:
	Vector<Pointf> s1, s2, s3;
	VectorPointf ds1, ds2, ds3;
	DataStackedY stacked;
};

class TabBubblePlot : public WithTabBubblePlot<ScatterDemo> {
public:
	typedef TabBubblePlot CLASSNAME;
	
	void Init();
	virtual ScatterCtrl &Scatter()	{return scatter;};
	
private:
	Vector<Vector <double> > s1;
	Vector<int> idsBubble;
	Vector<String> labels;
};

class Tab_UserPaint : public WithTabUserPaint<ScatterDemo> {
public:
	typedef Tab_UserPaint CLASSNAME;
	
	virtual void Init();
	virtual ScatterCtrl &Scatter()	{return scatter;};

	void OnPainter(Painter &w)		{OnPaint(w);}
	void OnDraw(Draw &w)			{OnPaint(w);}
	
	template <class T>
	void OnPaint(T& w) {
		int plotW = scatter.GetPlotWidth(), plotH = scatter.GetPlotHeight();
		DrawLineOpa(w, 0, 0, plotW, plotH, 1, 1, 1, LtRed(), "2 2");
		DrawText(w, plotW/5, plotH/5, 0, "Fixed line", Arial(15), Black());		
		
		String text = "Fixed text";
		Font font = Arial(30);
		Size sz = GetTextSize(text, font);
		FillRectangleOpa(w, plotW - sz.cx - 10, 10, plotW - 10, 10 + sz.cy, 0.3, Null, LtGreen());
		DrawText(w, plotW - sz.cx - 10, 10, 0, text, font, Black());
		
		if (scatter.GetXMin() < 1 && scatter.GetXMax() > 1) {
			Pointf p;
			for (int i = 0; i < s1.GetCount(); ++i) {
				if (s1[i].x >= 1) { 
					p = s1[i];
					break;
				}
			}
			if (scatter.GetYMin() < p.y && scatter.GetYMax() > p.y) {
				double posx = scatter.GetPosX(p.x);
				double posy = scatter.GetPosY(p.y);
				double sizex = scatter.GetSizeX(0.2);
				FillRectangleOpa(w, posx - sizex/2., posy - sizex/2., 
									posx + sizex/2., posy + sizex/2., 
									0.8, Null, LtGreen());
				Vector<Pointf> s;
				s << Pointf(posx - sizex/2., posy - sizex/2.) 
				  << Pointf(posx + sizex/2., posy - sizex/2.) 
				  << Pointf(posx + sizex/2., posy + sizex/2.) 
				  << Pointf(posx - sizex/2., posy + sizex/2.)
				  << Pointf(posx - sizex/2., posy - sizex/2.); 
				DrawPolylineOpa(w, s, 1, 1, 2, Green(), "", Null);
				DrawText(w, posx + sizex/2., posy, 0, Format("Pos %.2f, %.2f fixed font", p.x, p.y), Arial(15), Black());
				double sizeT = scatter.GetSizeY(0.05);
				Font font = Arial((int)sizeT);
				DrawText(w, posx + sizex/2., posy + 15, 0, Format("Pos %.2f, %.2f scaled font", p.x, p.y), font, Black());
			}
		}
	}

private:
	Vector<Pointf> s1;
};

class Tab19_Surf : public WithTabSurf<ScatterDemo> {
public:
	typedef Tab19_Surf CLASSNAME;
	
	virtual void Init();
	virtual ScatterCtrl &Scatter()	{return scatter;};
	
	void OnChange();
	void Init_DatasetSimple();
	void Init_Dataset();
	void Init_DataExplicit();
	
private:
	ExplicitData funData;
	TableDataVector data;
	TableDataVector dataFun;
	Vector<double> zData, xAxis, yAxis;
	Vector<double> zDataFun, xAxisFun, yAxisFun;
	
	Vector<Pointf> isolines, numbersPos;
	Vector<String> labels;
};

class TabPie : public WithTabPie<StaticRect> {
public:
	typedef TabPie CLASSNAME;
	
	void Init();
};

class ScatterCtrl_Demo : public WithScatterCtrl_Demo<TopWindow> {
	typedef ScatterCtrl_Demo CLASSNAME;
	
public:
	ScatterCtrl_Demo();
	TabPie tabPie;
	
private:
	void Preview();
	void SavePNG();
	void SaveJPG();
#ifdef PLATFORM_WIN32
	void SaveEMF();
#endif
	void CopyClipboard();
	void SetMode();

	void OnSel();

	Vector<StaticRect *> examplesRects;
};

#endif

