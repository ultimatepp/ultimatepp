#include "ScatterCtrl.h"

NAMESPACE_UPP

#define IMAGECLASS ScatterImgP
#define IMAGEFILE <ScatterCtrl/ScatterCtrl.iml>
#include <Draw/iml.h>

#define  LAYOUTFILE <ScatterCtrl/ScatterCtrl.lay>
#include <CtrlCore/lay.h>

#include "Properties.h"

void ScatterCtrl::DoShowEditDlg() 
{
	PropertiesDlg(*this).Run(true);
}

void GeneralTab::Init(ScatterCtrl& scatter) 
{
	CtrlLayout(*this);
	SizePos();
	
	pscatter = &scatter;
	
	title <<= scatter.GetTitle();
	title <<= THISBACK(Change);
	xLabel <<= scatter.GetLabelX();
	xLabel <<= THISBACK(Change);
	yLabel <<= scatter.GetLabelY();
	yLabel <<= THISBACK(Change);
	yLabel2 <<= scatter.GetLabelY2();
	yLabel2 <<= THISBACK(Change);
	showLegend <<= scatter.GetShowLegend();
	showLegend <<= THISBACK(Change);
	setLegendWidth <<= scatter.GetLegendWidth();
	setLegendWidth <<= THISBACK(Change);
}

void GeneralTab::Change() 
{
	ScatterCtrl &scatter = *pscatter;
	
	scatter.SetTitle(title);
    scatter.SetLabels(xLabel, yLabel, yLabel2);
    scatter.ShowLegend(showLegend);
    scatter.SetLegendWidth(setLegendWidth);
                  
	scatter.SetModify();
	scatter.Refresh();
}

void SeriesTab::Init(ScatterCtrl& scatter) 
{
	CtrlLayout(*this);
	SizePos();
	
	pscatter = &scatter;
	
	list.Reset();
	list.SetLineCy(EditField::GetStdHeight());
	list.AddColumn(t_("Name"));
	for(int i = 0; i < scatter.GetCount(); i++) 
		list.Add(scatter.GetLegend(i));
	list.SetCursor(0);
	list.WhenSel = THISBACK(UpdateFields);

	markstyle.Add(t_("No mark"));
	for(int i = 0; i < MarkPlot::GetCount(); i++)
		markstyle.Add(MarkPlot::TypeName(i));
	
	markstyle.SetIndex(0);
	UpdateFields();

	linecolor <<= THISBACK(Change);
	fillcolor <<= THISBACK(Change);
	visible <<= THISBACK(Change);
	dash <<= THISBACK(Change);
	linethickness <<= THISBACK(Change);

	markstyle <<= THISBACK(Change);
	markcolor <<= THISBACK(Change);
	markwidth <<= THISBACK(Change);

	unitsY <<= THISBACK(Change);
	unitsX <<= THISBACK(Change);

	name <<= THISBACK(Change);
	primary <<= THISBACK(Change);
	
	name.SetFocus();
}

void SeriesTab::Change() 
{
	int index = list.GetCursor();
	if (index < 0)
		return;
	
	ScatterCtrl &scatter = *pscatter;
	
	scatter.SetDataColor(index, Upp::Color(~linecolor));
	scatter.SetFillColor(index, ~fillcolor);
	scatter.ScatterDraw::Show(index, ~visible);
	scatter.Dash(index, dash.GetData().ToString());
	scatter.SetDataThickness(index, ~linethickness);
	
	scatter.MarkStyle(index, ~markstyle);
	scatter.SetMarkColor(index, Upp::Color(~markcolor));
	scatter.SetMarkWidth(index, ~markwidth);
	
	scatter.Units(index, ~unitsY, ~unitsX);
                         
 	scatter.SetDataPrimaryY(index, ~primary);                   
      
	list.Set(index, ~name);
	scatter.Legend(index, ~name);
                   
	scatter.SetModify();
	scatter.Refresh();
}

void SeriesTab::UpdateFields() 
{
	int index = list.GetCursor();
	if (index < 0)
		return;
	
	ScatterCtrl &scatter = *pscatter;
	
	name <<= list.Get(0);
	
	linecolor <<= scatter.GetDataColor(index);
	fillcolor <<= scatter.GetFillColor(index);
	visible <<= scatter.ScatterDraw::IsVisible(index);
	dash <<= scatter.GetDash(index);
	linethickness <<= scatter.GetDataThickness(index);

	markstyle <<= scatter.GetMarkStyleName(index);
	markcolor <<= scatter.GetMarkColor(index);
	markwidth <<= scatter.GetMarkWidth(index);
	
	unitsY <<= scatter.GetUnitsY(index);
	unitsX <<= scatter.GetUnitsX(index);
	
	primary <<= scatter.IsDataPrimaryY(index);
}


void DataTab::Init(ScatterCtrl& scatter) 
{
	CtrlLayout(*this);
	SizePos();
	
	pscatter = &scatter;

	tab.Reset();
	series.Clear();
	for(int c = 0; c < scatter.GetCount(); c++) 
		if (!IsNull(scatter.GetCount(c))) {
			WithDataSeries <StaticRect> &dataseries = series.Add();
			CtrlLayout(dataseries);
			tab.Add(dataseries.SizePos(), scatter.GetLegend(c));
		}
	OnTab(); 
	
	tab <<= THISBACK(OnTab);
}

Value DataTab::DataSourceX::Format(const Value& q) const 
{
	return pscatter->GetValueX(index, q);
}

Value DataTab::DataSourceY::Format(const Value& q) const 
{
	return pscatter->GetValueY(index, q);
}

void DataTab::OnTab() 
{
	int index = tab.Get();
	if (index < 0)
		return;
	
	ScatterCtrl &scatter = *pscatter;
	ArrayCtrl &data = series[index].data;	
	data.Reset();
	data.MultiSelect().SetLineCy(EditField::GetStdHeight());
	data.SetVirtualCount(int(scatter.GetCount(index)));
	dataSourceX.pscatter = dataSourceY.pscatter = pscatter;
	dataSourceX.index = dataSourceY.index = index;
	data.AddRowNumColumn("x").SetConvert(dataSourceX);
	data.AddRowNumColumn("y").SetConvert(dataSourceY);	
	data.WhenBar = THISBACK(OnArrayBar);
}

void DataTab::ArrayCopy() {
	int index = tab.Get();
	if (index < 0)
		return;
	
	ArrayCtrl &data = series[index].data;
	data.SetClipboard(true, true);
}

void DataTab::ArraySelect() {
	int index = tab.Get();
	if (index < 0)
		return;
	
	ArrayCtrl &data = series[index].data;
	data.Select(0, data.GetCount(), true);
}

void DataTab::OnArrayBar(Bar &menu) {
	menu.Add(t_("Select all"), Null, THISBACK(ArraySelect)).Key(K_CTRL_A).Help(t_("Select all rows"));
	menu.Add(t_("Copy"), ScatterImgP::Copy(), THISBACK(ArrayCopy)).Key(K_CTRL_C).Help(t_("Copy selected rows"));
}

PropertiesDlg::PropertiesDlg(ScatterCtrl& scatter) : scatter(scatter) 
{
	CtrlLayout(*this);
	
	Title(t_("Scatter properties"));
	
	tab.Add(general, t_("General"));
	tab.Add(series, t_("Series"));
	tab.Add(data, t_("Data"));
	OnTab(); 
	
	tab <<= THISBACK(OnTab);
	close <<= THISBACK(OnClose);
}

void PropertiesDlg::OnTab() 
{
	if (tab.IsAt(general))
		general.Init(scatter);
	else if (tab.IsAt(series))
		series.Init(scatter);
	else if (tab.IsAt(data))
		data.Init(scatter);
}

void PropertiesDlg::OnClose() {
	RejectBreak(IDOK);
	Close();	Close();
}

END_UPP_NAMESPACE