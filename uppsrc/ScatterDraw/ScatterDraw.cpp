#include "ScatterDraw.h"

ScatterDraw& ScatterDraw::SetColor(const Color& _color)
{
	graphColor = _color;
	return *this;
}

ScatterDraw& ScatterDraw::SetTitle(const String& _title)
{
	title = _title;
	return *this;
}

ScatterDraw& ScatterDraw::SetTitleFont(const Font& fontTitle)
{
	titleFont = fontTitle;
	return *this;
}

ScatterDraw& ScatterDraw::SetTitleColor(const Color& colorTitle)
{
	titleColor = colorTitle;
	return *this;
}

void ScatterDraw::SetLabels(const String& _xLabel, const String& _yLabel, const String& _yLabel2)
{
	xLabel = _xLabel;
	yLabel = _yLabel;
	yLabel2 = _yLabel2;	
}

ScatterDraw& ScatterDraw::SetLabelX(const String& _xLabel)
{
	xLabel = _xLabel;
	return *this;
}

ScatterDraw& ScatterDraw::SetLabelY(const String& _yLabel)
{
	yLabel = _yLabel;
	return *this;
}

ScatterDraw& ScatterDraw::SetLabelY2(const String& _yLabel)
{
	yLabel2 = _yLabel;
	return *this;
}

ScatterDraw& ScatterDraw::SetLabelsFont(const Font& fontLabels)
{
	labelsFont = fontLabels;
	return *this;
}

ScatterDraw& ScatterDraw::SetLabelsColor(const Color& colorLabels)
{
	labelsColor = colorLabels;
	return *this;
}

ScatterDraw& ScatterDraw::SetPlotAreaMargin(const int hLeft, const int hRight, const int vTop, const int vBottom)
{
	hPlotLeft   = hLeft;	
	hPlotRight  = hRight;
	vPlotTop    = vTop;
	vPlotBottom = vBottom;
	return *this;
}

ScatterDraw& ScatterDraw::SetPlotAreaLeftMargin(const int margin) {	
	hPlotLeft = margin;	
	return *this;
}

ScatterDraw& ScatterDraw::SetPlotAreaRightMargin(const int margin) {	
	hPlotRight = margin;	
	return *this;
}

ScatterDraw& ScatterDraw::SetPlotAreaTopMargin(const int margin) {	
	vPlotTop = margin;	
	return *this;
}

ScatterDraw& ScatterDraw::SetPlotAreaBottomMargin(const int margin) {	
	vPlotBottom = margin;	
	return *this;
}

ScatterDraw& ScatterDraw::SetPlotAreaColor(const Color& p_a_color)
{
	plotAreaColor = p_a_color;        
	return *this;
}

ScatterDraw& ScatterDraw::SetAxisColor(const Color& axis_color)
{
	axisColor = axis_color;
	return *this;
}

ScatterDraw& ScatterDraw::SetAxisWidth(const int& axis_width)
{
	axisWidth = axis_width;
	return *this;
}

ScatterDraw& ScatterDraw::SetGridColor(const Color& grid_color)
{
	gridColor = grid_color;
	return *this;
}

ScatterDraw& ScatterDraw::SetGridWidth(const int& grid_width)
{
	gridWidth = grid_width;
	return *this;
}

ScatterDraw& ScatterDraw::ShowHGrid(const bool& show)
{
	drawHGrid = show;
	return *this;
}

ScatterDraw& ScatterDraw::ShowVGrid(const bool& show)
{
	drawVGrid = show;
	return *this;
}

ScatterDraw& ScatterDraw::ShowLegend(const bool& show)
{
	showLegend = show;
	return *this;
}

ScatterDraw& ScatterDraw::SetLegendWeight(const int& weight)
{
	legendWeight = weight;
	return *this;
}

ScatterDraw &ScatterDraw::SetDrawXReticle(bool set) 
{
	drawXReticle = set;
	return *this;
}

ScatterDraw &ScatterDraw::SetDrawYReticle(bool set)
{
	drawYReticle = set;
	return *this;
}

ScatterDraw &ScatterDraw::SetDrawY2Reticle(bool set)
{
	drawY2Reticle = set;
	return *this;
}

void ScatterDraw::DrawLegend(Draw& w, const int& scale) const
{
	int nmr = fround((GetSize().cx - 2*(hPlotLeft + hPlotRight))/legendWeight);	//max number of labels per row
	if (nmr <= 0) 
		return;
	int nLab = series.GetCount();	//number of labels
	int Nc;							//number of complete rows
	int LCR;						//number of labels on complete row
	int R;							//number of remaining labels on incomplete row
	if(nmr > nLab) {
		Nc = 0;      	LCR = 0; 	R = nLab;
	} else if (nmr == nLab) {   
		Nc = 1;      	LCR = nLab; R = 0;
	} else {                
		Nc = nLab/nmr;  LCR = nmr;	R = nLab%nmr;
	}
	for(int j = 0; j <= Nc; j++) {
		int start = nLab - (j+1)*LCR;
		int end = nLab - j*LCR;
		if (j == Nc) {
			start = 0; 
			end = R;
		}
		for(int i = start; i < end; i++) {
			Vector <Point> vp;
			vp << Point(scale*(i-start)*legendWeight, scale*(4-12*(j+1))) << 
				  Point(scale*(i-start)*legendWeight+scale*23, scale*(4-12*(j+1)));
			if (series[i].opacity > 0 && series[i].seriesPlot)
				DrawPolylineOpa(w, vp, scale, 1, scale*series[i].thickness, series[i].color, series[i].dash);
			Point p(scale*((i-start)*legendWeight+7),scale*(4-12*(j+1))/*+scale*Thick.At(i)/12*/);
			if (series[i].markWidth >= 1 && series[i].markPlot)
				series[i].markPlot->Paint(w, scale, p, series[i].markWidth, series[i].markColor);   
			Font scaledFont;
			scaledFont.Height(scale*StdFont().GetHeight());
			DrawText(w, scale*(i-start)*legendWeight+scale*25, scale*(-2-12*(j+1)), 0,
						series[i].legend, scaledFont, series[i].color);                   
		}
	}
}	

void ScatterDraw::AdjustMinUnitX()
{
	if (xMajorUnit > 0) {
		while (xMinUnit < 0)
			xMinUnit += xMajorUnit;	
		while (xMinUnit > xMajorUnit)
			xMinUnit -= xMajorUnit;
	}
}
/*
void ScatterDraw::AdjustMinUnitY()
{
	if (yMajorUnit > 0) {
		while (yMinUnit < 0)
			yMinUnit += yMajorUnit;	
		while (yMinUnit > yMajorUnit)
			yMinUnit -= yMajorUnit;
	}
}

void ScatterDraw::AdjustMinUnitY2()
{
	if (yMajorUnit2 > 0) {
		while (yMinUnit2 < 0)
			yMinUnit2 += yMajorUnit2;	
		while (yMinUnit2 > yMajorUnit2)
			yMinUnit2 -= yMajorUnit2;
	}
}
*/

void ScatterDraw::AdjustMinUnitY()
{
	if (yMajorUnit > 0) {
		if (fabs(yMinUnit/yMajorUnit) > 10000000000) 
			yMinUnit = yMajorUnit;
		else {
			while (yMinUnit < 0)
				yMinUnit += yMajorUnit;	
		}
		if (fabs(yMinUnit/yMajorUnit) > 10000000000) 
			yMinUnit = yMajorUnit;
		else {
			while (yMinUnit > yMajorUnit)
				yMinUnit -= yMajorUnit;
		}
	}
}

void ScatterDraw::AdjustMinUnitY2()
{
	if (yMajorUnit2 > 0) {
		if (fabs(yMinUnit2/yMajorUnit2) > 10000000000) 
			yMinUnit2 = yMajorUnit2;
		else {
			while (yMinUnit2 < 0)
				yMinUnit2 += yMajorUnit2;	
		}
		if (fabs(yMinUnit2/yMajorUnit2) > 10000000000) 
			yMinUnit2 = yMajorUnit2;
		else {
			while (yMinUnit2 > yMajorUnit2)
				yMinUnit2 -= yMajorUnit2;
		}
	}
}

ScatterDraw &ScatterDraw::SetRange(double rx, double ry, double ry2)
{
	ASSERT(!(rx <= 0 || ry <= 0 || ry2 <= 0));
	xRange = rx;
	yRange = ry;
	yRange2 = ry2;
	xMajorUnit = xRange/10;
	AdjustMinUnitX();
	yMajorUnit = yRange/10;   
	AdjustMinUnitY();
	yMajorUnit2 = yRange2/10;   
	AdjustMinUnitY2();
	WhenSetRange();
	return *this;
}

ScatterDraw &ScatterDraw::SetMajorUnits(double ux, double uy) 
{
	if (!IsNull(ux)) {
		xMajorUnit = ux;
		AdjustMinUnitX();
	}
	if (!IsNull(uy)) {
		yMajorUnit = uy;
		yMajorUnit2 = yMajorUnit*yRange2/yRange;
		AdjustMinUnitY();
		AdjustMinUnitY2();
	}
	return *this;
}

ScatterDraw &ScatterDraw::SetMajorUnitsNum(int nx, int ny) 
{
	if (!IsNull(nx)) {
		xMajorUnit = xRange/nx;
		AdjustMinUnitX();
	}
	if (!IsNull(ny)) {
		yMajorUnit = yRange/ny;
		yMajorUnit2 = yMajorUnit*yRange2/yRange;
		AdjustMinUnitY();
		AdjustMinUnitY2();
	}
	return *this;
}

ScatterDraw &ScatterDraw::SetMinUnits(double ux, double uy)
{
	xMinUnit = ux;
	yMinUnit = uy;
	yMinUnit2 = yRange2*yMinUnit/yRange;
	AdjustMinUnitX();
	AdjustMinUnitY();
	AdjustMinUnitY2();	
	return *this;
}

ScatterDraw &ScatterDraw::SetXYMin(double xmin, double ymin, double ymin2)
{
	xMin = xmin;
	yMin = ymin;
	yMin2 = ymin2;
	WhenSetXYMin();
	return *this;
}

void ScatterDraw::FitToData(bool vertical) {
	double minx, maxx, miny, miny2, maxy, maxy2;
	minx = miny = miny2 = -DOUBLE_NULL;
	maxx = maxy = maxy2 = DOUBLE_NULL;
	
	for (int j = 0; j < series.GetCount(); j++) {
		if (series[j].opacity == 0)
			continue;
		for (int i = 0; i < series[j].PointsData()->GetCount(); i++) {
			if (series[j].PointsData()->x(i) < minx)
				minx = series[j].PointsData()->x(i);
			if (series[j].PointsData()->x(i) > maxx)
				maxx = series[j].PointsData()->x(i);
		}
	}
	if (vertical) {
		for (int j = 0; j < series.GetCount(); j++) {
			if (series[j].opacity == 0)
				continue;
			for (int i = 0; i < series[j].PointsData()->GetCount(); i++) {
				if (series[j].primaryY) {
					if (series[j].PointsData()->y(i) < miny)
						miny = series[j].PointsData()->y(i);
					if (series[j].PointsData()->y(i) > maxy)
						maxy = series[j].PointsData()->y(i);
				} else {
					if (series[j].PointsData()->y(i) < miny2)
						miny2 = series[j].PointsData()->y(i);
					if (series[j].PointsData()->y(i) > maxy2)
						maxy2 = series[j].PointsData()->y(i);
				}
			}
		}		
	}
	if (minx != -DOUBLE_NULL) {
		double deltaX = xMin - minx;
		xMin -= deltaX;
		xMinUnit += deltaX;
		AdjustMinUnitX();
		xRange = maxx - minx;
	}
	if (vertical) {
		if (miny != -DOUBLE_NULL) {
			/*
			if (!IsNull(maxy2)) {
				miny = min((miny-yMin)/yRange, (miny2-yMin2)/yRange2);
				maxy = max((maxy-yMin-yRange)/yRange, (maxy2-yMin2-yRange2)/yRange2);
			} else {
				miny = (miny-yMin)/yRange;
				maxy = (maxy-yMin-yRange)/yRange;
			}
			
			miny = miny*yRange + yMin;
			maxy = maxy*yRange + yMin + yRange;
			*/
			double fact = yRange2/yRange;
			double deltaY = yMin - miny;
			double deltaY2 = deltaY*fact;
			
			yMin -= deltaY;
			yMinUnit += deltaY;
			AdjustMinUnitY();
			yRange = maxy - miny;	
			
			yMin2 -= deltaY2;
			yMinUnit2 += deltaY2;
			AdjustMinUnitY2();
			yRange2 = yRange*fact;
		}
	}	
	WhenSetRange();
	WhenSetXYMin();
	Refresh();
}

ScatterDraw &ScatterDraw::Graduation_FormatX(Formats fi)
{
	switch (fi) {
		case EXP: cbModifFormatX = THISBACK(ExpFormat); break;
		case MON: cbModifFormatX = THISBACK(MonFormat);	break;
		case DY:  cbModifFormatX = THISBACK(DyFormat);	break;
		default:break;
	}
	return *this;
}

ScatterDraw &ScatterDraw::Graduation_FormatY(Formats fi)
{
	switch (fi) {
		case EXP: cbModifFormatY = THISBACK(ExpFormat);	break;
		case MON: cbModifFormatY = THISBACK(MonFormat);	break;
		case DY:  cbModifFormatY = THISBACK(DyFormat);	break;
		default:break;
	}
	return *this;
}

ScatterDraw &ScatterDraw::Graduation_FormatY2(Formats fi)
{
	switch (fi) {
		case EXP: cbModifFormatY2 = THISBACK(ExpFormat);	break;
		case MON: cbModifFormatY2 = THISBACK(MonFormat);	break;
		case DY:  cbModifFormatY2 = THISBACK(DyFormat);		break;
		default:break;
	}
	return *this;
}

Color ScatterDraw::GetNewColor(int id)
{
	switch(id) {
	case 0:		return LtBlue();
	case 1:		return LtRed();
	case 2:		return LtGreen();
	case 3:		return Black();
	case 4:		return LtGray();
	case 5:		return Brown();
	case 6:		return Blue();
	case 7:		return Red();
	case 8:		return Green();
	case 9:		return Gray();
	case 10:	return LtBlue();
	case 11:	return LtRed();
	case 12:	return LtGreen();
	case 13:	return Black();
	case 14:	return LtGray();
	case 15:	return Brown();
	case 16:	return Blue();
	case 17:	return Red();
	case 18:	return Green();
	case 19:	return Gray();
	}
	return Color(Random(), Random(), Random());
}
	
String ScatterDraw::GetNewDash(int id)
{
	switch(id) {
	case 0:		return LINE_SOLID;
	case 1:		return LINE_DOTTED;
	case 2:		return LINE_DASHED;
	case 3:		return LINE_DASH_DOT;
	case 4:		return LINE_SOLID;
	case 5:		return LINE_DOTTED;
	case 6:		return LINE_DASHED;
	case 7:		return LINE_DASH_DOT;
	case 8:		return LINE_SOLID;
	case 9:		return LINE_DOTTED;
	case 10:	return LINE_DASHED;
	case 11:	return LINE_DASH_DOT;
	}
	dword r = Random();
	if (r < 8000)
		r += 2000;
	String ret = FormatInt(r).Right(4);
	String space = " ";
	return ret.Mid(0, 1) + space + ret.Mid(1, 1) + space + ret.Mid(2, 1) + space + ret.Mid(3, 1);
}

MarkPlot *ScatterDraw::GetNewMarkPlot(int id)
{
	switch(id) {
	case 0:	return new CircleMarkPlot();
	case 1:	return new SquareMarkPlot();
	case 2:	return new TriangleMarkPlot();
	case 3:	return new CrossMarkPlot();
	case 4:	return new XMarkPlot();
	case 5:	return new RhombMarkPlot();
	}
	return new CircleMarkPlot();
}

Color GetOpaqueColor(const Color &color, const Color &background, const double opacity) 
{
	if (opacity == 1)
		return color;
	if (opacity == 0)
		return background;
	return Color(int(opacity*(color.GetR() - background.GetR()) + background.GetR()),
	             int(opacity*(color.GetG() - background.GetG()) + background.GetG()),
	             int(opacity*(color.GetB() - background.GetB()) + background.GetB()));
} 
		
ScatterDraw::ScatterBasicSeries::ScatterBasicSeries()
{
	color = Null;
	thickness = 3;
	legend = "";
	opacity = 1;
	primaryY = true;
	sequential = false;
	dash = LINE_SOLID;	
	seriesPlot = new LineSeriesPlot();
	markPlot = new CircleMarkPlot();
	markWidth = 8;
	markColor = Null;
	fillColor = Null;
}

void ScatterDraw::ScatterBasicSeries::Init(int id)
{
	color = GetNewColor(id);
	markColor = Color(max(color.GetR()-30, 0), max(color.GetG()-30, 0), max(color.GetB()-30, 0));
	
	dash = GetNewDash(id);
	markPlot = GetNewMarkPlot(id);
}

ScatterDraw &ScatterDraw::AddSeries(double *yData, int numData, double x0, double deltaX)
{
	return AddSeries<CArray>(yData, numData, x0, deltaX);
}
	
ScatterDraw &ScatterDraw::AddSeries(double *xData, double *yData, int numData)
{
	return AddSeries<CArray>(xData, yData, numData);
}

ScatterDraw &ScatterDraw::AddSeries(Vector<double> &xData, Vector<double> &yData)
{
	return AddSeries<VectorDouble>(xData, yData);
}

ScatterDraw &ScatterDraw::AddSeries(Array<double> &xData, Array<double> &yData)
{
	return AddSeries<ArrayDouble>(xData, yData);
}
		
ScatterDraw &ScatterDraw::AddSeries(Vector<Pointf> &points)
{
	return AddSeries<VectorPointf>(points);
}

ScatterDraw &ScatterDraw::AddSeries(Array<Pointf> &points)
{
	return AddSeries<ArrayPointf>(points);
}

ScatterDraw &ScatterDraw::AddSeries(double (*function)(double))	
{
	return AddSeries<FuncSource>(function);
}

ScatterDraw &ScatterDraw::AddSeries(Pointf (*function)(double), int np, double from, double to)	
{
	return AddSeries<FuncSourcePara>(function, np, from, to);
}

ScatterDraw &ScatterDraw::AddSeries(PlotFunc &function)	
{									
	return AddSeries<PlotFuncSource>(function);
}

ScatterDraw &ScatterDraw::AddSeries(PlotParamFunc function, int np, double from, double to)	
{
	return AddSeries<PlotParamFuncSource>(function, np, from, to);
}

ScatterDraw &ScatterDraw::AddSeries(DataSource &data)
{
	ScatterSeries &s = series.Add();
	s.Init(series.GetCount()-1);
	s.SetDataSource(&data, false);
	Refresh();
	return *this;	
}

ScatterDraw &ScatterDraw::_AddSeries(DataSource *data)
{
	ScatterSeries &s = series.Add();
	s.Init(series.GetCount()-1);
	s.SetDataSource(data);
	Refresh();
	return *this;	
}

void ScatterDraw::InsertSeries(int id, double *yData, int numData, double x0, double deltaX)
{
	InsertSeries<CArray>(id, yData, numData, x0, deltaX);
}
	
void ScatterDraw::InsertSeries(int id, double *xData, double *yData, int numData)
{
	InsertSeries<CArray>(id, xData, yData, numData);
}

void ScatterDraw::InsertSeries(int id, Vector<double> &xData, Vector<double> &yData)
{
	InsertSeries<VectorDouble>(id, xData, yData);
}

void ScatterDraw::InsertSeries(int id, Array<double> &xData, Array<double> &yData)
{
	InsertSeries<ArrayDouble>(id, xData, yData);
}
		
void ScatterDraw::InsertSeries(int id, Vector<Pointf> &points)
{
	InsertSeries<VectorPointf>(id, points);
}

void ScatterDraw::InsertSeries(int id, Array<Pointf> &points)
{
	InsertSeries<ArrayPointf>(id, points);
}

void ScatterDraw::InsertSeries(int id, double (*function)(double))	
{
	InsertSeries<FuncSource>(id, function);
}

void ScatterDraw::InsertSeries(int id, Pointf (*function)(double), int np, double from, double to)	
{
	InsertSeries<FuncSourcePara>(id, function, np, from, to);
}

void ScatterDraw::InsertSeries(int id, PlotFunc &function)	
{									
	InsertSeries<PlotFuncSource>(id, function);
}

void ScatterDraw::InsertSeries(int id, PlotParamFunc function, int np, double from, double to)	
{
	InsertSeries<PlotParamFuncSource>(id, function, np, from, to);
}

void ScatterDraw::_InsertSeries(int id, DataSource *data)
{
	ASSERT(IsValid(id));
	ScatterSeries &s = series.Insert(id);
	s.Init(id);
	s.SetDataSource(data);
	Refresh();	
}

ScatterDraw &ScatterDraw::PlotStyle(SeriesPlot *data)
{
	int id = series.GetCount() - 1;
	
	series[id].seriesPlot = data;
	return *this;
}

ScatterDraw &ScatterDraw::MarkStyle(MarkPlot *data)
{
	int id = series.GetCount() - 1;
	
	series[id].markPlot = data;
	return *this;
}

ScatterDraw &ScatterDraw::Stroke(double thickness, Color color)
{
	int id = series.GetCount() - 1;

	if (IsNull(color))
		color = GetNewColor(id);
	series[id].color = color;
	series[id].thickness = thickness;
	//series[id].dash = GetNewDash(id);
	
	Refresh();
	return *this;	
}

ScatterDraw &ScatterDraw::Fill(Color color)
{
	int id = series.GetCount() - 1;

	if (IsNull(color)) {
		color = GetNewColor(id);
		color = Color(min(color.GetR()+30, 255), min(color.GetG()+30, 255), min(color.GetB()+30, 255));
	}
	series[id].fillColor = color;
	
	Refresh();
	return *this;	
}

ScatterDraw &ScatterDraw::MarkColor(Color color)
{
	int id = series.GetCount() - 1;

	if (IsNull(color)) {
		color = GetNewColor(id);
		color = Color(max(color.GetR()-30, 0), max(color.GetG()-30, 0), max(color.GetB()-30, 0));
	}
	series[id].markColor = color;
	
	Refresh();
	return *this;	
}

ScatterDraw &ScatterDraw::MarkWidth(const double& markWidth)
{
	int id = series.GetCount() - 1;
	
	series[id].markWidth = markWidth;
	
	Refresh();
	return *this;
}

ScatterDraw &ScatterDraw::Dash(const char *dash)
{
	int id = series.GetCount() - 1;
	
	series[id].dash = dash;
	Refresh();
	return *this;		
}

ScatterDraw &ScatterDraw::Legend(const String legend)
{
	int id = series.GetCount() - 1;
	
	series[id].legend = legend;
	return *this;
}

void ScatterDraw::SetDataColor(const int& j, const Color& color)
{
	ASSERT(IsValid(j));
	series[j].color = color;
	Refresh();
}

Color ScatterDraw::GetDataColor(const int& j) const
{
	ASSERT(IsValid(j));
	return series[j].color;
}

void ScatterDraw::SetDataThickness(const int& j, const double& thickness)
{
	ASSERT(IsValid(j));
	series[j].thickness = thickness;
	Refresh();
}

double ScatterDraw::GetDataThickness(const int& j) const
{
	ASSERT(IsValid(j));
	return series[j].thickness;
}

void ScatterDraw::SetFillColor(const int& j, const ::Color& color)
{
	ASSERT(IsValid(j));
	series[j].fillColor = color;
	Refresh();
}

Color ScatterDraw::GetFillColor(const int& j) const
{
	ASSERT(IsValid(j));
	return series[j].fillColor;
}

ScatterDraw &ScatterDraw::SetMarkWidth(const int& j, const double& markWidth)
{
	ASSERT(IsValid(j));
	series[j].markWidth = markWidth;
	Refresh();
	return *this;
}

double ScatterDraw::GetMarkWidth(const int& j) const
{
	ASSERT(IsValid(j));
	return series[j].markWidth;
}


void ScatterDraw::SetMarkColor(const int& j, const ::Color& color)
{
	ASSERT(IsValid(j));
	series[j].markColor = color;
	Refresh();
}

Color ScatterDraw::GetMarkColor(const int& j) const
{
	ASSERT(IsValid(j));
	return series[j].markColor;
}

void ScatterDraw::NoMark(const int& j)
{
	ASSERT(IsValid(j));
	series[j].markWidth = 0;
}

bool ScatterDraw::IsShowMark(const int& j) const throw (Exc)
{
	ASSERT(IsValid(j));
	return series[j].markWidth > 0;
}


void ScatterDraw::SetDataPrimaryY(const int& j, const bool& primary) 
{
	ASSERT(IsValid(j));
	series[j].primaryY = primary;
	Refresh();
}

ScatterDraw &ScatterDraw::SetDataPrimaryY(const bool& primary) 
{
	SetDataPrimaryY(series.GetCount()-1, primary);
	return *this;
}

bool ScatterDraw::IsDataPrimaryY(const int& j) const throw (Exc)
{
	ASSERT(IsValid(j));
	return series[j].primaryY;	
}

void ScatterDraw::SetSequentialX(const int& j, const bool& sequential) 
{
	ASSERT(IsValid(j));
	series[j].sequential = sequential;
	Refresh();
}

ScatterDraw &ScatterDraw::SetSequentialX(const bool& sequential) 
{
	SetSequentialX(series.GetCount()-1, sequential);
	return *this;
}

ScatterDraw &ScatterDraw::SetSequentialXAll(const bool& sequential) 
{
	for (int i = 0; i < series.GetCount(); ++i)
		SetSequentialX(i, sequential);
	sequentialXAll = sequential;
	return *this;
}

void ScatterDraw::Show(const int& j, const bool& show) 
{
	ASSERT(IsValid(j));
	series[j].opacity = show ? 1 : 0;
	Refresh();
}

bool ScatterDraw::IsVisible(const int& j) 
{
	ASSERT(IsValid(j));
	return series[j].opacity > 0;
}

ScatterDraw &ScatterDraw::ShowAll(const bool& show) 
{
	for (int i = 0; i < series.GetCount(); ++i)
		series[i].opacity = 1;
	return *this;
}

ScatterDraw& ScatterDraw::Id(int id)
{
	return Id(series.GetCount()-1, id);
}

ScatterDraw& ScatterDraw::Id(const int& j, int id)
{
	ASSERT(IsValid(j));
	series[j].id = id;
	return *this;
}

int ScatterDraw::GetId(const int& j)
{
	ASSERT(IsValid(j));
	return series[j].id;
}

void ScatterDraw::RemoveSeries(const int& j)
{
	ASSERT(IsValid(j));
	series.Remove(j);
	Refresh();
}

void ScatterDraw::RemoveAllSeries()
{
	series.Clear();
	Refresh();
}

Drawing ScatterDraw::GetDrawing()
{
	DrawingDraw ddw(6*GetSize());
	
	SetDrawing (ddw, 6);
	return ddw;
}

Image ScatterDraw::GetImage(const int &scale)
{
#ifndef flagGUI
	ASSERT(mode != MD_DRAW);
#endif

	ImageBuffer ib(scale*GetSize());	
	BufferPainter bp(ib, mode);
	
	SetDrawing(bp, scale);
	
	return ib;
}

double ScatterDraw::GetXByPoint(const int x) 
{
	return (x - hPlotLeft)*GetXRange()/(GetSize().cx - (hPlotLeft + hPlotRight) - 1) + GetXMin();		
}

double ScatterDraw::GetYByPoint(const int y) 
{
	return (GetSize().cy - vPlotTop - y - 1)*GetYRange()/(GetSize().cy - (vPlotTop + vPlotBottom) - GetTitleFont().GetHeight() - 1) + GetYMin();
}

double ScatterDraw::GetY2ByPoint(const int y) 
{
	return (GetSize().cy - vPlotTop - y - 1)*GetY2Range()/(GetSize().cy - (vPlotTop + vPlotBottom) - GetTitleFont().GetHeight() - 1) + GetYMin2();
}

double ScatterDraw::GetXPointByValue(const double x) 
{
	return (x - GetXMin())/GetXRange()*(GetSize().cx - (hPlotLeft + hPlotRight) - 1) + hPlotLeft;
}

double ScatterDraw::GetYPointByValue(const double y) 
{
	return (GetSize().cy - vPlotTop - 1) - (y - GetYMin())/GetYRange()*(GetSize().cy - (vPlotTop + vPlotBottom) - GetTitleFont().GetHeight() - 1);
}

void ScatterDraw::Zoom(double scale, bool mouseX, bool mouseY) 
{
	mouseX = mouseX && ((minXZoom > 0 && xRange*scale > minXZoom) || (minXZoom < 0));
	mouseX = mouseX && ((maxXZoom > 0 && xRange*scale < maxXZoom) || (maxXZoom < 0));
	mouseY = mouseY && ((minYZoom > 0 && yRange*scale > minYZoom) || (minYZoom < 0));
	mouseY = mouseY && ((maxYZoom > 0 && yRange*scale < maxYZoom) || (maxYZoom < 0));
	if (mouseX) {
		if (zoomStyleX == TO_CENTER) {
			double oldXMin = xMin;
			xMin += xRange*(1-scale)/2.;
			xMinUnit = oldXMin + xMinUnit - xMin;
			AdjustMinUnitX();
		}
		xRange *= scale;
		if (!IsNull(maxMajorUnitsX)) {
			if (xRange < xMajorUnit)
				xMajorUnit /= 10;
			else if (xRange/xMajorUnit > maxMajorUnitsX)
				xMajorUnit *= 10;
		}
	}
	if (mouseY) {
		if (zoomStyleY == TO_CENTER) {
			double oldYMin = yMin;
			yMin += yRange*(1-scale)/2.;
			yMinUnit = oldYMin + yMinUnit - yMin;
			AdjustMinUnitY();
			double oldYMin2 = yMin2;
			yMin2 += yRange2*(1-scale)/2.;
			yMinUnit2 = oldYMin2 + yMinUnit2 - yMin2;
			AdjustMinUnitY2();
		}
		yRange *= scale;
		yRange2 *= scale;
		if (!IsNull(maxMajorUnitsY)) {
			if (yRange < yMajorUnit) {
				yMajorUnit /= 10;
				yMajorUnit2 /= 10;
			} else if (yRange/yMajorUnit > maxMajorUnitsY) {
				yMajorUnit *= 10;
				yMajorUnit2 *= 10;
			}
		}		
	}
	if (mouseX || mouseY) {
		WhenSetRange();
		Refresh();
		WhenZoomScroll();
	}
}

void ScatterDraw::Scroll(double factorX, double factorY)
{
	if (factorX != 0) {
		double deltaX = factorX*xRange;
		xMin -= deltaX;
		xMinUnit += deltaX;
		AdjustMinUnitX();
	}
	if (factorY != 0) {
		double deltaY = -factorY*yRange;
		yMin -= deltaY;
		yMinUnit += deltaY;
		AdjustMinUnitY();
		if (drawY2Reticle) {
			double deltaY2 = -factorY*yRange2;
			yMin2 -= deltaY2;
			yMinUnit2 += deltaY2;
			AdjustMinUnitY2();
		}
	}
	if (factorX != 0 || factorY != 0) {		
		Refresh();
		WhenZoomScroll();
	}
}

Pointf PointAtLen(const Pointf &p0, const Pointf &p1, double len) 
{
	Pointf ret;
	if (p1.y == p0.y) {
		ret.x = p0.x + ((p0.x < p1.x) ? len : -len);
		ret.y = p0.y;
	} else if (p1.x == p0.x) {
		ret.x = p0.x;
		ret.y = p0.y + ((p0.y < p1.y) ? len : -len);
	} else {
		double k = sqr((p1.x-p0.x)/(p1.y-p0.y));
		double deltax = len/sqrt(1/k + 1);
		double deltay = len/sqrt(k + 1);
		ret.x = p0.x + ((p0.x < p1.x) ? deltax : -deltax);
		ret.y = p0.y + ((p0.y < p1.y) ? deltay : -deltay);
	}
	return ret;
}

inline bool Even(int val)	  	{return !(val%2);}

Vector <double> GetLineDash(String dash) 
{
	Vector<double> d;
	CParser p(dash);
	try {
		while(!p.IsEof())
			if(!p.Char(':'))
				d.Add(p.ReadDouble());
	}
	catch(CParser::Error) {}

	if(d.GetCount() & 1) {
		Vector<double> dash1;
		dash1.Append(d);
		dash1.Append(d);
		return dash1;
	}		
	return d;
}

Vector <double> &GetDashedArray(String dash) 
{
	static VectorMap <String, Vector <double> > pats;
	
	int pos = pats.Find(dash);
	if (pos < 0) {
		pats.Add(dash, GetLineDash(dash));
		pos = pats.GetCount()-1;
	} 
	return pats.GetValues()[pos];
}

void ScatterDraw::ParseTextMultiline(const String &text, Font fnt, Upp::Array <String> &texts, Upp::Array <Size> &sizes) {
	Size ret(0, 0);
	int npos = 0;
	for (int pos = 0; npos != -1; pos = npos+1) {
		npos = text.Find('\n', pos);
		String &t = texts.Add();
		if (npos != -1)
			t = text.Mid(pos, npos-pos);
		else
			t = text.Mid(pos);
		Size &s = sizes.Add();
		s.cx = GetTextSize(t, fnt).cx;
		s.cy = GetTextSize(t, fnt).cy;
	}
}

Size GetTextSizeMultiline(Array <Size> &sizes) {
	Size ret(0, 0);
	for (int i = 0; i < sizes.GetCount(); ++i) {
		if (sizes[i].cx > ret.cx)
			ret.cx = sizes[i].cx;
		ret.cy += sizes[i].cy;
	}
	return ret;
}
	
ScatterDraw::ScatterDraw()
{
	mode = MD_ANTIALIASED;
	size = Size(0, 0);
	titleColor = SColorText();
	graphColor = White();
	titleFont = Roman(20);
	labelsFont = StdFont();
	labelsColor = SColorText();
	plotAreaColor = SColorLtFace();
	axisColor = SColorText();
	axisWidth = 6;
	hPlotLeft = hPlotRight = vPlotTop = vPlotBottom = 30;
	xRange = yRange = yRange2 = 100.0;
	xMin = yMin = yMin2 = xMinUnit = yMinUnit = yMinUnit2 = 0.0;
	logX = logY = logY2 = false;
	gridColor = SColorDkShadow();
	gridWidth = 1;
	drawXReticle = true; drawYReticle = true;
	drawY2Reticle = false;
	drawVGrid = drawHGrid = showLegend = true;
	legendWeight = 80;
	minXZoom = maxXZoom = minYZoom = maxYZoom = -1;
	fastViewX = false;
	sequentialXAll = false;
	zoomStyleX = zoomStyleY = TO_CENTER;
	maxMajorUnitsX = maxMajorUnitsY = Null;
	SetMajorUnitsNum(5, 10);
	Color(graphColor);	
}

void DrawLine(Draw &w, double x0, double y0, double x1, double y1, double width, const Color &color)
{
	w.DrawLine(fround(x0), fround(y0), fround(x1), fround(y1), fround(width), color);
}

void DrawLine(Painter &w, double x0, double y0, double x1, double y1, double width, const Color &color)
{
	w.Move(x0, y0).Line(x0, y0).Line(x1, y1).Stroke(width, color);
}

void DrawVArrow(Draw &w, double x0, double y0, double x1, double y1, double width, double aWidth, double aHeight, const Color &color)
{
	DrawLine(w, x0, y0, x1, y1, width, color);
	if (fabs(y0 - y1) > 2*aHeight) {
		if (x0 > x1)
			Swap(x0, x1);
		if (y0 > y1)
			Swap(y0, y1);
		Vector <Point> arrowU, arrowL;
		arrowU << Point(fround(x0), fround(y0)) << Point(fround(x0-aWidth), fround(y0+aHeight)) << Point(fround(x0+aWidth), fround(y0+aHeight));
		w.DrawPolygon(arrowU, SColorHighlight());
		arrowL << Point(fround(x0), fround(y1)) << Point(fround(x0-aWidth), fround(y1-aHeight)) << Point(fround(x0+aWidth), fround(y1-aHeight));
		w.DrawPolygon(arrowL, SColorHighlight());
	}
}

void DrawVArrow(Painter &w, double x0, double y0, double x1, double y1, double width, double aWidth, double aHeight, const Color &color)
{
	DrawLine(w, x0, y0, x1, y1, width, color);
	if (fabs(y0 - y1) > 2*aHeight) {
		if (x0 > x1)
			Swap(x0, x1);
		if (y0 > y1)
			Swap(y0, y1);
		w.Move(x0, y0).Line(x0-aWidth, y0+aHeight).Line(x0+aWidth, y0+aHeight).Fill(SColorHighlight());
		w.Move(x0, y1).Line(x0-aWidth, y1-aHeight).Line(x0+aWidth, y1-aHeight).Fill(SColorHighlight());
	}
}

void DrawHArrow(Draw &w, double x0, double y0, double x1, double y1, double width, double aWidth, double aHeight, const Color &color)
{
	DrawLine(w, x0, y0, x1, y1, width, color);
	if (fabs(x0 - x1) > 2*aHeight) {
		if (x0 > x1)
			Swap(x0, x1);
		if (y0 > y1)
			Swap(y0, y1);
		Vector <Point> arrowL, arrowR;
		arrowL << Point(fround(x0), fround(y0)) << Point(fround(x0+aHeight), fround(y0+aWidth)) << Point(fround(x0+aHeight), fround(y0-aWidth));
		w.DrawPolygon(arrowL, SColorHighlight());
		arrowR << Point(fround(x1), fround(y0)) << Point(fround(x1-aHeight), fround(y0+aWidth)) << Point(fround(x1-aHeight), fround(y0-aWidth));
		w.DrawPolygon(arrowR, SColorHighlight());
	}
}

void DrawHArrow(Painter &w, double x0, double y0, double x1, double y1, double width, double aWidth, double aHeight, const Color &color)
{
	DrawLine(w, x0, y0, x1, y1, width, color);
	if (fabs(x0 - x1) > 2*aHeight) {
		if (x0 > x1)
			Swap(x0, x1);
		if (y0 > y1)
			Swap(y0, y1);
		w.Move(x0, y0).Line(x0+aHeight, y0+aWidth).Line(x0+aHeight, y0-aWidth).Fill(SColorHighlight());
		w.Move(x1, y0).Line(x1-aHeight, y0+aWidth).Line(x1-aHeight, y0-aWidth).Fill(SColorHighlight());
	}
}

void DrawText(Draw &w, double x, double y, int angle, const String &text, Font font, Color color)
{
	w.DrawText(fround(x), fround(y), angle, text, font, color);
}

void DrawText(Painter &w, double x, double y, int angle, const String &text, Font font, Color color)
{
	w.Begin();
	w.Translate(x-0.5, y-0.5).Rotate(-angle*M_PI/1800.);
	w.Text(0, 0, text, font).Fill(color);
	w.End();
}

void Clip(Draw &w, double x, double y, double cx, double cy)
{
	w.Clip(fround(x), fround(y), fround(cx), fround(cy));	
}

void Clip(Painter &w, double x, double y, double cx, double cy)
{
	w.Rectangle(x, y, cx, cy).Clip();	
}

void ClipEnd(Draw &w)
{
	w.End();	
}

void ClipEnd(Painter &w)
{
	;	
}

void DrawLineOpa(Draw& w, const int x0, const int y0, const int x1, const int y1, const int &scale, 
				const double opacity, double thick, const Color &_color, String dash, 
				const Color &background) 
{
	Vector<Point> p;
	p << Point(x0, y0) << Point(x1, y1);
	DrawPolylineOpa(w, p, scale, opacity, thick, _color, dash, background);
}

void DashScaled(Painter& w, const String dash, double scale)
{
	if (!dash.IsEmpty()) {		
		Vector<double> d;
		double start = 0;
		CParser p(dash);
		try {
			while(!p.IsEof())
				if(p.Char(':'))
					start = p.ReadDouble();
				else
					d.Add(scale*p.ReadDouble());
		}
		catch(CParser::Error) {}
		w.Dash(d, scale*start);
	}
}

void DrawLineOpa(Painter& w, const int x0, const int y0, const int x1, const int y1, const int &scale, 
				const double opacity, double thick, const Color &color, String dash, 
				const Color &background) 
{	
	w.Move(Pointf(x0, y0));
	w.Line(Pointf(x1, y1));
	DashScaled(w, dash, scale);
	w.Opacity(opacity);				// Before Stroke()
	w.Stroke(thick*scale, color);
}

void FillRectangleOpa(Draw &w, double x0, double y0, double x1, double y1, const int &scale, 
					const double opacity, const Color &background, const Color &color)
{
	if (IsNull(color))
		return;
	Color opacolor = GetOpaqueColor(color, background, opacity);
	if (x0 > x1) 
		Swap(x0, x1);
	if (y0 > y1) 
		Swap(y0, y1);
	w.DrawRect(int(x0), int(y0), abs(int(x1 - x0)), abs(int(y1 - y0)), opacolor);
}

void FillRectangleOpa(Painter &w, double x0, double y0, double x1, double y1, const int &scale, 
					const double opacity, const Color &background, const Color &color)
{
	if (IsNull(color))
		return;
	w.Rectangle(x0, y0, x1 - x0, y1 - y0).Opacity(opacity).Fill(color);
}

void DrawPolylineOpa(Draw& w, const Vector<Point> &p, const int &scale, const double opacity, 
				double thick, const Color &_color, String dash, const Color &background) 
{
	ASSERT(!p.IsEmpty());
	Color color = GetOpaqueColor(_color, background, opacity) ;
	if (dash == LINE_SOLID) 
		w.DrawPolyline(p, fround(thick), color);
	else {
		Vector <double> &pat = GetDashedArray(dash);
		int iPat = 0; 
		
		double len = pat[0]*scale;			// Pixels per bar
		Pointf begin, end;
		begin = p[0];
		for (int i = 1; i < p.GetCount();) {
			double d = Distance(begin, p[i]);
			if (d >= len) 
				end = PointAtLen(begin, p[i], len);
			else {
				end = p[i];
				len -= d;
				++i;
			}
			if (Even(iPat)) 
				w.DrawLine(begin, end, fround(thick), color);
			if (d >= len) {
				iPat++;
				if (iPat == pat.GetCount())
					iPat = 0;				 
				len = pat[iPat]*scale;
			}
			begin = end;
		}
	}		
}

void DrawPolylineOpa(Painter& w, const Vector<Point> &p, const int &scale, const double opacity, 
				double thick, const Color &color, String dash, const Color &background) 
{	
	ASSERT(!p.IsEmpty());
	w.Move(p[0]);
	for (int i = 1; i < p.GetCount(); ++i) 
		w.Line(p[i]);
	DashScaled(w, dash, scale);
	w.Opacity(opacity);				// Before Stroke()
	w.Stroke(thick*scale, color);
}

void FillPolylineOpa(Draw& w, const Vector<Point> &p, const int &scale, const double opacity, 
				const Color &background, const Color &fillColor) 
{
	ASSERT(!p.IsEmpty());
	Color opacolor = GetOpaqueColor(fillColor, background, opacity) ;

	w.DrawPolygon(p, opacolor);
}

void FillPolylineOpa(Painter& w, const Vector<Point> &p, const int &scale, const double opacity, 
				const Color &background, const Color &fillColor) 
{	
	ASSERT(!p.IsEmpty());
	w.Move(p[0]);
	for (int i = 1; i < p.GetCount(); ++i) 
		w.Line(p[i]);
	w.Opacity(opacity);
	w.Fill(fillColor);		// Before Stroke()
}

INITBLOCK{
	SeriesPlot::Register<LineSeriesPlot>("Line");
	SeriesPlot::Register<StaggeredSeriesPlot>("Staggered");
	SeriesPlot::Register<BarSeriesPlot>("Bar");
	
	MarkPlot::Register<CircleMarkPlot>("Circle");
	MarkPlot::Register<SquareMarkPlot>("Square");
	MarkPlot::Register<TriangleMarkPlot>("Triangle");
	MarkPlot::Register<CrossMarkPlot>("Cross");
	MarkPlot::Register<XMarkPlot>("X");
	MarkPlot::Register<RhombMarkPlot>("Rhomb");
}

void GoBreakpoint()
{
	int kk = 1;
}