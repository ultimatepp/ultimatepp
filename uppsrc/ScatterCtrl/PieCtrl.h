#ifndef _ScatterDraw_PieCtrl_h
#define _ScatterDraw_PieCtrl_h

#include <CtrlLib/CtrlLib.h>
#include <ScatterDraw/PieDraw.h>

NAMESPACE_UPP


class PieCtrl: public StaticRect, public PieDraw {
public:
	typedef PieCtrl CLASSNAME;
	PieCtrl();

 	void Paint(Draw& w); 	

 	virtual void Refresh() {Ctrl::Refresh();};
 	virtual Size GetSize() const {return Ctrl::GetSize();};
 	
 	void SaveAsMetafile(const char* file);
	void SaveToClipboard(bool saveAsMetafile);
	
	PieCtrl& SetCopyRatio(int ratio)				{copyRatio = ratio; return *this;}
	int GetCopyRatio()								{return copyRatio;} 

private:
	int copyRatio;
	
public:
	PieCtrl& SetTitle(const String& title)		 		{PieDraw::SetTitle(title); 				return *this;}
	
	PieCtrl& ShowPercent(bool show = true)				{PieDraw::ShowPercent(show); 			return *this;}
	PieCtrl& SetPercentBack(const Upp::Color& pbcolor)	{PieDraw::SetPercentBack(pbcolor); 		return *this;}
	
	PieCtrl& ShowLegend(bool show = true)				{PieDraw::ShowLegend(show); 			return *this;}
	PieCtrl& SetLegendFont(const Font& font)			{PieDraw::SetLegendFont(font); 			return *this;}
	PieCtrl& SetLegendTextColor(const Upp::Color& color){PieDraw::SetLegendTextColor(color); 	return *this;}
	PieCtrl& SetLegendBackColor(const Upp::Color& color){PieDraw::SetLegendBackColor(color); 	return *this;}
	PieCtrl& SetLegendLeft(const int& h)				{PieDraw::SetLegendLeft(h); 			return *this;}
	PieCtrl& SetLegendTop(const int& v)					{PieDraw::SetLegendTop(v); 				return *this;}
	PieCtrl& SetLegendHeight(const int& height)			{PieDraw::SetLegendHeight(height); 		return *this;};
};

END_UPP_NAMESPACE

#endif


