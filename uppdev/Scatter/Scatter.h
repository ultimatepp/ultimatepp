#ifndef _Scatter_Scatter_h
#define _Scatter_Scatter_h

#include <CtrlLib/CtrlLib.h>


struct XY:Moveable<XY>{
	double x;
	double y;
	XY()
	{
		x=0;
		y=0;
	}
	XY(double dx, double dy)
	{
		x=dx;
		y=dy;
	}
	XY(const XY & point)
	{
		x=point.x;
		y=point.y;
	}

};

//Support for a possible PieChart
struct DrawExtension: Draw
{
	typedef DrawExtension CLASSNAME;

	DrawExtension()
	{
		Draw::Draw();
	}
	void DrawPie(const Rect& r,Point p1,Point p2, Color color, int pen, Color pencolor)
	{
		SetColor(color);
		SetDrawPen(pen, pencolor);

		::Pie(GetHandle(), r.left, r.top, r.right, r.bottom,p1.x,p1.y,p2.x,p2.y);

	}
	void DrawRoundRect(const Rect& r,int nWidth,int nHeight, Color color, int pen, Color pencolor)
	{
		SetColor(color);
		SetDrawPen(pen, pencolor);
		::RoundRect(GetHandle(),r.left, r.top, r.right, r.bottom,nWidth,nHeight);
	}
};


class Scatter : public StaticRect {
public:
	enum MarkStyle
	{
		CIRCLE,
		RECTANGLE,
		SQUARE,
		TRIANGLE,
		CROSS,
		X,
		RHOMB
	};

	virtual void  Paint(Draw& w);

	void Paint(DrawExtension& we);
	void SetDrawing(Draw& w,int scale);
	void Circle(Draw& w, int scale, Point cp, int size, class::Color markColor);
	void Square(Draw& w, int scale, Point cp, int size, class::Color markColor);
	void Rectangle(Draw& w, int scale, Point cp, int size, class::Color markColor);
	void Triangle(Draw& w, int scale, Point cp, int size, class::Color markColor);
	void Cross(Draw& w, int scale, Point cp, int size, class::Color markColor);
	void XFunct(Draw& w, int scale, Point cp, int size, class::Color markColor);
	void Rhomb(Draw& w, int scale, Point cp, int size, class::Color markColor);
	inline void DrawMark(int style,Draw& w, int scale, Point cp, int size, class::Color markColor);

	//virtual void  LeftDown(Point, dword);
	Scatter& SetColor(class::Color _color);
	Scatter& SetTitle(String title);
	Scatter& SetFontTitle(Font& fontTitle);
	Scatter& SetColorTitle(class::Color colorTitle);

	void SetLabels(String _xLabel,String _yLabel);
	Scatter& SetLabelX(String _xLabel);
	Scatter& SetLabelY(String _yLabel);
	Scatter& SetFontLabels(Font& fontLabels);
	Scatter& SetColorLabels(class::Color colorLabels);

	Scatter& SetPlotAreaPoz(int poz_x,int poz_y);
	Scatter& H_Border(int poz_x);
	Scatter& V_Border(int poz_y);
	Scatter& SetPlotAreaColor(class ::Color p_a_color);

	Scatter& SetAxisColor(class::Color axis_color);
	Scatter& SetAxisWidth(int axis_width);

	Scatter& SetGridColor(class::Color grid_color);
	Scatter& SetGridWidth(int grid_width);
	Scatter& ShowVGrid(bool show);
	Scatter& ShowHGrid(bool show);

	Scatter& showLegend(bool show=true);
	Scatter& SetLegendWeight(int weight);
	void DrawLegend(Draw& w,int scale);

	void SetRange(double rx, double ry);
	void SetMajorUnits(double ux, double uy);
	void SetXYMin(double xmin,double ymin);


	void AddSeries (Vector<XY> & points,String legend="", bool join=false,class::Color pcolor=LtBlue,int width=30,int thickness=6);

	void SetData(int nbSeries, int index, XY & point);
	void AddPoint(int nbSeries, XY & point);
	void InsertPoint(int nbSeries, int index, XY & point);
	void RemovePoint(int nbSeries, int index);

	void SetDataColor(int j,class::Color pcolor);
	void SetFunctColor(int j, class::Color fcolor);
	class::Color GetDataColor (int j);
	class::Color GetFunctColor (int j);


	void SetDataThickness(int j, int thick_dots);
	void SetFunctThickness(int j,int thick_dots);
	int GetDataThickness(int j);
	int GetFunctThickness(int j);
	void SetMarkWidth(int j, int width_dots);
	int GetMarkWidth(int j);
	void SetMarkStyle(int j, int noStyle);
	int GetMarkStyle(int j);
	void SetMarkColor(int j,class::Color pcolor);
	class::Color GetMarkColor (int j);
	void SetShowMark(int j, bool show=true);
	bool IsMarkShow(int j);

	void SetJoin(int j, bool join=true);
	bool IsJoined(int j);

	void SetSmooth(int j, bool smooth=true);
	bool IsSmooth(int j);

	void RemoveSeries(int j);
	void SetData(int nbSeries, Vector<XY> & points);
	Vector<Vector<XY> > & GetData() {return PointsData;}

	void plotFunction(double (*f)(double),String legend="", class::Color fcolor=Green,int weight=6);
	void plotParaFunction(XY (*pf)(double),String legend="", class::Color fcolor=Green,int weight=6,int Np=50);
	void RemoveFSeries(int j);
	int GetNbMax(); //max number of points along X axis

	Vector<XY> Cubic (Vector<XY>& DataSet, int fineness=10, double tension=0.4);

	Drawing& getDrawing();
	Image& getImage(int scale=1);

private:
	class ::Color graphColor;
	String Title;
	Font FontTitle;
	class::Color ColorTitle;

	String xLabel, yLabel;
	Font FontLabels;
	class::Color ColorLabels;

	int px, py;
	class ::Color plotAreaColor;

	class::Color axisColor;
	int axisWidth;

	double xRange,yRange;
	double xMin,yMin;
	double xMajorUnit,yMajorUnit;
	bool DrawXReticle, DrawYReticle;

	class::Color gridColor;
	int gridWidth;
	bool DrawVGrid, DrawHGrid;

	Vector<Vector<XY> > PointsData,FunctionData;
	Vector<class::Color> PColors,FColors;
	Vector<int> FThickness,PThickness;
	Vector<int> PWidth;
	Vector<bool> Join;
	Vector<bool> Smooth;
	Vector<class::Color> MarkColors;
	Vector<bool> ShowMark;
	Vector<int> MarkStyles;
	Drawing drawing;
	Image img;

	Vector<String> Legend;
	Vector<String> FLegend;
	bool ShowLegend;
	int LegendWeight;

public:
	Scatter();
	~Scatter();
};


#endif
