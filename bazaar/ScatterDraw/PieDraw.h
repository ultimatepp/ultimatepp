#ifndef _ScatterDraw_PieDraw_h_
#define _ScatterDraw_PieDraw_h_

#include <Draw/Draw.h>
#include <Painter/Painter.h>
#include "DataSource.h"

namespace Upp {

class PieDraw {
public:
	PieDraw();
	virtual ~PieDraw() noexcept {};
	
	enum titlePosition {BOTTOM, TOP};
	
	PieDraw& SetColor(const Color& _color) 		{backColor = _color; 
	RGBA r;
	r = backColor;
	r.a = 0;
	backColor = r;
	
	return *this;}
	PieDraw& SetTitle(const String& _title) 	{title = _title; return *this;}
	String GetTitle()							{return title;}
	PieDraw& SetTitleFont(const Upp::Font& font){titleFont = font; return *this;}
	PieDraw& SetTitleColor(const Color& color)	{titleColor = color; return *this;}
	PieDraw& SetTitlePos(titlePosition pos)		{titlePos = pos; return *this;}
	PieDraw& SetTitleGap(const int& gap)		{titleGap = gap; return *this;}
	
	PieDraw& ShowPercent(bool show = true)			{showPercent = show; return *this;}
	PieDraw& SetPercentBack(const Color& pbcolor)	{percentBack = pbcolor; return *this;}
	
	PieDraw& ShowLegend(bool show = true)			{showLegend = show; return *this;}
	PieDraw& SetLegendFont(const Upp::Font& font)	{legendFont = font; return *this;}
	PieDraw& SetLegendTextColor(const Color& color)	{legendTextColor = color; return *this;}
	PieDraw& SetLegendBackColor(const Color& color)	{legendBackColor = color; return *this;}
	PieDraw& SetLegendLeft(const int& left)			{legendLeft = left; return *this;}
	PieDraw& SetLegendTop(const int& top)			{legendTop = top; return *this;}

	PieDraw& SetPieAngle(const double& angle)		{pieAngle = angle; return *this;}
	PieDraw& SetPieMarginLeft(const int& left)		{pieMarginLeft = left; return *this;}
	PieDraw& SetPieMarginTop(const int& top)		{pieMarginTop = top; return *this;}
	PieDraw& SetPieMarginRight(const int& right)	{pieMarginRight = right; return *this;}
	PieDraw& SetPieMarginBottom(const int& bottom)	{pieMarginBottom = bottom; return *this;}
	
	void AddCategory(const String& name, const double& value, const Color& catcolor);
	void RemoveCategory(const int& index);
	PieDraw& SetCatValue(const int& index, const double& value);
	PieDraw& SetCatName(const int& index, const String& name);
	PieDraw& SetCatColor(const int& index, const Color& catcolor);
	double GetCatValue (const int& index)const;
	String GetCatName (const int& index)const;
	Color GetCatColor (const int& index)const;
	int GetCatCount() const							{return vColors.GetCount();}
	
	Drawing GetDrawing(int scale = 3);
	Image GetImage(int scale = 1);
	
	virtual void Refresh() {};
	
	PieDraw& SetSize(Size sz) {size = sz; return *this;};
	virtual Size GetSize() const {return size;};

protected:
	void PaintPie(Draw& w, int scale);

private:	
	Color backColor;
	String title;
	Upp::Font titleFont;
	Color titleColor;
	titlePosition titlePos;
	int titleGap;
	
	bool showPercent;
	Color percentBack;
	
	Upp::Font legendFont;
	Color legendTextColor, legendBackColor;
	bool showLegend;	
	int legendLeft, legendTop;
	
	double pieAngle;
	int pieMarginLeft, pieMarginTop, pieMarginRight, pieMarginBottom;
	
	Vector<String> vNames;
	Vector<double> vValues;
	Vector<Color> vColors;
	
	Size size;
		
	String GetPercent(double a, double total);
};

}

#endif
