#include "Scatter.h"

#include <Painter/Painter.h>

#define IMAGECLASS ChartImg
#define IMAGEFILE <Scatter/Chart.iml>
#include <Draw/iml.h>


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
	if(poz_x>=0) px=poz_x;
	return *this;
}
Scatter& Scatter::V_Border(const int& poz_y)
{
	if(poz_y>=0) py=poz_y;
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
void Scatter::SetDrawXReticle(bool set) 
{
	drawXReticle = set;
}
void Scatter::SetDrawYReticle(bool set)
{
	drawYReticle = set;
}
void Scatter::SetDrawY2Reticle(bool set)
{
	drawY2Reticle = set;
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
	Vector<int> MS;
	MS.Append(vMarkStyles);
	Vector<class::Color> MC;
	MC.Append(vMarkColors);
	
	int NMR=int((GetSize().cx-2*px)/legendWeight);//max number of labels per row
	if (NMR>0)
	{
		int N=L.GetCount();//number of labels
		int Nc;//number of complete rows
		int LCR;//number of labels on complete row
		int R;//number of remaining labels on incomplete row
		if(NMR>N)           {Nc=0;      LCR=0;      R=N;}
		else if (NMR==N)    {Nc=1;      LCR=N;      R=0;}
		else                {Nc=N/NMR;  LCR=NMR;    R=N%NMR;}
		for(int j=0;j<=Nc;j++)          
		{
			int start=N-(j+1)*LCR;
			int end=N-j*LCR;
			if (j==Nc) {start=0; end=R;}
			for(int i=start;i<end;i++)
			{
				w.DrawRect(scale*(i-start)*legendWeight,
								scale*(4-12*(j+1)),
								14*scale,
								scale*Thick.At(i)/6,
								LC.At(i));
				Point p(scale*((i-start)*legendWeight+7),scale*(4-12*(j+1))+scale*Thick.At(i)/12);
				DrawMark(MS.At(i),w,scale,p,LW.At(i,0),MC.At(i));                                           
				Font scaledFont;
				scaledFont.Height(scale*StdFont().GetHeight());
				w.DrawText(scale*(i-start)*legendWeight+scale*16,
							scale*(-2-12*(j+1)),
							L.At(i),
							scaledFont,LC.At(i));                   
			}
		}
	}
		
}	

void Scatter::SetRange(double rx, double ry, double ry2)
{
	xRange=rx;
	yRange=ry;
	yRange2=ry2;
	xMajorUnit=xRange/10;
	yMajorUnit=yRange/10;   
	yMajorUnit2=yRange2/10;   
}
void Scatter::SetMajorUnits(double ux, double uy) 
{
	if (ux > xRange)	throw (Exc(t_("Invalid X major units!")));
	if (uy > yRange)	throw (Exc(t_("Invalid Y major units!")));
	xMajorUnit=ux;
	yMajorUnit=uy;
	yMajorUnit2=yRange2*yMajorUnit/yRange;
}
void Scatter::SetMinUnits(double ux, double uy)
{
	xMinUnit=ux;
	yMinUnit=uy;
	yMinUnit2=yRange2*yMinUnit/yRange;
}
void Scatter::SetXYMin(double xmin, double ymin, double ymin2)
{
	xMin=xmin;
	yMin=ymin;
	yMin2=ymin2;
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

void Scatter::AddSeries(Vector<XY> & points,const String& legend,const bool& join, const class::Color& pcolor, const int& width, const int& thickness)
{
	vPointsData.AddPick(points);
	vJoin.Add(join);
	vSmooth.Add(false);
	vPColors.Add(pcolor);
	vPWidth.Add(width);
	vPThickness.Add(thickness);
	vLegend.Add(legend);
	vMarkColors.Add(pcolor);
	vShowMark.Add(true);
	vMarkStyles.Add(CIRCLE);	
	vPPrimaryY.Add(true);
	
	Refresh();
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
Acolor Scatter::GetDataColor(const int& j) const
{
	if(IsValid(j))
	{
		return vPColors[j];
	}	
	throw (Exc(t_("Invalid series index!")));
}

Acolor Scatter::GetFunctColor(const int& j) const
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

void Scatter::SetMarkColor(const int& j, const Acolor& mcolor)
{
	if(IsValid(j))
	{
		vMarkColors[j]=mcolor;
		Refresh();
	}
}

Acolor Scatter::GetMarkColor(const int& j) const
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
	if(!IsValid(j)) throw (Exc(t_("Invalid series index!")));
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
	if(!IsValid(j)) throw (Exc(t_("Invalid series index!")));
	
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
	if(!IsValid(j)) throw (Exc(t_("Invalid series index!")));
	
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

bool Scatter::IsDataPrimaryY(const int& j) const throw (Exc)
{
	if(!IsValid(j)) throw (Exc(t_("Invalid series index!")));
	
	return vPPrimaryY[j];	
}

void Scatter::SetFunctPrimaryY(const int& j, const bool& primary) 
{
	if(IsValid(j))
	{
		vFPrimaryY[j]=primary;
		Refresh();
	}
}

bool Scatter::IsFunctPrimaryY(const int& j) const throw (Exc)
{
	if(!IsValid(j)) throw (Exc(t_("Invalid series index!")));
	
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
	
	Refresh();
}


void Scatter::PlotFunction(double (*f)(double), const String& legend, const class::Color& fcolor, const int& weight)
{
	Vector<XY> series;
	vFunctionData.AddPick(series);
	
	vAdress.Add(f);
	
	vFColors.Add(fcolor);
	vFThickness.Add(weight);	
	vFLegend.Add(legend);
	vFPrimaryY.Add(true);
	Refresh();  
}
void Scatter::PlotParaFunction(XY (*pf)(double), const String& legend, const class::Color& fcolor, const int& weight,const int& Np)
{
	double t;//t must be choosed between [0,1]
	Vector<XY> series;
	for (int i=0; i<=Np;i++)
	{
		t=(double)i/Np;
		series<<pf(t);
	}
	vFColors.Add(fcolor);
	vFThickness.Add(weight);
	vFunctionData.AddPick(series);
	vFPrimaryY.Add(true);
	vFLegend.Add(legend);
	Refresh();
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
				if (vFunctionData[i].IsEmpty()) nf++;
			}
			vAdress.Remove(nf);
		}
				
		vFunctionData.Remove(j);
		vFColors.Remove(j);
		vFThickness.Remove(j);
		vFLegend.Remove(j);
		vFPrimaryY.Remove(j);
			
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
	WinMetaFileDraw wmfd;	
	wmfd.Create(6*GetSize().cx,6*GetSize().cy,"Scatter","chart",file);
	SetDrawing (wmfd, 6);	
	wmfd.Close();	
}

void Scatter::SaveToClipboard() 
{
	WinMetaFileDraw wmfd;	
	wmfd.Create(6*GetSize().cx,6*GetSize().cy,"Scatter","chart");
	SetDrawing (wmfd, 6);	
	WinMetaFile wmf = wmfd.Close();	
	wmf.WriteClipboard();
}
#else

void Scatter::SaveToClipboard() 
{
	Image img = GetImage(3);
	WriteClipboardImage(img);
}

#endif



void Scatter::Paint(Draw& w) 
{
	SetDrawing(w,1);
}

void Scatter::ShowInfo(bool show)
{
	paintInfo=show;
}

void Scatter::LeftDown(Point pt, dword)
{
	if(paintInfo && px <=pt.x && pt.x<= GetSize().cx-px && (py + titleFont.GetHeight())<=pt.y && pt.y<= GetSize().cy-py)
	{
		double x=(pt.x-px)*xRange/(GetSize().cx-2*px-1)+xMin;		
		double y=(GetSize().cy-py-pt.y-1)*yRange/(GetSize().cy-2*py-titleFont.GetHeight()-1)+yMin;
		double y2=(GetSize().cy-py-pt.y-1)*yRange2/(GetSize().cy-2*py-titleFont.GetHeight()-1)+yMin2;
		if(logX) x=pow(10.0, x);
		if(logY) y=pow(10.0, y);
		if(logY2) y2=pow(10.0, y2);
		String strx, stry;
		if (cbModifFormatX)
			cbModifFormatX(strx, 0, x); 
		else
			strx = VariableFormatX(x);
		if (cbModifFormatY)
			cbModifFormatY(stry, 0, y);
		else
			stry = VariableFormatY(y);
		String str="x: "+strx+"\ny: "+stry;
		if (drawY2Reticle) {
			String stry2;
			if (cbModifFormatY2)
				cbModifFormatY2(stry2, 0, y2);
			else
				stry2 = VariableFormatY2(y2);
			str << "\ny2: "+stry2;
		}
		const Point p2=pt+offset;
		popText.SetText(str).Appear(this,p2.x,p2.y);
	}	
}
void Scatter::LeftUp(Point, dword)
{
	if(paintInfo) popText.Close();
}

void Scatter::MiddleDown(Point pt, dword)
{
	if((mouseHandlingX || mouseHandlingY) && px <=pt.x && pt.x<= GetSize().cx-px && (py + titleFont.GetHeight())<=pt.y && pt.y<= GetSize().cy-py)
	{
		butDownX = pt.x;
		butDownY = pt.y;	
		isButDown = true;
	}
}
void Scatter::MouseMove(Point pt, dword)
{
	if (isButDown) {
		int shiftX = pt.x - butDownX;
		if (mouseHandlingX && shiftX != 0) {
			double deltaX = shiftX*xRange/(GetSize().cx - 2*px - 1);
			xMin -= deltaX;
			xMinUnit += deltaX;
			while (xMinUnit < 0)
				xMinUnit += xMajorUnit;	
			while (xMinUnit > xMajorUnit)
				xMinUnit -= xMajorUnit;
			Refresh();
			WhenZoomScroll();
			butDownX = pt.x;
		}
		int shiftY = pt.y - butDownY;
		if (mouseHandlingY && shiftY != 0) {
			double deltaY = -shiftY*yRange/(GetSize().cy - 2*py - 1);
			yMin -= deltaY;
			yMinUnit += deltaY;
			while (yMinUnit < 0)
				yMinUnit += yMajorUnit;	
			while (yMinUnit > yMajorUnit)
				yMinUnit -= yMajorUnit;
			if (drawY2Reticle) {
				double deltaY2 = -shiftY*yRange2/(GetSize().cy - 2*py - 1);
				yMin2 -= deltaY2;
				yMinUnit2 += deltaY2;
				while (yMinUnit2 < 0)
					yMinUnit2 += yMajorUnit2;	
				while (yMinUnit2 > yMajorUnit2)
					yMinUnit2 -= yMajorUnit2;
			}
			butDownY = pt.y;
		}		
		if ((mouseHandlingX && shiftX != 0) || (mouseHandlingY && shiftY != 0)) {
			Refresh();
			WhenZoomScroll();
		}
	}
}
void Scatter::MiddleUp(Point pt, dword d)
{
	if (isButDown) {
		MouseMove(pt, d);
		isButDown = false;
	}
}

void Scatter::MouseWheel(Point, int zdelta, dword) {
	double scale = zdelta > 0 ? zdelta/100. : -100./zdelta;
	if (mouseHandlingX) {
		xMin += xRange*(1-scale)/2.;
		xRange *= scale;
	}
	if (mouseHandlingY) {
		yRange *= scale;
		yRange2 *= scale;		
	}
	if (mouseHandlingX || mouseHandlingY) {
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
	w.DrawLine(cp,cp,int(scale*size/6),markColor);
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

void Scatter::SetMouseHandling(bool valx, bool valy) 
{
	mouseHandlingX = valx;
	mouseHandlingY = valy;
}

void Scatter::Plot(Draw& w, const int& scale,const int& l,const int& h)const
{
	double d1=xRange/xMajorUnit;
	double d2=yRange/yMajorUnit;
	double d22=yRange2/yMajorUnit2;
	int nMajorX=(d1-int(d1*1.001)>0.05 ? int(d1*1.001) : int(d1*1.001)-1);
	int nMajorY=(d2-int(d2*1.001)>0.05 ? int(d2*1.001) : int(d2*1.001)-1);
	int nMajorY2=(d22-int(d22*1.001)>0.05 ? int(d22*1.001) : int(d22*1.001)-1);
	w.DrawRect(1,1,l-2,h-2,plotAreaColor);	//grosimea liniei nu este scalata
	int gW=int(gridWidth*scale/6);
	if(gridWidth<0) gW=gridWidth;   
	if (drawVGrid)       
		for(int i=0; xMinUnit+i*xMajorUnit < xRange;i++){
			w.DrawLine(int(scale*l*xMinUnit/xRange+i*l/d1),
					   0,
					   int(scale*l*xMinUnit/xRange+i*l/d1),
				       h,
					   gW,gridColor);
		}
		
	if (drawHGrid)
		for(int i=0; yMinUnit+i*yMajorUnit < yRange;i++){
			w.DrawLine(0,
					   int(-scale*h*yMinUnit/yRange + h-i*(h-1)/d2)-1,
					   l,
					   int(-scale*h*yMinUnit/yRange + h-i*(h-1)/d2)-1,
					   gW,gridColor);
		}
		
	w.Clip(Rect(0,0,l,h));
	int ix;//int x points coordinates
	int iy;//int y points coordinates
	if (!vPointsData.IsEmpty()){
	for (int j=0; j<vPointsData.GetCount(); j++){
		Vector<Point> p1;
		for (int i=0; i<vPointsData[j].GetCount(); i++)
				{
					ix=int((l-1)*(vPointsData[j][i].x-xMin)/xRange);
					if (vPPrimaryY[j])
						iy=int((h-1)*(vPointsData[j][i].y-yMin)/yRange);
					else
						iy=int((h-1)*(vPointsData[j][i].y-yMin2)/yRange2);
					p1<<Point(ix,h-iy-1);
				}
		if(vJoin[j])
		{
			if(vSmooth[j]&&vPointsData[j].GetCount()>2)
			{
				Vector<Point> p2;
				Vector<XY> v(Cubic(vPointsData[j]));         
				for (int i=0; i<v.GetCount(); i++)
				{
					ix=int((l-1)*(v[i].x-xMin)/xRange);
					if (vPPrimaryY[j])
						iy=int((h-1)*(v[i].y-yMin)/yRange);
					else
						iy=int((h-1)*(v[i].y-yMin2)/yRange2);
					p2<<Point(ix,h-iy-1);
				}
				if(!p2.IsEmpty()) w.DrawPolyline(p2,int(scale*vPThickness[j]/6),vPColors[j],Null);
			}
			
			else if (!p1.IsEmpty()) w.DrawPolyline(p1,int(scale*vPThickness[j]/6),vPColors[j],Null);
		}
			
		if(vShowMark[j])
				for (int i=0; i<vPointsData[j].GetCount(); i++){
				
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
							
				ix=int((l-1)*(vFunctionData[j][i].x-xMin)/xRange);
				if (vFPrimaryY[j])
					iy=int((h-1)*(vFunctionData[j][i].y-yMin)/yRange);				
				else
					iy=int((h-1)*(vFunctionData[j][i].y-yMin2)/yRange2);				
					p1<<Point(ix,h-iy-1);				                        
			}
			w.DrawPolyline(p1,int(scale*vFThickness[j]/6),vFColors[j],Null);
		}
		else
		{
			Vector<Point> p1;
			for(int i=0; i<l; i++)
			{
				double x=xMin+i*(xRange/(l-1));
				if (vFPrimaryY[j])
					iy=int((h-1)*(vAdress[nf](x)-yMin)/yRange);
				else
					iy=int((h-1)*(vAdress[nf](x)-yMin2)/yRange2);
				p1<<Point(i,h-iy-1);
			}
			w.DrawPolyline(p1,int(scale*vFThickness[j]/6),vFColors[j],Null);
			nf++;
		}
			
	}
	w.End();
	
}
Vector<XY> Scatter::Cubic(const Vector<XY>& DataSet, const int& fineness,double tension) const
{
	Vector<XY> OutSet;
	if(DataSet.GetCount()>2){   
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
void Scatter::SetDrawing(Draw& w, const int& scale) const
{
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
	if(showLegend) DrawLegend(w,scale);
	int l=scale*GetSize().cx-2*scale*px;
	int h=scale*GetSize().cy-2*scale*py-scale*titleFont.GetHeight();
	w.DrawRect(0,0,l,h,Gray);
	Font FontLabel6;
	FontLabel6=labelsFont;
	FontLabel6.Height(scale*labelsFont.GetHeight());
	
	Size lx=GetTextSize(xLabel,FontLabel6);
	Size ly=GetTextSize(yLabel,FontLabel6);
	Size ly2=GetTextSize(yLabel2,FontLabel6);
	w.DrawText(int((l-lx.cx)/2),h+scale*py-lx.cy-scale*2,xLabel,FontLabel6,labelsColor);
	w.DrawText(scale*2-scale*px,int((h+ly.cx)/2),900,yLabel,FontLabel6,labelsColor);
	w.DrawText(int(l+scale*px-ly2.cy-2*scale),int((h+ly2.cx)/2),900,yLabel2,FontLabel6,labelsColor);
	
	//int nMajorX=int(1.001*xRange/xMajorUnit);
	//int nMajorY=int(1.001*yRange/yMajorUnit);
	//int nrX=int(l/nMajorX);
	//int nrY=int(h/nMajorY);		
	
	if (drawXReticle)
		for(int i=0; xMinUnit+i*xMajorUnit<=xRange;i++){
			w.DrawLine(int(scale*l*xMinUnit/xRange+i*(l-1)/(xRange/xMajorUnit)),
				h,   
				int(scale*l*xMinUnit/xRange+i*(l-1)/(xRange/xMajorUnit)),
				h+scale*4, 
				int(scale/2),
				axisColor);             
			Font Standard6;
			Standard6.Height(scale*StdFont().GetHeight());
			double gridX=xMinUnit+i*xMajorUnit+xMin;
			String gridLabelX;
			if (cbModifFormatX)
				cbModifFormatX(gridLabelX, i, gridX);
			else
				gridLabelX = VariableFormatX(gridX);
			int dx=scale*int(GetTextSize(gridLabelX,StdFont()).cx/2);  
			w.DrawText(int(scale*l*xMinUnit/xRange+i*l/(xRange/xMajorUnit))-dx, h+scale*4, gridLabelX, Standard6, axisColor);
			//w.DrawText(int x¸ int y¸ int angle¸ const WString& text¸ Font font = StdFont()¸ Color ink = DefaultInk¸ const int *dx = NULL)
			//w.DrawText(int(i*l/(xRange/xMajorUnit))-dx,h+scale*4, 450, gridLabelX,Standard6,axisColor);  
		}
	if (drawYReticle)
		for(int i=0; yMinUnit+i*yMajorUnit<=yRange;i++){
			w.DrawLine(-(scale*4),
				int(-scale*h*yMinUnit/yRange+h-i*(h-1)/(yRange/yMajorUnit))-1,
				0,
				int(-scale*h*yMinUnit/yRange+h-i*(h-1)/(yRange/yMajorUnit))-1,
				int(scale/2),
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
			w.DrawText(-dx-scale*6, int(-scale*h*yMinUnit/yRange+h-i*(h-1)/(yRange/yMajorUnit))-scale*8, gridLabelY, Standard6, axisColor);
		}	
	if (drawY2Reticle)
		for(int i=0; yMinUnit+i*yMajorUnit<=yRange;i++){
			w.DrawLine(l+(scale*4),
				int(-scale*h*yMinUnit2/yRange2+h-i*(h-1)/(yRange/yMajorUnit))-1,
				l-1,
				int(-scale*h*yMinUnit2/yRange2+h-i*(h-1)/(yRange/yMajorUnit))-1,
				int(scale/2),
				axisColor);
			double gridY2=yMinUnit2+i*yMajorUnit2+yMin2;
			String gridLabelY2;
			if (cbModifFormatY2)
				cbModifFormatY2(gridLabelY2, i, gridY2);
			else
				gridLabelY2 = VariableFormatY2(gridY2);
			//int dx=scale*GetTextSize(gridLabelY,StdFont()).cx;
			Font Standard6;
			Standard6.Height(scale*StdFont().GetHeight());  
			w.DrawText(l+scale*10,int(-scale*h*yMinUnit2/yRange2+h-i*(h-1)/(yRange/yMajorUnit))-scale*8,gridLabelY2,Standard6,axisColor);
		}	
		
	if(antialiasing && w.IsGui())
	{
		ImageDraw imdraw(3*l,3*h);	
		Plot (imdraw,3,3*l,3*h);
		w.DrawImage(0,0,l,h,imdraw);
	}	
	else Plot(w,scale,l,h);		

	w.DrawRect(0,h-int(axisWidth*scale/6),l,int(axisWidth*scale/6),axisColor);
	w.DrawRect(0,0,int(axisWidth*scale/6),h,axisColor);
	w.End();
}
	
Scatter::Scatter():
	title (""),
	titleColor(Black),
	graphColor(White),
	titleFont(Roman(20)),
	xLabel(""),yLabel(""),
	labelsFont(StdFont()),
	labelsColor(Black),
	plotAreaColor(Color::Color(245,245,248)),
	axisColor(Black),
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
	gridColor(Color::Color(102,102,102)),
	gridWidth(-4),
	paintInfo(false),
	mouseHandlingX(false), mouseHandlingY(false), isButDown(false),       
	drawXReticle(true), drawYReticle(true), drawY2Reticle(false),
	drawVGrid(true), drawHGrid(true),
	showLegend(true),legendWeight(80),
	antialiasing(false),
	offset(-10,12)
{
	Color(graphColor);	
	BackPaint();
	popText.SetColor(::Color(200,220,255));        
}

Scatter::~Scatter()
{
}

