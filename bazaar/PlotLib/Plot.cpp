#include "PlotLib.h"

Plot::Plot():
	mode(PLOT_AA),
	bgcol(0xffffff,0),
	framecol(0xffffff,0),
	fontcol(0x0,0),
	axiscol(0x0,0),
	margin(10)
{
	f=StdFont();
	fi=f.Info();
	marginB=margin+fi.GetHeight();
	marginT=margin+fi.GetHeight()*1.5;
}

Plot& Plot::SetLimits(double x_min,double x_max,double y_min,double y_max){
	g=Rectf(x_min,y_min,x_max,y_max);
	spaceH=margin+StrWidth("-99");
	marginL=max(spaceH,StrWidth("×100")+margin);
	marginR=margin+spaceH/2+StrWidth("×100");
	return *this;
}
Rectf Plot::BoundingBox(bool visibleonly){
	Rectf b=Null;
	for(int i = 0; i < data.GetCount(); i++){
		if(!visibleonly||data[i].visible){
			Rectf& bb(data[i].BB);
			if(bb.IsNullInstance()||bb.Width()==0||bb.Height()==0){data[i].RefreshBB();}
			b.Union(bb);
		}
	}
	return b;
}

Plot& Plot::SetPlotSize(const Size& sz){
	p=size=sz;
	p.Deflate(marginL,marginT,marginR,marginB);
	return *this;	
}

bool Plot::CheckRange()const{
	return ((g.Width()!=0)&&(g.Height()!=0));
}

Pointf Plot::ImgToPlot(const Pointf& X)const{
	Pointf pf=X;
	pf.Offset(-p.left,-p.top);
	return G(pf);
}
Rectf Plot::ImgToPlot(const Rectf& X)const{
	return G(X.Offseted(-p.TopLeft()));
}
Pointf Plot::PlotToImg(const Pointf& X)const{
	Pointf pf=X;
	pf=S(X);
	pf.Offset(p.left,p.top);
	return pf;
}
Rectf Plot::PlotToImg(const Rectf& X)const{
	return S(X.Offseted(p.TopLeft()));
}
String Plot::ImgToPlotFormatted(const Point& X)const{
	Pointf p=ImgToPlot(X);
	return String("[").Cat()<<FormatDoubleFix(p.x*sx,3)<<","
	                        <<FormatDoubleFix(p.y*sy,3)<<"]";
}
String Plot::ImgToPlotFormatted(const Rect& X)const{
	Rectf r=ImgToPlot(X);
	return String("[").Cat()<<FormatDoubleFix(r.left  *sx,3)<<":"
	                        <<FormatDoubleFix(r.right *sx,3)<<","
	                        <<FormatDoubleFix(r.top   *sy,3)<<":"
	                        <<FormatDoubleFix(r.bottom*sy,3)<<"]";
}

double Plot::Gx(double x)const{
	return x*g.Width()/double(p.Width())+g.left;
}
double Plot::Gy(double y)const{
	return g.bottom-y*g.Height()/double(p.Height());
}
Pointf Plot::G(Pointf X)const{
	return Pointf(Gx(X.x),Gy(X.y));
}
Rectf Plot::G(Rectf X)const{
	return Rectf(Gx(X.left),Gy(X.bottom),Gx(X.right),Gy(X.top));
}
double Plot::Sx(double x)const{
	return (x-g.left)/g.Width()*p.Width();
}
double Plot::Sy(double y)const{
	return (1-(y-g.top)/g.Height())*p.Height();
}
Pointf Plot::S(Pointf X)const{
	return Pointf(Sx(X.x),Sy(X.y));
}
Rectf Plot::S(Rectf X)const{
	return Rectf(Sx(X.left),Sy(X.bottom),Sx(X.right),Sy(X.top));
}

int Plot::StrWidth(const char *str)const{
	int w=0;
	for(const char *s=str;*s;s++){w+=fi[*s];}
	return w;
}
void Plot::TickH(double x,int e,Painter& sw){
	String lbl=DblStr(x);
	x*=ipow10(e);
	sw.DrawText(p.left+Sx(x)+1-StrWidth(lbl)*0.5,p.bottom+6,lbl,f,fontcol);
	sw.DrawRect(p.left+Sx(x)+1,p.bottom,-1,4,axiscol);
}
void Plot::TickV(double y,int e,Painter& sw){
	String lbl=DblStr(y);
	y*=ipow10(e);
	sw.DrawText(p.left-StrWidth(lbl)-6,Sy(y)+marginT-fi.GetHeight()*0.5,lbl,f,fontcol);
	sw.DrawRect(p.left,Sy(y)+marginT,-4,1,axiscol);
}
void Plot::NormalizeStep(double& s,int& e)const{
	s=normalize(s,e);
	if((s>2)&(s<=5)){s=5;}
	else if((s>1)&(s<=2)){s=2;}
	else if((s>5)&(s<=10)){s=1;e++;}
	else {s=1;}
}
bool Plot::Axes(Painter& sw){
	String str="×10";
	int fh=f.GetHeight();
	int e;
	double d,s; 
	//Ticks X
	s=g.Width()/p.Width()*spaceH; //step based on axis length and spacing
	NormalizeStep(s,e); // makes step s 1,2 or 5; e is order of magnitude
	if(fabs(log10(fabs(g.left-g.right))-e)>16){
		//TODO: find correct check
		g.SetSize(0,0);
		CheckRange();
		return false;
	}
	sx=ipow10(-e);
	d=s*ipow10(e); //full value of step
	for(double x=ceil(g.left/d)*s; x<=g.right*sx; x+=s){TickH(x,e,sw);}
	//Scale X
	sw.DrawText(p.right+spaceH/2,p.bottom+6,str,f,fontcol);
	sw.DrawText(p.right+spaceH/2+StrWidth(str)-2,p.bottom+4,IntStr(e),f.Height(fh*0.6),fontcol);
	f.Height(fh);
	//Ticks Y
	s=g.Height()/p.Height()*fh*2;
	NormalizeStep(s,e);
	if(fabs(log10(fabs(g.top-g.bottom))-e)>16){
		//TODO: find correct check
		g.SetSize(0,0);
		CheckRange();
		return false;
	}
	sy=ipow10(-e);
	d=s*ipow10(e);
	for(double y=ceil(g.top/d)*s; y<=g.bottom*sy; y+=s){TickV(y,e,sw);}
	//Scale y
	sw.DrawText(5,margin,str,f,fontcol);
	sw.DrawText(5+StrWidth(str)-2,margin-2,IntStr(e),f.Height(fh*0.6),fontcol);
	f.Height(fh);
	//Frame
	Rect t=p.Inflated(1);
	sw.DrawRect(t.left,t.bottom,p.Width()+5,-1,axiscol);
	sw.DrawRect(t.left,t.bottom,1,-p.Height()-5,axiscol);
	sw.DrawLine(t.left+p.Width()+6,p.bottom,t.left+p.Width()+2,p.bottom+4,1,axiscol);
	sw.DrawLine(t.left+p.Width()+6,p.bottom,t.left+p.Width()+2,p.bottom-3,1,axiscol);
	sw.DrawLine(p.left-1,p.bottom-p.Height()-4,p.left+3,p.bottom-p.Height(),1,axiscol);
	sw.DrawLine(p.left,p.bottom-p.Height()-4,p.left-4,p.bottom-p.Height(),1,axiscol);
	return true;
}
void Plot::PaintData(const PlotData& data,Painter& sw){
	if(data.visible&&data.GetCount()>=2){
		sw.Move(S(data[0]));
		for(int i=1;i<data.GetCount();i++) {
			double x1=data[i-1].x;   double y1=data[i-1].y;
			double x2=data[i].x;     double y2=data[i].y;
			switch(LineClip(x1,y1,x2,y2)){
			case 0:
				sw.Line(Sx(x2),Sy(y2));
				break;
			case 1: 
				sw.Move(Sx(x1),Sy(y1)).Line(Sx(x2),Sy(y2));
				break;
			case 2:
				sw.Line(Sx(x2),Sy(y2)).Dash(data.dash).Stroke(data.t,data.color);
				break;
			case 3:
				sw.Move(Sx(x1),Sy(y1)).Line(Sx(x2),Sy(y2)).Dash(data.dash).Stroke(data.t,data.color);
				break;
			}
		}
		if(g.Contains(data[data.GetCount()-1])){sw.Dash(data.dash).Stroke(data.t,data.color);}
		if(data.symbol&&data.symbol->GetSize().cx>0&&data.symbol->GetSize().cy>0){
			for(int i = 0; i < data.GetCount(); i++){
				if(g.Contains(data[i])){data.symbol->Paint(sw,S(data[i]));}
			}
		}
	}
}
int Plot::LineClip(double& x1, double& y1, double& x2, double& y2)const{
	int r=0;
	if(y1>g.bottom){
		if(y2>g.bottom) return -1; //no intersect
		x1=(x2-x1)*(g.bottom-y1)/(y2-y1)+x1;
		y1=g.bottom; 
		r=1; // 1st endpoint below viewport
	}else if(y1<g.top){
		if(y2<g.top) return -1; //no intersect
		x1=(x2-x1)*(g.top-y1)/(y2-y1)+x1;
		y1=g.top; 
		r=1; // 1st endpoint above viewport
	}
	if(y2>g.bottom){
		x2=(x2-x1)*(g.bottom-y1)/(y2-y1)+x1;
		y2=g.bottom; 
		r|=2; // 2nd endpoint below viewport
	}else if(y2<g.top){
		x2=(x2-x1)*(g.top-y1)/(y2-y1)+x1;
		y2=g.top; 
		r|=2; // 2nd endpoint above viewport
	}
	if(x1<g.left){
		if(x2<g.left) return -1; //no intersect
		y1=(y2-y1)*(g.left-x1)/(x2-x1)+y1;
		if(x2>g.right){
			y2=(y2-y1)*(g.right-x1)/(x2-x1)+y1;
			x1=g.left;
			x2=g.right;
			return 3; // both endpoints outside
		}
		x1=g.left;
		return r|1; // 1st endpoint outside
	}else if(x1>g.right){
		if(x2>g.right) return -1; //no intersect
		y1=(y2-y1)*(g.right-x1)/(x2-x1)+y1;
		if(x2<g.left){
			y2=(y2-y1)*(g.left-x1)/(x2-x1)+y1;
			x1=g.right;
			x2=g.left;
			return 3; // both endpoints outside
		}
		x1=g.right;
		return r|1; // 1st endpoint outside 
	}
	if(x2<g.left){
		y2=(y2-y1)*(g.left-x1)/(x2-x1)+y1;
		x2=g.left;
		return r|2; // 2nd endpoint outside
	}else if(x2>g.right){
		y2=(y2-y1)*(g.right-x1)/(x2-x1)+y1;
		x2=g.right;
		return r|2; // 2nd endpoint outside
	}
	return r; //depends on data gathered during clipping
}
//#include <plugin/png/png.h>
Image Plot::GetImage(){
	ImageBuffer ib(size);
	BufferPainter bp(ib, mode);
	bp.Clear(framecol);
	bp.DrawRect(p,bgcol);
	if(!Axes(bp)){LOG("Axes failed");return Image();} // something is wrong with limits, let user deal with it ;-)
	bp.Offset(p.TopLeft());
	 for(int i=0; i<data.GetCount(); i++){PaintData(data[i],bp);}
	bp.End();
//	static int i=0; PNGEncoder().SaveFile("/tmp/"+IntStr(i)+".png",ib); i++;
	return ib;
}
unsigned Plot::GetHashValue()const{
	CombineHash hash;
	for(int i = 0; i < data.GetCount(); i++){
		hash<<data[i].visible;
		hash<<data[i].t;
		hash<<data[i].title;
		hash<<data[i].dash;
		hash<<Color(data[i].color);
		hash<<((data[i].symbol)?data[i].symbol->GetHashValue():(unsigned)0);
	}
	return hash;
}
