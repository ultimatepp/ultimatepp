#ifndef _ScatterCtrl_Properties_h_
#define _ScatterCtrl_Properties_h_

class MeasuresTab : public WithMeasures<StaticRect> {
public:
	typedef MeasuresTab CLASSNAME;
	
	void Init(ScatterCtrl &scatter);
	void Change();

private:
	ScatterCtrl *pscatter;
};

class TextsTab : public WithTexts<StaticRect> {
public:
	typedef TextsTab CLASSNAME;
	
	void Init(ScatterCtrl &scatter);

private:
	ScatterCtrl *pscatter;
	
	void Change();
};

class LegendTab : public WithLegend<StaticRect> {
public:
	typedef LegendTab CLASSNAME;
	
	void Init(ScatterCtrl &scatter);

private:
	ScatterCtrl *pscatter;
	
	void Change();
	void ChangeAnchor(Option *op);
};

class SeriesTab : public WithSeries<StaticRect> {
public:
	typedef SeriesTab CLASSNAME;
	
	void Init(ScatterCtrl& scatter);
	
private:
	ScatterCtrl *pscatter;	
	
	void Change();
	void UpdateFields();
};

class DataDlg : public WithData<TopWindow> {
public:
	typedef DataDlg CLASSNAME;
	
	void OnClose();
	
	DataDlg(ScatterCtrl& scatter);
	void OnTab(); 
	void OnArrayBar(Bar &menu);
	void ArrayCopy();
	void ArraySelect();
	
	class DataSourceX : public Convert {
	public:
		Value Format(const Value& q) const;
		ScatterDraw *pscatter;
		int index;
	} dataSourceX;
	
	class DataSourceY : public Convert {
	public:
		Value Format(const Value& q) const;
		ScatterDraw *pscatter;
		int index;
	} dataSourceY;
	
private:
	ScatterCtrl *pscatter;	
	
	Array <WithDataSeries <StaticRect> > series;
};

class PropertiesDlg : public WithProperties<TopWindow> {
public:
	typedef PropertiesDlg CLASSNAME;
	
	PropertiesDlg(ScatterCtrl& scatter, int tab = 0);
	void OnTab(); 
	void OnClose();
		
private:
	ScatterCtrl& scatter;
	MeasuresTab measures;
	TextsTab texts;
	LegendTab legend;
	SeriesTab series;
};


class ProcessingTab : public WithProcessingTab<StaticRect> {
public:
	typedef ProcessingTab CLASSNAME;

	ProcessingTab();
	void Init(ScatterCtrl& scatter) {pscatter = &scatter;}
	void UpdateField(const String name, int id);
	void OnFFT();
	void OnOp();
	void OnShowEquation();
	void UpdateEquations();
	void OnFromTo();
	void OnOperation();
	void OnSet();
	void OnUpdateSensitivity();
		
private:
	ScatterCtrl* pscatter;
	int id;
	
	WithProcessingTabFit<StaticRect> tabFit;	
	WithProcessingTabFrequency<StaticRect> tabFreq;
	WithProcessingTabOp<StaticRect> tabOp;
	
	bool avgFirst, linearFirst, cuadraticFirst, cubicFirst, sinusFirst, splineFirst;
	double r2Linear, r2Cuadratic, r2Cubic, r2Sinus;
	bool tabFreqFirst, tabOpFirst;
	
	Vector<Pointf> fft;
	Vector<double> resampledSeries;
	AvgEquation average;
	LinearEquation linear;
	PolynomialEquation2 cuadratic;
	PolynomialEquation3 cubic;
	SinEquation sinus;
	SplineEquation spline;
	Vector<Pointf> upperEnvelope, lowerEnvelope;
	Vector<Pointf> movAvg;
	DataSetCond dataSetCond;
	bool exclamationOpened;
	double newWidthMax, newWidthMin, newWidthMovAvg;
};

class ProcessingDlg : public WithProcessing<TopWindow> {
public:
	typedef ProcessingDlg CLASSNAME;

	ProcessingDlg(ScatterCtrl& scatter);
	void OnClose();
	
private:
	ScatterCtrl& scatter;	
	Array<ProcessingTab> tabs;
	
	void UpdateFields();
};



#endif
