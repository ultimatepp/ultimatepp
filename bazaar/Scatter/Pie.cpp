#include "Pie.h"
static void DrawPie(Draw& w, Point centre, int r, int start, int alpha, int width, class::Color fill, class::Color outline,uint64 pattern,class::Color background )
{
	const int dalpha=1;
	int n= alpha/dalpha;
	Vector <Point> vP;
	vP<<centre;
	int ix;
	int iy;	
	for (int i=0; i<=n; i++)
	{
		double x=centre.x+ r*cos((start+i*dalpha)*M_PI/1800);
		ix=fround(x);		
		double y=centre.y+r*sin((start+i*dalpha)*M_PI/1800);
		iy=fround(y);
		double dxy=(x-ix)*(x-ix)+(y-iy)*(y-iy);
		if(dxy<0.1 || i ==0 || i==n) 
			vP<<Point(ix,iy);
		//if(!(dynamic_cast<const DrawingDraw *>(&w)) && !w.IsPrinter()) w.DrawRect(ix,iy,1,1,Blend(fill,background,150));
		if(w.IsGui()) w.DrawRect(ix,iy,1,1,Blend(fill,background,150));			
	}
	vP<<centre;	
	w.DrawPolygon(vP,fill,width,outline,pattern,Null);
		
}

static void DrawScaledPie(Draw& w, Point centre, int r, int start, int alpha, int width=0, class::Color fill=Null, class::Color outline=Black, int scale=1,uint64 pattern=0,class::Color background=White)
{
	DrawPie(w, centre, scale*r, start, alpha, scale*width, fill, outline,pattern,background);
}

static void DrawMLText(Draw& w, const Rect& box, String content, const Font& f, class::Color textColor=Black)
{
	int L=box.Width();
	int H=box.Height();
	int count=0;
	Size sz;
	content.Cat(' ');
	while(!content.IsEmpty())
	{
		int pos=0;//current position
		int lastpos=0;	
		int toDraw=0;	
	
		while(toDraw<L && pos>=0)
		{
			lastpos=pos+1;
			pos=content.Find(' ',lastpos);			
			sz= GetTextSize(content.Left(pos+1), f);			
			toDraw=sz.cx;			
		}
		if (pos<0)
		{
			sz= GetTextSize(content, f);
			toDraw=sz.cx;
			if(toDraw<L) lastpos=content.GetCount();						
		}
		
		w.DrawText(box.left, box.top+count*fround(f.GetHeight()*1.2),content.Left(lastpos), f,textColor);
		sz=GetTextSize(content.Mid(lastpos,pos-lastpos), f);
		if(sz.cx>=L) return;
		count++;
		content=content.Mid(lastpos);
	}
}
	
	
PieChart& PieChart::SetColor(const class::Color& _color)
{
	backColor=_color;
	return *this;
}

PieChart& PieChart::SetTitle(const String& _title)
{
	title=_title;
	return *this;
}
PieChart& PieChart::SetTitleFont(const Font& fontTitle)
{
	titleFont=fontTitle;
	return *this;
}
PieChart& PieChart::SetTitleColor(const class::Color& colorTitle)
{
	titleColor=colorTitle;
	return *this;
}
PieChart& PieChart::TitlePos(titlePosition pos)
{
	titlePos=pos;
	return *this;
}
PieChart& PieChart::SetTitleGap(const int& gap)
{
	titleGap=gap;
	return *this;
}
PieChart& PieChart::SetRadius(const int& r)
{
	radius=r;
	return *this;
}
PieChart& PieChart::ShowPercent(bool show)
{
	showPercent=show;
	return *this;
}
PieChart& PieChart::SetPercentBack(const class::Color& pbcolor)
{
	percentBack=pbcolor;
	return *this;
}
PieChart& PieChart::SetLegendFont(const Font& fontLegend)
{
	legendFont=fontLegend;
	return *this;
}
PieChart& PieChart::SetLegendTextColor(const class::Color& colorLegend)
{
	legendTextColor=colorLegend;
	return *this;
}
PieChart& PieChart::SetLegendBackColor(const class::Color& colorBack)
{
	legendBackColor=colorBack;
	return *this;
}
PieChart& PieChart::SetLegend_cx(const int& h)
{
	legend_cx=h;
	return *this;
}
PieChart& PieChart::SetLegend_cy(const int& v)
{
	legend_cy=v;
	return *this;
}
PieChart& PieChart::SetShowLegend(bool show)
{
	showLegend=show;
	return *this;
}
void PieChart::AddCategory(const String& name, const double& value, const class::Color& catcolor)
{
	vNames.Add(name);
	vValues.Add(value);
	vColors.Add(catcolor);
	Refresh();
}
void PieChart::RemoveCategory(const int& index)
{
	if(!vValues.IsEmpty() && vValues.GetCount()>index)
	{
		vNames.Remove(index);
		vValues.Remove(index);
		vColors.Remove(index);
		Refresh();
	}
}
PieChart& PieChart::SetCatValue(const int& index, const double& value)
{
	if(!vValues.IsEmpty() && vValues.GetCount()>index)
	{
		vValues[index]=value;
		Refresh();
	}
	return *this;
}
PieChart& PieChart::SetCatName(const int& index, const String& name)
{
	if(!vNames.IsEmpty() && vNames.GetCount()> index)
	{
		vNames[index]=name;
		Refresh();
	}
	return *this;
}
PieChart& PieChart::SetCatColor(const int& index, const class::Color& catcolor)
{
	if(!vColors.IsEmpty() && vColors.GetCount()> index)
	{
		vColors[index]=catcolor;
		Refresh();
	}
	return *this;
}
double PieChart::GetCatValue(const int& index) const
{
	if(!vValues.IsEmpty() && vValues.GetCount()>index)
	{
		return vValues[index];
	}
	else return -1;
}
String PieChart::GetCatName(const int& index) const
{
	if(!vNames.IsEmpty() && vNames.GetCount()> index)
	{
		return vNames[index];
	}
	else return "";
}
Acolor PieChart::GetCatColor(const int& index) const
{
	if(!vColors.IsEmpty() && vColors.GetCount()> index)
	{
		return vColors[index];
	}
	else return Null;
}
int PieChart::GetCatCount() const
{
	return vColors.GetCount();
}

void PieChart::PaintPie(Draw& w, int scale)
{
	if(vValues.GetCount()>0)
	{
		double sum=0;
		int alfa0=-900;
		int a0=0;
		for(int i=0;i<vValues.GetCount(); i++)
		{
			sum+=vValues[i];
		}
		int cx,cy;
		if(showLegend) cx=scale*(GetSize().cx+radius-legend_cx)/3;
		else cx=(scale*GetSize().cx)/2;
			
		if(titlePos==BOTTOM)
			cy=scale*(GetSize().cy-titleGap-titleFont.GetHeight())/2;			
		else
			cy=scale*(GetSize().cy+titleGap+titleFont.GetHeight())/2;
		Acolor bc;
		if (backColor==(Acolor)Null ) bc=LtGray;
		else bc=backColor;
	
		for(int i=0;i<vValues.GetCount(); i++)
		{
			
			DrawScaledPie(w,Point(cx,cy),
		      radius, alfa0, fround(3600.0*vValues.At(i)/sum),1,vColors.At(i),vColors.At(i),scale,0,bc);
			                        		              
			alfa0+=fround(3600.0*vValues[i]/sum);
			
		}
		if(showPercent)
		{
			alfa0=-900;
			for(int i=0;i<vValues.GetCount(); i++)
			{
				a0=alfa0;                            		              
				alfa0+=fround(3600.0*vValues[i]/sum);
				String percent=GetPercent(vValues[i],sum);
				Font scaledFont;
				scaledFont.Height(scale*StdFont().GetHeight());
				scaledFont.Width(scale*StdFont().GetWidth());
				Size sz= GetTextSize(percent, scaledFont);
			
				int px=cx+int(scale*radius*cos(3.14*(alfa0+a0)/3600)/1.3)-sz.cx/2;
				int py=cy+int(scale*radius*sin(3.14*(alfa0+a0)/3600)/1.3)-sz.cy/2;
				w.DrawRect(px+1-1,py,sz.cx-2+2,1,percentBack);
				w.DrawRect(px-1,py+1,sz.cx+2,sz.cy-2,percentBack);
				w.DrawRect(px+1-1,py+sz.cy-1,sz.cx-2+2,1,percentBack);
				w.DrawText(px,	py,	percent,scaledFont);
			}
		}
		if(showLegend)
		{
			int lx=2*scale*(GetSize().cx+radius-legend_cx)/3;
			int ly=cy-scale*legend_cy/2;
			w.DrawRect(lx,ly,scale*legend_cx,scale*legend_cy,legendBackColor);
			int nr=GetCatCount();
			int dly=scale*legend_cy/nr;
			for(int i=0;i<nr;i++)
			{
				int fh=scale*(legendFont.GetHeight()-3);
				w.DrawRect(lx+2*scale,ly+scale*3+i*dly,fh,fh,vColors[i]);
				Rect box;
				box.Set(Point(lx+fround(1.8*fh),ly+i*dly),Point(lx+scale*legend_cx,ly+(i+1)*dly));
				Font scaledFont;
				scaledFont.Height(scale*legendFont.GetHeight());
				scaledFont.Width(scale*legendFont.GetWidth());
				::DrawMLText(w, box,vNames[i],scaledFont,legendTextColor);
			}
		}
	}
}
		              
		              
void PieChart::DrawChart(Draw& w, int scale)
{
	Size pcsz = GetSize();	
	w.DrawRect(scale*pcsz,backColor);	
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
		if(titlePos==TOP) w.DrawText((scale*GetSize().cx-sz.cx)/2,scale*titleGap,title,FontTitle6,titleColor);
		else if(titlePos==BOTTOM) w.DrawText((scale*GetSize().cx-sz.cx)/2,scale*(GetSize().cy-titleFont.GetHeight()-titleGap),title,FontTitle6,titleColor);
		  
	}
	PaintPie(w, scale);
	
}
	
void PieChart::Paint(Draw& w)
{
	DrawChart(w,1);
}
String PieChart::GetPercent(double a, double total)
{
	double p=a*100/total;
	String perc=FormatDouble(p,1);
	perc<<'%';
	return perc;	
}
Drawing PieChart::GetDrawing() 
{
	DrawingDraw ddw(6*GetSize());
	DrawChart (ddw, 6);
	return ddw;
}
Image PieChart::GetImage(int scale) 
{
	ImageDraw idraw(scale*GetSize());
	DrawChart (idraw,scale);
	return idraw;
}

#ifdef PLATFORM_WIN32
void PieChart::SaveAsMetafile(const char* file)
{
	WinMetaFileDraw wmfd;	
	wmfd.Create(6*GetSize().cx,6*GetSize().cy,"Pie","PieChart",file);
	DrawChart (wmfd, 6);	
	wmfd.Close();	
}
#endif

PieChart::PieChart():
backColor(White),
titleFont(StdFont(16)),
titlePos(TOP), titleGap(2),titleColor(Black),
radius(60),
showPercent(true),
percentBack(Null),
legendFont(StdFont()),
legendTextColor(Black),legendBackColor(Null),
legend_cx(60), legend_cy(120),
showLegend(true)
{
	Transparent();
	BackPaint();
}
PieChart::~PieChart(){}

