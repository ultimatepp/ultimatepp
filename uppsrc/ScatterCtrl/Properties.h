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
	
	PropertiesDlg(ScatterCtrl& scatter);
	void OnTab(); 
	void OnClose();
		
private:
	ScatterCtrl& scatter;
	MeasuresTab measures;
	TextsTab texts;
	LegendTab legend;
	SeriesTab series;
};


#endif
