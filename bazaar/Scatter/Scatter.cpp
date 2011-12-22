#include "Scatter.h"

#define IMAGECLASS ChartImg
#define IMAGEFILE <Scatter/Chart.iml>
#include <Draw/iml.h>

Scatter::MouseBehaviour defaultMouse[] = {
	{false, false, false, true , false, 0, false, Scatter::SHOW_INFO}, 
	{false, false, false, false, false, 0, true , Scatter::CONTEXT_MENU},
	{false, false, false, false, true , 0, false, Scatter::SCROLL},
	{false, false, false, false, false, 1, false, Scatter::ZOOM_H_RED},
	{false, false, false, false, false, 1, false, Scatter::ZOOM_V_RED},
	{false, false, false, false, false,-1, false, Scatter::ZOOM_H_ENL},
	{false, false, false, false, false,-1, false, Scatter::ZOOM_V_ENL},
	{false, false, false, false, false, 0, false, Scatter::NO_ACTION}};
	

Scatter& Scatter::SetColor(const class::Color& _color)
{
	graphColor=_color;
	return *this;
}

Scatter& Scatter::SetTitle(const String& _title)
{
	title=_title;
	return *this;
}
Scatter& Scatter::SetTitleFont(const Font& fontTitle)
{
	titleFont=fontTitle;
	return *this;
}
Scatter& Scatter::SetTitleColor(const class::Color& colorTitle)
{
	titleColor=colorTitle;
	return *this;
}
void Scatter::SetLabels(const WString& _xLabel, const WString& _yLabel, const WString& _yLabel2)
{
	xLabel=_xLabel;
	yLabel=_yLabel;
	yLabel2=_yLabel2;	
}
Scatter& Scatter::SetLabelX(const WString& _xLabel)
{
	xLabel=_xLabel;
	return *this;
}
Scatter& Scatter::SetLabelY(const WString& _yLabel)
{
	yLabel=_yLabel;
	return *this;
}
Scatter& Scatter::SetLabelY2(const WString& _yLabel)
{
	yLabel2=_yLabel;
	return *this;
}
Scatter& Scatter::SetLabelsFont(const Font& fontLabels)
{
	labelsFont=fontLabels;
	return *this;
}
Scatter& Scatter::SetLabelsColor(const class::Color& colorLabels)
{
	labelsColor=colorLabels;
	return *this;
}
Scatter& Scatter::SetPlotAreaPoz(const int& poz_x, const int& poz_y)
{
	px=poz_x;
	py=poz_y;
	return *this;
}
Scatter& Scatter::H_Border(const int& poz_x)
{
	if(poz_x>=0) 
		px=poz_x;
	return *this;
}
Scatter& Scatter::V_Border(const int& poz_y)
{
	if(poz_y>=0) 
		py=poz_y;
	return *this;
}

Scatter& Scatter::SetPlotAreaColor(const class::Color& p_a_color)
{
	plotAreaColor=p_a_color;        
	return *this;
}
Scatter& Scatter::SetAxisColor(const class::Color& axis_color)
{
	axisColor=axis_color;
	return *this;
}
Scatter& Scatter::SetAxisWidth(const int& axis_width)
{
	axisWidth=axis_width;
	return *this;
}
Scatter& Scatter::SetGridColor(const class::Color& grid_color)
{
	gridColor=grid_color;
	return *this;
}
Scatter& Scatter::SetGridWidth(const int& grid_width)
{
	gridWidth=grid_width;
	return *this;
}
Scatter& Scatter::ShowHGrid(const bool& show)
{
	drawHGrid=show;
	return *this;
}
Scatter& Scatter::ShowVGrid(const bool& show)
{
	drawVGrid=show;
	return *this;
}
Scatter& Scatter::ShowLegend(const bool& show)
{
	showLegend=show;
	return *this;
}
Scatter& Scatter::SetLegendWeight(const int& weight)
{
	legendWeight=weight;
	return *this;
}
Scatter& Scatter::SetAntialiasing(const bool& aa)
{
	antialiasing=aa;
	return *this;
}
Scatter &Scatter::SetDrawXReticle(bool set) 
{
	drawXReticle = set;
	return *this;
}
Scatter &Scatter::SetDrawYReticle(bool set)
{
	drawYReticle = set;
	return *this;
}
Scatter &Scatter::SetDrawY2Reticle(bool set)
{
	drawY2Reticle = set;
	return *this;
}
void Scatter::DrawLegend(Draw& w,const int& scale) const
{
	Vector<String> L;
	L.Append(vLegend);
	L.Append(vFLegend);
	Vector<int> Thick;
	Thick.Append(vPThickness);
	Thick.Append(vFThickness);
	Vector<class::Color> LC;
	LC.Append(vPColors);
	LC.Append(vFColors);
	Vector<int> LW;
	LW.Append(vPWidth);
	Vector<bool>VJ;
	VJ.Append(vJoin);
	Vector<int> MS;
	MS.Append(vMarkStyles);
	Vector<class::Color> MC;
	MC.Append(vMarkColors);
	Vector<String> Patt;
	Patt.Append(vPPattern);
	Patt.Append(vFPattern);
	
	int NMR=fround((GetSize().cx-2*px)/legendWeight);//max number of labels per row
	if (NMR>0)
	{
		int N=L.GetCount();//number of labels
		int Nc;//number of complete rows
		int LCR;//number of labels on complete row
		int R;//number of remaining labels on incomplete row
		if(NMR>N)           
			{Nc=0;      LCR=0;      R=N;}
		else if (NMR==N)    
			{Nc=1;      LCR=N;      R=0;}
		else                
			{Nc=N/NMR;  LCR=NMR;    R=N%NMR;}
		for(int j=0;j<=Nc;j++)          
		{
			int start=N-(j+1)*LCR;
			int end=N-j*LCR;
			if (j==Nc) 
				{start=0; end=R;}
			for(int i=start;i<end;i++)
			{
				/*w.DrawRect(scale*(i-start)*legendWeight,
								scale*(4-12*(j+1)),
								14*scale,
								scale*Thick.At(i)/6,
								LC.At(i));*/
				Vector <Point> vp;
				vp << Point(scale*(i-start)*legendWeight, scale*(4-12*(j+1))) << 
					  Point(scale*(i-start)*legendWeight+scale*23, scale*(4-12*(j+1)));
				if (VJ.At(i))
					DrawPolylineX(w, vp, scale*Thick.At(i)/6, LC.At(i), Patt.At(i), scale);
				Point p(scale*((i-start)*legendWeight+7),scale*(4-12*(j+1))/*+scale*Thick.At(i)/12*/);
				if (LW.At(i,0) > 1)
					DrawMark(MS.At(i),w,scale,p,LW.At(i,0),MC.At(i));                                           
				Font scaledFont;
				scaledFont.Height(scale*StdFont().GetHeight());
				w.DrawText(scale*(i-start)*legendWeight+scale*25,
							scale*(-2-12*(j+1)),
							L.At(i),
							scaledFont,LC.At(i));                   
			}
		}
	}		
}	

void Scatter::AdjustMinUnitX()
{
	if (xMajorUnit > 0) {
		while (xMinUnit < 0)
			xMinUnit += xMajorUnit;	
		while (xMinUnit > xMajorUnit)
			xMinUnit -= xMajorUnit;
	}
}
void Scatter::AdjustMinUnitY()
{
	if (yMajorUnit > 0) {
		while (yMinUnit < 0)
			yMinUnit += yMajorUnit;	
		while (yMinUnit > yMajorUnit)
			yMinUnit -= yMajorUnit;
	}
}
void Scatter::AdjustMinUnitY2()
{
	if (yMajorUnit2 > 0) {
		while (yMinUnit2 < 0)
			yMinUnit2 += yMajorUnit2;	
		while (yMinUnit2 > yMajorUnit2)
			yMinUnit2 -= yMajorUnit2;
	}
}
Scatter &Scatter::SetRange(double rx, double ry, double ry2)
{
	if (rx <= 0 || ry <= 0 || ry2 <= 0)
		throw (Exc(t_("Invalid graph range")));	
	xRange=rx;
	yRange=ry;
	yRange2=ry2;
	xMajorUnit=xRange/10;
	AdjustMinUnitX();
	yMajorUnit=yRange/10;   
	AdjustMinUnitY();
	yMajorUnit2=yRange2/10;   
	AdjustMinUnitY2();
	WhenSetRange();
	return *this;
}
Scatter &Scatter::SetMajorUnits(double ux, double uy) 
{
	//if (ux > xRange)	throw (Exc(t_("Invalid X major units!")));
	//if (uy > yRange)	throw (Exc(t_("Invalid Y major units!")));
	xMajorUnit=ux;
	yMajorUnit=uy;
	yMajorUnit2=yRange2*yMajorUnit/yRange;
	AdjustMinUnitX();
	AdjustMinUnitY();
	AdjustMinUnitY2();
	return *this;
}
Scatter &Scatter::SetMinUnits(double ux, double uy)
{
	xMinUnit=ux;
	yMinUnit=uy;
	yMinUnit2=yRange2*yMinUnit/yRange;
	AdjustMinUnitX();
	AdjustMinUnitY();
	AdjustMinUnitY2();	
	return *this;
}
Scatter &Scatter::SetXYMin(double xmin, double ymin, double ymin2)
{
	xMin=xmin;
	yMin=ymin;
	yMin2=ymin2;
	WhenSetXYMin();
	return *this;
}

void Scatter::FitToData(bool Y) {
	double minx, maxx, miny, miny2, maxy, maxy2;
	minx = miny = miny2 = -DOUBLE_NULL;
	maxx = maxy = maxy2 = DOUBLE_NULL;
	
	for (int j=0; j<vPointsData.GetCount(); j++) {
		for (int i=0; i<vPointsData[j].GetCount(); i++) {
			if (vPointsData[j][i].x < minx)
				minx = vPointsData[j][i].x;
			if (vPointsData[j][i].x > maxx)
				maxx = vPointsData[j][i].x;
		}
	}
	for (int j=0; j<vFunctionData.GetCount(); j++) {
		if(!vFunctionData[j].IsEmpty()) {
			for (int i=0; i<vFunctionData[j].GetCount(); i++) {
				if (vFunctionData[j][i].x < minx)
					minx = vFunctionData[j][i].x;
				if (vFunctionData[j][i].x < maxx)
					maxx = vFunctionData[j][i].x;
			}
		}
	}
	if (Y) {
		for (int j=0; j<vPointsData.GetCount(); j++) {
			for (int i=0; i<vPointsData[j].GetCount(); i++) {
				if (vPPrimaryY[j]) {
					if (vPointsData[j][i].y < miny)
						miny = vPointsData[j][i].y;
					if (vPointsData[j][i].y > maxy)
						maxy = vPointsData[j][i].y;
				} else {
					if (vPointsData[j][i].y < miny2)
						miny2 = vPointsData[j][i].y;
					if (vPointsData[j][i].y > maxy2)
						maxy2 = vPointsData[j][i].y;
				}
			}
		}		
		for (int j=0; j<vFunctionData.GetCount(); j++) {
			if(!vFunctionData[j].IsEmpty()) {
				for (int i=0; i<vFunctionData[j].GetCount(); i++) {
					if (vFPrimaryY[j]) {
						if (vFunctionData[j][i].y < miny)
							miny = vFunctionData[j][i].y;
						if (vFunctionData[j][i].y < maxy)
							maxy = vFunctionData[j][i].y;
					} else {
						if (vFunctionData[j][i].y < miny2)
							miny2 = vFunctionData[j][i].y;
						if (vFunctionData[j][i].y < maxy2)
							maxy2 = vFunctionData[j][i].y;
					}
				}
			}
		}
	}
	double deltaX = xMin - minx;
	xMin -= deltaX;
	xMinUnit += deltaX;
	AdjustMinUnitX();
	xRange = maxx - minx;

	if (Y) {
		miny = min((miny-yMin)/yRange, (miny2-yMin2)/yRange2);
		maxy = max((maxy-yMin-yRange)/yRange, (maxy2-yMin2-yRange2)/yRange2);
		miny = miny*yRange + yMin;
		maxy = maxy*yRange + yMin + yRange;
		
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
	WhenSetRange();
	WhenSetXYMin();
	Refresh();
}

void Scatter::Graduation_FormatX(Formats fi)
{
	switch (fi)
	{
		case EXP: cbModifFormatX=THISBACK(ExpFormat); 	break;
		case MON: cbModifFormatX=THISBACK(MonFormat);	break;
		case DY:  cbModifFormatX=THISBACK(DyFormat);	break;
		default:break;
	}
}

void Scatter::Graduation_FormatY(Formats fi)
{
	switch (fi)
	{
		case EXP: cbModifFormatY=THISBACK(ExpFormat);	break;
		case MON: cbModifFormatY=THISBACK(MonFormat);	break;
		case DY:  cbModifFormatY=THISBACK(DyFormat);	break;
		default:break;
	}
}

void Scatter::Graduation_FormatY2(Formats fi)
{
	switch (fi)
	{
		case EXP: cbModifFormatY2=THISBACK(ExpFormat);	break;
		case MON: cbModifFormatY2=THISBACK(MonFormat);	break;
		case DY:  cbModifFormatY2=THISBACK(DyFormat);	break;
		default:break;
	}
}

Scatter &Scatter::AddSeries(Vector<XY> & points,const String& legend,const bool& join, const class::Color& pcolor, const int& width, const int& thickness)
{
	vPointsData.AddPick(points);
	vJoin.Add(join);
	vSmooth.Add(false);
	vPColors.Add(pcolor);
	vPWidth.Add(width);
	vPThickness.Add(thickness);
	vLegend.Add(legend);
	vMarkColors.Add(pcolor);
	if (width <= 1)
		vShowMark.Add(false);
	else
		vShowMark.Add(true);
	vMarkStyles.Add(CIRCLE);	
	vPPrimaryY.Add(true);
	vPSequential.Add(sequentialXAll);
	vPPattern.Add(LINE_SOLID);	
	
	Refresh();
	return *this;
}

Color GetNewColor(int id)
{
	switch(id) {
	case 0:	return LtBlue();
	case 1:	return LtRed();
	case 2:	return LtGreen();
	case 3:	return Black();
	case 4:	return LtGray();
	case 5:	return Yellow();
	case 6:	return Brown();
	case 7:	return Blue();
	case 8:	return Red();
	case 9:	return Green();
	case 10:return Gray();
	}
	return Color(Random(), Random(), Random());
}

	#define LINE_DOTTED 	"o..."
	#define LINE_DOTTED_SEP	"o..........."
	#define LINE_DASHED 	"oooooo......"
	#define LINE_DASH_DOT 	"ooooo...o..."
	#define LINE_SOLID 		"oooooooooooo"
	
String GetNewPattern(int id)
{
	switch(id) {
	case 0:	return LINE_SOLID;
	case 1:	return LINE_DOTTED;
	case 2:	return LINE_DASHED;
	case 3:	return LINE_DASH_DOT;
	case 4:	return LINE_SOLID;
	case 5:	return LINE_DOTTED;
	case 6:	return LINE_DASHED;
	case 7:	return LINE_DASH_DOT;
	}
	String ret = "oo";
	dword r = Random();
	return ret << (r & 1 ? "oo" : "..") << (r & 2 ? "oo" : "..") << (r & 4 ? "oo" : "..");
}

Scatter &Scatter::Stroke(int width, class::Color pcolor, String pattern)
{
	int id = vPointsData.GetCount() - 1;

	vJoin[id] = width > 0;	
	if (IsNull(pcolor))
		pcolor = GetNewColor(id);
	vPColors[id] = pcolor;
	vPThickness[id] = width;
	if (IsNull(pattern))
		pattern = GetNewPattern(id);
	vPPattern[id] = pattern;
	
	Refresh();
	return *this;	
}

Scatter &Scatter::Mark(int thickness, class::Color pcolor, MarkStyle style)
{
	int id = vPointsData.GetCount() - 1;

	vPWidth[id] = thickness;	
	if (IsNull(pcolor))
		pcolor = GetNewColor(id);
	vMarkColors[id] = pcolor;
	vShowMark[id] = thickness > 0;
	vMarkStyles[id] = style;
		
	Refresh();
	return *this;
}

Scatter &Scatter::SetPattern(const String pattern)
{
	vPPattern[vPPattern.GetCount()-1] = pattern;
	return *this;
}

void Scatter::AddPoint(const int& j, const XY & point, const bool& refresh)
{
	if(IsValid(j))
	{
		vPointsData[j].Add(point);
		if(refresh) Refresh();
	}
}
void Scatter::InsertPoint(const int& j, const int& i, const XY & point)
{
	if(IsValid(j))
	{
		if(i>=0 && vPointsData[j].GetCount()>=i)
		{
			vPointsData[j].Insert(i,point);
			Refresh();
		}
	}
}
void Scatter::RemovePoint(const int& j, const int& i)
{
	if(IsValid(j))
	{
		if(i>=0 && vPointsData[j].GetCount()>i)
		{
			vPointsData[j].Remove(i);
			Refresh();
		}
	}
}
void Scatter::SetData(const int& j, const int& i, const XY & point)
{
	if(IsValid(j))
	{
		if(i>=0 && vPointsData[j].GetCount()>i)
		{
			vPointsData[j][i]=point; 
			Refresh();
		}
	}
}

void Scatter::SetData(const int& j, Vector<XY> & points)
{
	if(IsValid(j))
	{
		vPointsData[j]<<=points;
		Refresh();
	}
}
void Scatter::SetDataColor(const int& j, const class::Color& pcolor)
{
	if(IsValid(j))
	{
		vPColors[j]=pcolor;
		Refresh();
	}
}
void Scatter::SetFunctColor(const int& j, const class::Color& fcolor)
{
	if(IsValid(j))
	{
		vFColors[j]=fcolor;
		Refresh();
	}       
}
Color Scatter::GetDataColor(const int& j) const
{
	if(IsValid(j))
	{
		return vPColors[j];
	}	
	throw (Exc(t_("Invalid series index!")));
}

Color Scatter::GetFunctColor(const int& j) const
{
	if(IsValid(j))
	{
		return vFColors[j];
	}
	throw (Exc(t_("Invalid function index!")));
}

void Scatter::SetDataThickness(const int& j, const int& thick_dots)
{
	if(IsValid(j))
	{
		vPThickness[j]=thick_dots;
		Refresh();
	}
}

void Scatter::SetFunctThickness(const int& j, const int& thick_dots)
{
	if(IsValid(j))
	{
		vFThickness[j]=thick_dots;
		Refresh();
	}
}
int Scatter::GetDataThickness(const int& j) const
{
	if(IsValid(j))
	{
		return vPThickness[j];
	}
	throw (Exc(t_("Invalid series index!")));
}

int Scatter::GetFunctThickness(const int& j) const
{
	if(IsValid(j))
	{
		return vFThickness[j];
	}
	throw (Exc(t_("Invalid series index!")));
}

void Scatter::SetMarkWidth(const int& j, const int& width_dots)
{
	if(IsValid(j))
	{
		vPWidth[j]=width_dots;
		Refresh();
	}
}

int Scatter::GetMarkWidth(const int& j) const
{
	if(IsValid(j))
	{
		return vPWidth[j];
	}
	throw (Exc(t_("Invalid series index!")));
}

void Scatter::SetMarkStyle(const int& j, MarkStyle noStyle)
{
	if(IsValid(j))
	{
		vMarkStyles[j]=noStyle;
		Refresh();
	}
}

int Scatter::GetMarkStyle(const int& j) const
{
	if(IsValid(j))
	{
		return vMarkStyles[j];
	}
	throw (Exc(t_("Invalid series index!")));
}

void Scatter::SetMarkColor(const int& j, const ::Color& mcolor)
{
	if(IsValid(j))
	{
		vMarkColors[j]=mcolor;
		Refresh();
	}
}

Color Scatter::GetMarkColor(const int& j) const
{
	if(IsValid(j))
	{
		return vMarkColors[j];
	}	
	throw (Exc(t_("Invalid series index!")));
}

void Scatter::SetShowMark(const int& j, const bool& show)
{
	if(IsValid(j))
	{
		vShowMark[j]=show;       
		Refresh();
	}
}

bool Scatter::IsMarkShow(const int& j) const throw (Exc)
{
	if(!IsValid(j)) 
		throw (Exc(t_("Invalid series index!")));
	return vShowMark[j];
}

void Scatter::SetJoin(const int& j, const bool& join) 
{
	if(IsValid(j))
	{
		vJoin[j]=join;
		Refresh();
	}
}

bool Scatter::IsJoined(const int& j) const throw (Exc)
{
	if(!IsValid(j)) 
		throw (Exc(t_("Invalid series index!")));
	
	return vJoin[j];	
}

void Scatter::SetSmooth(const int& j, const bool& smooth) 
{
	if(IsValid(j))
	{
		vSmooth[j]=smooth;
		Refresh();
	}
}

bool Scatter::IsSmooth(const int& j) const throw (Exc)
{
	if(!IsValid(j)) 
		throw (Exc(t_("Invalid series index!")));
	
	return vSmooth[j];	
}

void Scatter::SetDataPrimaryY(const int& j, const bool& primary) 
{
	if(IsValid(j))
	{
		vPPrimaryY[j]=primary;
		Refresh();
	}
}

Scatter &Scatter::SetDataPrimaryY(const bool& primary) 
{
	SetDataPrimaryY(vPPrimaryY.GetCount()-1, primary);
	return *this;
}

bool Scatter::IsDataPrimaryY(const int& j) const throw (Exc)
{
	if(!IsValid(j)) 
		throw (Exc(t_("Invalid series index!")));
	return vPPrimaryY[j];	
}

void Scatter::SetSequentialX(const int& j, const bool& sequential) 
{
	if(IsValid(j))
	{
		vPSequential[j]=sequential;
		Refresh();
	}
}

Scatter &Scatter::SetSequentialX(const bool& sequential) 
{
	SetSequentialX(vPSequential.GetCount()-1, sequential);
	return *this;
}

Scatter &Scatter::SetSequentialXAll(const bool& sequential) 
{
	for (int i = 0; i < vPSequential.GetCount(); ++i)
		SetSequentialX(i, sequential);
	sequentialXAll = sequential;
	return *this;
}

void Scatter::SetFunctPrimaryY(const int& j, const bool& primary) 
{
	if(IsValid(j))
	{
		vFPrimaryY[j]=primary;
		Refresh();
	}
}

Scatter &Scatter::SetFunctPrimaryY(const bool& primary) 
{
	SetFunctPrimaryY(vFPrimaryY.GetCount()-1, primary);
	return *this;
}

bool Scatter::IsFunctPrimaryY(const int& j) const throw (Exc)
{
	if(!IsValid(j)) 
		throw (Exc(t_("Invalid series index!")));
	return vFPrimaryY[j];	
}
   
void Scatter::RemoveSeries(const int& j)
{
	if (IsValid(j))
	{
		vPointsData.Remove(j);
		vJoin.Remove(j);
		vSmooth.Remove(j);
		vPColors.Remove(j);
		vPWidth.Remove(j);
		vPThickness.Remove(j);
		vLegend.Remove(j);           
		vMarkColors.Remove(j);
		vShowMark.Remove(j);
		vMarkStyles.Remove(j);
		vPPrimaryY.Remove(j);
		vPPattern.Remove(j);
		vPSequential.Remove(j);
		
		Refresh();
	}
}

void Scatter::RemoveAllSeries()
{
	vPointsData.Clear();
	vJoin.Clear();
	vSmooth.Clear();
	vPColors.Clear();
	vPWidth.Clear();
	vPThickness.Clear();
	vLegend.Clear();
	vMarkColors.Clear();
	vShowMark.Clear();
	vMarkStyles.Clear();
	vPPrimaryY.Clear();
	vPSequential.Clear();
	vPPattern.Clear();
	
	Refresh();
}


void Scatter::PlotFunction(PlotFunc f, const String& legend, const class::Color& fcolor, const int& weight)
{
	Vector<XY> series;
	vFunctionData.AddPick(series);
	
	vAdress.Add(f);
	
	vFColors.Add(fcolor);
	vFThickness.Add(weight);	
	vFLegend.Add(legend);
	vFPrimaryY.Add(true);
	vFPattern.Add(LINE_SOLID);
	Refresh();  
}
void Scatter::PlotParaFunction(PlotParamFunc f, const String& legend, const class::Color& fcolor, const int& weight,const int& Np)
{
	double t;//t must be choosed between [0,1]
	Vector<XY> series;
	for (int i=0; i<=Np;i++)
	{
		t=(double)i/Np;
		//series<<pf(t);
		f(series.Add(), t);
	}
	vFColors.Add(fcolor);
	vFThickness.Add(weight);
	vFunctionData.AddPick(series);
	vFPrimaryY.Add(true);
	vFPattern.Add(LINE_SOLID);
	vFLegend.Add(legend);
	Refresh();
}	

Scatter &Scatter::SetFunctPattern(const String pattern)
{
	vFPattern[vFPattern.GetCount()-1] = pattern;
	return *this;
}

void Scatter::RemoveFSeries(const int& j)
{
	if (!vFunctionData.IsEmpty() &&  vFunctionData.GetCount()>j)
	{
		if(vFunctionData[j].IsEmpty())
		{
			int nf=-1;
			for(int i=0; i<=j; i++)
			{
				if (vFunctionData[i].IsEmpty()) 
					nf++;
			}
			vAdress.Remove(nf);
		}
				
		vFunctionData.Remove(j);
		vFColors.Remove(j);
		vFThickness.Remove(j);
		vFLegend.Remove(j);
		vFPrimaryY.Remove(j);
		vFPattern.Remove(j);
			
		Refresh();
	}
}

void Scatter::RemoveAllFSeries()
{
	vAdress.Clear();
	vFunctionData.Clear();
	vFColors.Clear();
	vFThickness.Clear();
	vFLegend.Clear();
	vFPrimaryY.Clear();
	vFPattern.Clear();
	
	Refresh();
}

Drawing Scatter::GetDrawing() const
{
	DrawingDraw ddw(6*GetSize());
	SetDrawing (ddw, 6);
	return ddw;
}

Image Scatter::GetImage(const int& scale)
{
	bool aa=antialiasing;
	antialiasing=false;
	ImageDraw idraw(scale*GetSize());
	SetDrawing (idraw,scale);
	antialiasing=aa;
	return idraw;
}

#ifdef PLATFORM_WIN32

void Scatter::SaveAsMetafile(const char* file) const
{
	GuiLock __;
	WinMetaFileDraw wmfd;	
	wmfd.Create(6*GetSize().cx,6*GetSize().cy,"Scatter","chart",file);
	SetDrawing (wmfd, 6);	
	wmfd.Close();	
}

void Scatter::SaveToClipboard(bool saveAsMetafile) 
{
	GuiLock __;
	if (saveAsMetafile) {
		WinMetaFileDraw wmfd;	
		wmfd.Create(6*GetSize().cx,6*GetSize().cy,"Scatter","chart");
		SetDrawing (wmfd, 6);	
		WinMetaFile wmf = wmfd.Close();	 
		wmf.WriteClipboard();
	} else {
		Image img = GetImage(3);
		WriteClipboardImage(img);	
	}
}
#else

void Scatter::SaveToClipboard(bool) 
{
	GuiLock __;
	Image img = GetImage(3);
	WriteClipboardImage(img);
}

#endif

double Scatter::GetXByPoint(const int x) 
{
	return (x-GetH_Border())*GetXRange()/(GetSize().cx-2*GetH_Border()-1)+GetXMin();		
}

double Scatter::GetYByPoint(const int y) 
{
	return (GetSize().cy-GetV_Border()-y-1)*GetYRange()/(GetSize().cy-2*GetV_Border()-GetTitleFont().GetHeight()-1)+GetYMin();
}

double Scatter::GetY2ByPoint(const int y) 
{
	return (GetSize().cy-GetV_Border()-y-1)*GetY2Range()/(GetSize().cy-2*GetV_Border()-GetTitleFont().GetHeight()-1)+GetYMin2();
}

double Scatter::GetXPointByValue(const double x) 
{
	return (x-GetXMin())/GetXRange()*(GetSize().cx-2*GetH_Border()-1)+GetH_Border();
}

double Scatter::GetYPointByValue(const double y) 
{
	return (GetSize().cy-GetV_Border()-1)-(y-GetYMin())/GetYRange()*(GetSize().cy-2*GetV_Border()-GetTitleFont().GetHeight()-1);
}

void Scatter::Paint(Draw& w) 
{
	SetDrawing(w,1);
}

Scatter &Scatter::ShowInfo(bool show)
{
	paintInfo=show;
	return *this;
}

void Scatter::ProcessPopUp(const Point & pt)
{
	double _x= (popLT.x-px)*xRange/(GetSize().cx-2*px-1)+xMin;		
	double _y= (GetSize().cy-py-popLT.y-1)*yRange/(GetSize().cy-2*py-titleFont.GetHeight()-1)+yMin;
	double _y2=(GetSize().cy-py-popLT.y-1)*yRange2/(GetSize().cy-2*py-titleFont.GetHeight()-1)+yMin2;
	double x= (pt.x-px)*xRange/(GetSize().cx-2*px-1)+xMin;		
	double y= (GetSize().cy-py-pt.y-1)*yRange/(GetSize().cy-2*py-titleFont.GetHeight()-1)+yMin;
	double y2=(GetSize().cy-py-pt.y-1)*yRange2/(GetSize().cy-2*py-titleFont.GetHeight()-1)+yMin2;
	
	double dx  = x  - _x;
	double dy  = y  - _y;
	double dy2 = y2 - _y2;
	if(logX) {
		x  = pow(10.0, x);
		_x = pow(10.0, _x);
		dx = pow(10.0, dx);
	}
	if(logY) {
		y  = pow(10.0, y);
		_y = pow(10.0, _y);
		dy = pow(10.0, dy);
	}
	if(logY2) {
		y2  = pow(10.0, y2);
		_y2 = pow(10.0, _y2);
		dy2 = pow(10.0, dy2);
	}
	String strx, _strx, dstrx, stry, _stry, dstry;
	if (cbModifFormatX) {
		cbModifFormatX(strx,  0, x); 		strx.Replace("\n", " ");
		cbModifFormatX(_strx, 0, _x); 		_strx.Replace("\n", " ");
	} else {
		strx  = VariableFormatX(x);
		_strx = VariableFormatX(_x);
	}
	if (cbModifFormatDeltaX) {
		cbModifFormatDeltaX(dstrx, 0, dx);	dstrx.Replace("\n", " "); 
	} else {
		dstrx = VariableFormatX(dx);
	}	
	if (cbModifFormatY) {
		cbModifFormatY(stry,  0, y);		stry.Replace("\n", " ");
		cbModifFormatY(_stry, 0, _y);		_stry.Replace("\n", " ");
	} else {
		stry  = VariableFormatY(y);
		_stry = VariableFormatY(_y);
	}
	if (cbModifFormatDeltaY) {
		cbModifFormatDeltaY(dstry, 0, dy);	dstry.Replace("\n", " ");
	} else {
		dstry = VariableFormatY(dy);
	}
	String str= popTextX + ": " + _strx;
	if (strx != _strx)
		str << "; " + popTextX + "': " + strx + "; Δ" + popTextX + ": " + dstrx;
	str << "\n" + popTextY + ": " + _stry;
	if (stry != _stry)	
 		str << "; " + popTextY + "': " + stry + "; Δ" + popTextY + ": " + dstry;
	if (drawY2Reticle) {
		String stry2, _stry2, dstry2;
		if (cbModifFormatY2) {
			cbModifFormatY2(stry2,  0, y2);			stry2.Replace("\n", " ");
			cbModifFormatY2(_stry2, 0, _y2);		_stry2.Replace("\n", " ");
		} else {
			stry2  = VariableFormatY2(y2);
			_stry2 = VariableFormatY2(_y2);
		}
		if (cbModifFormatDeltaY2) {
			cbModifFormatDeltaY2(dstry2, 0, dy2);	dstry2.Replace("\n", " ");
		} else {
			dstry2 = VariableFormatY(dy2);
		}
		str << "\n" + popTextY2 + ": " + _stry2;
		if (stry2 != _stry2)		
			str << "; " + popTextY2 + ": " + stry2 + "; Δ" + popTextY2 + ": " + dstry2;
	}
	const Point p2 = pt+offset;
	popText.SetText(str).Move(this,p2.x,p2.y);
}

void Scatter::DoMouseAction(bool down, Point pt, MouseAction action, int value)
{
	switch (action) {
	case SCROLL: 		Scrolling(down, pt);
						break;
	case ZOOM_H_ENL:	 
	case ZOOM_H_RED:	MouseZoom(value, true, false); 
						break;
	case ZOOM_V_ENL:
	case ZOOM_V_RED:	MouseZoom(value, false, true); 
						break;
	case SHOW_INFO:		LabelPopUp(down, pt);
						break;
	case CONTEXT_MENU:	if(showContextMenu)
							MenuBar::Execute(THISBACK(ContextMenu));
						break;
	}
}

bool Scatter::SetMouseBehavior(MouseBehaviour *_mouseBehavior) 
{
	if (!_mouseBehavior)
		return false;
	int i;
	for (i = 0; _mouseBehavior[i].action != NO_ACTION && i < MAX_MOUSEBEHAVIOR; ++i) ;
	if (i == MAX_MOUSEBEHAVIOR)
		return false;
	mouseBehavior = _mouseBehavior;
	return true;
}

void Scatter::ProcessMouse(bool down, Point &pt, bool ctrl, bool alt, bool shift, bool left, bool middle, int middleWheel, bool right) 
{
	for (int i = 0; mouseBehavior[i].action != NO_ACTION && i < MAX_MOUSEBEHAVIOR; ++i) {
		if (mouseBehavior[i].ctrl   == ctrl   && mouseBehavior[i].alt   == alt   &&
		    mouseBehavior[i].shift  == shift  && mouseBehavior[i].left  == left  &&
		    mouseBehavior[i].middle == middle && mouseBehavior[i].right == right &&
		    ((mouseBehavior[i].middleWheel == 0) || mouseBehavior[i].middleWheel == ((middleWheel > 0) - (middleWheel < 0))))
		    DoMouseAction(down, pt, mouseBehavior[i].action, middleWheel);
	}	
}

void Scatter::LabelPopUp(bool down, Point &pt) 
{
	if (down) {
		if(paintInfo && px <=pt.x && pt.x<= GetSize().cx-px && (py + titleFont.GetHeight())<=pt.y && pt.y<= GetSize().cy-py)
		{
			popText.AppearOnly(this);
			
			isLabelPopUp = true;
			if (IsNull(popLT))
				popLT = pt;
			popRB = pt;
			ProcessPopUp(pt);		
		}	
	} else {
		if(paintInfo && isLabelPopUp) 
		{
			popText.Close();
			isLabelPopUp = false;
			popLT = popRB = Null;
			Refresh();
		}		
	}
}

#ifdef PLATFORM_LINUX
	#include <X11/cursorfont.h>
#endif

void Scatter::Scrolling(bool down, Point &pt, bool isOut)
{
	static Image mouseImg;
	if (down) {
		if((mouseHandlingX || mouseHandlingY) && px <=pt.x && pt.x<= GetSize().cx-px && (py + titleFont.GetHeight())<=pt.y && pt.y<= GetSize().cy-py)
		{
			butDownX = pt.x;
			butDownY = pt.y;	
			isScrolling = true;
			INTERLOCKED { 
#ifdef PLATFORM_WIN32
				static Image img = Win32Cursor(IDC_SIZEALL);
#else
				static Image img = X11Cursor(XC_fleur);
#endif
				mouseImg = Ctrl::OverrideCursor(img);
			}
		}
	} else {
		if (isScrolling) {
			if (!isOut)
				MouseMove(pt, 0);
			isScrolling = false;
			Ctrl::OverrideCursor(mouseImg);
		}
	}
}

void Scatter::LeftDown(Point pt, dword keyFlags) 
{
	ProcessMouse(true, pt, keyFlags & K_CTRL, keyFlags & K_ALT, keyFlags & K_SHIFT, true, false, 0, false);
}

void Scatter::LeftUp(Point pt, dword keyFlags)
{
	ProcessMouse(false, pt, keyFlags & K_CTRL, keyFlags & K_ALT, keyFlags & K_SHIFT, true, false, 0, false); 
}

void Scatter::MiddleDown(Point pt, dword keyFlags)
{
	ProcessMouse(true, pt, keyFlags & K_CTRL, keyFlags & K_ALT, keyFlags & K_SHIFT, false, true, 0, false);
}

void Scatter::MiddleUp(Point pt, dword keyFlags)
{
	ProcessMouse(false, pt, keyFlags & K_CTRL, keyFlags & K_ALT, keyFlags & K_SHIFT, false, true, 0, false);
}

void Scatter::RightDown(Point pt, dword keyFlags) 
{
	ProcessMouse(true, pt, keyFlags & K_CTRL, keyFlags & K_ALT, keyFlags & K_SHIFT, false, false, 0, true);
}

void Scatter::RightUp(Point pt, dword keyFlags)
{
	ProcessMouse(false, pt, keyFlags & K_CTRL, keyFlags & K_ALT, keyFlags & K_SHIFT, false, false, 0, true); 
}

void Scatter::MouseWheel(Point pt, int zdelta, dword keyFlags) 
{
	if (zdelta == 0)
		return;
	ProcessMouse(true, pt, keyFlags & K_CTRL, keyFlags & K_ALT, keyFlags & K_SHIFT, false, false, zdelta, false);
}

void Scatter::MouseMove(Point pt, dword)
{
	if (isScrolling) 
	{
		int shiftX = pt.x - butDownX;
		if (mouseHandlingX && shiftX != 0) {
			double deltaX = shiftX*xRange/(GetSize().cx - 2*px - 1);
			xMin -= deltaX;
			xMinUnit += deltaX;
			AdjustMinUnitX();
			butDownX = pt.x;
		}
		int shiftY = pt.y - butDownY;
		if (mouseHandlingY && shiftY != 0) {
			double deltaY = -shiftY*yRange/(GetSize().cy - 2*py - 1);
			yMin -= deltaY;
			yMinUnit += deltaY;
			AdjustMinUnitY();
			if (drawY2Reticle) {
				double deltaY2 = -shiftY*yRange2/(GetSize().cy - 2*py - 1);
				yMin2 -= deltaY2;
				yMinUnit2 += deltaY2;
				AdjustMinUnitY2();
			}
			butDownY = pt.y;
		}		
		if ((mouseHandlingX && shiftX != 0) || (mouseHandlingY && shiftY != 0)) {
			WhenSetXYMin();
			Refresh();
			WhenZoomScroll();
		}
	} 
	if(isLabelPopUp) {
		if (paintInfo && px <=pt.x && pt.x<= GetSize().cx-px && (py + titleFont.GetHeight())<=pt.y && pt.y<= GetSize().cy-py) 
		{
			if (IsNull(popLT))
				popLT = pt;
			popRB = pt;
			popText.AppearOnlyOpen(this);
			
			ProcessPopUp(pt);
			Refresh();
		}
	}	
}

void Scatter::MouseLeave()
{
	Point p = Null;
	Scrolling(false, p, true);
}

void Scatter::MouseZoom(int zdelta, bool hor, bool ver) 
{
	double scale = zdelta > 0 ? zdelta/100. : -100./zdelta;
	Zoom(scale, mouseHandlingX, mouseHandlingY);
}

void Scatter::Zoom(double scale, bool mouseX, bool mouseY) 
{
	mouseX = mouseX && ((minXZoom > 0 && xRange*scale > minXZoom) || (minXZoom < 0));
	mouseX = mouseX && ((maxXZoom > 0 && xRange*scale < maxXZoom) || (maxXZoom < 0));
	mouseY = mouseY && ((minYZoom > 0 && yRange*scale > minYZoom) || (minYZoom < 0));
	mouseY = mouseY && ((maxYZoom > 0 && yRange*scale < maxYZoom) || (maxYZoom < 0));
	//mouseX = mouseX && (!mouseHandlingY || mouseY);	
	//mouseY = mouseY && (!mouseHandlingX || mouseX);
	if (mouseX)
	{
		double oldXMin = xMin;
		xMin += xRange*(1-scale)/2.;
		xMinUnit = oldXMin + xMinUnit - xMin;
		AdjustMinUnitX();
		xRange *= scale;
	}
	if (mouseY)
	{
		yRange *= scale;
		yRange2 *= scale;		
	}
	if (mouseX || mouseY) {
		WhenSetRange();
		Refresh();
		WhenZoomScroll();
	}
}

void Scatter::Scroll(double factorX, double factorY)
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

Image Scatter::CursorImage(Point p, dword keyflags)
{
	return ChartImg::cursor1();
}

void Scatter::Circle(Draw& w, const int& scale, const Point& cp, const int& size, const class::Color& markColor)const
{
	int radius = fround(scale*size/6);		int radius2 = radius/2;
	w.DrawEllipse(cp.x-radius2, cp.y-radius2, radius, radius, markColor, 1, markColor);
}

void Scatter::Square(Draw& w, const int& scale, const Point& cp, const int& size, const class::Color& markColor)const
{
	Vector <Point> p;
	p<<Point(cp.x-int(size*scale/12+0.5),cp.y-int(size*scale/12+0.5))<<Point(cp.x+int(size*scale/12+0.5),cp.y-int(size*scale/12+0.5))
	<<Point(cp.x+int(size*scale/12+0.5),cp.y+int(size*scale/12+0.5))<<Point(cp.x-int(size*scale/12+0.5),cp.y+int(size*scale/12+0.5))
	<<Point(cp.x-int(size*scale/12+0.5),cp.y-int(size*scale/12+0.5));
	w.DrawPolyline(p,scale/2,markColor); 
	
}
void Scatter::Rectangle(Draw& w, const int& scale, const Point& cp, const int& size, const class::Color& markColor)const
{
	w.DrawRect(cp.x-int(size*scale/12+0.5),cp.y-int(size*scale/12+0.5),int(size*scale/6+1.5),int(size*scale/6+1.5),markColor);
}
void Scatter::Triangle(Draw& w, const int& scale, const Point& cp, const int& size, const class::Color& markColor)const
{
	Vector <Point> p;
	p<<Point(cp.x,cp.y-int(size*scale/12+0.5))<<Point(cp.x+int(size*scale/12+0.5),cp.y+int(0.6*size*scale/12+0.5))
	<<Point(cp.x-int(size*scale/12+0.5),cp.y+int(0.6*size*scale/12+0.5))
	<<Point(cp.x,cp.y-int(size*scale/12+0.5));
	w.DrawPolyline(p,scale/2,markColor); 
	
}
void Scatter::Cross(Draw& w, const int& scale, const Point& cp, const int& size, const class::Color& markColor)const
{
	w.DrawLine(cp.x-int(size*scale/12+0.5),cp.y,cp.x+int(size*scale/12+1.5),cp.y,scale,markColor);
	w.DrawLine(cp.x,cp.y-int(size*scale/12+0.5),cp.x,cp.y+int(size*scale/12+1.5),scale,markColor);
}
void Scatter::XFunct(Draw& w, const int& scale, const Point& cp, const int& size, const class::Color& markColor)const
{
	w.DrawLine(cp.x-int(size*scale/12+0.5),cp.y-int(size*scale/12+0.5),cp.x+int(size*scale/12+1.5),cp.y+int(size*scale/12+1.5),scale,markColor);
	w.DrawLine(cp.x+int(size*scale/12+0.5),cp.y-int(size*scale/12+0.5),cp.x-int(size*scale/12+1.5),cp.y+int(size*scale/12+1.5),scale,markColor);
}
void Scatter::Rhomb(Draw& w, const int& scale, const Point& cp, const int& size, const class::Color& markColor)const
{
	Vector <Point> p;
	p<<Point(cp.x,cp.y-int(size*scale/12+0.5))<<Point(cp.x+int(size*scale/12+0.5),cp.y)
	<<Point(cp.x,cp.y+int(size*scale/12+0.5))<<Point(cp.x-int(size*scale/12+0.5),cp.y)
	<<Point(cp.x,cp.y-int(size*scale/12+0.5));
	w.DrawPolyline(p,scale,markColor); 
}

inline void Scatter::DrawMark(const int& style, Draw& w, const int& scale, const Point& cp, const int& size, const class::Color& markColor)const
{
	void (Scatter::*ptMark) (Draw&, const int&, const Point&, const int&, const class::Color&)const=NULL;
	switch (style)
	{
		case CIRCLE :ptMark=&Scatter::Circle;		break;
		case RECTANGLE :ptMark=&Scatter::Rectangle;	break;
		case SQUARE : ptMark=&Scatter::Square;		break;
		case TRIANGLE : ptMark=&Scatter::Triangle;	break;
		case CROSS : ptMark=&Scatter::Cross;		break;
		case X : ptMark=&Scatter::XFunct;			break;
		case RHOMB:ptMark=&Scatter::Rhomb;			break;
		default : ptMark=&Scatter::Circle;			break;
	}
	
	(*this.*ptMark)(w,scale,cp,size,markColor);	
}

Scatter &Scatter::SetMouseHandling(bool valx, bool valy) 
{
	mouseHandlingX = valx;
	mouseHandlingY = valy;
	return *this;
}

void Scatter::ContextMenu(Bar& bar)
{
	if (mouseHandlingX || mouseHandlingY) {
		bar.Add(t_("Fit to data"), ChartImg::ShapeHandles(), THISBACK1(FitToData, mouseHandlingY));
		bar.Add(t_("Zoom +"), ChartImg::ZoomPlus(), THISBACK3(Zoom, 1/1.2, true, mouseHandlingY));
		bar.Add(t_("Zoom -"), ChartImg::ZoomMinus(), THISBACK3(Zoom, 1.2, true, mouseHandlingY));
		bar.Add(t_("Scroll Left"), ChartImg::LeftArrow(), THISBACK2(Scroll, -0.2, 0));
		bar.Add(t_("Scroll Right"), ChartImg::RightArrow(), THISBACK2(Scroll, 0.2, 0));
		if (mouseHandlingY) {
			bar.Add(t_("Scroll Up"), ChartImg::UpArrow(), THISBACK2(Scroll, 0, -0.2));
			bar.Add(t_("Scroll Down"), ChartImg::DownArrow(), THISBACK2(Scroll, 0, 0.2));			
		}
		bar.Separator();
	}
	bar.Add(t_("Copy"), ChartImg::Copy(), THISBACK1(SaveToClipboard, false)).Key(K_CTRL_C);
	bar.Add(t_("Save to file"), ChartImg::Save(), THISBACK1(SaveToImage, Null));
}

void Scatter::SaveToImage(String fileName)
{
	GuiLock __;
	if (IsNull(fileName)) {
		FileSel fs;
		
		fs.Type("PNG file", "*.png");
		fs.Type("JPEG file", "*.jpg");
	    if(!fs.ExecuteSaveAs(t_("Saving plot to PNG or JPEG file"))) {
	        Exclamation(t_("Plot has not been saved"));
	        return;
	    }
        fileName = fs;
	}
	if (GetFileExt(fileName) == ".png") {
		PNGEncoder encoder;
		encoder.SaveFile(fileName, GetImage(3));
	} else if (GetFileExt(fileName) == ".jpg") {	
		JPGEncoder encoder(90);
		encoder.SaveFile(fileName, GetImage(3));		
	} else
		Exclamation(Format(t_("File format %s not found"), GetFileExt(fileName)));
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

double Dist(const Pointf &p0, const Pointf &p1) 
{
	return sqrt(sqr(p0.x-p1.x) + sqr(p0.y-p1.y));
}

inline bool Even(int val)	  	{return !(val%2);}

Vector <double> GetLinePattern(String pattern) 
{
	Vector <double> ret;
	
	ret.Add();
	int retPos = 0;
	ret[0] = 2;	
	for (int i = 1; i < pattern.GetCount(); ++i) {
		if (pattern[i] != pattern[i-1]) {
			ret.Add();
			retPos++;
			ret[retPos] = 0;
		}
		ret[retPos] += 2;
	}
	return ret;
}

Vector <double> &GetPatternArray(String pattern) 
{
	static VectorMap <String, Vector <double> > pats;
	
	int pos = pats.Find(pattern);
	if (pos < 0) {
		pats.Add(pattern, GetLinePattern(pattern));
		pos = pats.GetCount()-1;
	} 
	return pats.GetValues()[pos];
}

void Scatter::DrawLineX(Draw& w, const int x0, const int y0, const int x1, const int y1, int thick, const class::Color &color, String pattern, const int &scale) 
{
	Vector<Point> p;
	p.SetCount(2);       
	p[0].x = x0;
	p[0].y = y0;
	p[1].x = x1;
	p[1].y = y1;
	DrawPolylineX(w, p, thick, color, pattern, scale);
}

void Scatter::DrawPolylineX(Draw& w, const Vector<Point> &p, int thick, const class::Color &color, String pattern, const int &scale) 
{
	if (pattern == LINE_SOLID) 
		w.DrawPolyline(p, thick, color);
	else {
		Vector <double> &pat = GetPatternArray(pattern);
		int iPat = 0; 
		
		double len = pat[0]*scale;			// Pixels por barra
		Pointf begin, end;
		begin = p[0];
		for (int i = 1; i < p.GetCount();) {
			double d = Dist(begin, p[i]);
			if (d >= len) 
				end = PointAtLen(begin, p[i], len);
			else {
				end = p[i];
				len -= d;
				++i;
			}
			if (Even(iPat)) 
				w.DrawLine(begin, end, thick, color);
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

void Scatter::Plot(Draw& w, const int& scale,const int& l,const int& h)const
{
	double d1=xRange/xMajorUnit;
	double d2=yRange/yMajorUnit;

	w.DrawRect(1,1,l-2,h-1,plotAreaColor);	//grosimea liniei nu este scalata
	int gW=fround(gridWidth*scale/6);
	if(gridWidth<0) 
		gW=gridWidth;   
	Vector<Point> p;
	p.SetCount(2);       
	if (drawVGrid) 
		for(int i=0; xMinUnit+i*xMajorUnit < xRange;i++) {
			p[0].x = fround(l*xMinUnit/xRange+i*l/d1);		p[0].y = 0;
			p[1].x = fround(l*xMinUnit/xRange+i*l/d1);		p[1].y = h;
			DrawPolylineX(w, p, gW, gridColor, "oo..", scale);
		}
		
	if (drawHGrid)
		for(int i=0; yMinUnit+i*yMajorUnit < yRange;i++) {
			p[0].x = 0;		p[0].y = fround(-h*yMinUnit/yRange + h-i*h/d2);
			p[1].x = l;		p[1].y = fround(-h*yMinUnit/yRange + h-i*h/d2);
			DrawPolylineX(w, p, gW, gridColor, "oo..", scale);
		}
	w.DrawLine(0, h, l, h, scale, Black);
	w.DrawLine(0, 0, l, 0, scale, Black);
	w.DrawLine(0, 0, 0, h, scale, Black);
	w.DrawLine(l, 0, l, h+1, scale, Black);
	w.Clip(Rect(0,0,l,h));
	int ix;//int x points coordinates
	int iy;//int y points coordinates
	if (!vPointsData.IsEmpty()){
		for (int j=0; j<vPointsData.GetCount(); j++){
			Vector<Point> p1;
			int imin, imax;
			if (vPSequential[j]) {
				imin = imax = Null;
				for (int i = 1; i<vPointsData[j].GetCount()-1; ++i) {
					if (IsNull(imin)) {
						if (vPointsData[j][i].x >= xMin)
							imin = i-1;
					} else if (IsNull(imax)) {
						if (vPointsData[j][i].x >= xMin+xRange)
							imax = i+1;
					}
				}
				if (IsNull(imin))
				    imin = 0;
				if (IsNull(imax))
				    imax = vPointsData[j].GetCount();
			} else {
			    imin = 0;
			    imax = vPointsData[j].GetCount();
			}
			int numV;
			if (fastViewX)
				numV = 1+(imax-imin)/l;
			else
				numV = 1;
			for (int i=imin; i<imax; i+=numV) {
				double xx, yy;
				if (fastViewX && numV > 1) {
					yy = 0;
					int ii;
					for (ii = 0; ii < numV && i+ii < imax; ++ii) 
						yy += vPointsData[j][i+ii].y;
					yy /= double(ii);
					xx = (vPointsData[j][i].x + vPointsData[j][i+ii-1].x)/2;
				} else {
					xx = vPointsData[j][i].x;
					yy = vPointsData[j][i].y;
				}
				ix=fround(l*(xx-xMin)/xRange);
				if (vPPrimaryY[j])
					iy=fround(h*(yy-yMin)/yRange);
				else
					iy=fround(h*(yy-yMin2)/yRange2);
				p1<<Point(ix,h-iy);
			}
			if(vJoin[j])
			{
				if(vSmooth[j]&&vPointsData[j].GetCount()>2)
				{
					Vector<Point> p2;
					Vector<XY> v(Cubic(vPointsData[j]));         
					for (int i=0; i<v.GetCount(); i++)
					{
						ix=fround(l*(v[i].x-xMin)/xRange);
						if (vPPrimaryY[j])
							iy=fround(h*(v[i].y-yMin)/yRange);
						else
							iy=fround(h*(v[i].y-yMin2)/yRange2);
						p2<<Point(ix,h-iy);
					}
					if(!p2.IsEmpty()) 
						DrawPolylineX(w, p2,fround(scale*vPThickness[j]/6),vPColors[j],vPPattern[j], scale);
				} else if (!p1.IsEmpty()) 
					DrawPolylineX(w, p1,fround(scale*vPThickness[j]/6),vPColors[j],vPPattern[j], scale);
			}
				
			if(vShowMark[j])
				if (vPWidth[j] >= 1) {
					for (int i=0; i<(imax-imin)/numV; i++) 
						DrawMark(vMarkStyles[j],w,scale,p1[i],vPWidth[j],vMarkColors[j]);              
				}
		}
	}
	
	int nf=0; //number of functions to draw

	for (int j=0; j<vFunctionData.GetCount(); j++){
		if(!vFunctionData[j].IsEmpty())
		{
			Vector<Point> p1;
			for (int i=0; i<vFunctionData[j].GetCount(); i++){
				ix=fround(l*(vFunctionData[j][i].x-xMin)/xRange);
				if (vFPrimaryY[j])
					iy=fround(h*(vFunctionData[j][i].y-yMin)/yRange);				
				else
					iy=fround(h*(vFunctionData[j][i].y-yMin2)/yRange2);				
				p1<<Point(ix,h-iy);				                        
			}
			DrawPolylineX(w, p1,fround(scale*vFThickness[j]/6),vFColors[j],vFPattern[j], scale);
		}
		else
		{
			Vector<Point> p1;
			for(int i=0; i<l; i++)
			{
				double x=xMin+i*(xRange/l);
				double y;
				if (vFPrimaryY[j])
				{
					vAdress[nf](y, x);
					iy = fround(h*(y-yMin)/yRange);
				}
				else
				{
					vAdress[nf](y, x);
					iy = fround(h*(y-yMin2)/yRange2);
				}
				p1<<Point(i,h-iy);
			}
			DrawPolylineX(w, p1,fround(scale*vFThickness[j]/6),vFColors[j],vFPattern[j], scale);
			nf++;
		}
	}
	if (!IsNull(popLT) && popLT != popRB) {
		int x0 = px;
		int y0 = py + titleFont.GetHeight();
		int rfrom = min(popLT.y-y0, popRB.y-y0);
		int rto   = max(popLT.y-y0, popRB.y-y0);
		for (int r = rfrom+scale*5; r < rto; r += scale*5)
			DrawLineX(w, popLT.x-x0, r, popRB.x-x0, r, scale, SColorHighlight, "o..", scale);
		DrawLineX(w, 1, 		 popLT.y-y0, l, 		 popLT.y-y0, scale, SColorHighlight, "o.", scale);
		DrawLineX(w, 1, 	   	 popRB.y-y0, l, 	   	 popRB.y-y0, scale, SColorHighlight, "o.", scale);
		DrawLineX(w, popLT.x-x0, 1, 	   	 popLT.x-x0, h, 	   	 scale, SColorHighlight, "o.", scale);
		DrawLineX(w, popRB.x-x0, 1, 	   	 popRB.x-x0, h, 	     scale, SColorHighlight, "o.", scale);
	}
	w.End();
}

Vector<XY> Scatter::Cubic(const Vector<XY>& DataSet, const int& fineness,double tension) const
{
	Vector<XY> OutSet;
	if(DataSet.GetCount()>2) {   
		OutSet<<DataSet[0];
		double t;
		double a0,a1,a2,a3;
		double b0,b1,b2,b3;
		a2=tension*(DataSet[1].y-DataSet[0].y); 
		a0=a2+tension*(DataSet[2].y-DataSet[0].y)+2*DataSet[0].y-2*DataSet[1].y;
		a1=-2*a2- tension*(DataSet[2].y-DataSet[0].y)-3*DataSet[0].y+3*DataSet[1].y;        
		a3=DataSet[0].y;
		
		b2=tension*(DataSet[1].x-DataSet[0].x);
		b0=b2+tension*(DataSet[2].x-DataSet[0].x)+2*DataSet[0].x-2*DataSet[1].x;
		b1=-2*b2- tension*(DataSet[2].x-DataSet[0].x)-3*DataSet[0].x+3*DataSet[1].x;    
		b3=DataSet[0].x;
		
		for (int n=1; n<=fineness; n++)
		{
			t=(double)n/fineness;
			OutSet<<XY(b0*t*t*t+b1*t*t+b2*t+b3, a0*t*t*t+a1*t*t+a2*t+a3);
		}	
		for (int i=3; i<DataSet.GetCount(); i++)
		{
			a2=tension*(DataSet[i-1].y-DataSet[i-3].y);
			a0=a2+tension*(DataSet[i].y-DataSet[i-2].y)+2*DataSet[i-2].y-2*DataSet[i-1].y;
			a1=-2*a2- tension*(DataSet[i].y-DataSet[i-2].y)-3*DataSet[i-2].y+3*DataSet[i-1].y;      
			a3=DataSet[i-2].y;
			
			b2=tension*(DataSet[i-1].x-DataSet[i-3].x);
			b0=b2+tension*(DataSet[i].x-DataSet[i-2].x)+2*DataSet[i-2].x-2*DataSet[i-1].x;
			b1=-2*b2- tension*(DataSet[i].x-DataSet[i-2].x)-3*DataSet[i-2].x+3*DataSet[i-1].x;      
			b3=DataSet[i-2].x;
			
			for (int n=1; n<=fineness; n++)
			{
				t=(double)n/fineness;
				OutSet<<XY(b0*t*t*t+b1*t*t+b2*t+b3, a0*t*t*t+a1*t*t+a2*t+a3);
			}
		}
		int nd=DataSet.GetCount()-1;
		a2=tension*(DataSet[nd].y-DataSet[nd-2].y); 
		a0=a2+tension*(DataSet[nd].y-DataSet[nd-1].y)+2*DataSet[nd-1].y-2*DataSet[nd].y;
		a1=-2*a2- tension*(DataSet[nd].y-DataSet[nd-1].y)-3*DataSet[nd-1].y+3*DataSet[nd].y;        
		a3=DataSet[nd-1].y;
			
		b2=tension*(DataSet[nd].x-DataSet[nd-2].x);
		b0=b2+tension*(DataSet[nd].x-DataSet[nd-1].x)+2*DataSet[nd-1].x-2*DataSet[nd].x;
		b1=-2*b2- tension*(DataSet[nd].x-DataSet[nd-1].x)-3*DataSet[nd-1].x+3*DataSet[nd].x;        
		b3=DataSet[nd-1].x;
		
		for (int n=1; n<=fineness; n++)
		{
			t=(double)n/fineness;
			OutSet<<XY(b0*t*t*t+b1*t*t+b2*t+b3, a0*t*t*t+a1*t*t+a2*t+a3);
		}
	}
	return Vector<XY>(OutSet);
}

void ParseTextMultiline(const String &text, Font fnt, Array <String> &texts, Array <Size> &sizes) {
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
			
void Scatter::SetDrawing(Draw& w, const int& scale) const
{
	GuiLock __;
	w.DrawRect(scale*GetSize(),graphColor);
	Size sz;
	sz.cx=0;
	sz.cy=0;    
	if(!title.IsEmpty()) 
	{
		Font FontTitle6;
		FontTitle6=titleFont;
		FontTitle6.Height(scale*titleFont.GetHeight());
		FontTitle6.Width(scale*titleFont.GetWidth());
		sz= GetTextSize(title, FontTitle6);
		w.DrawText((scale*GetSize().cx-sz.cx)/2,scale*2,title,FontTitle6,titleColor);   
	}
	
	w.Offset(Point(scale*px,scale*py+scale*titleFont.GetHeight()));
	if(showLegend) 
		DrawLegend(w,scale);
	int l=scale*GetSize().cx-2*scale*px;
	int h=scale*GetSize().cy-2*scale*py-scale*titleFont.GetHeight();

	Font FontLabel6;
	FontLabel6=labelsFont;
	FontLabel6.Height(scale*labelsFont.GetHeight());
	
	Size lx=GetTextSize(xLabel,FontLabel6);
	Size ly=GetTextSize(yLabel,FontLabel6);
	Size ly2=GetTextSize(yLabel2,FontLabel6);
	w.DrawText(fround((l-lx.cx)/2),h+scale*py-lx.cy-scale*2,xLabel,FontLabel6,labelsColor);
	w.DrawText(scale*2-scale*px,fround((h+ly.cx)/2),900,yLabel,FontLabel6,labelsColor);
	w.DrawText(fround(l+scale*px-ly2.cy-2*scale),fround((h+ly2.cx)/2),900,yLabel2,FontLabel6,labelsColor);
	
	if (drawXReticle)
		for(int i=0; xMinUnit+i*xMajorUnit<=xRange;i++){
			w.DrawLine(fround(l*xMinUnit/xRange+i*l/(xRange/xMajorUnit)),
					   h,   
					   fround(l*xMinUnit/xRange+i*l/(xRange/xMajorUnit)), 
					   h+scale*4, 
				fround(scale/2),
				axisColor);             
			Font Standard6;
			Standard6.Height(scale*StdFont().GetHeight());
			double gridX=xMinUnit+i*xMajorUnit+xMin;
			String gridLabelX;
			if (cbModifFormatX)
				cbModifFormatX(gridLabelX, i, gridX);
			else
				gridLabelX = VariableFormatX(gridX);
			
			Array <String> texts;
			Array <Size> sizes;
			ParseTextMultiline(gridLabelX, StdFont(), texts, sizes);
			for (int ii = 0; ii < texts.GetCount(); ++ii) {
				int cy = ii == 0 ? 0 : sizes[ii-1].cy;
				w.DrawText(fround(l*xMinUnit/xRange+i*l/(xRange/xMajorUnit) - scale*sizes[ii].cx/2), h+scale*(4+ii*cy), texts[ii], Standard6, axisColor);
			}
		}
	if (drawYReticle)
		for(int i=0; yMinUnit+i*yMajorUnit<=yRange;i++){
			w.DrawLine(-(scale*4),
				fround(-h*yMinUnit/yRange+h-i*h/(yRange/yMajorUnit)),
				0,
				fround(-h*yMinUnit/yRange+h-i*h/(yRange/yMajorUnit)),
				fround(scale/2),
				axisColor);
			double gridY=yMinUnit+i*yMajorUnit+yMin;
			String gridLabelY;
			if (cbModifFormatY)
				cbModifFormatY(gridLabelY, i, gridY);
			else
				gridLabelY = VariableFormatY(gridY);
			int dx=scale*GetTextSize(gridLabelY,StdFont()).cx;
			Font Standard6;
			Standard6.Height(scale*StdFont().GetHeight());  
			w.DrawText(-dx-scale*6, fround(-h*yMinUnit/yRange+h-i*h/(yRange/yMajorUnit))-scale*8, gridLabelY, Standard6, axisColor);
		}	
	if (drawY2Reticle)
		for(int i=0; yMinUnit+i*yMajorUnit<=yRange;i++){
			w.DrawLine(l+(scale*4),
				fround(-h*yMinUnit2/yRange2+h-i*h/(yRange/yMajorUnit)),
				l,
				fround(-h*yMinUnit2/yRange2+h-i*h/(yRange/yMajorUnit)),
				fround(scale/2),
				axisColor);
			double gridY2=yMinUnit2+i*yMajorUnit2+yMin2;
			String gridLabelY2;
			if (cbModifFormatY2)
				cbModifFormatY2(gridLabelY2, i, gridY2);
			else
				gridLabelY2 = VariableFormatY2(gridY2);
			Font Standard6;
			Standard6.Height(scale*StdFont().GetHeight());  
			w.DrawText(l+scale*10,fround(-h*yMinUnit2/yRange2+h-i*h/(yRange/yMajorUnit))-scale*8,gridLabelY2,Standard6,axisColor);
		}	
	
	if(antialiasing && w.IsGui())	// IsGui() is always false in Linux
	{
		ImageDraw imdraw(3*l,3*h);	
		Plot (imdraw,3,3*l,3*h);
		w.DrawImage(0,0,l,h,imdraw);
	} else 
		Plot(w,scale,l,h);		

	w.End();
}
	
Scatter::Scatter():
	title (""),
	titleColor(SColorText),
	graphColor(White),
	titleFont(Roman(20)),
	xLabel(""),yLabel(""),
	labelsFont(StdFont()),
	labelsColor(SColorText),
	plotAreaColor(SColorLtFace),
	axisColor(SColorText),
	axisWidth(6),
	px(30),
	py(30),
	xRange(100.0),
	yRange(100.0),
	yRange2(100.0),
	xMajorUnit(xRange/10),
	yMajorUnit(yRange/5),
	xMin(0.0), yMin(0.0), yMin2(0.0),
	xMinUnit(0.0), yMinUnit(0.0), yMinUnit2(0.0),
	logX(false), logY(false), logY2(false),
	cbModifFormatX(NULL),cbModifFormatY(NULL),cbModifFormatY2(NULL),
	cbModifFormatDeltaX(NULL),cbModifFormatDeltaY(NULL),cbModifFormatDeltaY2(NULL),
	gridColor(SColorDkShadow),
	gridWidth(4),
	paintInfo(false),
	mouseHandlingX(false), mouseHandlingY(false), isScrolling(false), isLabelPopUp(false),      
	drawXReticle(true), drawYReticle(true), drawY2Reticle(false),
	drawVGrid(true), drawHGrid(true),
	showLegend(true),legendWeight(80),
	antialiasing(false),
	offset(10,12),
	minXZoom(-1), maxXZoom(-1), minYZoom(-1), maxYZoom(-1), fastViewX(false), 
	sequentialXAll(false), popTextX("x"), popTextY("y1"), popTextY2("y2"), 
	popLT(Null), popRB(Null), showContextMenu(false)
{
	Color(graphColor);	
	BackPaint();
	popText.SetColor(SColorFace);        
	SetMouseBehavior(defaultMouse);
}

