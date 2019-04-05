#ifndef _ScatterDraw_ScatterDraw_h
#define _ScatterDraw_ScatterDraw_h

#include <Draw/Draw.h>
#include <Painter/Painter.h>
#include "DataSource.h"
#include "Equation.h"

using namespace Upp;

#include "DrawingFunctions.h"
#include "SeriesPlot.h"
#include "MarkPlot.h"


Color GetOpaqueColor(const Color &color, const Color &background, double opacity);

void debug_h();			// Dummy function used to debug .h files

class DashStyle {
public:
	static int Register(const String& name, const String& style) {
		return map().FindAdd(name, style);
		return map().FindAdd(name, style);
	}
	static void Unregister(const String& name) {
		int id = TypeIndex(name);
		ASSERT(id >= 0);
		map().Remove(id);
	}
	static void UnregisterFrom(int id) {
		for (int i = GetCount() - 1; i >= id; --i)
			map().Remove(i);
	}
	static String         TypeName(int i)         		{return map().GetKey(i);}
	static String         Style(int i)         			{return map()[i];}
	static int            TypeIndex(const String& name) {return map().Find(name);}
	static int            StyleIndex(const String& style) {
		for (int i = 0; i < GetCount(); ++i) {
			if (map()[i] == style)
				return i;
		}
		return -1;
	}
	static int            GetCount()   		{return map().GetCount();}
	     
protected:
	static VectorMap<String, String>& map()	{static VectorMap<String, String> map; 	 return map;}
};

enum RAINBOW {BLUE_YELLOW_RED, RED_YELLOW_BLUE, GREEN_YELLOW_RED, RED_YELLOW_GREEN, 
			  BLUE_WHITE_RED, RED_WHITE_BLUE, WHITE_BLACK, BLACK_WHITE};

Color GetRainbowColor(double frac, RAINBOW rainbow, int numScales);
Color GetRainbowColor(double frac, const Color &from, const Color &to, int numScales);
Color GetRainbowColor(double frac, const Color &col0, const Color &col1, const Color &col2, int numScales);

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
	
	#define LINE_SOLID 		  ""
	#define LINE_DOTTED_FINER "2 10"
	#define LINE_DOTTED_FINE  "2 6"
	#define LINE_DOTTED 	  "4 10"
	#define LINE_DOTTED_SEP	  "4 20"
	#define LINE_DASHED 	  "12 12"
	#define LINE_DASH_DOT 	  "20 10 5 10"
	
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
		
		double barWidth;
		bool isClosed;

		Vector<String> *labels;
		Font labelsFont;
		Color labelsColor;
		int labelsDx, labelsDy;
		Alignment labelsAlign;
		
		bool showLegend;

		int id;
	
	private:
		template <class T>
		void Ize(T& io) {
			int seriesP = Null;
			int markP = Null;
			if (io.IsStoring()) {
				if (markPlot)
					markP = markPlot->GetType();
				if (seriesP)
					seriesP = seriesPlot->GetType();
			}
			io
				("primaryY", primaryY)
				("sequential", sequential)
				("thickness", thickness)
				("color", color)
				("dash", dash)
				("markWidth", markWidth)
				("markColor", markColor)
				("markBorderWidth", markBorderWidth)
				("markWidth", markWidth)
				("markBorderColor", markBorderColor)
				("fillColor", fillColor)
				("markBorderColor", markBorderColor)
				("legend", legend)
				("unitsX", unitsX)
				("unitsY", unitsY)
				("opacity", opacity)
				("id", id)
				("seriesP", seriesP)
				("markPlot", markP)
				("seriesPlot", seriesP)
				("barWidth", barWidth)
				("isClosed", isClosed)
				("showLegend", showLegend)
			;
			if (io.IsLoading()) {
				if (!IsNull(markP))
					markPlot = MarkPlot::Create(markP);
				else
					markPlot = 0;
				if (!IsNull(seriesP))
					seriesPlot = SeriesPlot::Create(seriesP);
				else
					seriesPlot = 0;
			}
		}
	
	public:
		void Xmlize(XmlIO& xml) 	{Ize(xml);}
		void Jsonize(JsonIO& json) 	{Ize(json);}

		void Serialize(Stream& s) {
			int seriesP = Null;
			int markP = Null;
			if (s.IsStoring()) {
				if (markPlot)
					markP = markPlot->GetType();
				if (seriesP)
					seriesP = seriesPlot->GetType();
			}
			s	% primaryY
				% sequential
				% thickness
				% color
				% dash
				% markWidth
				% markColor
				% markBorderWidth
				% markWidth
				% markBorderColor
				% fillColor
				% markBorderColor
				% legend
				% unitsX
				% unitsY
				% opacity
				% id
				% seriesP
				% markP
				% seriesP
				% barWidth
				% isClosed
				% showLegend
			;
			if (s.IsLoading()) {
				if (!IsNull(markP))
					markPlot = MarkPlot::Create(markP);
				else
					markPlot = 0;
				if (!IsNull(seriesP))
					seriesPlot = SeriesPlot::Create(seriesP);
				else
					seriesPlot = 0;
			}
		} 
	};
		
	class ScatterSeries : public Moveable<ScatterSeries>, public ScatterBasicSeries {
	public:
		ScatterSeries()	: userpD(0), owns(false), serializeData(false), pD(0) {dataS.Init(&data);}
		void SetDataSource(DataSource *pointsData, bool ownsData = true) {
			DeletePD();
			pD = userpD = pointsData; 
			owns = ownsData;
		}
		void SetDataSource() {
			pD = userpD;
		}
		void SetDataSource_Internal(bool copy = true) {
			pD = &dataS;
			if (copy) 
				CopyInternal();
		}
		DataSource &GetDataSource() {return *pD;}
		inline DataSource *PointsData()	{
			ASSERT_(!pD || !pD->IsDeleted(), "DataSource in ScatterCtrl/Draw has been deleted.\nIt has been probably declared in a function.");	
			return pD;
		}
		~ScatterSeries()   {DeletePD();}
		void SerializeData(bool ser = true) 	{serializeData = ser;}
		void SerializeFormat(bool ser = false) 	{serializeFormat = ser;}
		void Xmlize(XmlIO& xio) {
			XmlizeByJsonize(xio, *this);
		}
		void Jsonize(JsonIO& json) {
			ScatterBasicSeries::Jsonize(json);
			if (json.IsStoring() && userpD) 
				CopyInternal();
			json("data", data);
			if (json.IsLoading()) {
				if (!data.IsEmpty()) {
					pD = &dataS;
					serializeData = true;
				}
			}
		}
		void Serialize(Stream& s) { 
			ScatterBasicSeries::Serialize(s);
			if (s.IsStoring() && userpD) 
				CopyInternal();
			s % data;
			if (s.IsLoading()) {
				if (!data.IsEmpty()) {
					pD = &dataS;
					serializeData = true;
				}
			}
		}
		
	private:
		DataSource *userpD;
		bool owns;
		Vector<Pointf> data;
		VectorPointf dataS;
		bool serializeData, serializeFormat;
		DataSource *pD;
	
		void CopyInternal() {
			int64 sz = userpD->GetCount();
			data.SetCount(int(sz));
			for (int64 i = 0; i < sz; ++i) {
				data[int(i)].x = userpD->x(i);
				data[int(i)].y = userpD->y(i);
			}
		}
		void DeletePD() {
			if(userpD && owns) {
				delete userpD;
				userpD = 0;
			}
		}
	};

	static Color GetNewColor(int index, int version = 1);
	static String GetNewDash(int index);
	static MarkPlot *GetNewMarkPlot(int index);
	
	void WhenPaint(Painter &w)	{WhenPainter(w);}
	void WhenPaint(Draw &w) 	{WhenDraw(w);}
	
public:	
	template<class T>
	void SetDrawing(T& w, bool ctrl = false);
	
	Callback3<String&, int, double> cbModifFormatX;
	Callback3<String&, int, double> cbModifFormatXGridUnits;
	Callback3<String&, int, double> cbModifFormatDeltaX;
	Callback3<String&, int, double> cbModifFormatY;
	Callback3<String&, int, double> cbModifFormatYGridUnits;
	Callback3<String&, int, double> cbModifFormatDeltaY;
	Callback3<String&, int, double> cbModifFormatY2;
	Callback3<String&, int, double> cbModifFormatY2GridUnits;
	Callback3<String&, int, double> cbModifFormatDeltaY2;
			
	Callback WhenZoomScroll;
	Callback WhenSetRange;
	Callback WhenSetXYMin;
	Callback1<Painter&> WhenPainter;
	Callback1<Draw&> WhenDraw;
	Callback WhenZoomToFit;
	
	ScatterDraw& SetSize(const Size &sz) {
		size = sz; 
		plotScaleX   = responsive ? responsivenessFactor*size.cx/600. : 1;
		plotScaleY   = responsive ? responsivenessFactor*size.cy/400. : 1;
		plotScaleAvg = responsive ? (plotScaleX + plotScaleY)/2. : 1;
		return *this;
	};
	Size GetSize() const				{return size;};
	ScatterDraw& Responsive(bool responsive = true, double factor = 1) {
		this->responsive = responsive;
		responsivenessFactor = factor;
		plotScaleX   = responsive ? responsivenessFactor*size.cx/600. : 1;
		plotScaleY   = responsive ? responsivenessFactor*size.cy/400. : 1;
		plotScaleAvg = responsive ? (plotScaleX + plotScaleY)/2. : 1;
		return *this;
	}
	bool IsResponsive()		{return responsive;}
	double GetResponsivenessFactor() {return responsivenessFactor;}
	double GetPlotScaleX()	{return plotScaleX;}
	double GetPlotScaleY()	{return plotScaleY;}
	double GetPlotScaleAvg(){return plotScaleAvg;}
	
	ScatterDraw& SetColor(const Color& _color);
	ScatterDraw& SetTitle(const String& _title);
	const String& GetTitle();
	ScatterDraw& SetTitleFont(const Upp::Font& fontTitle);
	Upp::Font& GetTitleFont() {return titleFont;};
	ScatterDraw& SetTitleColor(const Color& colorTitle);
	Upp::Color& GetTitleColor() {return titleColor;};
	
	ScatterDraw& SetLabels(const String& _xLabel, const String& _yLabel, const String& _yLabel2 = "");
	ScatterDraw& SetLabelX(const String& _xLabel);
	const String &GetLabelX()	{return xLabel_base;} 
	ScatterDraw& SetLabelY(const String& _yLabel);
	const String &GetLabelY()	{return yLabel_base;} 
	ScatterDraw& SetLabelY2(const String& _yLabel);
	const String &GetLabelY2()	{return yLabel2_base;}
	ScatterDraw& SetLabelsFont(const Upp::Font& fontLabels);
	Upp::Font GetLabelsFont() 	{return labelsFont;};
	ScatterDraw& SetLabelsColor(const Color& colorLabels);
	Upp::Color GetLabelsColor() {return labelsColor;};
	
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
	
	ScatterDraw& SetGridColor(const Color& grid_color) 	{gridColor = grid_color;	return *this;}
	Color &GetGridColor() 								{return gridColor;}
	ScatterDraw& SetGridWidth(double grid_width)		{gridWidth = grid_width;	return *this;}
	double GetGridWidth() 								{return gridWidth;}
	ScatterDraw& SetGridDash(const char *dash)			{gridDash = dash;			return *this;}
	const char *GetGridDash()							{return gridDash;}
	
	ScatterDraw& ShowVGrid(bool show);
	ScatterDraw& ShowHGrid(bool show);
	
	ScatterDraw& ShowLegend(bool show = true) 		{showLegend = show;		return *this;}
	bool GetShowLegend()							{return showLegend;}
	ScatterDraw& SetLegendPos(const Point &pos) 	{legendPos = pos;		return *this;}
	ScatterDraw& SetLegendPosX(int x) 				{legendPos.x = x;		return *this;}
	ScatterDraw& SetLegendPosY(int y) 				{legendPos.y = y;		return *this;}
	Point& GetLegendPos() 							{return legendPos;}
	ScatterDraw& SetLegendFont(const Font &fnt) 	{legendFont = fnt;		return *this;}
	Font &GetLegendFont()							{return legendFont;}
	ScatterDraw& SetLegendNumCols(int num) 			{legendNumCols = num;	return *this;}
	int GetLegendNumCols() 							{return legendNumCols;}
	ScatterDraw& SetLegendRowSpacing(int num) 		{legendRowSpacing = num;return *this;}
	int GetLegendRowSpacing() 						{return legendRowSpacing;}
	enum LEGEND_POS {
		TOP,
		LEFT_TOP, 
		RIGHT_TOP, 
		LEFT_BOTTOM, 
		RIGHT_BOTTOM
	};
	ScatterDraw& SetLegendAnchor(LEGEND_POS anchor) 		{legendAnchor = anchor;	return *this;}
	LEGEND_POS GetLegendAnchor() 							{return legendAnchor;}
	ScatterDraw& SetLegendFillColor(const Color &color) 	{legendFillColor = color;	return *this;}
	ScatterDraw& SetLegendBorderColor(const Color &color) 	{legendBorderColor = color;	return *this;}	
	Color& GetLegendFillColor() 							{return legendFillColor;}
	Color& GetLegendBorderColor() 							{return legendBorderColor;}
	
	ScatterDraw& SetMode(int mode = MD_ANTIALIASED)			{this->mode = mode; Refresh(); return *this;};
	int GetMode()											{return mode;};
	
	void ZoomToFit(bool horizontal = true, bool vertical = false, double factor = 0);
	//ScatterDraw &ZoomToFit(bool horizontal, double minx, double maxx, bool vertical, double minxy, double maxy, 
	//				bool vertical2, double miny2, double maxy2, double factor);
	//ScatterDraw &ZoomToFitSmart(bool horizontal, double minx, double maxx, bool vertical, double minxy, double maxy, 
	//				bool vertical2, double miny2, double maxy2, double factor);
	void Zoom(double scale, bool hor = true, bool ver = true); 
	void Scroll(double factorX, double factorY);
	
	enum ZoomStyle {TO_CENTER, FROM_BASE};
	ScatterDraw &SetZoomStyleX(ZoomStyle style = TO_CENTER) {zoomStyleX = style; return *this;}
	ScatterDraw &SetZoomStyleY(ZoomStyle style = TO_CENTER) {zoomStyleY = style; return *this;}

	ScatterDraw& SetRange(double rx, double ry = Null, double ry2 = Null);
	ScatterDraw& SetRangeLinked(double rx, double ry, double ry2 = 100);
	double GetXRange()const {return xRange;}
	double GetYRange()const {return yRange;}
	double GetY2Range()const {return yRange2;}
	ScatterDraw &SetMajorUnits(double ux, double uy = Null);
	ScatterDraw &SetMajorUnitsNum(int nx, int ny = Null);
	double GetMajorUnitsX() {return xMajorUnit;}
	double GetMajorUnitsY() {return yMajorUnit;}
	double GetMajorUnitsY2() {return yMajorUnit2;}
	ScatterDraw& SetMinUnits(double ux, double uy = Null);
	double GetXMinUnit () const {return xMinUnit;}
	double GetYMinUnit () const {return yMinUnit;}	
	double GetYMinUnit2 () const {return yMinUnit2;}	
	
	ScatterDraw& SetXYMin(double xmin, double ymin = Null, double ymin2 = Null);
	ScatterDraw& SetXYMinLinked(double xmin, double ymin = Null, double ymin2 = Null);
	double GetXMin() 	const {return xMin;}
	double GetYMin() 	const {return yMin;}	
	double GetYMin2() 	const {return yMin2;}
	double GetY2Min() 	const {return yMin2;}
	double GetXMax() 	const {return xMin + xRange;}
	double GetYMax() 	const {return yMin + yRange;}	
	double GetY2Max() 	const {return yMin2 + yRange2;}
	
	ScatterDraw &Graduation_FormatX(Formats fi);	
	ScatterDraw &Graduation_FormatY(Formats fi);
	ScatterDraw &Graduation_FormatY2(Formats fi);
	
	//ScatterDraw &SetPolar(bool polar = true)			{isPolar = polar; 	return *this;};
	
	ScatterDraw &AddSeries(double *yData, int numData, double x0, double deltaX)
														{return AddSeries<CArray>(yData, numData, x0, deltaX);}
	ScatterDraw &AddSeries(double *xData, double *yData, int numData)
														{return AddSeries<CArray>(yData, xData, numData);}
	ScatterDraw &AddSeries(Vector<double> &xData, Vector<double> &yData)
														{return AddSeries<VectorDouble>(yData, xData);}
	ScatterDraw &AddSeries(Upp::Array<double> &xData, Upp::Array<double> &yData)
														{return AddSeries<ArrayDouble>(yData, xData);}		
	ScatterDraw &AddSeries(Vector<Pointf> &points)		{return AddSeries<VectorPointf>(points);}
	ScatterDraw &AddSeries(Upp::Array<Pointf> &points)	{return AddSeries<ArrayPointf>(points);}
	template <class Y>
	ScatterDraw &AddSeries(Vector<Vector <Y> > &data, int idx, int idy, 
		Vector<int> &idsx, Vector<int> &idsy, Vector<int> &idsFixed, bool useCols = true, int beginData = 0, int numData = Null) {
		return AddSeries<VectorVectorY<Y> >(data, idx, idy, idsx, idsy, idsFixed, useCols, beginData, numData);
	}
	ScatterDraw &AddSeries(Function <double(double)> function)	{return AddSeries<FuncSource>(function);}
	ScatterDraw &AddSeries(double (*function)(double))			{return AddSeries<FuncSource>(function);}
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
	
	DataSource &GetSeries(int index);
		
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
	Value GetStringX(int index, int64 idata);
	double GetValueY(int index, int64 idata);
	Value GetStringY(int index, int64 idata);
	
	ScatterDraw &SetNoPlot(int index);
	
	ScatterDraw &PlotStyle()								{return PlotStyle(0);};
	template <class C>
	ScatterDraw &PlotStyle()								{return PlotStyle(new C());}
	template <class C, class T1>
	ScatterDraw &PlotStyle(T1 arg1)							{return PlotStyle(new C(arg1));}
	template <class C, class T1, class T2>
	ScatterDraw &PlotStyle(T1 arg1, T2 arg2)				{return PlotStyle(new C(arg1, arg2));}
	template <class C, class T1, class T2, class T3>
	ScatterDraw &PlotStyle(T1 arg1, T2 arg2, T3 arg3)		{return PlotStyle(new C(arg1, arg2, arg3));}		
	
	ScatterDraw &PlotStyle(int index, SeriesPlot *data);
	ScatterDraw &PlotStyle(SeriesPlot *data)				{return PlotStyle(series.GetCount() - 1, data);}
	ScatterDraw &PlotStyle(int index, const String name);
	ScatterDraw &PlotStyle(const String name)				{return PlotStyle(series.GetCount() - 1, name);}
	const String GetPlotStyleName(int index);	
	
	ScatterDraw &NoPlot()	{return PlotStyle();};

	ScatterDraw &Stacked(bool _stacked = true)				{stacked = _stacked; return *this;}

	ScatterDraw &MarkStyle()								{return MarkStyle(0);}
	template <class C>
	ScatterDraw &MarkStyle()								{return MarkStyle(new C());}
	template <class C, class T1>
	ScatterDraw &MarkStyle(T1 arg1)							{return MarkStyle(new C(arg1));}
	template <class C, class T1, class T2>
	ScatterDraw &MarkStyle(T1 arg1, T2 arg2)				{return MarkStyle(new C(arg1, arg2));}
	template <class C, class T1, class T2, class T3>
	ScatterDraw &MarkStyle(T1 arg1, T2 arg2, T3 arg3)		{return MarkStyle(new C(arg1, arg2, arg3));}		
	
	ScatterDraw &MarkStyle(int index, MarkPlot *data);
	ScatterDraw &MarkStyle(MarkPlot *data)					{return MarkStyle(series.GetCount() - 1, data);}
	ScatterDraw &MarkStyle(int index, const String name);
	ScatterDraw &MarkStyle(int index, int typeidx);
	ScatterDraw &MarkStyle(const String name)				{return MarkStyle(series.GetCount() - 1, name);}
	const String GetMarkStyleName(int index);
	ScatterDraw &SetMarkStyleType(int index, int type);
	ScatterDraw &SetMarkStyleType(int type)					{return SetMarkStyleType(series.GetCount() - 1, type);}
	int GetMarkStyleType(int index);
	
	ScatterDraw &NoMark()	{return MarkStyle();};
		
	ScatterDraw &Stroke(int index, double thickness, Color color);
	ScatterDraw &Stroke(double thickness, Color color = Null)   {return Stroke(series.GetCount() - 1, thickness, color);}
	ScatterDraw &SetLineColor(int index, Color color);
	void GetStroke(int index, double &thickness, Color &color);
	ScatterDraw &Closed(int index, bool closed);
	ScatterDraw &Closed(bool closed)							{return Closed(series.GetCount() - 1, closed);}
	bool IsClosed(int index);
	ScatterDraw &BarWidth(int index, double width);
	ScatterDraw &BarWidth(double width)							{return BarWidth(series.GetCount() - 1, width);}
	double GetBarWidth(int index);
	ScatterDraw &Dash(const char *dash);
	ScatterDraw &Dash(int index, const char *dash);
	ScatterDraw &NoDash();
	ScatterDraw &NoDash(int index);
	const String GetDash(int index);
	ScatterDraw &Fill(Color color = Null);
	ScatterDraw &MarkColor(Color color = Null);
	ScatterDraw &MarkBorderColor(Color color = Null);
	ScatterDraw &MarkWidth(double markWidth = 8);
	ScatterDraw &MarkBorderWidth(double markWidth = 1);
	ScatterDraw &Hide() {series[series.GetCount() - 1].opacity = 0;	return *this;}
	
	ScatterDraw &ShowSeriesLegend(int index, bool show = false);
	ScatterDraw &ShowSeriesLegend(bool show = false)	{return ShowSeriesLegend(series.GetCount() - 1, show);}
	bool GetShowSeriesLegend(int index)					{return series[index].showLegend;}
		
	ScatterDraw &Opacity(double opacity = 1) {series[series.GetCount() - 1].opacity = opacity;	return *this;}
	ScatterDraw &Legend(const String legend);
	ScatterDraw &Legend(int index, const String legend);
	const String &GetLegend(int index);
	ScatterDraw &Units(const String unitsY, const String unitsX = "");
	ScatterDraw &Units(int index, const String unitsY, const String unitsX = "");
	const String GetUnitsX(int index);
	const String GetUnitsY(int index);
	
	inline bool IsValid(int index) const {return (index >= 0 && index < series.GetCount());}
	
	ScatterDraw &SetDrawXReticle(bool set = true)  	{drawXReticle = set;	return *this;}
	ScatterDraw &SetDrawYReticle(bool set = true)  	{drawYReticle = set;	return *this;}
	ScatterDraw &SetDrawY2Reticle(bool set = true) 	{drawY2Reticle = set;	return *this;}
	bool GetDrawXReticle()							{return drawXReticle;}
	bool GetDrawYReticle()							{return drawYReticle;}
	bool GetDrawY2Reticle()							{return drawY2Reticle;}
	ScatterDraw &SetReticleFont(const Font &fnt)	{reticleFont = fnt;		return *this;}
	Font &GetReticleFont()							{return reticleFont;}
	ScatterDraw &SetReticleColor(const Color &col)	{reticleColor = col;	return *this;}
	Color &GetReticleColor()						{return reticleColor;}
		
	ScatterDraw &SetFillColor(int index, const Color& color);
	ScatterDraw &SetFillColor(const Color& color) {return SetFillColor(series.GetCount() - 1, color);}
	Color GetFillColor(int index) const;

	ScatterDraw &SetMarkWidth(int index, double width);
	ScatterDraw &SetMarkWidth(double width) {return SetMarkWidth(series.GetCount() - 1, width);}
	double GetMarkWidth(int index);
	ScatterDraw &SetMarkColor(int index, const Color& pcolor);
	ScatterDraw &SetMarkColor(const Color& pcolor) {return SetMarkColor(series.GetCount() - 1, pcolor);}
	Color GetMarkColor(int index) const;
	ScatterDraw &SetMarkBorderWidth(int index, double width);
	double GetMarkBorderWidth(int index);
	ScatterDraw &SetMarkBorderColor(int index, const Color& pcolor);
	Color GetMarkBorderColor(int index) const;
	void NoMark(int index);
	bool IsShowMark(int index);
	
	void SetDataPrimaryY(int index, bool primary = true);
	ScatterDraw &SetDataPrimaryY(bool primary = true); 	
	void SetDataSecondaryY(int index, bool secondary = true);
	ScatterDraw &SetDataSecondaryY(bool secondary = true); 	
	bool IsDataPrimaryY(int index);	
	
	void SetSequentialX(int index, bool sequential);
	ScatterDraw &SetSequentialX(bool sequential = true);
	ScatterDraw &SetSequentialXAll(bool sequential = true);
	bool GetSequentialX(int index);
	bool GetSequentialX();
		
	void Show(int index, bool show = true);
	bool IsVisible(int index);
	ScatterDraw &ShowAll(bool show = true);

	void RemoveSeries(int index);
	void RemoveAllSeries();
	
	ScatterDraw& Id(int id);
	ScatterDraw& Id(int index, int id);
	int GetId(int index);
	
	Drawing GetDrawing();
	Image GetImage();
	
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
	bool GetFastViewX() 								{return fastViewX;}
	
	double GetXByPoint(double x);
	double GetYByPoint(double y);
	double GetY2ByPoint(double y);
	double GetXPointByValue(double x);
	double GetYPointByValue(double y);

	int GetCount() 	{return series.GetCount();}
	bool IsEmpty()	{return series.IsEmpty();}
	
	ScatterDraw& LinkedWith(ScatterDraw& ctrl);
	void Unlinked();
	
	double GetMinX()			{return xMin;}
	int GetPlotWidth()			{return plotW;}
	int GetPlotHeight()			{return plotH;}
	double GetPosX(double x)	{return plotW*(x - xMin)/xRange;}
	double GetSizeX(double cx) 	{return plotW*cx/xRange;}
	double GetPosY(double y)	{return plotH - plotH*(y - yMin)/yRange;}
	double GetSizeY(double cy) 	{return plotH*cy/yRange;}		
	double GetPosY2(double y)	{return plotH - plotH*(y - yMin2)/yRange2;}
	double GetSizeY2(double cy) {return plotH*cy/yRange2;}
	
	double GetRealPosX(int x) 	{return xMin + (x - hPlotLeft*plotScaleX)*xRange/plotW;}
	double GetRealPosY(int y)	{return yMin + yRange - (y - plotScaleY*vPlotTop - titleHeight)*yRange/plotH;}		
	double GetRealPosY2(int y)	{return yMin2 + yRange2 - (y - plotScaleY*vPlotTop - titleHeight)*yRange2/plotH;}		
	
	double GetPixelThickX()		{return xRange/plotW;}
	double GetPixelThickY()		{return yRange/plotH;}
	
	ScatterDraw& SetMouseHandling(bool valx = true, bool valy = false);
	ScatterDraw& SetMouseHandlingLinked(bool valx = true, bool valy = false);
	bool GetMouseHandlingX()	{return mouseHandlingX;}
	bool GetMouseHandlingY()	{return mouseHandlingY;}
	
	ScatterDraw& AddSurf(DataSourceSurf &surf);
	ScatterDraw& SetSurfMinZ(double val)		{surfMinZ = val;	return *this;}
	double GetSurfMinZ() const					{return surfMinZ;}
	ScatterDraw& SetSurfMaxZ(double val)		{surfMaxZ = val;	return *this;}
	double GetSurfMaxZ() const					{return surfMaxZ;}
	ScatterDraw& ZoomToFitZ()					{surfMinZ = surf->MinZ();	surfMaxZ = surf->MaxZ(); return *this;}
	
	ScatterDraw& SurfNumColor(int num, bool cont){surfNumColor = num; continuousColor = cont; return *this;}
	ScatterDraw& SurfRainbow(RAINBOW val)		{surfRainbow = val;  return *this;}
	RAINBOW SurfRainbow()						{return surfRainbow;}
	
	ScatterDraw& SurfUnits(const String &units)	{surfUnits = units;	 return *this;}
	enum SURF_UNITS_POS {
		UNITS_TOP,
		UNITS_BOTTOM, 
		UNITS_LEFT, 
		UNITS_RIGHT
	};
	ScatterDraw& SurfUnitsPos(SURF_UNITS_POS pos)			{surfUnitsPos = pos;	return *this;}
	enum SURF_LEGEND_POS {
		LEGEND_LEFT, 
		LEGEND_RIGHT
	};
	ScatterDraw& SurfLegendPos(SURF_LEGEND_POS pos)			{surfLegendPos = pos;	return *this;}
	
	ScatterDraw& ShowRainbowPalette(bool show = true)		{showRainbow = show;	return *this;}
	bool GetShowRainbowPalette()							{return showRainbow;}
	ScatterDraw& SetRainbowPalettePos(const Point &p) 		{rainbowPos = p;		return *this;}	
	Point& GetRainbowPalettePos() 							{return rainbowPos;}
	ScatterDraw& SetRainbowPaletteSize(const Size &sz) 		{rainbowSize = sz;		return *this;}	
	Size& GetRainbowPaletteSize() 							{return rainbowSize;}
	ScatterDraw& SetRainbowPaletteAnchor(LEGEND_POS anchor) {rainbowAnchor = anchor;return *this;}
	LEGEND_POS GetRainbowPaletteAnchor() 					{return rainbowAnchor;}
	ScatterDraw& SetRainbowPaletteBorderColor(const Color &color) 	{rainbowBorderColor = color;return *this;}	
	Color& GetRainbowPaletteBorderColor() 					{return rainbowBorderColor;}
	ScatterDraw& SetRainbowPaletteFont(const Font &fnt) 	{rainbowPaletteFont = fnt;return *this;}	
	Font& GetRainbowPaletteFont() 							{return rainbowPaletteFont;}
	ScatterDraw& SetRainbowPaletteTextColor(const Color &color) 	{rainbowPaletteTextColor = color;return *this;}	
	Color& GetRainbowPaletteTextColor() 					{return rainbowPaletteTextColor;}
	
	ScatterDraw &AddLabelSeries(Vector<String> &labels, int dx = 0, int dy = 0, Font font = StdFont(), 
					Alignment align = ALIGN_CENTER, Color color = Black()) {
		int index = series.GetCount() - 1;
		
		return AddLabelSeries(index, labels, dx, dy, font, align, color);
	}
	ScatterDraw& AddLabelSeries(int index, Vector<String> &labels, int dx = 0, int dy = 0, Font font = StdFont(), 
					Alignment align = ALIGN_CENTER, Color color = Black()) {		
		series[index].labels = &labels;
		series[index].labelsDx = dx;
		series[index].labelsDy = dy;
		series[index].labelsFont = font;
		series[index].labelsAlign = align;
		series[index].labelsColor = color;
		return *this;
	}
	
	ScatterDraw& SetDataSource_Internal(bool copy = true) {
		for (int i = 0; i < series.GetCount(); ++i)
			series[i].SetDataSource_Internal(copy);
		return *this;
	}
	ScatterDraw& SetDataSource() {
		for (int i = 0; i < series.GetCount(); ++i)
			series[i].SetDataSource();
		return *this;
	}
	ScatterDraw& SerializeData(bool ser = true) {
		for (int i = 0; i < series.GetCount(); ++i)
			series[i].SerializeData(ser);
		return *this;
	}
	ScatterDraw& SerializeFormat(bool ser = true) {
		for (int i = 0; i < series.GetCount(); ++i)
			series[i].SerializeFormat(ser);
		serializeFormat = ser;
		return *this;
	}
	
	template <class T>
	void Ize(T& io) {
		if (serializeFormat) {
			int intlegendAnchor = 0, intrainbowAnchor = 0, intsurfRainbow = 0, intsurfUnitsPos = 0, intsurfLegendPos = 0;
			if (io.IsStoring()) {
				intlegendAnchor = legendAnchor;
				intrainbowAnchor = rainbowAnchor;
				intsurfRainbow = surfRainbow;
				intsurfUnitsPos = surfUnitsPos;
				intsurfLegendPos = surfLegendPos;
			}
			io
				("title", title)
				("titleFont", titleFont)
				("titleColor", titleColor)
				("titleHeight", titleHeight)
				("xLabel_base", xLabel_base)
				("yLabel_base", yLabel_base)
				("yLabel2_base", yLabel2_base)
				("labelsFont", labelsFont)
				("labelsColor", labelsColor)
				("xRange", xRange)
				("yRange", yRange)
				("yRange2", yRange2)
				("xMin", xMin)
				("yMin", yMin)
				("yMin2", yMin2)
				("xMajorUnit", xMajorUnit)
				("yMajorUnit", yMajorUnit)
				("yMajorUnit2", yMajorUnit2)
				("xMinUnit", xMinUnit)
				("yMinUnit", yMinUnit)
				("yMinUnit2", yMinUnit2)
				("minXRange", minXRange)
				("maxXRange", maxXRange)
				("minYRange", minYRange)
				("maxYRange", maxYRange)
				("minXmin", minXmin)
				("minYmin", minYmin)
				("maxXmax", maxXmax)
				("maxYmax", maxYmax)
				("hPlotLeft", hPlotLeft)
				("hPlotRight", hPlotRight)
				("vPlotTop", vPlotTop)  
				("vPlotBottom", vPlotBottom)
				("size", size)
				("responsive", responsive)
				("legendAnchor", intlegendAnchor)
				("legendPos", legendPos)
				("legendFillColor", legendFillColor)
				("legendBorderColor", legendBorderColor)
				("legendFont", legendFont)
				("legendNumCols", legendNumCols)
				("legendRowSpacing", legendRowSpacing)
				("series", series)
				("mouseHandlingX", mouseHandlingX)
				("mouseHandlingY", mouseHandlingY)
				("plotAreaColor", plotAreaColor)
				("drawXReticle", drawXReticle)
				("drawYReticle", drawYReticle)
				("drawY2Reticle", drawY2Reticle)
				("reticleFont", reticleFont)
				("reticleColor", reticleColor)
				("gridColor", gridColor)
				("gridWidth", gridWidth)
				("gridDash",  gridDash)
				("drawVGrid", drawVGrid)
				//("drawVGrid2", drawVGrid2)
				("drawHGrid", drawHGrid)
				//("drawHGrid2", drawHGrid2)
				//("freqGrid2", freqGrid2)
				("showRainbow", showRainbow)
				("rainbowPos", rainbowPos)
				("rainbowSize", rainbowSize)
				("rainbowAnchor", intrainbowAnchor)
				("rainbowBorderColor", rainbowBorderColor)
				("rainbowPaletteFont", rainbowPaletteFont)
				("rainbowPaletteTextColor", rainbowPaletteTextColor)
				("surfRainbow", intsurfRainbow)
				("surfNumColor", surfNumColor)
				("continuousColor", continuousColor)
				("surfMinZ", surfMinZ)
				("surfMaxZ", surfMaxZ)
				("surfUnits", surfUnits)
				("surfUnitsPos", intsurfUnitsPos)
				("surfLegendPos", intsurfLegendPos)
			;
			if (io.IsLoading()) {
				labelsChanged = true;
				legendAnchor = static_cast<LEGEND_POS>(intlegendAnchor);
				rainbowAnchor = static_cast<LEGEND_POS>(intrainbowAnchor);
				surfRainbow = static_cast<RAINBOW>(intsurfRainbow);
				surfUnitsPos = static_cast<SURF_UNITS_POS>(intsurfUnitsPos);
				surfLegendPos = static_cast<SURF_LEGEND_POS>(intsurfLegendPos);
			}
		} else
			io("series", series);
	}
		
	void Xmlize(XmlIO& xml) 	{Ize(xml);}
	void Jsonize(JsonIO& json) 	{Ize(json);}
		
	void Serialize(Stream& s) { 
		if (serializeFormat) {
			int intlegendAnchor = 0, intrainbowAnchor = 0, intsurfRainbow = 0, intsurfUnitsPos = 0, intsurfLegendPos = 0;
			if (s.IsStoring()) {
				intlegendAnchor = legendAnchor;
				intrainbowAnchor = rainbowAnchor;
				intsurfRainbow = surfRainbow;
				intsurfUnitsPos = surfUnitsPos;
				intsurfLegendPos = surfLegendPos;
			}
			s 	% title
				% titleFont
				% titleColor
				% titleHeight
				% xLabel_base
				% yLabel_base
				% yLabel2_base
				% labelsFont
				% labelsColor
				% xRange
				% yRange
				% yRange2
				% xMin
				% yMin
				% yMin2
				% xMajorUnit
				% yMajorUnit
				% yMajorUnit2
				% xMinUnit
				% yMinUnit
				% yMinUnit2
				% minXRange
				% maxXRange
				% minYRange
				% maxYRange
				% minXmin
				% minYmin
				% maxXmax
				% maxYmax
				% hPlotLeft
				% hPlotRight
				% vPlotTop
				% vPlotBottom
				% size
				% intlegendAnchor
				% legendPos
				% legendFillColor
				% legendBorderColor
				% legendFont
				% series
				% mouseHandlingX
				% mouseHandlingY
				% plotAreaColor
				% drawXReticle
				% drawYReticle
				% drawY2Reticle
				% reticleFont
				% reticleColor
				% responsive
				% showRainbow
				% rainbowPos
				% rainbowSize
				% intrainbowAnchor
				% rainbowBorderColor
				% rainbowPaletteFont
				% rainbowPaletteTextColor
				% intsurfRainbow
				% surfNumColor
				% continuousColor
				% surfMinZ
				% surfMaxZ
				% surfUnits
				% intsurfUnitsPos
				% intsurfLegendPos
			;
			if (s.IsLoading()) {
				labelsChanged = true;
				legendAnchor = static_cast<LEGEND_POS>(intlegendAnchor);
				rainbowAnchor = static_cast<LEGEND_POS>(intrainbowAnchor);
				surfRainbow = static_cast<RAINBOW>(intsurfRainbow);
				surfUnitsPos = static_cast<SURF_UNITS_POS>(intsurfUnitsPos);
				surfLegendPos = static_cast<SURF_LEGEND_POS>(intsurfLegendPos);
			}
		} else
			s % series;
	}
	
	void SwapOrder(int i1, int i2)			{series.Swap(i1, i2);}
	void Remove(int id)						{series.Remove(id);}
	
	String VariableFormatX(double d) const  {return VariableFormat(xRange, d);}
	String VariableFormatY(double d) const  {return VariableFormat(yRange, d);} 
	String VariableFormatY2(double d) const {return VariableFormat(yRange2, d);}
	String VariableFormatZ(double d) const  {return VariableFormat(GetSurfMaxZ()-GetSurfMinZ(), d);}
		
protected:
	ScatterDraw &_AddSeries(DataSource *data);
	virtual void Refresh() {};

	int mode;
	Color graphColor;	
	String title;
	Upp::Font titleFont;
	Color titleColor;
	int titleHeight;
	
	String xLabel, yLabel, yLabel2;
	String xLabel_base, yLabel_base, yLabel2_base;
	Upp::Font labelsFont;
	Color labelsColor;
	
	int   hPlotLeft, hPlotRight, vPlotTop, vPlotBottom;
	Color plotAreaColor;
	
	bool fastViewX, sequentialXAll;
	
	Color axisColor;
	int axisWidth;
	
	double xRange, yRange, yRange2;
	double xMin, yMin, yMin2;
	double xMajorUnit, yMajorUnit, yMajorUnit2;
	double xMajorUnitNum, yMajorUnitNum;
	double xMinUnit, yMinUnit, yMinUnit2;
	double xMinUnit0, yMinUnit0, yMinUnit20;
	double minXRange, maxXRange, minYRange, maxYRange;
	double minXmin, minYmin, maxXmax, maxYmax;
	double lastxRange, lastyRange;
	bool drawXReticle, drawYReticle, drawY2Reticle;	
	Font reticleFont;
	Color reticleColor;
	
	//int maxMajorUnitsX, maxMajorUnitsY;
	
	Color gridColor;
	double gridWidth;
	String gridDash;
	bool drawVGrid, drawHGrid;	
	
	bool drawVGrid2, drawHGrid2;
	
	int butDownX, butDownY;
	bool isScrolling, isLabelPopUp;
	ZoomStyle zoomStyleX, zoomStyleY;	
	
	Upp::Array<ScatterSeries> series;
	
	bool showLegend;
	
	bool isPolar;
	
	int lastRefresh_sign;
	int highlight_0;
	
	Point legendPos;
	int legendNumCols;
	LEGEND_POS legendAnchor;
	int legendRowSpacing;
	Color legendFillColor;
	Color legendBorderColor;
	Font legendFont;
	
	void DrawLegend(Draw& w) const;

	void Scrolling(bool down, Point &pt, bool isOut = false);
	
	void ExpFormat(String& s, int i, double d)	{s = FormatDoubleExp(d, 1);}
	void MonFormat(String& s, int i, double d)	{s = Format("%Mon", int(d));}
	void DyFormat(String& s, int i, double d)	{s = Format("%Dy", int(d));}
	
	static String VariableFormat(double range, double d);	

	template<class T>
	void Plot(T& w);	
	template<class T>
	bool PlotTexts(T& w, bool boldX = false, bool boldY = false);
		
	void AdjustMinUnitX();
	void AdjustMinUnitY();
	void AdjustMinUnitY2();
	
	void AdjustMajorUnitX();
	void AdjustMajorUnitY();
	void AdjustMajorUnitY2();
	
	bool PointInPlot(Point &pt);
	bool PointInBorder(Point &pt);
	bool PointInLegend(Point &pt);
	
	Upp::Index<ScatterDraw *> linkedCtrls;
	ScatterDraw *linkedMaster;
	
	void ChangeMouseHandlingX()					{mouseHandlingX = !mouseHandlingX;}
	void ChangeMouseHandlingY()					{mouseHandlingY = !mouseHandlingY;}
	
	bool mouseHandlingX, mouseHandlingY;
	
	DataSourceSurf *surf;
	RAINBOW surfRainbow;
	int surfNumColor;
	bool continuousColor;
	double surfMinZ, surfMaxZ;
	String surfUnits;
	SURF_UNITS_POS surfUnitsPos;
	SURF_LEGEND_POS surfLegendPos;
	//Vector<Pointf> isolines;
	//int isolinesId;
	
	bool showRainbow;
	Point rainbowPos;
	Size rainbowSize;
	LEGEND_POS rainbowAnchor;
	Color rainbowBorderColor;
	Font rainbowPaletteFont;
	Color rainbowPaletteTextColor;
	
	void DrawRainbowPalette(Draw& w) const;
	
private:
	Size size;		// Size to be used for all but screen painting
	double plotScaleX, plotScaleY, plotScaleAvg;
	bool responsive;
	double responsivenessFactor;
	
	static void ParseTextMultiline(const String &text, Upp::Font &fnt, 
								   Upp::Vector <String> &texts, Upp::Vector <Size> &sizes);
	
	ScatterDraw &DoFitToData(bool horizontal, bool vertical, double factor);
	//ScatterDraw &DoFitToData(bool horizontal, double minx, double maxx, bool vertical, double minxy, double maxy, 
	//				bool vertical2, double miny2, double maxy2, double factor = 0);
	//ScatterDraw &DoFitToDataSmart(bool horizontal, double minx, double maxx, bool vertical, double minxy, double maxy, 
	//				bool vertical2, double miny2, double maxy2, double factor = 0);
	void DoZoom(double scale, bool hor, bool ver); 
	void DoScroll(double factorX, double factorY);
	
	void SetXYMinLinkedEach(double xmin, double xmin0, double ymin, double ymin0, double ymin2, double ymin20);
	void SetRangeLinkedEach(double rx, double rx0, double ry, double ry0, double ry2, double ry20);
	
	int NumSeriesLegend() const;
		
	int plotW, plotH;
	bool labelsChanged;
	bool stacked;
	bool serializeFormat;
	//int selectedSeries;
};

template <class T>
void ScatterDraw::SetDrawing(T& w, bool ctrl)
{
	w.DrawRect(size, graphColor);
	
	titleHeight = !title.IsEmpty() ? fround(min(plotScaleX, plotScaleY)*titleFont.GetHeight()) : 0;
	
	plotW = size.cx - fround((hPlotLeft + hPlotRight)*plotScaleX);
	plotH = size.cy - fround((vPlotTop + vPlotBottom)*plotScaleY) - titleHeight;
	
	Plot(w);	
		
	if (!ctrl) {
		if (!PlotTexts(w)) 
			return;
	} 
}

template <class T>
bool ScatterDraw::PlotTexts(T& w, const bool boldX, bool boldY)
{
	if(titleHeight > 0) {
		Upp::Font fontTitle6;
		fontTitle6 = titleFont;
		fontTitle6.Height(titleHeight);
		Size sz = GetTextSize(title, fontTitle6);
		if (sz.cx > size.cx*0.95) {
			fontTitle6.Height(fround(fontTitle6.GetHeight()*size.cx*(0.95/sz.cx)));
			sz = GetTextSize(title, fontTitle6);
		}
		DrawText(w, fround((size.cx - sz.cx)/2.), plotScaleY*2, 0, title, fontTitle6, titleColor);   
	}	
	if(showLegend) 
		DrawLegend(w);
	
	if (surf && showRainbow)
		DrawRainbowPalette(w);
		
	if (plotW < 0 || plotH < 0)
		return false;
	
	w.Offset(Point(fround(plotScaleX*hPlotLeft), fround(plotScaleY*vPlotTop + titleHeight)));
	
	Upp::Font fontLabel;
	fontLabel = labelsFont;
	fontLabel.Height(fround(min(plotScaleX, plotScaleY)*labelsFont.GetHeight()));
	Upp::Font fontX = fontLabel;
	if (boldX)
		fontX.Bold();
	Upp::Font fontY = fontLabel;
	if (boldY)
		fontY.Bold();
	Upp::Font fontY2 = fontY;
	fontY2.Italic();
	
	if (labelsChanged) {
		xLabel = xLabel_base;
		yLabel = yLabel_base;
		yLabel2 = yLabel2_base;
		String yLabelLegends, yLabelLegends2;
		Upp::Index<String> xUnits, yUnits, yUnits2;
		for (int i = 0; i < series.GetCount(); ++i) {
			ScatterSeries &serie = series[i];
			if (serie.primaryY) {
				if (yLabel.IsEmpty()) {
					if (!yLabelLegends.IsEmpty())
						yLabelLegends << ";";
					yLabelLegends << serie.legend;
				}
			} else {
				if (yLabel2.IsEmpty()) {
					if (!yLabelLegends2.IsEmpty())
						yLabelLegends2 << ";";
					yLabelLegends2 << serie.legend;
				}
			}
			if (!serie.unitsX.IsEmpty())
				xUnits.FindAdd(serie.unitsX);
			if (!serie.unitsY.IsEmpty()) {
				if (serie.primaryY) 
					yUnits.FindAdd(serie.unitsY);
				else
					yUnits2.FindAdd(serie.unitsY);
			}
		}
		yLabel << yLabelLegends;
		yLabel2 << yLabelLegends2;
		if (xLabel.Find('[') < 0 && !xUnits.IsEmpty()) {
			xLabel += " ";
			for (int i = 0; i < xUnits.GetCount(); ++i)
				xLabel += "[" + xUnits[i] + "]";
		}
		if (yLabel.Find('[') < 0 && !yUnits.IsEmpty()) {
			yLabel += " ";
			for (int i = 0; i < yUnits.GetCount(); ++i)
				yLabel += "[" + yUnits[i] + "]";
		}				
		if (yLabel2.Find('[') < 0 && !yUnits2.IsEmpty()) {
			yLabel2 += " ";
			for (int i = 0; i < yUnits2.GetCount(); ++i)
				yLabel2 += "[" + yUnits2[i] + "]";
		}						
		labelsChanged = false;	
	}
	Size lx  = GetTextSize(xLabel, 	fontX);
	Size ly  = GetTextSize(yLabel, 	fontY);
	Size ly2 = GetTextSize(yLabel2, fontY2);
	DrawText(w, (plotW - lx.cx)/2., plotH + plotScaleY*(vPlotBottom - 2) - lx.cy, 0, xLabel, fontX, labelsColor);
	DrawText(w, plotScaleX*(2 - hPlotLeft), (plotH + ly.cx)/2., 900, yLabel,  fontY, labelsColor);
	DrawText(w, size.cx - plotScaleX*(2 + hPlotLeft) - ly2.cy, (plotH + ly2.cx)/2., 900, yLabel2, fontY2, labelsColor);

	drawXReticle  &= (xRange != 0  && xMajorUnit != 0);
	drawYReticle  &= (yRange != 0  && yMajorUnit != 0);
	drawY2Reticle &= (yRange2 != 0 && yMajorUnit != 0);
	
	Upp::Font standard6 = reticleFont;
	standard6.Height(fround(min(plotScaleX, plotScaleY)*standard6.GetHeight()));
	Upp::Font fontXNum = standard6;
	if (boldX)
		fontXNum.Bold();
	Upp::Font fontYNum = standard6;
	if (boldY)
		fontYNum.Bold();
	Upp::Font fontY2Num = fontYNum;
	fontY2Num.Italic();

	if (drawXReticle) 
		for(int i = 0; xMinUnit + i*xMajorUnit <= xRange; i++) {
			double reticleX = plotW*xMinUnit/xRange + i*plotW/(xRange/xMajorUnit);
			double gridX = xMinUnit + i*xMajorUnit + xMin;
			String gridLabelX;
			if (cbModifFormatXGridUnits)
				cbModifFormatXGridUnits(gridLabelX, i, gridX);
			else if (cbModifFormatX)
				cbModifFormatX(gridLabelX, i, gridX);
			else
				gridLabelX = VariableFormatX(gridX);
			
			if (!gridLabelX.IsEmpty()) {
				Upp::Vector <String> texts;
				Upp::Vector <Size> sizes;
				ParseTextMultiline(gridLabelX, fontXNum, texts, sizes);
				for (int ii = 0; ii < texts.GetCount(); ++ii) {
					int cy = ii == 0 ? 0 : sizes[ii - 1].cy;
					DrawText(w, reticleX - sizes[ii].cx/2., 
								plotH + (4 + ii*cy), 0, texts[ii], fontXNum, reticleColor);
				}
				w.DrawLine(fround(reticleX), plotH,   
						   fround(reticleX), fround(plotH + plotScaleY*4.), 
						   fround(gridWidth*plotScaleAvg), axisColor);             
			}
		}
	
	if (drawYReticle)
		for(int i = 0; yMinUnit + i*yMajorUnit <= yRange; i++) {
			int reticleY = fround(-plotH*yMinUnit/yRange + plotH - i*plotH/(yRange/yMajorUnit));
			w.DrawLine(fround(-plotScaleX*4), reticleY, 0, reticleY, fround(gridWidth*plotScaleAvg), axisColor);
			if (drawY2Reticle)
				w.DrawLine(fround(plotW + plotScaleX*4.), reticleY, plotW, reticleY, fround(gridWidth*plotScaleAvg), axisColor);
			double gridY = yMinUnit + i*yMajorUnit + yMin;
			String gridLabelY;
			if (cbModifFormatYGridUnits)
				cbModifFormatYGridUnits(gridLabelY, i, gridY);
			else if (cbModifFormatY)
				cbModifFormatY(gridLabelY, i, gridY);
			else
				gridLabelY = VariableFormatY(gridY);
			Size sz = GetTextSize(gridLabelY, fontYNum);
			DrawText(w, -sz.cx - plotScaleX*6, reticleY - sz.cy/2, 0, gridLabelY, fontYNum, axisColor);
			if (drawY2Reticle) {
				double gridY2 = (gridY - yMin)/yRange*yRange2 + yMin2;
				String gridLabelY2;
				if (cbModifFormatY2GridUnits)
					cbModifFormatY2GridUnits(gridLabelY2, i, gridY2);
				else if (cbModifFormatY2)
					cbModifFormatY2(gridLabelY2, i, gridY2);
				else
					gridLabelY2 = VariableFormatY2(gridY2);
				DrawText(w, plotW + plotScaleX*10, reticleY - sz.cy/2, 0, gridLabelY2, fontY2Num, axisColor);
			}
		}
	
	int borderWidth = fround(gridWidth*plotScaleAvg);
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
	DrawRectangle(w, 0, 0, plotW, plotH, 1, borderWidth, Black);
	
	w.End();	
	
	return true;
}
	
template <class T>
void ScatterDraw::Plot(T& w)
{
	if (plotW < 0 || plotH < 0)
		return;
	
	w.Offset(Point(fround(plotScaleX*hPlotLeft), fround(plotScaleY*vPlotTop + titleHeight)));
	Clip(w, 0, 0, plotW, plotH);
		
	double d1 = xRange/xMajorUnit;
	double d2 = yRange/yMajorUnit;

	double left, top, d = min(plotW, plotH);//, r = d/2.;
	if (!isPolar) {
		if (!surf)
			w.DrawRect(0, 0, plotW, plotH, plotAreaColor);	
		else {
			ImageBuffer out_image(plotW, plotH);
			::Fill(~out_image, plotAreaColor, out_image.GetLength());

			double dx = xRange/plotW;
			double dy = yRange/plotH;
			double deltaz = surfMaxZ - surfMinZ;
			if (deltaz == 0) 
				::Fill(~out_image, GetRainbowColor(0, surfRainbow, 0), out_image.GetLength());	
			else {
				CoWork co;
				for (int ix = 0; ix < plotW; ++ix) {
					co & [=, &out_image] {
						for (int iy = 0; iy < plotH; ++iy) {
							double x = xMin + (ix + 0.5)*dx;
							double y = yMin + (iy + 0.5)*dy;
							double z = surf->z(x, y);
							if (!IsNull(z))
								out_image[plotH - iy - 1][ix] = GetRainbowColor((z - surfMinZ)/deltaz, 
										surfRainbow, continuousColor ? 0 : surfNumColor);	
						}
					};
				}
			}
			w.DrawImage(0, 0, out_image);
		}
	} else {
		if (plotW > plotH) {
			left = (plotW - d)/2.;
			top = 0;
		} else {
			left = 0;
			top = (plotH - d)/2.;
		}
		w.DrawEllipse(fround(left), fround(top), fround(d), fround(d), plotAreaColor);
	}
	//double x_c = plotW/2;
	//double y_c = plotH/2;
	
	if (drawVGrid) {
		if (!isPolar) {
			double x0 = plotW*xMinUnit/xRange;
			for(int i = 0; xMinUnit + i*xMajorUnit < xRange; i++) {
				double xg = x0 + i*plotW/d1;
				if (xg > 2*gridWidth*plotScaleAvg && xg < plotW - 2*gridWidth*plotScaleAvg)
					DrawLineOpa(w, xg, 0, xg, plotH, plotScaleAvg, 1, gridWidth, gridColor, gridDash);
			}
		} /*else {
			double ang0 = 2*M_PI*xMinUnit/xRange;
			for(double i = 0; xMinUnit + i*xMajorUnit < xRange; i++) {
				double ang = ang0 + i*2*M_PI*xMajorUnit/xRange;
				DrawLineOpa(w, x_c, y_c, x_c + r*cos(ang), y_c + r*sin(ang), plotScaleAvg, 1, gridWidth*plotScaleAvg, gridColor, gridDash);
			}				
		}*/
	}
	if (drawHGrid) {
		if (!isPolar) {
			double y0 = -plotH*yMinUnit/yRange + plotH;
			for(int i = 0; yMinUnit + i*yMajorUnit < yRange; i++) {
				double yg = y0 - i*plotH/d2;
				if (yg > 2*gridWidth*plotScaleAvg && yg < plotH - 2*gridWidth*plotScaleAvg) 
					DrawLineOpa(w, 0, yg, plotW, yg, plotScaleAvg, 1, gridWidth, gridColor, gridDash);
			}
		} /*else {
			double y0 = -plotH*yMinUnit/r + plotH;
			for(double i = 0; yMinUnit + i*yMajorUnit < yRange; i++) {
				double yg = y0 + i*r*yRange/yMajorUnit;
				DrawCircleOpa(w, plotW/2, plotH/2, yg, 1, 1, gridWidth, gridColor, gridDash);
			}
		}*/
	}

	if (!series.IsEmpty()) {
		try {
			for (int j = 0; j < series.GetCount(); j++) {
				if (series[j].opacity == 0 || (!series[j].seriesPlot && !series[j].markPlot) || 
					(!series[j].PointsData()->IsExplicit() && series[j].PointsData()->GetCount() == 0))
					continue;
				Vector<Pointf> points;
				if (series[j].PointsData()->IsParam()) {
					double xmin = 0;
					double xmax = double(series[j].PointsData()->GetCount());
					for (double x = xmin; x <= xmax; x++) {
						double xx = series[j].PointsData()->x(x);
						double yy = series[j].PointsData()->y(x);
						if (IsNull(xx) || IsNull(yy)) 
							points << Null;
						else {
							int ix = fround(plotW*(xx - xMin)/xRange);
							int iy;
							if (series[j].primaryY)
								iy = fround(plotH*(yy - yMin)/yRange);
							else
								iy = fround(plotH*(yy - yMin2)/yRange2);
							points << Point(ix, plotH - iy);
						}
					}
				} else if (series[j].PointsData()->IsExplicit()) {
					double xmin = xMin - 1;
					double xmax = xMin + xRange + 1; 	
					double dx = double(xmax - xmin)/plotW;		
					for (double xx = xmin; xx < xmax; xx += dx) {
						double yy = series[j].PointsData()->f(xx);
						if (IsNull(yy))
							points << Null;
						else {
							int ix = fround(plotW*(xx - xMin)/xRange);
							int iy;
							if (series[j].primaryY)
								iy = fround(plotH*(yy - yMin)/yRange);
							else
								iy = fround(plotH*(yy - yMin2)/yRange2);
							points << Point(ix, plotH - iy);
						}
					}
				} else {
					int64 imin, imax;
					if (series[j].sequential) {
						imin = imax = Null;
						for (int64 i = 0; i < series[j].PointsData()->GetCount(); ++i) {
							double xx = series[j].PointsData()->x(i);
							if (!IsNull(xx)) {
								if (IsNull(imin)) {
									if (xx >= xMin) 
										imin = i;
								} else if (IsNull(imax)) {
									if (xx >= xMin + xRange) 
										imax = i;
								}
							}
						}
						if (IsNull(imin))
						    imin = 0;
						if (IsNull(imax))
						    imax = series[j].PointsData()->GetCount() - 1;
					} else {
						imin = 0;
						imax = series[j].PointsData()->GetCount() - 1;
					}
					double dxpix;
					if (fastViewX) 
						dxpix = (series[j].PointsData()->x(imax) - series[j].PointsData()->x(imin))/plotW;			
					int npix = 1;
					for (int64 i = imin; i <= imax; ) {
						double xx, yy;
						if (fastViewX && dxpix < 1) {	
							debug_h();				
							yy = series[j].PointsData()->y(i);
							if (IsNull(yy)) {
								++i;
								continue;
							}
							int64 ii;
							double maxv = series[j].PointsData()->x(imin) + dxpix*npix; 
							double maxY = yy, minY = yy;
							for (ii = 1; i + ii < imax && series[j].PointsData()->x(i + ii) < maxv; ++ii) {
								double dd = series[j].PointsData()->y(i + ii);
								if (IsNull(dd))
									continue;
								maxY = max(maxY, dd);
								minY = min(minY, dd);
							}
							xx = series[j].PointsData()->x(i);
							if (IsNull(xx)) {
								++i;
								continue;
							}
							i += ii;
							npix++;
							int ix = fround(plotW*(xx - xMin)/xRange);
							int iMax, iMin;
							if (series[j].primaryY) {
								iMax = fround(plotH*(maxY - yMin)/yRange);
								iMin = fround(plotH*(minY - yMin)/yRange);
							} else {
								iMax = fround(plotH*(maxY - yMin2)/yRange2);
								iMin = fround(plotH*(minY - yMin2)/yRange2);
							}
							points << Point(ix, plotH - iMax);
							if (iMax != iMin)
								points << Point(ix, plotH - iMin);	
						} else {
							xx = series[j].PointsData()->x(i);
							yy = series[j].PointsData()->y(i);
							++i;
							if (IsNull(xx) || IsNull(yy)) 
								points << Null;
							else {
								int ix = fround(plotW*(xx - xMin)/xRange);
								int iy;
								if (series[j].primaryY)
									iy = fround(plotH*(yy - yMin)/yRange);
								else
									iy = fround(plotH*(yy - yMin2)/yRange2);
								points << Point(ix, plotH - iy);
							}
						}
					}
				}
				if (!points.IsEmpty() && series[j].seriesPlot && series[j].thickness > 0) 
					series[j].seriesPlot->Paint(w, points, plotScaleAvg, series[j].opacity, 
												series[j].thickness, series[j].color, 
												series[j].dash, plotAreaColor, series[j].fillColor, plotW/xRange, plotH/yRange, 
												plotH*(1 + yMin/yRange), series[j].barWidth, 
												series[j].isClosed);
			
				if (series[j].markWidth >= 1 && series[j].markPlot) {
					if (!series[j].markPlot->IsMultiPlot()) {
						for (int i = 0; i < points.GetCount(); i++) 
							series[j].markPlot->Paint(w, plotScaleAvg, points[i], 
								series[j].markWidth, series[j].markColor, 
								series[j].markBorderWidth, series[j].markBorderColor);              
					} else {
						for (int64 i = 0; i < series[j].PointsData()->GetCount(); ++i) {
							int ix = fround(plotW*(series[j].PointsData()->x(i) - xMin)/xRange);
							int iy;
							if (series[j].primaryY)
								iy = plotH - fround(plotH*(series[j].PointsData()->y(i) - yMin)/yRange);
							else
								iy = plotH - fround(plotH*(series[j].PointsData()->y(i) - yMin2)/yRange2);
							Vector<int> dataX, dataY;
							Vector<double> dataFixed;
							for (int ii = 0; ii < series[j].PointsData()->GetznxCount(i); ++ii) 
								dataX << fround(plotW*(series[j].PointsData()->znx(ii, i) - xMin)/xRange);
							if (series[j].primaryY) {
								for (int ii = 0; ii < series[j].PointsData()->GetznyCount(i); ++ii) 
									dataY << (plotH - fround(plotH*(series[j].PointsData()->zny(ii, i) - yMin)/yRange));
							} else {
								for (int ii = 0; ii < series[j].PointsData()->GetznyCount(i); ++ii) 
									dataY << (plotH - fround(plotH*(series[j].PointsData()->zny(ii, i) - yMin2)/yRange2));
							}
							for (int ii = 0; ii < series[j].PointsData()->GetznFixedCount(); ++ii) 
								dataFixed << series[j].PointsData()->znFixed(ii, i);
							series[j].markPlot->Paint(w, plotScaleAvg, ix, iy, dataX, dataY, dataFixed, 
								series[j].markWidth, series[j].markColor, 
								series[j].markBorderWidth, series[j].markBorderColor);   
						}
					}
				}
				if (series[j].labels) {
					int dx = int(series[j].labelsDx*plotScaleX);
					int dy = int(series[j].labelsDy*plotScaleY);
					Font fnt = series[j].labelsFont;
					fnt.Height(int(fnt.GetHeight()*min(plotScaleX, plotScaleY)));
					for (int i = 0; i < points.GetCount() && i < series[j].labels->GetCount(); i++) {
						String txt = (*(series[j].labels))[i];
						Size sz = GetTextSize(txt, fnt);
						int ddy = -sz.cy/2;
						int ddx;
						switch (series[j].labelsAlign) {
						case ALIGN_LEFT:	ddx = 0;		break;
						case ALIGN_CENTER:	ddx = -sz.cx/2;	break;
						case ALIGN_RIGHT:	ddx = -sz.cx;	break;
						default: ; // to avoid warning
						}
						double x = points[i].x + dx + ddx;
						double y = points[i].y + dy + ddy;
						DrawText(w, x, y, 0, txt, fnt, series[j].labelsColor);
					}
				}
			}
		} catch(ValueTypeError error) {
			ASSERT_(true, error);
		}
	}
	WhenPaint(w);
	ClipEnd(w);
	w.End();
}
		
#endif

