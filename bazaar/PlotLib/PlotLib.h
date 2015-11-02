#ifndef _PlotLib_PlotLib_h
#define _PlotLib_PlotLib_h

#include <Painter/Painter.h>
using namespace Upp;

#include "PlotSymbol.h"

enum{
	PLOT_AA          = MODE_ANTIALIASED,
	PLOT_NOAA        = MODE_NOAA,
	PLOT_SUBPIXEL    = MODE_SUBPIXEL
};
class Plot;

class PlotData:public Vector<Pointf>, public MoveableAndDeepCopyOption<PlotData>{
protected:
	RGBA color;
	double t;
	String dash,title;
	bool visible;
	Rectf BB;
	void RefreshBB();
	One<PlotSymbol> symbol;
public:
	PlotData& SetColor(RGBA col)            {color=col;     return *this;}
	PlotData& Visible(bool vis)             {visible=vis;   return *this;}
	PlotData& Invisible()                   {visible=false; return *this;}
	PlotData& SetDash(String pattern)       {dash=pattern;  return *this;}
	PlotData& SetTitle(String str)          {title=str;     return *this;}
	PlotData& SetThickness(double thickness){t=thickness;   return *this;}
	
	RGBA GetColor()const                    {return color;   }
	bool IsVisible()const                   {return visible; }
	String GetDash()const                   {return dash;    }
	String GetTitle()const                  {return title;   }
	double GetThickness()const              {return t;       }
	PlotSymbol* GetSymbol()                 {return ~symbol; }

	PlotData&   SetDefaults();
	PlotData&   SetSymbol(String symbol,Sizef size,RGBA color=RGBAZero(),RGBA frame=RGBAZero(),double thickness=1);
	PlotData&   SetSymbol(String symbol);
	PlotData&   SetSymbol(PlotSymbol* symbol);
	void AddXY(double x,double y);
	Image       GetIcon();
	String      ToString()const;
	friend class Legend;
	friend class Plot;
	PlotData();
	PlotData(const PlotData& pd,int);
	PlotData(const Vector<Pointf>& data,const char* name="");
	PlotData(const Array<Pointf>& data,const char* name="");
	template <class T>
	PlotData(const Vector<T>& data,const char* name=""){
		SetDefaults();
		SetCount(data.GetCount());
		for(int i=0;i<data.GetCount();i++){
			(*this)[i]=Pointf(i,data[i]);
//			BB.Union((*this)[i]);
		}
		title=name;
	}
	template <class T>
	PlotData(const Array<T>& data,const char* name=""){
		SetDefaults();
		SetCount(data.GetCount());
		for(int i=0;i<data.GetCount();i++){
			(*this)[i]=Pointf(i,data[i]);
///			BB.Union((*this)[i]);
		}
		title=name;
	}
	template <class K, class T>
	PlotData(const VectorMap<K,T>& data,const char* name=""){
		SetDefaults();
		SetCount(data.GetCount());
		for(int i=0;i<data.GetCount();i++){
			(*this)[i]=Pointf(data.GetKey(i),data[i]);
//			BB.Union((*this)[i]);
		}
		title=name;
	}
	template <class K, class T>
	PlotData(const ArrayMap<K,T>& data,const char* name=""){
		SetDefaults();
		SetCount(data.GetCount());
		for(int i=0;i<data.GetCount();i++){
			(*this)[i]=Pointf(data.GetKey(i),data[i]);
//			BB.Union((*this)[i]);
		}
		title=name;
	}
};
class Legend{
public:
	Legend(Plot& plot);
	Legend(Plot& plot,RGBA frame,RGBA background,double width=1);
	Image    GetLegend()const;
	Size     GetSizeHint()const;
	unsigned GetHashValue()const;

	RGBA    GetFrameColor()const          {return framecol;}
	RGBA    GetBackground()const          {return bgcol;   }
	Plot&   GetPlot()const                {return p;       }
	double  GetFrameWidth()const          {return fwidth;  }
	Size    GetSize()const                {return size;}

	Legend& SetFrameColor(RGBA frame)     {framecol=frame;   return *this;}
	Legend& SetBackground(RGBA background){bgcol=background; return *this;}
	Legend& SetPlot(Plot& plot);
	Legend& SetFrameWidth(double width)   {fwidth=width;     return *this;}
	void    SetSize(Size sz)              {size=sz;}

	friend class Plot;

private:
	Size size;
	Plot& p;
	double fwidth;
	RGBA bgcol,framecol;
};
class Plot{
public:
	rval_default(Plot);

	Plot();
	Vector<PlotData> data;

	Plot& SetLimits(const Rectf& rect)     {return SetLimits(rect.left,rect.right,rect.top,rect.bottom);}
	Plot& SetLimits(double x_min,double x_max,double y_min,double y_max);
	Plot& SetFont(const Font& font)        {f=font;       return *this;}
	Plot& SetBackground(const Color& c)    {bgcol=c;      return *this;}
	Plot& SetFrameColor(const Color& c)    {framecol=c;   return *this;}
	Plot& SetFontColor(const Color& c)     {fontcol=c;    return *this;}
	Plot& SetAxisColor(const Color& c)     {axiscol=c;    return *this;}
	Plot& SetMode(int quality)             {mode=quality; return *this;}
	Plot& SetPlotSize(double cx,double cy) {return SetPlotSize(Size(cx,cy));}
	Plot& SetPlotSize(const Size& sz);
	
	Rectf GetLimits()const             {return g;}
	Font GetFont()const                {return f;}
	Color GetBackground()const         {return bgcol;}
	Color GetFrameColor()const         {return framecol;}
	Color GetFontColor()const          {return fontcol;}
	Color GetAxisColor()const          {return axiscol;}
	Size GetPlotSize()const            {return size;}

	Rectf BoundingBox(bool visibleonly=false);

	Image GetImage();
	bool CheckRange()const;
	String ImgToPlotFormatted(const Point& X)const;
	String ImgToPlotFormatted(const Rect& X)const;
	int StrWidth(const char *str)const;
	Pointf ImgToPlot(const Pointf& X)const;
	Rectf ImgToPlot(const Rectf& X)const;
	Pointf PlotToImg(const Pointf& X)const;
	Rectf PlotToImg(const Rectf& X)const;
	unsigned GetHashValue()const;
	friend class Legend;
private:
	int mode;
	double sx,sy;
	Rectf g;
	Rect p;
	Font f;
	FontInfo fi;
	Size size;
	Color bgcol,framecol,fontcol,axiscol;
	int marginB,marginL,marginR,marginT,margin,spaceH;
	double Gx(double x)const;
	double Gy(double y)const;
	Pointf G(Pointf X)const;
	Rectf G(Rectf X)const;
	double Sx(double x)const;
	double Sy(double y)const;
	Pointf S(Pointf X)const;
	Rectf S(Rectf X)const;
	void TickH(double x,int e,Painter& sw);
	void TickV(double y,int e,Painter& sw);
	void NormalizeStep(double& s,int& e)const;
	bool Axes(Painter& sw);
	void PaintData(const PlotData& data,Painter& sw);
	int LineClip(double& x1, double& y1, double& x2, double& y2)const;
};

#endif

