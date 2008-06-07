/*
PieChart control
V0.3, Sept. 2007
Copyright (C) 2006-2007, Andrei-Catalin
*/

#ifndef _Scatter_Pie_h_
#define _Scatter_Pie_h_
#include <CtrlLib/CtrlLib.h>
using namespace Upp;


typedef class::Color Acolor;
class PieChart: public Ctrl
{
  public:
	enum titlePosition {BOTTOM, TOP};
	
	PieChart& SetColor(const class::Color& _color);
	PieChart& SetTitle(const String& _title);
	PieChart& SetTitleFont(const Font& fontTitle);
	PieChart& SetTitleColor(const class::Color& colorTitle);
	PieChart& TitlePos(titlePosition pos=TOP);
	PieChart& SetTitleGap(const int& gap);
	
	PieChart& SetRadius(const int& r);
	
	PieChart& ShowPercent(bool show=true);
	PieChart& SetPercentBack(const class::Color& pbcolor);
	
	PieChart& SetLegendFont(const Font& fontLegend);
	PieChart& SetLegendTextColor(const class::Color& colorLegend);
	PieChart& SetLegendBackColor(const class::Color& colorBack);
	PieChart& SetLegend_cx(const int& h);
	PieChart& SetLegend_cy(const int& v);
	PieChart& SetShowLegend(bool show);
	
	void AddCategory(const String& name, const double& value, const class::Color& catcolor);
	void RemoveCategory(const int& index);
	PieChart& SetCatValue(const int& index, const double& value);
	PieChart& SetCatName(const int& index, const String& name);
	PieChart& SetCatColor(const int& index, const class::Color& catcolor);
	double GetCatValue (const int& index)const;
	String GetCatName (const int& index)const;
	Acolor GetCatColor (const int& index)const;
	int GetCatCount() const;	
	
	PieChart();
	~PieChart();
	
	void Paint(Draw& w);
	
	Drawing GetDrawing();
	Image GetImage(int scale=1);
	#ifdef PLATFORM_WIN32
	void SaveAsMetafile(const char* file);
	#endif
	
	
  private:	
	class::Color backColor;
	String title;
	Font titleFont;
	class::Color titleColor;
	titlePosition titlePos;
	int titleGap;
	
	int radius;
	bool showPercent;
	Acolor percentBack;
	
	Font legendFont;
	class::Color legendTextColor, legendBackColor;
	int legend_cx, legend_cy;
	bool showLegend;
	
	Vector<String> vNames;
	Vector<double> vValues;
	Vector<class::Color> vColors;
	
	String GetPercent(double a, double total);
	
	void PaintPie(Draw& w,int scale);
	void DrawChart(Draw& w,int scale);
	
	
};
#endif


