#ifndef _ScatterCtrl_Properties_h_
#define _ScatterCtrl_Properties_h_

class GeneralTab : public WithGeneral<StaticRect> {
public:
	typedef GeneralTab CLASSNAME;
	
	void Init(ScatterCtrl &scatter);

private:
	ScatterCtrl *pscatter;
	
	void Change();
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

class DataTab : public WithData<StaticRect> {
public:
	typedef DataTab CLASSNAME;
	
	void Init(ScatterCtrl& scatter);
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
	GeneralTab general;
	SeriesTab series;
	DataTab data;
};

#endif
