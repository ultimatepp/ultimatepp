#include "ScatterCtrl.h"

NAMESPACE_UPP

#define IMAGECLASS ScatterImgP
#define IMAGEFILE <ScatterCtrl/ScatterCtrl.iml>
#include <Draw/iml.h>

#define  LAYOUTFILE <ScatterCtrl/ScatterCtrl.lay>
#include <CtrlCore/lay.h>

#include "Properties.h"	

void ScatterCtrl::DoShowEditDlg(int itab) {
	PropertiesDlg(*this, itab).Run(true);
}

void ScatterCtrl::DoShowData() {
	DataDlg(*this).Run(true);
}

void MeasuresTab::Init(ScatterCtrl& scatter) {
	CtrlLayout(*this);
	SizePos();
	
	pscatter = &scatter;
	
	xMin <<= scatter.GetXMin();
	xMax <<= scatter.GetXRange() + scatter.GetXMin();
	yMin <<= scatter.GetYMin();
	yMax <<= scatter.GetYRange() + scatter.GetYMin();
	yMin2 <<= scatter.GetYMin2();
	yMax2 <<= scatter.GetY2Range() + scatter.GetYMin2();
	
	butUpdate.WhenAction = THISBACK(Change);
	
	Change();
}
	
void MeasuresTab::Change() {
	ScatterCtrl &scatter = *pscatter;
	
    scatter.SetXYMin(xMin, yMin, yMin2);
    scatter.SetMinUnits(xMin, yMin);
	scatter.SetRange(xMax - xMin, yMax - yMin, yMax2 - yMin2);

	scatter.SetModify();
	scatter.Refresh();
}

void TextsTab::Init(ScatterCtrl& scatter) {
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
	leftMargin <<= scatter.GetPlotAreaLeftMargin();
	leftMargin <<= THISBACK(Change);
	rightMargin <<= scatter.GetPlotAreaRightMargin();
	rightMargin <<= THISBACK(Change);
	topMargin <<= scatter.GetPlotAreaTopMargin();
	topMargin <<= THISBACK(Change);
	bottomMargin <<= scatter.GetPlotAreaBottomMargin();
	bottomMargin <<= THISBACK(Change);
	
	Change();
}
	
void TextsTab::Change() {
	ScatterCtrl &scatter = *pscatter;
	
	scatter.SetTitle(title);
    scatter.SetLabels(xLabel, yLabel, yLabel2);
	scatter.SetPlotAreaMargin(~leftMargin, ~rightMargin, ~topMargin, ~bottomMargin);

	scatter.SetModify();
	scatter.Refresh();
}

void LegendTab::Init(ScatterCtrl& scatter) {
	CtrlLayout(*this);
	SizePos();
	
	pscatter = &scatter;
	
	showLegend <<= scatter.GetShowLegend();
	showLegend <<= THISBACK(Change);
	
	legendPosition <<= (scatter.GetLegendAnchor() == ScatterDraw::LEGEND_TOP ? 0 : 1);
	legendPosition <<= THISBACK(Change);
	legendAnchorRT = true;	
	switch(scatter.GetLegendAnchor()) {
	case ScatterDraw::LEGEND_ANCHOR_LEFT_TOP: 		legendAnchorLT = true;	break;
	case ScatterDraw::LEGEND_ANCHOR_RIGHT_TOP: 		legendAnchorRT = true;	break;
	case ScatterDraw::LEGEND_ANCHOR_LEFT_BOTTOM: 	legendAnchorLB = true;	break;
	case ScatterDraw::LEGEND_ANCHOR_RIGHT_BOTTOM: 	legendAnchorRB = true;	break;
	}
	legendAnchorLT <<= THISBACK1(ChangeAnchor, &legendAnchorLT);
	legendAnchorRT <<= THISBACK1(ChangeAnchor, &legendAnchorRT);
	legendAnchorLB <<= THISBACK1(ChangeAnchor, &legendAnchorLB);
	legendAnchorRB <<= THISBACK1(ChangeAnchor, &legendAnchorRB);
	
	fillColor <<= scatter.GetLegendFillColor();
	fillColor <<= THISBACK(Change);
	borderColor <<= scatter.GetLegendBorderColor();
	borderColor <<= THISBACK(Change);
	Point p = scatter.GetLegendPos();	
	tableHoriz <<= p.x;
	tableHoriz <<= THISBACK(Change);
	tableVert <<= p.y;
	tableVert <<= THISBACK(Change);
	numCols <<= scatter.GetLegendNumCols();
	numCols <<= THISBACK(Change);
	
	rowSpacing <<= scatter.GetLegendRowSpacing();
	rowSpacing <<= THISBACK(Change);
	
	Change();
}

void LegendTab::ChangeAnchor(Option *op) {
	legendAnchorLT <<= false;
	legendAnchorRT <<= false;
	legendAnchorLB <<= false;
	legendAnchorRB <<= false;
	*op <<= true;
	
	Change();
}
	
void LegendTab::Change() {
	ScatterCtrl &scatter = *pscatter;
	
    scatter.ShowLegend(showLegend);
    scatter.SetLegendRowSpacing(rowSpacing);
    int legendTableAnchor;
    if (legendPosition == 0)
        legendTableAnchor = ScatterDraw::LEGEND_TOP;
    else if (legendAnchorLT)
        legendTableAnchor = ScatterDraw::LEGEND_ANCHOR_LEFT_TOP;
    else if (legendAnchorRT)
        legendTableAnchor = ScatterDraw::LEGEND_ANCHOR_RIGHT_TOP;
    else if (legendAnchorLB)
        legendTableAnchor = ScatterDraw::LEGEND_ANCHOR_LEFT_BOTTOM;
    else if (legendAnchorRB)
        legendTableAnchor = ScatterDraw::LEGEND_ANCHOR_RIGHT_BOTTOM;
    scatter.SetLegendAnchor(legendTableAnchor);
    bool enable = (legendPosition != 0);
	table.Enable(enable);    
	fillColor.Enable(enable);		borderColor.Enable(enable);
	tableHoriz.Enable(enable);		tableVert.Enable(enable);
	numCols.Enable(enable);			rowSpacing.Enable(enable);
	labelFill.Enable(enable);		labelBorder.Enable(enable);
	labelDistance.Enable(enable);
	labelHoriz.Enable(enable);		labelVert.Enable(enable);
	labelNumCols.Enable(enable);	labelRowSpacing.Enable(enable);
	labelAnchorCorner.Enable(enable);	
	legendAnchorLT.Enable(enable);	legendAnchorRT.Enable(enable);
	legendAnchorLB.Enable(enable);	legendAnchorRB.Enable(enable);	
		
	scatter.SetLegendFillColor(fillColor.GetData()).SetLegendBorderColor(borderColor.GetData());
	scatter.SetLegendPosX(tableHoriz).SetLegendPosY(tableVert);
	scatter.SetLegendNumCols(numCols);
    
	scatter.SetModify();
	scatter.Refresh();
}

void SeriesTab::Init(ScatterCtrl& scatter) {
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

void SeriesTab::Change() {
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

void SeriesTab::UpdateFields() {
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


void DataDlg::OnClose() {
	RejectBreak(IDOK);
	Close();	Close();
}

DataDlg::DataDlg(ScatterCtrl& scatter) 
{
	CtrlLayout(*this);
	Sizeable().Zoomable();
	
	Title(t_("Scatter data"));
	close <<= THISBACK(OnClose);
	close.Exit();
	
	pscatter = &scatter;

	tab.Reset();
	series.Clear();
	for(int c = 0; c < scatter.GetCount(); c++) 
		if (!IsNull(scatter.GetCount(c))) {
			WithDataSeries <StaticRect> &dataseries = series.Add();
			CtrlLayout(dataseries);
			dataseries.scatterIndex.Hide();
			dataseries.scatterIndex = c;
			tab.Add(dataseries.SizePos(), scatter.GetLegend(c));
		}
	OnTab(); 
	
	tab <<= THISBACK(OnTab);
}

Value DataDlg::DataSourceX::Format(const Value& q) const 
{
	return pscatter->GetValueX(index, q);
}

Value DataDlg::DataSourceY::Format(const Value& q) const 
{
	return pscatter->GetValueY(index, q);
}

void DataDlg::OnTab() 
{
	int index = tab.Get();
	if (index < 0)
		return;
	
	ScatterCtrl &scatter = *pscatter;
	ArrayCtrl &data = series[index].data;	
	int scatterIndex = series[index].scatterIndex;
	data.Reset();
	data.MultiSelect().SetLineCy(EditField::GetStdHeight());
	data.SetVirtualCount(int(scatter.GetCount(scatterIndex)));
	dataSourceX.pscatter = dataSourceY.pscatter = pscatter;
	dataSourceX.index = dataSourceY.index = scatterIndex;
	String textX = pscatter->GetLabelX() + 
				   (pscatter->GetUnitsX(scatterIndex).IsEmpty() ? "" : " [" + pscatter->GetUnitsX(scatterIndex) + "]"); 
	String textY = pscatter->GetLegend(scatterIndex) + 
				   (pscatter->GetUnitsY(scatterIndex).IsEmpty() ? "" : " [" + pscatter->GetUnitsY(scatterIndex) + "]"); 
	data.AddRowNumColumn(textX).SetConvert(dataSourceX);
	data.AddRowNumColumn(textY).SetConvert(dataSourceY);	
	data.WhenBar = THISBACK(OnArrayBar);
}

void DataDlg::ArrayCopy() {
	int index = tab.Get();
	if (index < 0)
		return;
	
	ArrayCtrl &data = series[index].data;
	data.SetClipboard(true, true);
}

void DataDlg::ArraySelect() {
	int index = tab.Get();
	if (index < 0)
		return;
	
	ArrayCtrl &data = series[index].data;
	data.Select(0, data.GetCount(), true);
}

void DataDlg::OnArrayBar(Bar &menu) {
	menu.Add(t_("Select all"), Null, THISBACK(ArraySelect)).Key(K_CTRL_A).Help(t_("Select all rows"));
	menu.Add(t_("Copy"), ScatterImgP::Copy(), THISBACK(ArrayCopy)).Key(K_CTRL_C).Help(t_("Copy selected rows"));
}

PropertiesDlg::PropertiesDlg(ScatterCtrl& scatter, int itab) : scatter(scatter) 
{
	CtrlLayout(*this);
	Sizeable().Zoomable();
	
	Title(t_("Scatter properties"));
	
	tab.Add(measures, t_("Measures"));
	tab.Add(texts, t_("Texts"));
	tab.Add(legend, t_("Legend"));
	tab.Add(series, t_("Series"));
	tab.Set(itab);
	OnTab(); 
	
	tab <<= THISBACK(OnTab);
	close <<= THISBACK(OnClose);
	close.Exit();
}

void PropertiesDlg::OnTab() 
{
	if (tab.IsAt(measures))
		measures.Init(scatter);
	else if (tab.IsAt(texts))
		texts.Init(scatter);
	else if (tab.IsAt(legend))
		legend.Init(scatter);
	else if (tab.IsAt(series))
		series.Init(scatter);
}

void PropertiesDlg::OnClose() {
	measures.Change();
	
	RejectBreak(IDOK);
	Close();	Close();
}

END_UPP_NAMESPACE