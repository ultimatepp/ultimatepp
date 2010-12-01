#include "PlotSymbol.h"

PlotSymbol::PlotSymbol():
	s(0,0),
	c(RGBAZero()),
	f(RGBAZero()),
	t(0)
{
	paint=THISBACK(PaintOp);
};
PlotSymbol::PlotSymbol(Sizef size,RGBA color,RGBA frame,double thickness):
	s(size),c(color),f(frame),t(thickness)
{
	paint=THISBACK(PaintOp);
};

void PlotSymbol::Paint(Painter& sw,Pointf pos)const{
	if(hash!=GetHashValue()){Refresh();}
	//sw.Rectangle(pos.x-s.cx*0.5,pos.y-s.cy*0.5,s.cx,s.cy);
	//sw.Fill(img,Pointf(pos.x-s.cx*0.5,pos.y-s.cy*0.5),Pointf(pos.x+s.cx*0.5,pos.y+s.cy*0.5));
	sw.Offset(pos.x-s.cx*0.5,pos.y-s.cy*0.5);
	 sw.Rectangle(0,0,s.cx,s.cy).Fill(img,0,0,s.cx,0);
	sw.End();
}
void PlotSymbol::DoPaint(Painter& sw)const{
	PaintOp(sw);
}
unsigned PlotSymbol::GetHashValue()const{
	return CombineHash(Color(c),Color(f),s,t);
}
void PlotSymbol::Refresh()const{
	if(s.cx<=0||s.cy<=0){img=Image();return;}
	ImageBuffer ib(s);
	BufferPainter bp(ib);
	bp.Clear(RGBAZero());
	DoPaint(bp);
	img=ib;
	hash=GetHashValue();
}

String PlotSymbol::ToString()const{
	return String().Cat()<<s.cx<<"x"<<s.cy<<";"<<Color(c)<<";"<<Color(f)<<";"<<t;
}

PlotSymbol* PlotSymbol::Copy()const{
	One<PlotSymbol> tmp(Create(GetType()));
	tmp->s=s;
	tmp->c=c;
	tmp->f=f;
	tmp->t=t;
	return tmp.Detach();
}

INITBLOCK{
	PlotSymbol::Register<PlotSymbol>("None");
	PlotSymbol::Register<Cross>("Cross");
	PlotSymbol::Register<XCross>("Diagonal cross");
	PlotSymbol::Register<Circle>("Circle");
	PlotSymbol::Register<Square>("Square");
	PlotSymbol::Register<Star>("Star");
}
