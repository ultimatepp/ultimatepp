#include "ScatterCtrl.h"

#define IMAGECLASS ScatterImgP
#define IMAGEFILE <ScatterCtrl/ScatterCtrl.iml>
#include <Draw/iml_source.h>

void ScatterCtrl::DoShowEditDlg(int itab) 
{
	PropertiesDlg dlg;
	dlg.Init(*this);
	dlg.Set(itab);
	dlg.Perform();
}

void PropertiesDlg::Perform()
{
	Run();
	measures.Change();
}

void MeasuresTab::Init(ScatterCtrl& scatter) 
{
	CtrlLayout(*this);
	SizePos();
	
	pscatter = &scatter;
	
	xMin <<= scatter.GetXMin();
	xMax <<= scatter.GetXRange() + scatter.GetXMin();
	yMin <<= scatter.GetYMin();
	yMax <<= scatter.GetYRange() + scatter.GetYMin();
	yMin2 <<= scatter.GetYMin2();
	yMax2 <<= scatter.GetY2Range() + scatter.GetYMin2();
	
	opAttachX <<= !scatter.GetMouseHandlingX();
	opAttachY <<= !scatter.GetMouseHandlingY();
	
	xMin.WhenEnter = THISBACK(Change);
	xMax.WhenEnter = THISBACK(Change);
	yMin.WhenEnter = THISBACK(Change);
	yMax.WhenEnter = THISBACK(Change);
	yMin2.WhenEnter = THISBACK(Change);
	yMax2.WhenEnter = THISBACK(Change);
	
	opAttachX.WhenAction = THISBACK(Change);
	opAttachY.WhenAction = THISBACK(Change);
	
	Change();
}
	
void MeasuresTab::Change() 
{	
	if (xMax <= xMin) {
		Exclamation(t_("X min has to be lower than X max"));
		return;
	}
	if (yMax <= yMin) {
		Exclamation(t_("Y min has to be lower than Y max"));
		return;
	}
	if (yMax2 <= yMin2) {
		Exclamation(t_("Y min2 has to be lower than Y max2"));
		return;
	}
	
	xMin.Enable(!opAttachX);
	xMax.Enable(!opAttachX);
	yMin.Enable(!opAttachY);
	yMax.Enable(!opAttachY);
	
	ScatterCtrl &scatter = *pscatter;

	scatter.SetMouseHandlingLinked(!opAttachX, !opAttachY);
	
    scatter.SetXYMinLinked(xMin, yMin, yMin2);
    //scatter.SetMinUnits(xMin, yMin);
	scatter.SetRangeLinked(xMax - xMin, yMax - yMin, yMax2 - yMin2);

	scatter.SetModify();
	scatter.Refresh();
}

void TextsTab::Init(ScatterCtrl& scatter) 
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
	
void TextsTab::Change() 
{
	ScatterCtrl &scatter = *pscatter;
	
	scatter.SetTitle(~title);
    scatter.SetLabels(~xLabel, ~yLabel, ~yLabel2);
	scatter.SetPlotAreaMargin(~leftMargin, ~rightMargin, ~topMargin, ~bottomMargin);

	scatter.SetModify();
	scatter.Refresh();
}

void LegendTab::Init(ScatterCtrl& scatter) 
{
	CtrlLayout(*this);
	SizePos();
	
	pscatter = &scatter;
	
	showLegend <<= scatter.GetShowLegend();
	showLegend <<= THISBACK(Change);
	
	legendPosition <<= (scatter.GetLegendAnchor() == ScatterDraw::LEGEND_TOP ? 0 : 1);
	legendPosition <<= THISBACK(Change);
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

void LegendTab::ChangeAnchor(Option *op) 
{
	legendAnchorLT <<= false;
	legendAnchorRT <<= false;
	legendAnchorLB <<= false;
	legendAnchorRB <<= false;
	*op <<= true;
	
	Change();
}
	
void LegendTab::Change() 
{
	ScatterCtrl &scatter = *pscatter;
	
    scatter.ShowLegend(showLegend);
    scatter.SetLegendRowSpacing(rowSpacing);
    ScatterDraw::LEGEND_POS legendTableAnchor;
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
    else {
        legendAnchorLT = true;
        legendTableAnchor = ScatterDraw::LEGEND_ANCHOR_LEFT_TOP;
    }
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

void SeriesTab::Init(ScatterCtrl& scatter) 
{
	CtrlLayout(left);
	CtrlLayout(right);
	Horz(left.SizePos(), right.SizePos());
	SetPos(3000, 0);
	SizePos();
	
	pscatter = &scatter;
	
	left.list.Reset();
	left.list.SetLineCy(EditField::GetStdHeight());
	left.list.AddColumn(t_("Name"));
	for(int i = 0; i < scatter.GetCount(); i++)
		left.list.Add(scatter.GetLegend(i));
	left.list.SetCursor(0);
	left.list.WhenSel = THISBACK(UpdateFields);

	right.marktype.Clear();

	right.markstyle.Add(t_("No mark"));
	for(int i = 0; i < MarkPlot::GetCount(); i++)
		right.markstyle.Add(MarkPlot::TypeName(i));
	
	right.markstyle.SetIndex(0);
	
	for(int i = 0; i < DashStyle::GetCount(); i++)
		right.dashStyle.Add(DashStyle::TypeName(i));
	
	UpdateFields();

	right.linecolor <<= THISBACK(Change);
	right.fillcolor <<= THISBACK(Change);
	right.visible <<= THISBACK(Change);
	right.dashStyle.WhenAction = THISBACK(Change);
	right.linethickness <<= THISBACK(Change);

	right.markstyle.WhenAction = THISBACK(Change);
	right.markcolor <<= THISBACK(Change);
	right.markwidth <<= THISBACK(Change);
	right.marktype.WhenAction = THISBACK(Change);
	
	right.unitsY <<= THISBACK(Change);
	right.unitsX <<= THISBACK(Change);

	right.name <<= THISBACK(Change);
	right.primary <<= THISBACK(Change);
	
	right.name.SetFocus();
}

void SeriesTab::ChangeMark() {
	int index = left.list.GetCursor();
	if (index < 0)
		return;
	
	ScatterCtrl &scatter = *pscatter;
	
	int id = MarkPlot::TypeIndex(~right.markstyle);
	
	right.marktype.Clear();
	
	if (id >= 0) {
		for (int i = 0; i < MarkPlot::GetTypeCount(id); ++i)
			right.marktype.Add(MarkPlot::TypeString(id, i));
	}
	if (right.marktype.GetCount() > 0)
		right.marktype.SetIndex(0);
	int idStyle = scatter.GetMarkStyleType(index);
	if (idStyle >= 0 && idStyle < right.marktype.GetCount())
		right.marktype.SetIndex(idStyle);
}

void SeriesTab::Change() 
{
	int index = left.list.GetCursor();
	if (index < 0)
		return;
	
	ScatterCtrl &scatter = *pscatter;
	
	scatter.SetFillColor(index, ~right.fillcolor);
	scatter.ScatterDraw::Show(index, ~right.visible);
	scatter.Dash(index, DashStyle::Style(DashStyle::TypeIndex(~right.dashStyle)));
	scatter.Stroke(index, ~right.linethickness, Upp::Color(~right.linecolor));
	
	scatter.MarkStyle(index, String(~right.markstyle));
	scatter.SetMarkColor(index, Upp::Color(~right.markcolor));
	scatter.SetMarkWidth(index, ~right.markwidth);
	ChangeMark();
	
	scatter.Units(index, ~right.unitsY, ~right.unitsX);
                         
	scatter.SetDataPrimaryY(index, ~right.primary);
      
	left.list.Set(index, ~right.name);
	scatter.Legend(index, ~right.name);
                   
	scatter.SetModify();
	scatter.Refresh();
}

void SeriesTab::UpdateFields() 
{
	int index = left.list.GetCursor();
	if (index < 0)
		return;
	
	ScatterCtrl &scatter = *pscatter;
	
	right.name <<= left.list.Get(0);
	
	right.fillcolor <<= scatter.GetFillColor(index);
	right.visible <<= scatter.ScatterDraw::IsVisible(index);
	int id = DashStyle::StyleIndex(scatter.GetDash(index));
	if (id < 0) {
		id = DashStyle::Register(Format(t_("Dash \"%s\""), scatter.GetDash(index)), scatter.GetDash(index));
		right.dashStyle.Add(DashStyle::TypeName(id));
	}
	right.dashStyle <<= DashStyle::TypeName(id);
	
	Upp::Color color;
	double thickness;
	
	scatter.GetStroke(index, thickness, color);
	right.linethickness <<= thickness;
	right.linecolor <<= color;
	right.markstyle <<= scatter.GetMarkStyleName(index);
	right.markcolor <<= scatter.GetMarkColor(index);
	right.markwidth <<= scatter.GetMarkWidth(index);
	ChangeMark();
	
	right.unitsY <<= scatter.GetUnitsY(index);
	right.unitsX <<= scatter.GetUnitsX(index);
	
	right.primary <<= scatter.IsDataPrimaryY(index);
}
