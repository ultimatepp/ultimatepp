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
		double markBorderWidth;
		Color markBorderColor;
		
		Color fillColor;
		
		String legend;
		String unitsX, unitsY;
		
		double opacity;

		int id;
	};
		
	class ScatterSeries : public Moveable<ScatterSeries>, public ScatterBasicSeries {
	public:
		ScatterSeries()	{pD = 0;}
		void SetDataSource(DataSource *pointsData, bool ownsData = true) {pD = pointsData; owns = ownsData;}
		inline DataSource *PointsData()	{ASSERT_(!pD->IsDeleted(), "DataSource in ScatterCtrl/Draw has been deleted.\nIt has been probably declared in a function.");	return pD;}
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
	virtual Size GetSize() const	{return size;};
	
	ScatterDraw& SetColor(const Color& _color);
	ScatterDraw& SetTitle(const String& _title);
	const String& GetTitle();
	ScatterDraw& SetTitleFont(const Font& fontTitle);
	ScatterDraw& SetTitleColor(const Color& colorTitle);
	Font& GetTitleFont() {return titleFont;};
	
	void SetLabels(const String& _xLabel, const String& _yLabel, const String& _yLabel2 = "");
	ScatterDraw& SetLabelX(const String& _xLabel);
	const String &GetLabelX()	{return xLabel_base;} 
	ScatterDraw& SetLabelY(const String& _yLabel);
	const String &GetLabelY()	{return yLabel_base;} 
	ScatterDraw& SetLabelY2(const String& _yLabel);
	const String &GetLabelY2()	{return yLabel2_base;}
	ScatterDraw& SetLabelsFont(const Font& fontLabels);
	Font GetLabelsFont() {return labelsFont;};
	ScatterDraw& SetLabelsColor(const Color& colorLabels);
	
	ScatterDraw& SetPlotAreaMargin(int hLeft, int hRight, int vTop, int vBottom);
	ScatterDraw& SetPlotAreaLeftMargin(int margin);	
	int GetPlotAreaLeftMargin()						{return hPlotLeft;}
	ScatterDraw& SetPlotAreaRightMargin(int margin);	
	int GetPlotAreaRightMargin()					{return hPlotRight;}
	ScatterDraw& SetPlotAreaTopMargin(int margin);	
	int GetPlotAreaTopMargin()						{return vPlotTop;}
	ScatterDraw& SetPlotAreaBottomMargin(int margin);
	int GetPlotAreaBottomMargin()					{return vPlotBottom;}
	
	ScatterDraw& SetPlotAreaColor(const Color& p_a_color);
	Color& GetPlotAreaColor()						{return plotAreaColor;}
	
	ScatterDraw& SetAxisColor(const Color& axis_color);
	ScatterDraw& SetAxisWidth(int axis_width);
	
	ScatterDraw& SetGridColor(const Color& grid_color);
	ScatterDraw& SetGridWidth(int grid_width);
	ScatterDraw& ShowVGrid(bool show);
	ScatterDraw& ShowHGrid(bool show);
	
	ScatterDraw& ShowLegend(bool show = true) 		{showLegend = show;		return *this;}
	bool GetShowLegend()							{return showLegend;}
	ScatterDraw& SetLegendPos(const Point &pos) 	{legendPos = pos;		return *this;}
	ScatterDraw& SetLegendPosX(int x) 				{legendPos.x = x;		return *this;}
	ScatterDraw& SetLegendPosY(int y) 				{legendPos.y = y;		return *this;}
	Point& GetLegendPos() 							{return legendPos;}
	ScatterDraw& SetLegendNumCols(int num) 			{legendNumCols = num;	return *this;}
	int GetLegendNumCols() 							{return legendNumCols;}
	ScatterDraw& SetLegendRowSpacing(int num) 		{legendRowSpacing = num;return *this;}
	int GetLegendRowSpacing() 						{return legendRowSpacing;}
	enum LEGEND_POS {
		LEGEND_TOP,
		LEGEND_ANCHOR_LEFT_TOP, 
		LEGEND_ANCHOR_RIGHT_TOP, 
		LEGEND_ANCHOR_LEFT_BOTTOM, 
		LEGEND_ANCHOR_RIGHT_BOTTOM
	};
	ScatterDraw& SetLegendAnchor(int anchor) 		{legendAnchor = anchor;	return *this;}
	int GetLegendAnchor() 							{return legendAnchor;}
	ScatterDraw& SetLegendFillColor(const Color &color) 	{legendFillColor = color;	return *this;}
	ScatterDraw& SetLegendBorderColor(const Color &color) 	{legendBorderColor = color;	return *this;}	
	Color& GetLegendFillColor() 					{return legendFillColor;}
	Color& GetLegendBorderColor() 					{return legendBorderColor;}
	
	ScatterDraw& SetMode(int _mode = MD_ANTIALIASED)	{mode = _mode; Refresh(); return *this;};
	int GetMode()	{return mode;};
	
	void FitToData(bool Y = false, double factor = 0);
	void Zoom(double scale, bool hor = true, bool ver = true); 
	void Scroll(double factorX, double factorY);
	
	enum ZoomStyle {TO_CENTER, FROM_BASE};
	ScatterDraw &SetZoomStyleX(ZoomStyle style = TO_CENTER) {zoomStyleX = style; return *this;}
	ScatterDraw &SetZoomStyleY(ZoomStyle style = TO_CENTER) {zoomStyleY = style; return *this;}

	ScatterDraw& SetRange(double rx, double ry, double ry2 = 100);
	ScatterDraw& SetRangeLinked(double rx, double ry, double ry2 = 100);
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
	ScatterDraw& SetXYMinLinked(double xmin,double ymin,double ymin2 = 0);
	double GetXMin () const {return xMin;}
	double GetYMin () const {return yMin;}	
	double GetYMin2 () const {return yMin2;}
	double GetY2Min () const {return yMin2;}
	
	ScatterDraw &Graduation_FormatX(Formats fi);	
	ScatterDraw &Graduation_FormatY(Formats fi);
	ScatterDraw &Graduation_FormatY2(Formats fi);
	
	ScatterDraw &SetPolar(bool polar = true)			{isPolar = polar; 	return *this;};
	
	ScatterDraw &AddSeries(double *yData, int numData, double x0, double deltaX)
														{return AddSeries<CArray>(yData, numData, x0, deltaX);}
	ScatterDraw &AddSeries(double *xData, double *yData, int numData)
														{return AddSeries<CArray>(yData, xData, numData);}
	ScatterDraw &AddSeries(Vector<double> &xData, Vector<double> &yData)
														{return AddSeries<VectorDouble>(yData, xData);}
	ScatterDraw &AddSeries(Array<double> &xData, Array<double> &yData)
														{return AddSeries<ArrayDouble>(yData, xData);}		
	ScatterDraw &AddSeries(Vector<Pointf> &points)		{return AddSeries<VectorPointf>(points);}
	ScatterDraw &AddSeries(Array<Pointf> &points)		{return AddSeries<ArrayPointf>(points);}
	ScatterDraw &AddSeries(Vector<Vector <double> > &data, int idx, int idy, 
		Vector<int> &idsx, Vector<int> &idsy, Vector<int> &idsFixed, bool useCols = true, int beginData = 0, int numData = Null) {
		ScatterDraw &series = AddSeries<VectorVectorY<double> >(data, idx, idy, idsx, idsy, idsFixed, useCols, beginData, numData);
//		series.multiPlot = true;
		return series;
	}
	ScatterDraw &AddSeries(double (*function)(double))	{return AddSeries<FuncSource>(function);}
	ScatterDraw &AddSeries(void (*function)(double&, double))
														{return AddSeries<FuncSourceV>(function);}
	ScatterDraw &AddSeries(Pointf (*function)(double), int np, double from = 0, double to = 1)	
														{return AddSeries<FuncSourcePara>(function, np, from, to);}
	ScatterDraw &AddSeries(PlotExplicFunc function)		{return AddSeries<PlotExplicFuncSource>(function);}
	ScatterDraw &AddSeries(PlotParamFunc function, int np, double from = 0, double to = 1)	
														{return AddSeries<PlotParamFuncSource>(function, np, from, to);}
	
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
	template <class C, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	ScatterDraw &AddSeries(T1 &arg1, T2 &arg2, T3 &arg3, T4 &arg4, T5 &arg5, T6 &arg6, T7 &arg7)	
								{return _AddSeries(new C(arg1, arg2, arg3, arg4, arg5, arg6, arg7));}
	template <class C, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	ScatterDraw &AddSeries(T1 &arg1, T2 &arg2, T3 &arg3, T4 &arg4, T5 &arg5, T6 &arg6, T7 &arg7, T8 &arg8)	
								{return _AddSeries(new C(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8));}									
	template <class C, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
	ScatterDraw &AddSeries(T1 &arg1, T2 &arg2, T3 &arg3, T4 &arg4, T5 &arg5, T6 &arg6, T7 &arg7, T8 &arg8, T9 &arg9)	
								{return _AddSeries(new C(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9));}	
																
	template <class Y>
	ScatterDraw &AddSeries(Vector<Y> &yData, double x0, double deltaX)		{return _AddSeries(new VectorY<Y>(yData, x0, deltaX));}
	template <class Y>
	ScatterDraw &AddSeries(Upp::Array<Y> &yData, double x0, double deltaX)	{return _AddSeries(new ArrayY<Y>(yData, x0, deltaX));}
	template <class X, class Y>
	ScatterDraw &AddSeries(VectorMap<X, Y> &data)	{return _AddSeries(new VectorMapXY<X, Y>(data));}
	template <class X, class Y>
	ScatterDraw &AddSeries(ArrayMap<X, Y> &data)	{return _AddSeries(new ArrayMapXY<X, Y>(data));}
	
	ScatterDraw &InsertSeries(int index, double *yData, int numData, double x0 = 0, double deltaX = 1);
	ScatterDraw &InsertSeries(int index, double *xData, double *yData, int numData);
	ScatterDraw &InsertSeries(int index, Vector<double> &xData, Vector<double> &yData);
	ScatterDraw &InsertSeries(int index, Upp::Array<double> &xData, Upp::Array<double> &yData);
	ScatterDraw &InsertSeries(int index, Vector<Pointf> &points);
	ScatterDraw &InsertSeries(int index, Upp::Array<Pointf> &points);
	ScatterDraw &InsertSeries(int index, double (*function)(double));
	ScatterDraw &InsertSeries(int index, Pointf (*function)(double), int np, double from = 0, double to = 1);
	ScatterDraw &InsertSeries(int index, PlotExplicFunc &function);
	ScatterDraw &InsertSeries(int index, PlotParamFunc function, int np, double from = 0, double to = 1);
	ScatterDraw &_InsertSeries(int index, DataSource *data);
	
	template <class C>
	ScatterDraw &InsertSeries(int index) 	{return _InsertSeries(index, new C());}	
	template <class C, class T1>
	ScatterDraw &InsertSeries(int index, T1 &arg1) 				
									{return _InsertSeries(index, new C(arg1));}
	template <class C, class T1, class T2>
	ScatterDraw &InsertSeries(int index, T1 &arg1, T2 &arg2) 		
									{return _InsertSeries(index, new C(arg1, arg2));}
	template <class C, class T1, class T2, class T3>
	ScatterDraw &InsertSeries(int index, T1 &arg1, T2 &arg2, T3 &arg3) 								
									{return _InsertSeries(index, new C(arg1, arg2, arg3));}
	template <class C, class T1, class T2, class T3, class T4>
	ScatterDraw &InsertSeries(int index, T1 &arg1, T2 &arg2, T3 &arg3, T4 &arg4)						
									{return _InsertSeries(index, new C(arg1, arg2, arg3, arg4));}
	template <class C, class T1, class T2, class T3, class T4, class T5>
	ScatterDraw &InsertSeries(int index, T1 &arg1, T2 &arg2, T3 &arg3, T4 &arg4, T5 &arg5)				
									{return _InsertSeries(index, new C(arg1, arg2, arg3, arg4, arg5));}
	template <class C, class T1, class T2, class T3, class T4, class T5, class T6>
	ScatterDraw &InsertSeries(int index, T1 &arg1, T2 &arg2, T3 &arg3, T4 &arg4, T5 &arg5, T6 &arg6)	
									{return _InsertSeries(index, new C(arg1, arg2, arg3, arg4, arg5, arg6));}

	template <class Y>
	ScatterDraw &InsertSeries(int index, Vector<Y> &yData, double x0, double deltaX)		{return _InsertSeries(index, new VectorY<Y>(yData, x0, deltaX));}
	template <class Y>
	ScatterDraw &InsertSeries(int index, Upp::Array<Y> &yData, double x0, double deltaX)	{return _InsertSeries(index, new ArrayY<Y>(yData, x0, deltaX));}
	template <class X, class Y>
	ScatterDraw &InsertSeries(int index, VectorMap<X, Y> &data)	{return _InsertSeries(index, new VectorMapXY<X, Y>(data));}
	template <class X, class Y>
	ScatterDraw &InsertSeries(int index, ArrayMap<X, Y> &data)	{return _InsertSeries(index, new ArrayMapXY<X, Y>(data));}
	
	int64 GetCount(int index);
	void GetValues(int index, int64 idata, double &x, double &y);
	double GetValueX(int index, int64 idata);
	double GetValueY(int index, int64 idata);
	
	ScatterDraw &PlotStyle()								{return PlotStyle(0);};
	template <class C>
	ScatterDraw &PlotStyle()								{return PlotStyle(new C());};
	template <class C, class T1>
	ScatterDraw &PlotStyle(T1 arg1)							{return PlotStyle(new C(arg1));};
	template <class C, class T1, class T2>
	ScatterDraw &PlotStyle(T1 arg1, T2 arg2)				{return PlotStyle(new C(arg1, arg2));};
	template <class C, class T1, class T2, class T3>
	ScatterDraw &PlotStyle(T1 arg1, T2 arg2, T3 arg3)		{return PlotStyle(new C(arg1, arg2, arg3));};			
	ScatterDraw &PlotStyle(SeriesPlot *data);
	
	ScatterDraw &NoPlot()	{return PlotStyle();};

	ScatterDraw &MarkStyle()								{return MarkStyle(0);};
	template <class C>
	ScatterDraw &MarkStyle()								{return MarkStyle(new C());};
	template <class C, class T1>
	ScatterDraw &MarkStyle(T1 arg1)							{return MarkStyle(new C(arg1));};
	template <class C, class T1, class T2>
	ScatterDraw &MarkStyle(T1 arg1, T2 arg2)				{return MarkStyle(new C(arg1, arg2));};
	template <class C, class T1, class T2, class T3>
	ScatterDraw &MarkStyle(T1 arg1, T2 arg2, T3 arg3)		{return MarkStyle(new C(arg1, arg2, arg3));};			
	
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
	ScatterDraw &MarkBorderColor(Color color = Null);
	ScatterDraw &MarkWidth(double markWidth = 8);
	ScatterDraw &MarkBorderWidth(double markWidth = 1);
	ScatterDraw &Hide() {series[series.GetCount() - 1].opacity = 0;	return *this;}
	
	ScatterDraw &Opacity(double opacity = 1) {series[series.GetCount() - 1].opacity = opacity;	return *this;}
	ScatterDraw &Legend(const String legend);
	ScatterDraw &Legend(int index, const String legend);
	const String &GetLegend(int index);
	ScatterDraw &Units(const String unitsY, const String unitsX = "");
	ScatterDraw &Units(int index, const String unitsY, const String unitsX = "");
	const String GetUnitsX(int index);
	const String GetUnitsY(int index);
	
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
	ScatterDraw &SetMarkBorderWidth(int index, double width);
	double GetMarkBorderWidth(int index);
	void SetMarkBorderColor(int index, const Color& pcolor);
	Color GetMarkBorderColor(int index) const;
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
	
	Drawing GetDrawing(const Size size = Null);
	Image GetImage(int scale = 2);
	Image GetImage(const Size &size, int scale = 2);
	
	#ifdef PLATFORM_WIN32
	void SaveAsMetafile(const char* file) const;
	#endif
	
	ScatterDraw& SetMinZoom(double x, double y = -1) 	{return SetMinRange(x, y);} 
	ScatterDraw& SetMaxZoom(double x, double y = -1) 	{return SetMaxRange(x, y);}
	ScatterDraw& SetMinRange(double x, double y = -1) 	{minXRange = x; minYRange = y; return *this;} 
	ScatterDraw& SetMaxRange(double x, double y = -1) 	{maxXRange = x; maxYRange = y; return *this;}
	ScatterDraw& SetMinXmin(double val)					{minXmin = val; return *this;}
	ScatterDraw& SetMinYmin(double val)					{minYmin = val; return *this;}
	ScatterDraw& SetMaxXmax(double val)					{maxXmax = val; return *this;}
	ScatterDraw& SetMaxYmax(double val)					{maxYmax = val; return *this;}

	ScatterDraw& SetFastViewX(bool set = true) 			{fastViewX = set;	return *this;}
	
	double GetXByPoint(double x);
	double GetYByPoint(double y);
	double GetY2ByPoint(double y);
	double GetXPointByValue(double x);
	double GetYPointByValue(double y);

	int GetCount() 	{return series.GetCount();}
	bool IsEmpty()	{return series.IsEmpty();}
	
	ScatterDraw& LinkedWith(ScatterDraw& ctrl);
	void Unlinked();
	
protected:
	ScatterDraw &_AddSeries(DataSource *data);
	virtual void Refresh() {};

	int mode;
	Color graphColor;	
	String title;
	Font titleFont;
	Color titleColor;
	int titleHeight;
	
	String xLabel, yLabel, yLabel2;
	String xLabel_base, yLabel_base, yLabel2_base;
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
	double minXRange, maxXRange, minYRange, maxYRange;
	double minXmin, minYmin, maxXmax, maxYmax;
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
	
	bool isPolar;
	
	int lastRefresh_sign;
	int highlight_0;
	
	Point legendPos;
	int legendNumCols;
	int legendAnchor;
	int legendRowSpacing;
	Color legendFillColor;
	Color legendBorderColor;
	
	void DrawLegend(Draw& w, const Size &size, int scale) const;

	void Scrolling(bool down, Point &pt, bool isOut = false);
	
	void ExpFormat(String& s, int i, double d)	{s = FormatDoubleExp(d, 1);}
	void MonFormat(String& s, int i, double d)	{s = Format("%Mon", int(d));}
	void DyFormat(String& s, int i, double d)	{s = Format("%Dy", int(d));}
	
	static String VariableFormat(double range, double d);	
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
	
	bool PointInPlot(Point &pt);
	bool PointInBorder(Point &pt);
	bool PointInLegend(Point &pt);
	
private:
	Size size;		// Size to be used for all but screen painting
	static void ParseTextMultiline(const String &text, Font fnt, 
								   Upp::Array <String> &texts, Upp::Array <Size> &sizes);
	
	void DoFitToData(bool Y, double factor = 0);
	void DoZoom(double scale, bool hor, bool ver); 
	void DoScroll(double factorX, double factorY);
	
	int plotW, plotH;
	bool labelsChanged;
	//bool multiPlot;
	
	Index<ScatterDraw *> linkedCtrls;
	ScatterDraw *linkedMaster;
};

template <class T>
void ScatterDraw::SetDrawing(T& w, const Size& size, int scale, bool ctrl)
{
	w.DrawRect(scale*size, graphColor);
	
	titleHeight = !title.IsEmpty() ? scale*titleFont.GetHeight() : 0;
	
	plotW = scale*(size.cx - (hPlotLeft + hPlotRight));
	plotH = scale*(size.cy - (vPlotTop + vPlotBottom)) - titleHeight;
	
	Plot(w, size, scale);	
		
	if (!ctrl) {
		if (!PlotTexts(w, size, scale)) 
			return;
	} 
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
	if(showLegend) 
		DrawLegend(w, size, scale);
		
	if (plotW < 0 || plotH < 0)
		return false;
	
	w.Offset(Point(scale*hPlotLeft, scale*vPlotTop + titleHeight));
	
	Font fontLabel;
	fontLabel = labelsFont;
	fontLabel.Height(scale*labelsFont.GetHeight());
	Font italicLabel = fontLabel;
	italicLabel.Italic();
	
	if (labelsChanged) {
		xLabel = xLabel_base;
		yLabel = yLabel_base;
		yLabel2 = yLabel2_base;
		Index<String> xUnits, yUnits, yUnits2;
		for (int i = 0; i < series.GetCount(); ++i) {
			ScatterSeries &serie = series[i];
			if (!serie.unitsX.IsEmpty())
				xUnits.FindAdd(serie.unitsX);
			if (!serie.unitsY.IsEmpty()) {
				if (serie.primaryY) 
					yUnits.FindAdd(serie.unitsY);
				else
					yUnits2.FindAdd(serie.unitsY);
			}
		}
		if (!xUnits.IsEmpty()) {
			xLabel += " ";
			for (int i = 0; i < xUnits.GetCount(); ++i)
				xLabel += "[" + xUnits[i] + "]";
		}
		if (!yUnits.IsEmpty()) {
			yLabel += " ";
			for (int i = 0; i < yUnits.GetCount(); ++i)
				yLabel += "[" + yUnits[i] + "]";
		}				
		if (!yUnits2.IsEmpty()) {
			yLabel2 += " ";
			for (int i = 0; i < yUnits2.GetCount(); ++i)
				yLabel2 += "[" + yUnits2[i] + "]";
		}						
		labelsChanged = false;	
	}
	Size lx  = GetTextSize(xLabel, 	fontLabel);
	Size ly  = GetTextSize(yLabel, 	fontLabel);
	Size ly2 = GetTextSize(yLabel2, italicLabel);
	DrawText(w, (plotW - lx.cx)/2., plotH + scale*(vPlotBottom - 2) - lx.cy, 0, xLabel, fontLabel, labelsColor);
	DrawText(w, scale*(2 - hPlotLeft), (plotH + ly.cx)/2.,  900, yLabel,  fontLabel, labelsColor);
	DrawText(w, scale*(size.cx - 2) - ly2.cy - hPlotLeft, (plotH + ly2.cx)/2., 900, yLabel2, italicLabel, labelsColor);

	drawXReticle &=  (xRange != 0 && xMajorUnit != 0);
	drawYReticle &=  (yRange != 0 && yMajorUnit != 0);
	drawY2Reticle &= (yRange2 != 0 && yMajorUnit != 0);
	
	Font standard6 = GetStdFont();
	standard6.Height(scale*GetStdFont().GetHeight());
	
	if (drawXReticle)
		for(int i = 0; xMinUnit + i*xMajorUnit <= xRange; i++) {
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

	Font italic = standard6;
	italic.Italic();
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
				DrawText(w, plotW + scale*10, reticleY - scale*8, 0, gridLabelY2, italic, axisColor);
			}
		}
	
	int borderWidth = fround(gridWidth*scale);
#ifdef flagGUI		// Control highlight
	if (!IsNull(highlight_0)) {
		double delayFactor = 4*(1000. - (GetTickCount() - highlight_0))/1000.;
		if (delayFactor < 1) {
			delayFactor = 1;
			highlight_0 = Null;
		} 
		borderWidth = fround(delayFactor*borderWidth);
	}
#endif
	w.DrawLine(0, plotH, plotW, plotH, borderWidth, Black);
	w.DrawLine(0, 0, plotW, 0, borderWidth, Black);
	w.DrawLine(0, 0, 0, plotH, borderWidth, Black);
	w.DrawLine(plotW, 0, plotW, plotH + 1, borderWidth, Black);
	
	w.End();	
	
	return true;
}

template <class T>
void ScatterDraw::Plot(T& w, const Size &size, int scale)
{
	if (plotW < 0 || plotH < 0)
		return;
	
	w.Offset(Point(scale*hPlotLeft, scale*vPlotTop + titleHeight));
	Clip(w, 0, 0, plotW, plotH);
		
	double d1 = xRange/xMajorUnit;
	double d2 = yRange/yMajorUnit;

	double left, top, d = min(plotW, plotH), r = d/2.;
	if (!isPolar)
		w.DrawRect(0, 0, plotW, plotH, plotAreaColor);	
	else {
		if (plotW > plotH) {
			left = (plotW - d)/2;
			top = 0;
		} else {
			left = 0;
			top = (plotH - d)/2;
		}
		w.DrawEllipse(fround(left), fround(top), fround(d), fround(d), plotAreaColor);
	}
	double x_c = plotW/2;
	double y_c = plotH/2;
	
	if (drawVGrid) {
		if (!isPolar) {
			double x0 = plotW*xMinUnit/xRange;
			for(int i = 0; xMinUnit + i*xMajorUnit < xRange; i++) {
				int xg = fround(x0 + i*plotW/d1);
				if (xg > 2*gridWidth && xg < plotW - 2*gridWidth)
					DrawLineOpa(w, xg, 0, xg, fround(plotH), 1, 1, gridWidth, gridColor, "2 2");
			}
		} else {
			double ang0 = 2*M_PI*xMinUnit/xRange;
			for(double i = 0; xMinUnit + i*xMajorUnit < xRange; i++) {
				double ang = ang0 + i*2*M_PI*xMajorUnit/xRange;
				DrawLineOpa(w, fround(x_c), fround(y_c), fround(x_c + r*cos(ang)), fround(y_c + r*sin(ang)), 1, 1, gridWidth, gridColor, "2 2");
			}				
		}
	}
	if (drawHGrid) {
		if (!isPolar) {
			double y0 = -plotH*yMinUnit/yRange + plotH;
			for(int i = 0; yMinUnit + i*yMajorUnit < yRange; i++) {
				int yg = fround(y0 - i*plotH/d2);
				if (yg > 2*gridWidth && yg < plotH - 2*gridWidth) 
					DrawLineOpa(w, 0, yg, fround(plotW), yg, 1, 1, gridWidth, gridColor, "2 2");
			}
		} /*else {
			double y0 = -plotH*yMinUnit/r + plotH;
			for(double i = 0; yMinUnit + i*yMajorUnit < yRange; i++) {
				double yg = y0 + i*r*yRange/yMajorUnit;
				DrawCircleOpa(w, fround(plotW/2), fround(plotH/2), yg, 1, 1, gridWidth, gridColor, "2 2");
			}
		}*/
	}

	if (!series.IsEmpty()) {
		try {
			for (int j = 0; j < series.GetCount(); j++) {
				if (series[j].opacity == 0 || (!series[j].seriesPlot && !series[j].markPlot) || 
					(!series[j].PointsData()->IsExplicit() && series[j].PointsData()->GetCount() == 0))
					continue;
				Vector<Point> points;
				if (series[j].PointsData()->IsParam()) {
					double xmin = 0;
					double xmax = double(series[j].PointsData()->GetCount());
					for (double x = xmin; x <= xmax; x++) {
						double xx = series[j].PointsData()->x(x);
						double yy = series[j].PointsData()->y(x);
						if (IsNull(xx) || IsNull(yy))
							continue;
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
						if (IsNull(yy))
							continue;
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
							double xx = series[j].PointsData()->x(i);
							if (IsNull(xx))
								continue;
							if (IsNull(imin)) {
								if (xx >= xMin)
									imin = i - 1;
							} else if (IsNull(imax)) {
								if (xx >= xMin + xRange) 
									imax = i + 1;
							}
						}
						if (IsNull(imin))
						    imin = 0;
						if (IsNull(imax))
						    imax = series[j].PointsData()->GetCount() - 1;
					} else {
						imin = 0;
						imax = series[j].PointsData()->GetCount();
					}
					double dxpix;
					if (fastViewX) 
						dxpix = (series[j].PointsData()->x(imax) - series[j].PointsData()->x(imin))/plotW;			
					int npix = 1;
					for (int64 i = imin; i < imax; ) {
						double xx, yy;
						if (fastViewX) {					
							yy = series[j].PointsData()->y(i);
							if (IsNull(yy))
								continue;
							int64 ii;
							double maxv = dxpix*npix;
							for (ii = 1; series[j].PointsData()->x(i + ii) < maxv && i + ii < imax; ++ii) {
								double dd = series[j].PointsData()->y(i + ii);
								if (IsNull(dd))
									continue;
								yy += dd;
							}
							yy /= double(ii);
							xx = series[j].PointsData()->x(i);
							if (IsNull(xx))
								continue;
							i += ii;
							npix++;
						} else {
							xx = series[j].PointsData()->x(i);
							yy = series[j].PointsData()->y(i);
							++i;
							if (IsNull(xx) || IsNull(yy))
								continue;
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
					if (!series[j].markPlot->IsMultiPlot()) 
						for (int i = 0; i < points.GetCount(); i++) 
							series[j].markPlot->Paint(w, scale, points[i], 
								series[j].markWidth, series[j].markColor, 
								series[j].markBorderWidth, series[j].markBorderColor);              
					else {
						for (int64 i = 0; i < series[j].PointsData()->GetCount(); ++i) {
							int ix = fround(plotW*(series[j].PointsData()->x(i) - xMin)/xRange);
							int iy;
							if (series[j].primaryY)
								iy = plotH - fround(plotH*(series[j].PointsData()->y(i) - yMin)/yRange);
							else
								iy = plotH - fround(plotH*(series[j].PointsData()->y(i) - yMin2)/yRange2);
							Vector<int> dataX, dataY;
							Vector<double> dataFixed;
							for (int ii = 0; ii < series[j].PointsData()->GetznxCount(); ++ii) 
								dataX << fround(plotW*(series[j].PointsData()->znx(ii, i) - xMin)/xRange);
							if (series[j].primaryY) {
								for (int ii = 0; ii < series[j].PointsData()->GetznyCount(); ++ii) 
									dataY << (plotH - fround(plotH*(series[j].PointsData()->zny(ii, i) - yMin)/yRange));
							} else {
								for (int ii = 0; ii < series[j].PointsData()->GetznyCount(); ++ii) 
									dataY << (plotH - fround(plotH*(series[j].PointsData()->zny(ii, i) - yMin2)/yRange2));
							}
							for (int ii = 0; ii < series[j].PointsData()->GetznFixedCount(); ++ii) 
								dataFixed << series[j].PointsData()->znFixed(ii, i);
							series[j].markPlot->Paint(w, scale, ix, iy, dataX, dataY, dataFixed, 
								series[j].markWidth, series[j].markColor, 
								series[j].markBorderWidth, series[j].markBorderColor);   
						}
					}
				}	
			}
		} catch(ValueTypeError error) {
			ASSERT_(true, error);
		}
	}
	ClipEnd(w);
	w.End();
}

END_UPP_NAMESPACE
		
#endif

