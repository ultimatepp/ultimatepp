#include "PlotLib.h"

Legend::Legend(Plot& plot):
	p(plot),fwidth(1),framecol(Black()),bgcol(RGBAZero()){size=GetSizeHint();}

Legend::Legend(Plot& plot,RGBA frame,RGBA background,double width):
	p(plot),fwidth(width),framecol(frame),bgcol(background){size=GetSizeHint();}

Size Legend::GetSizeHint()const{
	int maxlen=0;
	int vis=0;
	for(int i=0; i<p.data.GetCount(); i++){
		maxlen=max(maxlen,p.StrWidth(p.data[i].title));
		if(p.data[i].visible){vis++;}
	}
	return Size(maxlen+60,2*p.f.GetHeight()*vis);
}
Image Legend::GetLegend()const{
	ImageBuffer ib(size);
	BufferPainter bp(ib,p.mode);
	bp.Clear(bgcol);
	bp.Rectangle(fwidth*0.5,fwidth*0.5,ib.GetSize().cx-fwidth,ib.GetSize().cy-fwidth)
	  .Stroke(fwidth,framecol);
	int h=p.f.GetHeight();
	for(int i=0; i<p.data.GetCount(); i++){
		if(!p.data[i].visible){continue;}
		int y=2*h*i+3*h/2;
		bp.Dash(p.data[i].dash).DrawLine(10,y,40,y-h,p.data[i].t,p.data[i].color);
		bp.DrawText(50,y-h,p.data[i].title);
		if(p.data[i].symbol){
			p.data[i].symbol->Paint(bp,Pointf(25,y-h*0.5));
		}
	}
	return ib;
}

unsigned Legend::GetHashValue()const{
	CombineHash hash;
	hash<<p.GetHashValue();
	hash<<size.cx<<size.cy;
	hash<<Color(bgcol)<<Color(framecol);
	return hash;
}

Legend& Legend::SetPlot(Plot& plot){
	p=pick(plot);
	return *this;
}
