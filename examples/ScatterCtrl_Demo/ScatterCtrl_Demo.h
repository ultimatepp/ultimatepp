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

class Tab1 : public WithTab1<ScatterDemo> {
public:
	typedef Tab1 CLASSNAME;
	
	virtual void Init();
	virtual ScatterCtrl &Scatter()	{return scatter;};

private:
	Vector<Pointf> s1;
	double s2y[5];
	double s3x[5], s3y[5];
	Vector<double> s4x, s4y;
	VectorMap<double, double> s5;
};

class Tab2 : public WithTab2<ScatterDemo> {
public:
	typedef Tab2 CLASSNAME;
	
	void Init();
	virtual ScatterCtrl &Scatter()	{return scatter;};
	
	Callback back1, back2, back3;
};

class Tab3 : public WithTab3<ScatterDemo> {
public:
	typedef Tab3 CLASSNAME;
	
	void Init();
	virtual ScatterCtrl &Scatter()	{return scatter;};
};

class Tab4 : public WithTab4<ScatterDemo> {
public:
	typedef Tab4 CLASSNAME;
	
	void Init();
	virtual ScatterCtrl &Scatter()	{return scatter;};
	
private:
	Vector<Pointf> s1, s2;
};

class Tab5 : public WithTab5<ScatterDemo> {
public:
	typedef Tab5 CLASSNAME;
	
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

class Tab6 : public WithTab6<ScatterDemo> {
public:
	typedef Tab6 CLASSNAME;
	
	void Init();
	void FormatX(String& s, int i, double d);	
	virtual ScatterCtrl &Scatter()	{return scatter;};

private:
	Vector<Pointf> s1;
};

class Tab7 : public WithTab7<ScatterDemo> {
public:
	typedef Tab7 CLASSNAME;
	
	void Init();		
	void AddSeries();
	void RemoveFirstSeries();
	void RemoveLastSeries();
	void RemoveAllSeries();
	virtual ScatterCtrl &Scatter()	{return scatter;};

private:
	Array<Vector<Pointf> > series;
};

class Tab8 : public WithTab8<ScatterDemo> {
public:
	typedef Tab8 CLASSNAME;

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

class Tab9 : public WithTab9<ScatterDemo> {
public:
	typedef Tab9 CLASSNAME;
	
	void Init();
	void OnFastView()		{scatter.SetFastViewX(fastView);};
	void OnSequentialX()	{scatter.SetSequentialXAll(sequentialX);};
	virtual ScatterCtrl &Scatter()	{return scatter;};

private:
	Vector<Pointf> s1,s2;
};

class Tab10 : public WithTab10<ScatterDemo> {
public:
	typedef Tab10 CLASSNAME;
	
	void Init();
	virtual ScatterCtrl &Scatter()	{return scatter;};

private:
	double sx[5], sy[5];
};

class Tab11 : public WithTab11<ScatterDemo> {
public:
	typedef Tab11 CLASSNAME;

	Tab11() {};	
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

class Tab12 : public WithTab12<ScatterDemo> {
public:
	typedef Tab12 CLASSNAME;
	
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
	Vector<Vector <double> > s1;
	Vector<int> idsRange;
};

class TabBubblePlot : public WithTabBubblePlot<ScatterDemo> {
public:
	typedef TabBubblePlot CLASSNAME;
	
	void Init();
	virtual ScatterCtrl &Scatter()	{return scatter;};
	
private:
	Vector<Vector <double> > s1;
	Vector<int> idsBall;
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

