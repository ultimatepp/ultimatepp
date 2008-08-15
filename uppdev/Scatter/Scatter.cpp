#include "Scatter.h"


Scatter& Scatter::SetColor(class::Color _color)
{
	graphColor=_color;
	return *this;
}

Scatter& Scatter::SetTitle(String title)
{
	Title=title;
	return *this;
}
Scatter& Scatter::SetFontTitle(Font& fontTitle)
{
	FontTitle=fontTitle;
	return *this;
}
Scatter& Scatter::SetColorTitle(class::Color colorTitle)
{
	ColorTitle=colorTitle;
	return *this;
}
void Scatter::SetLabels(String _xLabel, String _yLabel)
{
	xLabel=_xLabel;
	yLabel=_yLabel;
}
Scatter& Scatter::SetLabelX(String _xLabel)
{
	xLabel=_xLabel;
	return *this;
}
Scatter& Scatter::SetLabelY(String _yLabel)
{
	yLabel=_yLabel;
	return *this;
}
Scatter& Scatter::SetFontLabels(Font& fontLabels)
{
	FontLabels=fontLabels;
	return *this;
}
Scatter& Scatter::SetColorLabels(class::Color colorLabels)
{
	ColorLabels=colorLabels;
	return *this;
}
Scatter& Scatter::SetPlotAreaPoz(int poz_x, int poz_y)
{
	px=poz_x;
	py=poz_y;
	return *this;
}
Scatter& Scatter::H_Border(int poz_x)
{
	if(poz_x>=0) px=poz_x;
	return *this;
}
Scatter& Scatter::V_Border(int poz_y)
{
	if(poz_y>=0) py=poz_y;
	return *this;
}

Scatter& Scatter::SetPlotAreaColor(class ::Color p_a_color)
{
	plotAreaColor=p_a_color;
	return *this;
}
Scatter& Scatter::SetAxisColor(class::Color axis_color)
{
	axisColor=axis_color;
	return *this;
}
Scatter& Scatter::SetAxisWidth(int axis_width)
{
	axisWidth=axis_width;
	return *this;
}
Scatter& Scatter::SetGridColor(class::Color grid_color)
{
	gridColor=grid_color;
	return *this;
}
Scatter& Scatter::SetGridWidth(int grid_width)
{
	gridWidth=grid_width;
	return *this;
}
Scatter& Scatter::ShowHGrid(bool show)
{
	DrawHGrid=show;
	return *this;
}
Scatter& Scatter::ShowVGrid(bool show)
{
	DrawVGrid=show;
	return *this;
}
Scatter& Scatter::showLegend(bool show)
{
	ShowLegend=show;
	return *this;
}
Scatter& Scatter::SetLegendWeight(int weight)
{
	LegendWeight=weight;
	return *this;
}
void Scatter::DrawLegend(Draw& w,int scale)
{
	Vector<String> L;
	L.Append(Legend);
	L.Append(FLegend);
	Vector<int> Thick;
	Thick.Append(PThickness);
	Thick.Append(FThickness);
	Vector<class::Color> LC;
	LC.Append(PColors);
	LC.Append(FColors);
	Vector<int> LW;
	LW.Append(PWidth);
	Vector<int> MS;
	MS.Append(MarkStyles);
	Vector<class::Color> MC;
	MC.Append(MarkColors);

	int NMR=int((GetSize().cx-2*px)/LegendWeight);//max number of labels per row
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
				w.DrawRect(scale*(i-start)*LegendWeight,
								scale*(4-12*(j+1)),
								14*scale,
								scale*Thick.At(i)/6,
								LC.At(i));
				Point p(scale*((i-start)*LegendWeight+7),scale*(4-12*(j+1))+scale*Thick.At(i)/12);
				DrawMark(MS.At(i),w,scale,p,LW.At(i,0),MC.At(i));
				Font scaledFont;
				scaledFont.Height(scale*StdFont().GetHeight());
				w.DrawText(scale*(i-start)*LegendWeight+scale*16,
							scale*(-2-12*(j+1)),
							L.At(i),
							scaledFont,LC.At(i));
			}
		}
	}

}

void Scatter::SetRange(double rx, double ry)
{
	xRange=rx;
	yRange=ry;
	xMajorUnit=xRange/10;
	yMajorUnit=yRange/10;
}
void Scatter::SetMajorUnits(double ux, double uy)
{
	xMajorUnit=ux;
	yMajorUnit=uy;
}
void Scatter::SetXYMin(double xmin, double ymin)
{
	xMin=xmin;
	yMin=ymin;
}
void Scatter::AddSeries(Vector<XY> & points,String legend,bool join, class::Color pcolor, int width, int thickness)
{
	PointsData.AddPick(points);
	Join.Add(join);
	Smooth.Add(false);
	PColors.Add(pcolor);
	PWidth.Add(width);
	PThickness.Add(thickness);
	Legend.Add(legend);
	MarkColors.Add(pcolor);
	ShowMark.Add(true);
	MarkStyles.Add(CIRCLE);

	Refresh();
}

void Scatter::AddPoint(int j, XY & point)
{
	if(!PointsData.IsEmpty() && PointsData.GetCount()>j)
	{
		PointsData[j].AddPick(point);
		Refresh();
	}
}
void Scatter::InsertPoint(int j, int i, XY & point)
{
	if(!PointsData.IsEmpty() && PointsData.GetCount()>j)
	{
		if(!PointsData[j].IsEmpty() && PointsData[j].GetCount()>=i)
		{
			PointsData[j].Insert(i,point);
			Refresh();
		}
	}
}
void Scatter::RemovePoint(int j, int i)
{
	if(!PointsData.IsEmpty() && PointsData.GetCount()>j)
	{
		if(!PointsData[j].IsEmpty() && PointsData[j].GetCount()>i)
		{
			PointsData[j].Remove(i);
			Refresh();
		}
	}
}
void Scatter::SetData(int j, int i, XY & point)
{
	if(!PointsData.IsEmpty() && PointsData.GetCount()>j)
	{
		if(!PointsData[j].IsEmpty() && PointsData[j].GetCount()>i)
		{
			PointsData[j][i]=point;
			Refresh();
		}
	}
}

void Scatter::SetData(int nbSeries, Vector<XY> & points)
{

	PointsData[nbSeries]<<=points;
	Refresh();
}
void Scatter::SetDataColor(int j, class::Color pcolor)
{
	if(!PColors.IsEmpty() && PColors.GetCount()>j)
	{
		PColors[j]=pcolor;
		Refresh();
	}
}
void Scatter::SetFunctColor(int j, class::Color fcolor)
{
	if(!FColors.IsEmpty() && FColors.GetCount()>j)
	{
		FColors[j]=fcolor;
		Refresh();
	}
}
class::Color Scatter::GetDataColor(int j)
{
	if(!PColors.IsEmpty() && PColors.GetCount()>j)
	{
		return PColors[j];
	}
	else return Null;
}
class::Color Scatter::GetFunctColor(int j)
{
	if(!FColors.IsEmpty() && FColors.GetCount()>j)
	{
		return FColors[j];
	}
	else return Null;
}
void Scatter::SetDataThickness(int j, int thick_dots)
{
	if(!PThickness.IsEmpty() && PThickness.GetCount()>j)
	{
		PThickness[j]=thick_dots;
		Refresh();
	}
}
void Scatter::SetFunctThickness(int j, int thick_dots)
{
	if(!FThickness.IsEmpty() && FThickness.GetCount()>j)
	{
		FThickness[j]=thick_dots;
		Refresh();
	}
}
int Scatter::GetDataThickness(int j)
{
	if(!PThickness.IsEmpty() && PThickness.GetCount()>j)
	{
		return PThickness[j];
	}
	else return NULL;
}
int Scatter::GetFunctThickness(int j)
{
	if(!FThickness.IsEmpty() && FThickness.GetCount()>j)
	{
		return FThickness[j];
	}
	else return NULL;
}
void Scatter::SetMarkWidth(int j, int width_dots)
{
	if(!PWidth.IsEmpty() && PWidth.GetCount()>j)
	{
		PWidth[j]=width_dots;
		Refresh();
	}
}
int Scatter::GetMarkWidth(int j)
{
	if(!PWidth.IsEmpty() && PWidth.GetCount()>j)
	{
		return PWidth[j];
	}
	else return NULL;
}
void Scatter::SetMarkStyle(int j, int noStyle)
{
	if(!MarkStyles.IsEmpty() && MarkStyles.GetCount()>j)
	{
		MarkStyles[j]=noStyle;
		Refresh();
	}
}
int Scatter::GetMarkStyle(int j)
{
	if(!MarkStyles.IsEmpty() && MarkStyles.GetCount()>j)
	{
		return MarkStyles[j];
	}
	else return NULL;
}
void Scatter::SetMarkColor(int j, class::Color mcolor)
{
	if(!MarkColors.IsEmpty() && MarkColors.GetCount()>j)
	{
		MarkColors[j]=mcolor;
		Refresh();
	}
}
class ::Color Scatter::GetMarkColor(int j)
{
	if(!MarkColors.IsEmpty() && MarkColors.GetCount()>j)
	{
		return MarkColors[j];
	}
	else return Null;
}
void Scatter::SetShowMark(int j, bool show)
{
	if(!ShowMark.IsEmpty() && ShowMark.GetCount()>j)
	{
		ShowMark[j]=show;
		Refresh();
	}
}
bool Scatter::IsMarkShow(int j)
{
	if(!ShowMark.IsEmpty() && ShowMark.GetCount()>j)
	{
		return ShowMark[j];
	}
	else return NULL;
}
void Scatter::SetJoin(int j, bool join)
{
	if(!Join.IsEmpty() && Join.GetCount()>j)
	{
		Join[j]=join;
		Refresh();
	}
}
bool Scatter::IsJoined(int j)
{
	if(!Join.IsEmpty() && Join.GetCount()>j)
	{
		return Join[j];
	}
	else return NULL;
}
void Scatter::SetSmooth(int j, bool smooth)
{
	if(!Smooth.IsEmpty() && Smooth.GetCount()>j)
	{
		Smooth[j]=smooth;
		Refresh();
	}
}
bool Scatter::IsSmooth(int j)
{
	if(!Smooth.IsEmpty() && Smooth.GetCount()>j)
	{
		return Smooth[j];
	}
	else return NULL;
}
void Scatter::RemoveSeries(int j)
{
	if (PointsData.GetCount()>j)
	{
		PointsData.Remove(j);
		Join.Remove(j);
		Smooth.Remove(j);
		PColors.Remove(j);
		PWidth.Remove(j);
		PThickness.Remove(j);
		Legend.Remove(j);
		MarkColors.Remove(j);
		ShowMark.Remove(j);
		MarkStyles.Remove(j);

		Refresh();
	}
}

void Scatter::plotFunction(double (*f)(double), String legend, class::Color fcolor, int weight)
{
	double x,y;
	Vector<XY> series;
	for (int i=0;i<=GetNbMax();i++){
		x=i*(xRange/GetNbMax())+xMin;
		y=f(x);
		series<<XY(x,y);
	}
	FColors.Add(fcolor);
	FThickness.Add(weight);
	FunctionData.AddPick(series);
	FLegend.Add(legend);
	Refresh();
}
void Scatter::plotParaFunction(XY (*pf)(double), String legend, class::Color fcolor, int weight,int Np)
{
	double t;//t must be choosed between [0,1]
	Vector<XY> series;
	for (int i=0; i<=Np;i++)
	{
		t=(double)i/Np;
		series<<pf(t);
	}
	FColors.Add(fcolor);
	FThickness.Add(weight);
	FunctionData.AddPick(series);
	FLegend.Add(legend);
	Refresh();
}

int Scatter::GetNbMax()
{
	return int((GetSize().cx-2*px));
}
void Scatter::RemoveFSeries(int j)
{
	if (FunctionData.GetCount()>j)
	{
		FunctionData.Remove(j);
		FColors.Remove(j);
		FThickness.Remove(j);
		FLegend.Remove(j);
		Refresh();
	}
}
void Scatter::Paint(DrawExtension& we)
{
	Rect r1;
	Point p1,p2;
	p1.x=10;
	p1.y=10;
	p2.x=90;
	p2.y=90;
	r1.Set(p1,p2);
	//we.DrawRect(r1,Blue);
	we.DrawPie(r1,Point(90,10),Point(90,90),Null,0,Null);
	Rect r2(Point(150,5),Point(250,50));
	we.DrawRoundRect(r2,50,40, LtBlue,2,White);
}

Drawing& Scatter::getDrawing()
{
	DrawingDraw ddw(6*GetSize());
	SetDrawing (ddw, 6);
	drawing=ddw;
	return drawing;
}
Image& Scatter::getImage(int scale)
{
	ImageDraw idraw(scale*GetSize());
	SetDrawing (idraw,scale);
	img=idraw;
	return img;
}

void Scatter::Paint(Draw& w)
{
	SetDrawing(w,1);
	//w.DrawImage(0,0,GetSize().cx,GetSize().cy,getImage(2));  // a simple AA method
}

void Scatter::Circle(Draw& w, int scale, Point cp, int size, class::Color markColor)
{
	w.DrawLine(cp,cp,int(scale*size/6),markColor);
}

void Scatter::Square(Draw& w, int scale, Point cp, int size, class::Color markColor)
{
	Vector <Point> p;
	p<<Point(cp.x-int(size*scale/12+0.5),cp.y-int(size*scale/12+0.5))<<Point(cp.x+int(size*scale/12+0.5),cp.y-int(size*scale/12+0.5))
	<<Point(cp.x+int(size*scale/12+0.5),cp.y+int(size*scale/12+0.5))<<Point(cp.x-int(size*scale/12+0.5),cp.y+int(size*scale/12+0.5))
	<<Point(cp.x-int(size*scale/12+0.5),cp.y-int(size*scale/12+0.5));
	w.DrawPolyline(p,scale/2,markColor);

}
void Scatter::Rectangle(Draw& w, int scale, Point cp, int size, class::Color markColor)
{
	w.DrawRect(cp.x-int(size*scale/12+0.5),cp.y-int(size*scale/12+0.5),int(size*scale/6+1.5),int(size*scale/6+1.5),markColor);
}
void Scatter::Triangle(Draw& w, int scale, Point cp, int size, class::Color markColor)
{
	Vector <Point> p;
	p<<Point(cp.x,cp.y-int(size*scale/12+0.5))<<Point(cp.x+int(size*scale/12+0.5),cp.y+int(0.6*size*scale/12+0.5))
	<<Point(cp.x-int(size*scale/12+0.5),cp.y+int(0.6*size*scale/12+0.5))
	<<Point(cp.x,cp.y-int(size*scale/12+0.5));
	w.DrawPolyline(p,scale/2,markColor);

}
void Scatter::Cross(Draw& w, int scale, Point cp, int size, class::Color markColor)
{
	w.DrawLine(cp.x-int(size*scale/12+0.5),cp.y,cp.x+int(size*scale/12+1.5),cp.y,scale,markColor);
	w.DrawLine(cp.x,cp.y-int(size*scale/12+0.5),cp.x,cp.y+int(size*scale/12+1.5),scale,markColor);
}
void Scatter::XFunct(Draw& w, int scale, Point cp, int size, class::Color markColor)
{
	w.DrawLine(cp.x-int(size*scale/12+0.5),cp.y-int(size*scale/12+0.5),cp.x+int(size*scale/12+1.5),cp.y+int(size*scale/12+1.5),scale,markColor);
	w.DrawLine(cp.x+int(size*scale/12+0.5),cp.y-int(size*scale/12+0.5),cp.x-int(size*scale/12+1.5),cp.y+int(size*scale/12+1.5),scale,markColor);
}
void Scatter::Rhomb(Draw& w, int scale, Point cp, int size, class::Color markColor)
{
	Vector <Point> p;
	p<<Point(cp.x,cp.y-int(size*scale/12+0.5))<<Point(cp.x+int(size*scale/12+0.5),cp.y)
	<<Point(cp.x,cp.y+int(size*scale/12+0.5))<<Point(cp.x-int(size*scale/12+0.5),cp.y)
	<<Point(cp.x,cp.y-int(size*scale/12+0.5));
	w.DrawPolyline(p,scale,markColor);
}
inline void Scatter::DrawMark(int style, Draw& w, int scale, Point cp, int size, class::Color markColor)
{
	switch (style)
	{
		case CIRCLE : Circle(w,scale,cp,size,markColor);break;
		case RECTANGLE :Rectangle(w,scale,cp,size,markColor);break;
		case SQUARE : Square(w,scale,cp,size,markColor);break;
		case TRIANGLE : Triangle(w,scale,cp,size,markColor);break;
		case CROSS : Cross(w,scale,cp,size,markColor);break;
		case X : XFunct(w,scale,cp,size,markColor);break;
		case RHOMB:Rhomb(w,scale,cp,size,markColor);break;
		default : Circle(w,scale,cp,size,markColor);break;
	}
}
Vector<XY> Scatter::Cubic(Vector<XY>& DataSet, int fineness,double tension)
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
void Scatter::SetDrawing(Draw& w, int scale)
{
	w.DrawRect(scale*GetSize(),graphColor);

	Size sz;
	sz.cx=0;
	sz.cy=0;
	if(!Title.IsEmpty())
	{
		Font FontTitle6;
		FontTitle6=FontTitle;
		FontTitle6.Height(scale*FontTitle.GetHeight());
		FontTitle6.Width(scale*FontTitle.GetWidth());
		sz= GetTextSize(Title, FontTitle6);
		w.DrawText((scale*GetSize().cx-sz.cx)/2,scale*2,Title,FontTitle6,ColorTitle);
	}

	w.Offset(Point(scale*px,scale*py+sz.cy));
	if(ShowLegend) DrawLegend(w,scale);
	int l=scale*GetSize().cx-2*scale*px;
	int h=scale*GetSize().cy-2*scale*py-sz.cy;
	w.DrawRect(0,-1*scale,l+1*scale,h+1*scale,Gray);
	w.DrawRect(0,0,l,h,plotAreaColor);

	w.DrawLine(0,h,l,h,int(axisWidth*scale/6),axisColor);
	w.DrawLine(0,h,0,0,int(axisWidth*scale/6),axisColor);

	Font FontLabel6;
	FontLabel6=FontLabels;
	FontLabel6.Height(scale*FontLabels.GetHeight());

	Size lx=GetTextSize(xLabel,FontLabel6);
	Size ly=GetTextSize(yLabel,FontLabel6);
	w.DrawText(int((l-lx.cx)/2),h+scale*py-lx.cy-scale*2,xLabel,FontLabel6,ColorLabels);
	w.DrawText(scale*2-scale*px,int((h+ly.cx)/2),900,yLabel,FontLabel6,ColorLabels);
	int nMajorX=int(xRange/xMajorUnit);
	int nMajorY=int(yRange/yMajorUnit);
	int nrX=int(l/nMajorX);
	int nrY=int(h/nMajorY);


	if (DrawXReticle)
		for(int i=1; i<=nMajorX;i++){
			w.DrawLine(int(i*l/(xRange/xMajorUnit)+0.5),
				h,
				int(i*l/(xRange/xMajorUnit)+0.5),
				h+scale*4,
				int(scale/2),
				axisColor);
			Font Standard6;
			Standard6.Height(scale*StdFont().GetHeight());
			int dx=scale*int(GetTextSize(AsString(i*xMajorUnit+xMin),StdFont()).cx/2);
			w.DrawText(int(i*l/(xRange/xMajorUnit))-dx,h+scale*4,AsString(i*xMajorUnit+xMin),Standard6,axisColor);
		}

	if (DrawYReticle)
		for(int i=1; i<=nMajorY;i++){
			w.DrawLine(-(scale*4),
				h-int(i*h/(yRange/yMajorUnit)+0.5),
				0,
				h-int(i*h/(yRange/yMajorUnit)+0.5),
				int(scale/2),
				axisColor);
			int dx=scale*GetTextSize(AsString(i*yMajorUnit+yMin),StdFont()).cx;
			Font Standard6;
			Standard6.Height(scale*StdFont().GetHeight());
			w.DrawText(-dx-scale*6,h-scale*6-int(i*h/(yRange/yMajorUnit)),AsString(i*yMajorUnit+yMin),Standard6,axisColor);
		}
	int gW=int(gridWidth*scale/6);
	if(gridWidth<0) gW=gridWidth;
	if (DrawVGrid)
		for(int i=1; i<nMajorX;i++){
			w.DrawLine(int(i*l/(xRange/xMajorUnit)+0.5),
				0,
				int(i*l/(xRange/xMajorUnit)+0.5),
				h,
				gW,gridColor);
		}

	if (DrawHGrid)
		for(int i=1; i<nMajorY;i++){
			w.DrawLine(0,
				h-int(i*h/(yRange/yMajorUnit)+0.5),
				l,
				h-int(i*h/(yRange/yMajorUnit)+0.5),
				gW,gridColor);
		}

	w.Clip(Rect(0,0,l,h));

	int ix;//int x points coordinates
	int iy;//int y points coordinates
	if (!PointsData.IsEmpty()){
	for (int j=0; j<PointsData.GetCount(); j++){
		Vector<Point> p1;
		for (int i=0; i<PointsData[j].GetCount(); i++)
				{
					ix=int(l*(PointsData[j][i].x-xMin)/xRange +0.5);
					iy=int(h*(PointsData[j][i].y-yMin)/yRange+0.5);
					p1<<Point(ix,h-iy);
				}
		if(Join[j])
		{
			if(Smooth[j]&&PointsData[j].GetCount()>2)
			{
				Vector<Point> p2;
				Vector<XY> v(Cubic(PointsData[j]));
				for (int i=0; i<v.GetCount(); i++)
				{
					ix=int(l*(v[i].x-xMin)/xRange +0.5);
					iy=int(h*(v[i].y-yMin)/yRange+0.5);
					p2<<Point(ix,h-iy);
				}
				if(!p2.IsEmpty()) w.DrawPolyline(p2,int(scale*PThickness[j]/6),PColors[j],Null);
			}

			else w.DrawPolyline(p1,int(scale*PThickness[j]/6),PColors[j],Null);
		}


		if(ShowMark[j])
				for (int i=0; i<PointsData[j].GetCount(); i++){

					DrawMark(MarkStyles[j],w,scale,p1[i],PWidth[j],MarkColors[j]);

				}
		}

	}

	for (int j=0; j<FunctionData.GetCount(); j++){
		if(!FunctionData[j].IsEmpty()){
			Vector<Point> p1;
			for (int i=0; i<FunctionData[j].GetCount(); i++){
				ix=int(l*(FunctionData[j][i].x-xMin)/xRange +0.5);
				iy=int(h*(FunctionData[j][i].y-yMin)/yRange+0.5);
				p1<<Point(ix,h-iy);
			}
			w.DrawPolyline(p1,int(scale*FThickness[j]/6),FColors[j],Null);
		}
	}
	w.End();
	w.End();
}

Scatter::Scatter():
	Title (""),
	ColorTitle(Black),
	graphColor(White),
	FontTitle(Roman(20)),
	xLabel(""),yLabel(""),
	FontLabels(StdFont()),
	ColorLabels(Black),
	plotAreaColor(Color::Color(245,245,248)),
	axisColor(Black),
	axisWidth(3),
	px(30),
	py(30),
	xRange(100.0),
	yRange(100.0),
	xMajorUnit(xRange/10),
	yMajorUnit(yRange/5),
	xMin(0.0),
	yMin(0.0),
	gridColor(Color::Color(102,102,102)),
	gridWidth(-4),
	DrawXReticle(true), DrawYReticle(true),
	DrawVGrid(true), DrawHGrid(true),
	ShowLegend(true),LegendWeight(80)
{
	Color(graphColor);

	BackPaint();
}

Scatter::~Scatter()
{
}
