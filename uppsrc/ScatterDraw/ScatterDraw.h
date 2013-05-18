#ifndef _ScatterDraw_ScatterDraw_h
#define _ScatterDraw_ScatterDraw_h

#include <Draw/Draw.h>
#include <Painter/Painter.h>
#include "DataSource.h"
#include "Equation.h"

NAMESPACE_UPP

#include "DrawingFunctions.h"
#include "SeriesPlot.h"
#include "MarkPlot.h"

Color GetOpaqueColor(const Color &color, const Color &background, double opacity);

void debug_h();			// Dummy function used to debug .h files
	
class ScatterDraw {
public:
	typedef ScatterDraw CLASSNAME;
	ScatterDraw();
	
	enum Formats {
		EXP,
		MON,
		DY,		
		CUSTOM
	};
	enum {
		MD_DRAW		   = -1,
		MD_ANTIALIASED = MODE_ANTIALIASED,
		MD_NOAA        = MODE_NOAA,
		MD_SUBPIXEL    = MODE_SUBPIXEL
	};
	
	#define LINE_DOTTED 	"4 10"
	#define LINE_DOTTED_SEP	"4 20"
	#define LINE_DASHED 	"12 12"
	#define LINE_DASH_DOT 	"20 10 5 10"
	#define LINE_SOLID 		""
	
protected:
	class ScatterBasicSeries {
	public:
		ScatterBasicSeries();
		void Init(int index);
		
		bool primaryY;
		bool sequential;
		
		One<SeriesPlot> seriesPlot;
		double thickness;
		Color color;
		String dash;
		
		One<MarkPlot> markPlot;
		double markWidth;
		Color markColor;
		
		Color fillColor;
		
		String legend;
		
		double opacity;

		int id;
		
		bool trendLine;
		//double trend
	};
		
	class ScatterSeries : public Moveable<ScatterSeries>, public ScatterBasicSeries {
	public:
		ScatterSeries()	{pD = 0;}
		void SetDataSource(DataSource *pointsData, bool ownsData = true) {pD = pointsData; owns = ownsData;}
		inline DataSource *PointsData()	{return pD;}
		~ScatterSeries()	{if(pD && owns) delete pD;}
	private:
		DataSource *pD;
		bool owns;
	};

	static Color GetNewColor(int index);
	static String GetNewDash(int index);
	static MarkPlot *GetNewMarkPlot(int index);
	
public:
	Callback3<String&, int, double> cbModifFormatX;
	Callback3<String&, int, double> cbModifFormatDeltaX;
	Callback3<String&, int, double> cbModifFormatY;
	Callback3<String&, int, double> cbModifFormatDeltaY;
	Callback3<String&, int, double> cbModifFormatY2;
	Callback3<String&, int, double> cbModifFormatDeltaY2;
			
	Callback WhenZoomScroll;
	Callback WhenSetRange;
	Callback WhenSetXYMin;
	
	ScatterDraw& SetSize(Size sz) 	{size = sz; return *this;};
	virtual Size GetSize() 			{return size;};
	
	ScatterDraw& SetColor(const Color& _color);
	ScatterDraw& SetTitle(const String& _title);
	const String& GetTitle();
	ScatterDraw& SetTitleFont(const Font& fontTitle);
	ScatterDraw& SetTitleColor(const Color& colorTitle);
	Font& GetTitleFont() {return titleFont;};
	
	void SetLabels(const String& _xLabel, const String& _yLabel, const String& _yLabel2 = "");
	ScatterDraw& SetLabelX(const String& _xLabel);
	const String &GetLabelX()	{return xLabel;} 
	ScatterDraw& SetLabelY(const String& _yLabel);
	const String &GetLabelY()	{return yLabel;} 
	ScatterDraw& SetLabelY2(const String& _yLabel);
	const String &GetLabelY2()	{return yLabel2;}
	ScatterDraw& SetLabelsFont(const Font& fontLabels);
	Font GetLabelsFont() {return labelsFont;};
	ScatterDraw& SetLabelsColor(const Color& colorLabels);
	
	ScatterDraw& SetPlotAreaMargin(int hLeft, int hRight, int vTop, int vBottom);
	ScatterDraw& SetPlotAreaLeftMargin(int margin);	
	ScatterDraw& SetPlotAreaRightMargin(int margin);	
	ScatterDraw& SetPlotAreaTopMargin(int margin);	
	ScatterDraw& SetPlotAreaBottomMargin(int margin);
	
	ScatterDraw& SetPlotAreaColor(const Color& p_a_color);
	
	ScatterDraw& SetAxisColor(const Color& axis_color);
	ScatterDraw& SetAxisWidth(int axis_width);
	
	ScatterDraw& SetGridColor(const Color& grid_color);
	ScatterDraw& SetGridWidth(int grid_width);
	ScatterDraw& ShowVGrid(bool show);
	ScatterDraw& ShowHGrid(bool show);
	
	ScatterDraw& ShowLegend(bool show = true) 			{showLegend = show;		return *this;}
	bool GetShowLegend()								{return showLegend;}
	ScatterDraw& SetLegendWidth(int width)				{legendWidth = width;	return *this;}
	int GetLegendWidth()								{return legendWidth;}
	
	ScatterDraw& SetMode(int _mode = MD_ANTIALIASED)	{mode = _mode; Refresh(); return *this;};
	int GetMode()	{return mode;};
	
	void FitToData(bool Y = false);
	void Zoom(double scale, bool hor = true, bool ver = true); 
	void Scroll(double factorX, double factorY);
	
	enum ZoomStyle {TO_CENTER, FROM_BASE};
	ScatterDraw &SetZoomStyleX(ZoomStyle style = TO_CENTER) {zoomStyleX = style; return *this;}
	ScatterDraw &SetZoomStyleY(ZoomStyle style = TO_CENTER) {zoomStyleY = style; return *this;}

	ScatterDraw& SetRange(double rx, double ry, double ry2 = 100);
	double GetXRange()const {return xRange;}
	double GetYRange()const {return yRange;}
	double GetY2Range()const {return yRange2;}
	ScatterDraw &SetMajorUnits(double ux, double uy);
	ScatterDraw &SetMajorUnitsNum(int nx, int ny);
	ScatterDraw &SetMaxMajorUnits(int maxX, int maxY)	{maxMajorUnitsX = maxX; maxMajorUnitsY = maxY; return *this;}
	double GetMajorUnitsX() {return xMajorUnit;}
	double GetMajorUnitsY() {return yMajorUnit;}
	ScatterDraw& SetMinUnits(double ux, double uy);
	double GetXMinUnit () const {return xMinUnit;}
	double GetYMinUnit () const {return yMinUnit;}	
	
	ScatterDraw& SetXYMin(double xmin,double ymin,double ymin2 = 0);
	double GetXMin () const {return xMin;}
	double GetYMin () const {return yMin;}	
	double GetYMin2 () const {return yMin2;}
	
	ScatterDraw &Graduation_FormatX(Formats fi);	
	ScatterDraw &Graduation_FormatY(Formats fi);
	ScatterDraw &Graduation_FormatY2(Formats fi);
	
	ScatterDraw &AddSeries(double *yData, int numData, double x0 = 0, double deltaX = 1)
														{return AddSeries<CArray>(yData, numData, x0, deltaX);}
	ScatterDraw &AddSeries(double *xData, double *yData, int numData)
														{return AddSeries<CArray>(yData, xData, numData);}
	ScatterDraw &AddSeries(Vector<double> &xData, Vector<double> &yData)
														{return AddSeries<VectorDouble>(yData, xData);}
	ScatterDraw &AddSeries(Array<double> &xData, Array<double> &yData)
														{return AddSeries<ArrayDouble>(yData, xData);}		
	ScatterDraw &AddSeries(Vector<Pointf> &points)		{return AddSeries<VectorPointf>(points);}
	ScatterDraw &AddSeries(Array<Pointf> &points)		{return AddSeries<ArrayPointf>(points);}
	ScatterDraw &AddSeries(double (*function)(double))	{return AddSeries<FuncSource>(function);}
	ScatterDraw &AddSeries(void (*function)(double&, double))
														{return AddSeries<FuncSourceV>(function);}
	ScatterDraw &AddSeries(Pointf (*function)(double), int np, double from = 0, double to = 1)	
														{return AddSeries<FuncSourcePara>(function, np, from, to);}
	ScatterDraw &AddSeries(PlotExplicFunc function)		{return AddSeries<PlotExplicFuncSource>(function);}
	ScatterDraw &AddSeries(PlotParamFunc function, int np, double from = 0, double to = 1)	
														{return AddSeries<PlotParamFuncSource>(function, np, from, to);}
	
	ScatterDraw &_AddSeries(DataSource *data);
	ScatterDraw &AddSeries(DataSource &data);
	
	template <class C>
	ScatterDraw &AddSeries() 	{return _AddSeries(new C());}	
	template <class C, class T1>
	ScatterDraw &AddSeries(T1 &arg1) 			
								{return _AddSeries(new C(arg1));}
	template <class C, class T1, class T2>
	ScatterDraw &AddSeries(T1 &arg1, T2 &arg2) 	
								{return _AddSeries(new C(arg1, arg2));}
	template <class C, class T1, class T2, class T3>
	ScatterDraw &AddSeries(T1 &arg1, T2 &arg2, T3 &arg3) 								
								{return _AddSeries(new C(arg1, arg2, arg3));}
	template <class C, class T1, class T2, class T3, class T4>
	ScatterDraw &AddSeries(T1 &arg1, T2 &arg2, T3 &arg3, T4 &arg4) 						
								{return _AddSeries(new C(arg1, arg2, arg3, arg4));}
	template <class C, class T1, class T2, class T3, class T4, class T5>
	ScatterDraw &AddSeries(T1 &arg1, T2 &arg2, T3 &arg3, T4 &arg4, T5 &arg5)			
								{return _AddSeries(new C(arg1, arg2, arg3, arg4, arg5));}
	template <class C, class T1, class T2, class T3, class T4, class T5, class T6>
	ScatterDraw &AddSeries(T1 &arg1, T2 &arg2, T3 &arg3, T4 &arg4, T5 &arg5, T6 &arg6)	
								{return _AddSeries(new C(arg1, arg2, arg3, arg4, arg5, arg6));}
		
	template <class Y>
	ScatterDraw &AddSeries(Vector<Y> &yData)		{return _AddSeries(new VectorY<Y>(yData));}
	template <class Y>
	ScatterDraw &AddSeries(Upp::Array<Y> &yData)	{return _AddSeries(new ArrayY<Y>(yData));}
	template <class X, class Y>
	ScatterDraw &AddSeries(VectorMap<X, Y> &data)	{return _AddSeries(new VectorMapXY<X, Y>(data));}
	template <class X, class Y>
	ScatterDraw &AddSeries(ArrayMap<X, Y> &data)	{return _AddSeries(new ArrayMapXY<X, Y>(data));}
	
	void InsertSeries(int index, double *yData, int numData, double x0 = 0, double deltaX = 1);
	void InsertSeries(int index, double *xData, double *yData, int numData);
	void InsertSeries(int index, Vector<double> &xData, Vector<double> &yData);
	void InsertSeries(int index, Upp::Array<double> &xData, Upp::Array<double> &yData);
	void InsertSeries(int index, Vector<Pointf> &points);
	void InsertSeries(int index, Upp::Array<Pointf> &points);
	void InsertSeries(int index, double (*function)(double));
	void InsertSeries(int index, Pointf (*function)(double), int np, double from = 0, double to = 1);
	void InsertSeries(int index, PlotExplicFunc &function);
	void InsertSeries(int index, PlotParamFunc function, int np, double from = 0, double to = 1);
	void _InsertSeries(int index, DataSource *data);
	
	template <class C>
	void InsertSeries(int index) 		{_InsertSeries(index, new C());}	
	template <class C, class T1>
	void InsertSeries(int index, T1 &arg1) 				
									{_InsertSeries(index, new C(arg1));}
	template <class C, class T1, class T2>
	void InsertSeries(int index, T1 &arg1, T2 &arg2) 		
									{_InsertSeries(index, new C(arg1, arg2));}
	template <class C, class T1, class T2, class T3>
	void InsertSeries(int index, T1 &arg1, T2 &arg2, T3 &arg3) 								
									{_InsertSeries(index, new C(arg1, arg2, arg3));}
	template <class C, class T1, class T2, class T3, class T4>
	void InsertSeries(int index, T1 &arg1, T2 &arg2, T3 &arg3, T4 &arg4)						
									{_InsertSeries(index, new C(arg1, arg2, arg3, arg4));}
	template <class C, class T1, class T2, class T3, class T4, class T5>
	void InsertSeries(int index, T1 &arg1, T2 &arg2, T3 &arg3, T4 &arg4, T5 &arg5)				
									{_InsertSeries(index, new C(arg1, arg2, arg3, arg4, arg5));}
	template <class C, class T1, class T2, class T3, class T4, class T5, class T6>
	void InsertSeries(int index, T1 &arg1, T2 &arg2, T3 &arg3, T4 &arg4, T5 &arg5, T6 &arg6)	
									{_InsertSeries(index, new C(arg1, arg2, arg3, arg4, arg5, arg6));}
			
	template <class Y>
	void InsertSeries(int index, Vector<Y> &yData)		{_InsertSeries(index, new VectorY<Y>(yData));}
	template <class Y>
	void InsertSeries(int index, Upp::Array<Y> &yData)	{_InsertSeries(index, new ArrayY<Y>(yData));}
	template <class X, class Y>
	void InsertSeries(int index, VectorMap<X, Y> &data){_InsertSeries(index, new VectorMapXY<X, Y>(data));}
	template <class X, class Y>
	void InsertSeries(int index, ArrayMap<X, Y> &data)	{_InsertSeries(index, new ArrayMapXY<X, Y>(data));}
	
	int64 GetCount(int index);
	void GetValues(int index, int64 idata, double &x, double &y);
	double GetValueX(int index, int64 idata);
	double GetValueY(int index, int64 idata);
	
	ScatterDraw &PlotStyle()								{return PlotStyle(0);};
	template <class C>
	ScatterDraw &PlotStyle()								{return PlotStyle(new C());};
	template <class C, class T1>
	ScatterDraw &PlotStyle(T1 &arg1)						{return PlotStyle(new C(arg1));};
	template <class C, class T1, class T2>
	ScatterDraw &PlotStyle(T1 &arg1, T2 &arg2)				{return PlotStyle(new C(arg1, arg2));};
	template <class C, class T1, class T2, class T3>
	ScatterDraw &PlotStyle(T1 &arg1, T2 &arg2, T3 &arg3)	{return PlotStyle(new C(arg1, arg2, arg3));};			
	ScatterDraw &PlotStyle(SeriesPlot *data);
	
	ScatterDraw &NoPlot()	{return PlotStyle();};

	ScatterDraw &MarkStyle()								{return MarkStyle(0);};
	template <class C>
	ScatterDraw &MarkStyle()								{return MarkStyle(new C());};
	template <class C, class T1>
	ScatterDraw &MarkStyle(T1 &arg1)						{return MarkStyle(new C(arg1));};
	template <class C, class T1, class T2>
	ScatterDraw &MarkStyle(T1 &arg1, T2 &arg2)				{return MarkStyle(new C(arg1, arg2));};
	template <class C, class T1, class T2, class T3>
	ScatterDraw &MarkStyle(T1 &arg1, T2 &arg2, T3 &arg3)	{return MarkStyle(new C(arg1, arg2, arg3));};			
	ScatterDraw &MarkStyle(MarkPlot *data);
	ScatterDraw &MarkStyle(int index, MarkPlot *data);
	ScatterDraw &MarkStyle(int index, const String name);
	const String GetMarkStyleName(int index);
	
	ScatterDraw &NoMark()	{return MarkStyle();};
		
	ScatterDraw &Stroke(double thickness = 3, Color color = Null);
	ScatterDraw &Dash(const char *dash);
	ScatterDraw &Dash(int index, const char *dash);
	const String GetDash(int index);
	ScatterDraw &Fill(Color color = Null);
	ScatterDraw &MarkColor(Color color = Null);
	ScatterDraw &MarkWidth(double markWidth = 8);
	ScatterDraw &Hide() {series[series.GetCount() - 1].opacity = 0;	return *this;}
	
	ScatterDraw &Opacity(double opacity = 1) {series[series.GetCount() - 1].opacity = opacity;	return *this;}
	ScatterDraw &Legend(const String legend);
	ScatterDraw& Legend(int index, const String legend);
	const String& GetLegend(int index);
	
	inline bool IsValid(int index) const {return (index >= 0 && index < series.GetCount());}
	
	ScatterDraw& SetDrawXReticle(bool set = true);
	ScatterDraw& SetDrawYReticle(bool set = true);
	ScatterDraw& SetDrawY2Reticle(bool set = true);
	
	void SetDataColor(int index, const Color& pcolor);
	Color GetDataColor (int index) const;
	void SetDataThickness(int index, double thick);
	double GetDataThickness(int index) const;
	void SetFillColor(int index, const Color& color);
	Color GetFillColor(int index) const;

	ScatterDraw &SetMarkWidth(int index, double width);
	double GetMarkWidth(int index);
	void SetMarkColor(int index, const Color& pcolor);
	Color GetMarkColor(int index) const;
	void NoMark(int index);
	bool IsShowMark(int index) const throw (Exc);
	
	void SetDataPrimaryY(int index, bool primary = true);
	ScatterDraw &SetDataPrimaryY(bool primary); 	
	bool IsDataPrimaryY(int index) const throw (Exc);	
	
	void SetSequentialX(int index, bool sequential = true);
	ScatterDraw &SetSequentialX(bool sequential = true);
	ScatterDraw &SetSequentialXAll(bool sequential = true);
	
	void Show(int index, bool show = true);
	bool IsVisible(int index);
	ScatterDraw &ShowAll(bool show = true);

	void RemoveSeries(int index);
	void RemoveAllSeries();
	
	ScatterDraw& Id(int id);
	ScatterDraw& Id(int index, int id);
	int GetId(int index);
	
	Drawing GetDrawing(bool ctrl = true);
	Image GetImage(const Size &size, int scale = 2, bool ctrl = true);
	Image GetImage(int scale = 2);
	
	#ifdef PLATFORM_WIN32
	void SaveAsMetafile(const char* file) const;
	#endif
	
	ScatterDraw& SetMinZoom(double x, double y = -1) 	{minXZoom = x; minYZoom = y; return *this;} 
	ScatterDraw& SetMaxZoom(double x, double y = -1) 	{maxXZoom = x; maxYZoom = y; return *this;}

	ScatterDraw& SetFastViewX(bool set = true) 			{fastViewX = set;	return *this;}
	
	double GetXByPoint(double x);
	double GetYByPoint(double y);
	double GetY2ByPoint(double y);
	double GetXPointByValue(double x);
	double GetYPointByValue(double y);

	int GetCount() 	{return series.GetCount();}
	bool IsEmpty()	{return series.IsEmpty();}
	
	virtual void Refresh() {};
	
protected:
	int mode;
	Color graphColor;	
	String title;
	Font titleFont;
	Color titleColor;
	int titleHeight;
	
	String xLabel, yLabel, yLabel2;
	Font labelsFont;
	Color labelsColor;
	
	int   hPlotLeft, hPlotRight, vPlotTop, vPlotBottom;
	Color plotAreaColor;
	
	bool fastViewX, sequentialXAll;
	
	Color axisColor;
	int axisWidth;
	
	double xRange, yRange, yRange2;
	double xMin, yMin, yMin2;
	double xMajorUnit, yMajorUnit, yMajorUnit2;
	double xMinUnit, yMinUnit, yMinUnit2;
	double minXZoom, maxXZoom, minYZoom, maxYZoom;
	double lastxRange, lastyRange;
	bool drawXReticle, drawYReticle, drawY2Reticle;	
	
	int maxMajorUnitsX, maxMajorUnitsY;
	
	Color gridColor;
	int gridWidth;
	bool drawVGrid, drawHGrid;	
		
	int butDownX, butDownY;
	bool isScrolling, isLabelPopUp;
	ZoomStyle zoomStyleX, zoomStyleY;	
	
	Vector<ScatterSeries> series;
	
	bool showLegend;
	int legendWidth;
	
	void DrawLegend(Draw& w, const Size &size, int scale) const;

	void Scrolling(bool down, Point &pt, bool isOut = false);
	
	void ExpFormat(String& s, int i, double d)	{s = FormatDoubleExp(d,1);}
	void MonFormat(String& s, int i, double d)	{s = Format("%Mon",int(d));}
	void DyFormat(String& s, int i, double d)	{s = Format("%Dy",int(d));}
	
	static String VariableFormat(double range, double d)
	{
		if (fabs(d) <= 1e-15)
			d = 0;
		if 		(0.001 <= range && range < 0.01)   return FormatDouble(d, 5);
		else if (0.01  <= range && range < 0.1)    return FormatDouble(d, 4);
		else if (0.1   <= range && range < 1) 	   return FormatDouble(d, 3);
		else if (1	   <= range && range < 10) 	   return FormatDouble(d, 2);
		else if (10	   <= range && range < 100)    return FormatDouble(d, 1);
		else if (100   <= range && range < 100000) {
							if (d < 1 && d > -1)   return "0";	// Never -0
							else				   return FormatDouble(d, 0);
		} else return FormatDoubleExp(d, 2);
	}	
	String VariableFormatX(double d) const  {return VariableFormat(xRange, d);}
	String VariableFormatY(double d) const  {return VariableFormat(yRange, d);} 
	String VariableFormatY2(double d) const {return VariableFormat(yRange2, d);}

	template<class T>
	void SetDrawing(T& w, const Size &size, int scale, bool ctrl = false);
	template<class T>
	void Plot(T& w, const Size &size, int scale);	
	template<class T>
	bool PlotTexts(T& w, const Size &size, int scale);
		
	void AdjustMinUnitX();
	void AdjustMinUnitY();
	void AdjustMinUnitY2();
	
private:
	Size size;		// Size to be used for all but screen painting
	static void ParseTextMultiline(const String &text, Font fnt, 
								   Upp::Array <String> &texts, Upp::Array <Size> &sizes);
	int plotW, plotH;
};

template <class T>
void ScatterDraw::SetDrawing(T& w, const Size& size, int scale, bool ctrl)
{
	if (size.cx == 0 || size.cy == 0)
		return;
	
	w.DrawRect(scale*size, graphColor);
	
	titleHeight = !title.IsEmpty() ? scale*titleFont.GetHeight() : 0;
	
	plotW = scale*(size.cx - (hPlotLeft + hPlotRight));
	plotH = scale*(size.cy - (vPlotTop + vPlotBottom)) - titleHeight;
	
	if (!ctrl) {
		if (!PlotTexts(w, GetSize(), scale)) 
			return;
	} else 
		w.Offset(Point(scale*hPlotLeft, scale*vPlotTop + titleHeight));
	
	Plot(w, size, scale);	
	
	ClipEnd(w);
}

template <class T>
bool ScatterDraw::PlotTexts(T& w, const Size &size, int scale)
{
	if(titleHeight > 0) {
		Font fontTitle6;
		fontTitle6 = titleFont;
		fontTitle6.Height(titleHeight);
		fontTitle6.Width(scale*titleFont.GetWidth());
		Size sz = GetTextSize(title, fontTitle6);
		DrawText(w, (scale*size.cx - sz.cx)/2., scale*2., 0, title, fontTitle6, titleColor);   
	}	
	w.Offset(Point(scale*hPlotLeft, scale*vPlotTop + titleHeight));
	if(showLegend) 
		DrawLegend(w, size, scale);
	
	if (plotW < 0 || plotH < 0) 
		return false;

	Font fontLabel;
	fontLabel = labelsFont;
	fontLabel.Height(scale*labelsFont.GetHeight());
	
	Size lx  = GetTextSize(xLabel, 	fontLabel);
	Size ly  = GetTextSize(yLabel, 	fontLabel);
	Size ly2 = GetTextSize(yLabel2, fontLabel);
	DrawText(w, (plotW - lx.cx)/2., plotH + scale*(vPlotBottom - 2) - lx.cy, 0, xLabel, fontLabel, labelsColor);
	DrawText(w, scale*(2 - hPlotLeft), (plotH + ly.cx)/2., 900, yLabel,  fontLabel, labelsColor);
	DrawText(w, scale*size.cx - ly2.cy - 42*scale, (plotH + ly2.cx)/2., 900, yLabel2, fontLabel, labelsColor);

	if (xRange == 0 || xMajorUnit == 0 || yRange == 0 || yMajorUnit == 0 || yRange2 == 0) 
		return false;
	
	Font standard6 = GetStdFont();
	standard6.Height(scale*GetStdFont().GetHeight());
	
	if (drawXReticle)
		for(int i = 0; xMinUnit + i*xMajorUnit <= xRange; i++){
			w.DrawLine(fround(plotW*xMinUnit/xRange + i*plotW/(xRange/xMajorUnit)), plotH,   
					   fround(plotW*xMinUnit/xRange + i*plotW/(xRange/xMajorUnit)), plotH + scale*4, 
					   fround(gridWidth*scale), axisColor);             
			double gridX = xMinUnit + i*xMajorUnit + xMin;
			String gridLabelX;
			if (cbModifFormatX)
				cbModifFormatX(gridLabelX, i, gridX);
			else
				gridLabelX = VariableFormatX(gridX);
			
			Array <String> texts;
			Array <Size> sizes;
			ParseTextMultiline(gridLabelX, GetStdFont(), texts, sizes);
			for (int ii = 0; ii < texts.GetCount(); ++ii) {
				int cy = ii == 0 ? 0 : sizes[ii - 1].cy;
				DrawText(w, plotW*xMinUnit/xRange + i*plotW/(xRange/xMajorUnit) - scale*sizes[ii].cx/2., 
							plotH + scale*(4 + ii*cy), 0, texts[ii], standard6, axisColor);
			}
		}

	if (drawYReticle)
		for(int i = 0; yMinUnit + i*yMajorUnit <= yRange; i++) {
			int reticleY = fround(-plotH*yMinUnit/yRange + plotH - i*plotH/(yRange/yMajorUnit));
			w.DrawLine(-scale*4, reticleY, 0, reticleY, fround(gridWidth*scale), axisColor);
			if (drawY2Reticle)
				w.DrawLine(plotW + scale*4, reticleY, plotW, reticleY, fround(gridWidth*scale), axisColor);
			double gridY = yMinUnit + i*yMajorUnit + yMin;
			String gridLabelY;
			if (cbModifFormatY)
				cbModifFormatY(gridLabelY, i, gridY);
			else
				gridLabelY = VariableFormatY(gridY);
			int dx = scale*GetTextSize(gridLabelY, GetStdFont()).cx;
			DrawText(w, -dx - scale*6, reticleY - scale*8, 0, gridLabelY, standard6, axisColor);
			if (drawY2Reticle) {
				double gridY2 = (gridY - yMin)/yRange*yRange2 + yMin2;
				String gridLabelY2;
				if (cbModifFormatY2)
					cbModifFormatY2(gridLabelY2, i, gridY2);
				else
					gridLabelY2 = VariableFormatY2(gridY2);
				DrawText(w, plotW + scale*10, reticleY - scale*8, 0, gridLabelY2, standard6, axisColor);
			}
		}
		
	w.DrawLine(0, plotH, plotW, plotH, fround(gridWidth*scale), Black);
	w.DrawLine(0, 0, plotW, 0, fround(gridWidth*scale), Black);
	w.DrawLine(0, 0, 0, plotH, fround(gridWidth*scale), Black);
	w.DrawLine(plotW, 0, plotW, plotH + 1, fround(gridWidth*scale), Black);
	
	return true;
}

template <class T>
void ScatterDraw::Plot(T& w, const Size &size, int scale)
{
	double d1 = xRange/xMajorUnit;
	double d2 = yRange/yMajorUnit;

	Clip(w, 0, 0, plotW, plotH);

	w.DrawRect(0, 0, plotW, plotH, plotAreaColor);	

	if (drawVGrid) {
		double x0 = plotW*xMinUnit/xRange;
		for(int i = 0; xMinUnit + i*xMajorUnit < xRange; i++) {
			int xg = fround(x0 + i*plotW/d1);
			if (xg > gridWidth || xg < plotW - gridWidth)
				DrawLineOpa(w, xg, 0, xg, fround(plotH), 1, 1, gridWidth, gridColor, "2 2");
		}
	}	
	if (drawHGrid) {
		double y0 = -plotH*yMinUnit/yRange + plotH;
		for(int i = 0; yMinUnit + i*yMajorUnit < yRange; i++) {
			int yg = fround(y0 - i*plotH/d2);
			if (yg > gridWidth || yg < plotH - gridWidth)
				DrawLineOpa(w, 0, yg, fround(plotW), yg, 1, 1, gridWidth, gridColor, "2 2");
		}
	}

	if (!series.IsEmpty()) {
		for (int j = 0; j < series.GetCount(); j++) {
			if (series[j].opacity == 0 || (!series[j].seriesPlot && !series[j].markPlot))
				continue;
			Vector<Point> points;
			if (series[j].PointsData()->IsParam()) {
				double xmin = 0;
				double xmax = double(series[j].PointsData()->GetCount());
				for (double x = xmin; x <= xmax; x++) {
					double xx = series[j].PointsData()->x(x);
					double yy = series[j].PointsData()->y(x);
					int ix = fround(plotW*(xx - xMin)/xRange);
					int iy;
					if (series[j].primaryY)
						iy = fround(plotH*(yy - yMin)/yRange);
					else
						iy = fround(plotH*(yy - yMin2)/yRange2);
					points << Point(ix, plotH - iy);
				}
			} else if (series[j].PointsData()->IsExplicit()) {
				double xmin = xMin - 1;
				double xmax = xMin + xRange + 1; 	
				double dx = double(xmax - xmin)/plotW;		
				for (double xx = xmin; xx < xmax; xx += dx) {
					double yy = series[j].PointsData()->f(xx);
					int ix = fround(plotW*(xx - xMin)/xRange);
					int iy;
					if (series[j].primaryY)
						iy = fround(plotH*(yy - yMin)/yRange);
					else
						iy = fround(plotH*(yy - yMin2)/yRange2);
					points << Point(ix, plotH - iy);
				}
			} else {
				int64 imin, imax;
				if (series[j].sequential) {
					imin = imax = Null;
					for (int64 i = 1; i < series[j].PointsData()->GetCount() - 1; ++i) {
						if (IsNull(imin)) {
							if (series[j].PointsData()->x(i) >= xMin)
								imin = i - 1;
						} else if (IsNull(imax)) {
							if (series[j].PointsData()->x(i) >= xMin + xRange)
								imax = i + 1;
						}
					}
					if (IsNull(imin))
					    imin = 0;
					if (IsNull(imax))
					    imax = series[j].PointsData()->GetCount();
				} else {
					imin = 0;
					imax = series[j].PointsData()->GetCount();
				}
				int64 dx;
				if (fastViewX)
					dx = max<int64>(1, (imax - imin)/plotW);			
				else
					dx = 1;
				for (int64 i = imin; i < imax; i += dx) {
					double xx, yy;
					if (fastViewX) {					
						yy = 0;
						int ii;
						for (ii = 0; ii < dx && i + ii < imax; ++ii) 
							yy += series[j].PointsData()->y(i + ii);
						yy /= double(ii);
						xx = (series[j].PointsData()->x(i) + series[j].PointsData()->x(i + ii - 1))/2;
					} else {
						xx = series[j].PointsData()->x(i);
						yy = series[j].PointsData()->y(i);
					}
					int ix = fround(plotW*(xx - xMin)/xRange);
					int iy;
					if (series[j].primaryY)
						iy = fround(plotH*(yy - yMin)/yRange);
					else
						iy = fround(plotH*(yy - yMin2)/yRange2);
					points << Point(ix, plotH - iy);
				}	
			}
			if (!points.IsEmpty() && series[j].seriesPlot) 
				series[j].seriesPlot->Paint(w, points, scale, series[j].opacity, 
											fround(series[j].thickness), series[j].color, 
											series[j].dash, plotAreaColor, series[j].fillColor, plotW/xRange, plotH/yRange, 
											int(plotH*(1 + yMin/yRange)));
		
			if (series[j].markWidth >= 1 && series[j].markPlot) {
				for (int i = 0; i < points.GetCount(); i++) 
					series[j].markPlot->Paint(w, scale, points[i], series[j].markWidth, series[j].markColor);              
			}	
		}
	}
	ClipEnd(w);
}

END_UPP_NAMESPACE
		
#endif

