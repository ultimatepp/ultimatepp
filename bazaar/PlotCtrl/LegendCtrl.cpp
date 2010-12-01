#include "PlotCtrl.h"

LegendCtrl::LegendCtrl(Plot& p):Legend(p){
	IgnoreMouse();
	Transparent();
}

void LegendCtrl::Paint(Draw& w){
	Legend::SetSize(Ctrl::GetSize());
	if(hash!=Legend::GetHashValue()){
		img=GetLegend();
		if(img.IsEmpty()){
			Refresh();
			Sync();
			return;
		}else{
			hash=Legend::GetHashValue();
			WhenSync();
		}
	}
	w.DrawImage(0,0,img);
}

Size LegendCtrl::GetSize()const{
	return Ctrl::GetSize();
}
void LegendCtrl::SetSize(const Size& sz){
	Rect r=GetRect();
	r.SetSize(sz);
	SetRect(r);
	//Legend::SetSize(sz);
}

