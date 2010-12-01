#include "PlotCtrl.h"

PlotStyleDlg::PlotStyleDlg(PlotCtrl& parent,int index):p(parent){
	CtrlLayout(*this);
	for(int i=0; i<p.data.GetCount(); i++){
		series.Add(p.data[i].GetTitle());
	}
	series.SetIndex(index);
	
	for(int i = 0; i < PlotSymbol::GetCount(); i++){
		symbol.Add(PlotSymbol::TypeName(i));
	}
	symbol.SetIndex(0);
	UpdateFields();
	series<<=THISBACK(UpdateFields);
	symbol<<=THISBACK(Change);
	scol<<=THISBACK(Change);
	frame<<=THISBACK(Change);
	sthick<<=THISBACK(Change);
	sizex<<=THISBACK(Change);
	sizey<<=THISBACK(Change);
	col<<=THISBACK(Change);
	vis<<=THISBACK(Change);
	dash<<=THISBACK(Change);
	thick<<=THISBACK(Change);
	rename<<=THISBACK(Rename);
	btn<<=THISBACK(Close);
	series.SetFocus();
}
void PlotStyleDlg::Rename(){
	int idx=series.GetIndex();
	RenameDlg(p,idx).Run();
	series.SetValue(idx,p.data[idx].GetTitle());
}
void PlotStyleDlg::Change(){
	p.data[series.GetIndex()].SetColor(Color(~col))
	                          .Visible(~vis)
	                          .SetDash(~dash)
	                          .SetThickness(~thick)
	                          .SetSymbol(~symbol,Sizef(~sizex,~sizey),Color(~scol),Color(~frame),~sthick);
	//DUMP(~vis);DUMP(Color(~scol));DUMP(Color(~frame));DUMP(~sthick);
	p.SetModify();
	p.Refresh();
	p.Sync();
}

void PlotStyleDlg::UpdateFields(){
	PlotData& sel=p.data[series.GetIndex()];
	PlotSymbol* symb=sel.GetSymbol();
	symbol.SetIndex(symb->GetType());
	scol<<=Color(symb->GetColor());
	frame<<=Color(symb->GetFrameColor());
	sthick<<=symb->GetThickness()>0?symb->GetThickness():1;
	sizex<<=symb->GetSize().cx;
	sizey<<=symb->GetSize().cy;
	col<<=Color(sel.GetColor());
	vis<<=sel.IsVisible();
	dash<<=sel.GetDash();
	thick<<=sel.GetThickness()>0?sel.GetThickness():1;
}

PlotStyleDlg::RenameDlg::RenameDlg(PlotCtrl& plot,int index):p(plot),idx(index){
	CtrlLayout(*this);
	name<<=p.data[idx].GetTitle();
	name.SetSelection();
	name<<=THISBACK(Update);
	ok<<=THISBACK(Close);
}

void PlotStyleDlg::RenameDlg::Update(){
	p.data[idx].SetTitle(~name);
	p.SetModify();
	p.Refresh();
}

