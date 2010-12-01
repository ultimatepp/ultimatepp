#include "PlotLib.h"

/* PlotData */

PlotData& PlotData::SetSymbol(String Symbol,Sizef size,RGBA color,RGBA frame,double thickness){
	symbol=PlotSymbol::Create(PlotSymbol::TypeIndex(Symbol));
	if(size.cx<0||size.cy<0){size.cx=size.cy=0;}
	if(symbol->GetThickness()<0){thickness=1;}
	symbol->SetSize(size).SetColor(color).SetFrameColor(frame).SetThickness(thickness);
	return *this;
}
PlotData& PlotData::SetSymbol(String Symbol){
	symbol=PlotSymbol::Create(PlotSymbol::TypeIndex(Symbol));
	if(symbol->GetSize().cx<0||symbol->GetSize().cy<0){symbol->SetSize(Sizef(0,0));}
	if(symbol->GetThickness()<0){symbol->SetThickness(1);}
	return *this;
}
PlotData& PlotData::SetSymbol(PlotSymbol* Symbol){
	symbol=Symbol->Copy();
	return *this;
}

Image PlotData::GetIcon(){
	ImageBuffer ib(40,20);
	BufferPainter bp(ib);
	bp.Clear(White());
	bp.Dash(dash).DrawLine(0,20,40,0,t,color);
	if(symbol){
		symbol->Paint(bp,Pointf(20,10));
	}
	return ib;
}
void PlotData::AddXY(double x,double y){Vector<Pointf>::Add(Pointf(x,y));}

String PlotData::ToString()const{
	return title+(visible?" (visible) ":" (invisible) ");
}

PlotData::PlotData(){
	SetDefaults();
}
PlotData::PlotData(const PlotData& pd,int):
	Vector<Pointf>(pd,1),
	color(pd.color),
	t(pd.t),
	dash(pd.dash),
	title(pd.title),
	visible(pd.visible),
	BB(pd.BB)
{
	symbol=pd.symbol->Copy();
}

PlotData::PlotData(const Vector<Pointf>& data,const char* name){
	SetDefaults();
	SetCount(data.GetCount());
	for(int i=0;i<data.GetCount();i++){
		(*this)[i]=data[i];
//		BB.Union(data[i]);
	}
}
PlotData::PlotData(const Array<Pointf>& data,const char* name){
	SetDefaults();
	SetCount(data.GetCount());
	for(int i=0;i<data.GetCount();i++){
		(*this)[i]=data[i];
//		BB.Union(data[i]);
	}
}
PlotData& PlotData::SetDefaults(){
	color=LtBlue();
	dash="";
	title="";
	visible=true;
	t=1;
	BB=Null;
	symbol=PlotSymbol::Create(0);
	return *this;
}
void PlotData::RefreshBB(){
	BB=Null;
	for(int i = 0; i < GetCount(); i++){
		BB.Union((*this)[i]);
	}
	if(BB.IsNullInstance()){BB.Clear();}
}
