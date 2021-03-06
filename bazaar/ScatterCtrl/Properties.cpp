#include "ScatterCtrl.h"

namespace Upp {

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
	xMinUnit	<<= scatter.GetXMinUnit();
	yMinUnit	<<= scatter.GetYMinUnit();
	xMajorUnit	<<= scatter.GetMajorUnitsX();
	yMajorUnit	<<= scatter.GetMajorUnitsY();
	
	opAttachX <<= !scatter.GetMouseHandlingX();
	opAttachY <<= !scatter.GetMouseHandlingY();
	opReticleX <<= scatter.GetDrawXReticle();
	opReticleY <<= scatter.GetDrawYReticle();
	opReticleY2 <<= scatter.GetDrawY2Reticle();
	opReticleXNumbers <<= scatter.GetDrawXReticleNumbers();
	opReticleYNumbers <<= scatter.GetDrawYReticleNumbers();
	opReticleY2Numbers <<= scatter.GetDrawY2ReticleNumbers();
	reticlethickness <<= scatter.GetAxisWidth();
	
	linethickness <<= scatter.GetGridWidth();
	linecolor <<= scatter.GetGridColor();
	dashStyle.Clear();
	for(int i = 0; i < DashStyle::GetCount(); i++)
		dashStyle.Add(DashStyle::TypeName(i));
	
	int id = DashStyle::StyleIndex(scatter.GetGridDash());
	if (id < 0) {
		id = DashStyle::Register(Format(t_("Dash \"%s\""), scatter.GetGridDash()), scatter.GetGridDash());
		dashStyle.Add(DashStyle::TypeName(id));
	}
	dashStyle <<= DashStyle::TypeName(id);
	
	xMin.WhenEnter = [=]{Change();};
	xMax.WhenEnter = [=]{Change();};
	yMin.WhenEnter = [=]{Change();};
	yMax.WhenEnter = [=]{Change();};
	yMin2.WhenEnter = [=]{Change();};
	yMax2.WhenEnter = [=]{Change();};
	xMinUnit.WhenEnter = [=]{Change();};
	yMinUnit.WhenEnter = [=]{Change();};
	xMajorUnit.WhenEnter = [=]{Change();};
	yMajorUnit.WhenEnter = [=]{Change();};
	
	opAttachX.WhenAction = [=]{Change();};
	opAttachY.WhenAction = [=]{Change();};
	opReticleX.WhenAction = [=]{Change();};
	opReticleY.WhenAction = [=]{Change();};
	opReticleY2.WhenAction = [=]{Change();};
	opReticleXNumbers.WhenAction = [=]{Change();};
	opReticleYNumbers.WhenAction = [=]{Change();};
	opReticleY2Numbers.WhenAction = [=]{Change();};
	reticlethickness.WhenAction = [=]{Change();};
	
	linecolor.WhenAction = [=]{Change();};
	linethickness.WhenAction = [=]{Change();};
	dashStyle.WhenAction = [=]{Change();};
	
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
	scatter.SetDrawXReticle(opReticleX).SetDrawYReticle(opReticleY).SetDrawY2Reticle(opReticleY2);
	scatter.SetDrawXReticleNumbers(opReticleXNumbers).SetDrawYReticleNumbers(opReticleYNumbers).SetDrawY2ReticleNumbers(opReticleY2Numbers);
	scatter.SetAxisWidth(~reticlethickness);
	
    scatter.SetXYMinLinked(xMin, yMin, yMin2);
	scatter.SetRangeLinked(xMax - xMin, yMax - yMin, yMax2 - yMin2);

	scatter.SetMinUnits(xMinUnit, yMinUnit);
	scatter.SetMajorUnits(xMajorUnit, yMajorUnit);

	scatter.SetGridDash(DashStyle::Style(DashStyle::TypeIndex(~dashStyle)));
	scatter.SetGridColor(~linecolor);
	scatter.SetGridWidth(~linethickness);

	scatter.SetModify();
	scatter.Refresh();
}

void TextsTab::Init(ScatterCtrl& scatter) 
{
	CtrlLayout(*this);
	SizePos();
	
	pscatter = &scatter;
	
	labBack.SetInk(White());
	
	title <<= scatter.GetTitle();
	title.WhenAction = [=]{Change();};
	xLabel <<= scatter.GetLabelX();
	xLabel.WhenAction = [=]{Change();};
	yLabel <<= scatter.GetLabelY();
	yLabel.WhenAction = [=]{Change();};
	yLabel2 <<= scatter.GetLabelY2();
	yLabel2.WhenAction = [=]{Change();};
	backcolor <<= scatter.GetPlotAreaColor();
	backcolor.WhenAction = [=]{Change();};
	leftMargin <<= scatter.GetPlotAreaLeftMargin();
	leftMargin.WhenAction = [=]{Change();};
	rightMargin <<= scatter.GetPlotAreaRightMargin();
	rightMargin.WhenAction = [=]{Change();};
	topMargin <<= scatter.GetPlotAreaTopMargin();
	topMargin.WhenAction = [=]{Change();};
	bottomMargin <<= scatter.GetPlotAreaBottomMargin();
	bottomMargin.WhenAction = [=]{Change();};
	butFontTitle.SetImage(ScatterImgP::ColorA);
	butFontTitle.WhenAction = [=]{Change();};
	colorTitle <<= scatter.GetTitleColor();
	colorTitle.WhenAction = [=]{Change();};
	butFontLabel.SetImage(ScatterImgP::ColorA);
	butFontLabel.WhenAction = [=]{OnFontLabel();};
	colorLabel <<= scatter.GetLabelsColor();
	colorLabel.WhenAction = [=]{Change();};
	
	Change();
}

void TextsTab::OnFontTitle() {
	FontSelect fontSelect;
	
	fontSelect.Set(pscatter->GetTitleFont());
	fontSelect.WhenAction = [=](Font f) {OnChangeFontTitle(f);};
	fontSelect.Execute(butFontTitle);
}

void TextsTab::OnChangeFontTitle(Font font) {
	pscatter->SetTitleFont(font);
	pscatter->Refresh();
}

void TextsTab::OnFontLabel() {
	FontSelect fontSelect;
	
	fontSelect.Set(pscatter->GetLabelsFont());
	fontSelect.WhenAction = [=](Font f) {OnChangeFontLabel(f);};
	fontSelect.Execute(butFontLabel);
}

void TextsTab::OnChangeFontLabel(Font font) {
	pscatter->SetLabelsFont(font);
	pscatter->Refresh();
}
	
void TextsTab::Change() 
{
	ScatterCtrl &scatter = *pscatter;
	
	scatter.SetTitle(~title);
    scatter.SetLabels(~xLabel, ~yLabel, ~yLabel2);
	scatter.SetPlotAreaMargin(~leftMargin, ~rightMargin, ~topMargin, ~bottomMargin);
	scatter.SetPlotAreaColor(~backcolor);
	scatter.SetTitleColor(~colorTitle);
	
	scatter.SetModify();
	scatter.Refresh();
}

void LegendTab::Init(ScatterCtrl& scatter) 
{
	CtrlLayout(*this);
	SizePos();
	
	pscatter = &scatter;
	
	showLegend <<= scatter.GetShowLegend();
	showLegend.WhenAction = [=] {Change();};
	
	legendPosition <<= (scatter.GetLegendAnchor() == ScatterDraw::TOP ? 0 : 1);
	legendPosition.WhenAction = [=] {Change();};
	switch(scatter.GetLegendAnchor()) {
	case ScatterDraw::LEFT_TOP: 	legendAnchorLT = true;	break;
	case ScatterDraw::RIGHT_TOP: 	legendAnchorRT = true;	break;
	case ScatterDraw::LEFT_BOTTOM: 	legendAnchorLB = true;	break;
	case ScatterDraw::RIGHT_BOTTOM: legendAnchorRB = true;	break;
	}
	legendAnchorLT.WhenAction = [&] {ChangeAnchor(&legendAnchorLT);};
	legendAnchorRT.WhenAction = [&] {ChangeAnchor(&legendAnchorRT);};
	legendAnchorLB.WhenAction = [&] {ChangeAnchor(&legendAnchorLB);};
	legendAnchorRB.WhenAction = [&] {ChangeAnchor(&legendAnchorRB);};
	
	fillColor <<= scatter.GetLegendFillColor();
	fillColor.WhenAction = [=] {Change();};
	borderColor <<= scatter.GetLegendBorderColor();
	borderColor.WhenAction = [=] {Change();};
	Point p = scatter.GetLegendPos();
	tableHoriz <<= p.x;
	tableHoriz.WhenAction = [=] {Change();};
	tableVert <<= p.y;
	tableVert.WhenAction = [=] {Change();};
	numCols <<= scatter.GetLegendNumCols();
	numCols.WhenAction = [=] {Change();};
	
	rowSpacing <<= scatter.GetLegendRowSpacing();
	rowSpacing.WhenAction = [=] {Change();};
	
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
        legendTableAnchor = ScatterDraw::TOP;
    else if (legendAnchorLT)
        legendTableAnchor = ScatterDraw::LEFT_TOP;
    else if (legendAnchorRT)
        legendTableAnchor = ScatterDraw::RIGHT_TOP;
    else if (legendAnchorLB)
        legendTableAnchor = ScatterDraw::LEFT_BOTTOM;
    else if (legendAnchorRB)
        legendTableAnchor = ScatterDraw::RIGHT_BOTTOM;
    else {
        legendAnchorLT = true;
        legendTableAnchor = ScatterDraw::LEFT_TOP;
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

void GeneralTab::Init(ScatterCtrl& scatter) 
{
	CtrlLayout(*this);
	SizePos();
	
	pscatter = &scatter;
	
	dropResolution.Clear();
	dropResolution.Add(ScatterDraw::MD_DRAW, t_("Draw"));
	dropResolution.Add(ScatterDraw::MD_NOAA, t_("Painter No Antialiased"));
	dropResolution.Add(ScatterDraw::MD_ANTIALIASED, t_("Painter Antialiased"));
	dropResolution.Add(ScatterDraw::MD_SUBPIXEL, t_("Painter Subpixel"));
	dropResolution = scatter.GetMode();
	dropResolution.WhenAction = [=] {Change();};
	dropRotate.Clear();
	dropRotate.Add(ScatterCtrl::Angle_0, t_("0º"));
	dropRotate.Add(ScatterCtrl::Angle_90, t_("90º"));
	dropRotate.Add(ScatterCtrl::Angle_180, t_("180º"));
	dropRotate.Add(ScatterCtrl::Angle_270, t_("270º"));
	dropRotate = scatter.GetRotate();
	dropRotate.WhenAction = [=] {Change();};
	editWidth <<= scatter.GetSaveSize().cx;
	editHeight <<= scatter.GetSaveSize().cy;
	opResponsive <<= scatter.ScatterDraw::IsResponsive();
	editSensitivity <<= scatter.ScatterDraw::GetResponsivenessFactor();
	editJpgQ <<= scatter.GetJPGQuality();
	editWidth.WhenAction = [=] {Change();};
	editHeight.WhenAction = [=] {Change();};
	opResponsive.WhenAction = [=] {Change();};
	editSensitivity.WhenAction = [=] {Change();};
	editJpgQ.WhenAction = [=] {Change();};
	
	butApplyToAll.WhenAction = [=] {ChangeAll();};
	
	Change();
}
	
void GeneralTab::Change() 
{
	ScatterCtrl &scatter = *pscatter;
	
    scatter.SetMode(~dropResolution);
    scatter.Rotate((ScatterCtrl::Angle)(int)dropRotate.GetData());
    Size size(~editWidth, ~editHeight);
    scatter.SetSaveSize(size);
    scatter.ScatterDraw::Responsive(~opResponsive, ~editSensitivity);
    scatter.SetJPGQuality(~editJpgQ);
    
	scatter.SetModify();
	scatter.Refresh();
}

void GeneralTab::ChangeAll() 
{
	for (int i = 0; i < ScatterCtrl::GetInstancesCount(); ++i) {
		ScatterCtrl &scatter = ScatterCtrl::GetInstance(i);
		
	    scatter.SetMode(~dropResolution);
	    scatter.Rotate((ScatterCtrl::Angle)(int)dropRotate.GetData());
	    Size size(~editWidth, ~editHeight);
	    scatter.SetSaveSize(size);
	    scatter.ScatterDraw::Responsive(~opResponsive, ~editSensitivity);
	    scatter.SetJPGQuality(~editJpgQ);
	    
		scatter.SetModify();
		scatter.Refresh();
	}
}

void SeriesTab::Init(ScatterCtrl& scatter) 
{
	CtrlLayout(left);
	CtrlLayout(right);
	Horz(left.SizePos(), right.SizePos());
	SetPos(3000, 0);
	SizePos();
	
	pscatter = &scatter;
	
	Init0();
}

void SeriesTab::Init0()
{
	ScatterCtrl &scatter = *pscatter;
	
	right.Enable(!scatter.IsEmpty());
	
	left.list.Reset();
	left.list.SetLineCy(EditField::GetStdHeight());
	left.list.AddColumn(t_("Name"));
	for(int i = 0; i < scatter.GetCount(); i++)
		left.list.Add(scatter.GetLegend(i));
	left.list.SetCursor(0);
	left.list.WhenSel = [=] {UpdateFields();};
	left.butMoveUp.WhenAction = [=] {OnMoveUp();};
	left.butMoveUp.SetImage(ScatterImgP::UpArrow);
	left.butMoveDown.WhenAction = [=] {OnMoveDown();};
	left.butMoveDown.SetImage(ScatterImgP::DownArrow);
	left.butDelete.WhenAction = [=] {OnDelete();};
	
	right.marktype.Clear();

	right.markstyle.Add(t_("No mark"));
	for(int i = 0; i < MarkPlot::GetCount(); i++)
		right.markstyle.Add(MarkPlot::TypeName(i));
	right.markstyle.SetIndex(0);
	
	right.dashStyle.Clear();
	for(int i = 0; i < DashStyle::GetCount(); i++)
		right.dashStyle.Add(DashStyle::TypeName(i));
	
	UpdateFields();

	right.linecolor.WhenAction = [=] {Change();};
	right.fillcolor.WhenAction = [=] {Change();};
	right.visible.WhenAction = [=] {Change();};
	right.dashStyle.WhenAction = [=] {Change();};
	right.linethickness.WhenAction = [=] {Change();};

	right.markstyle.WhenAction = [=] {Change();};
	right.markcolor.WhenAction = [=] {Change();};
	right.markwidth.WhenAction = [=] {Change();};
	right.marktype.WhenAction = [=] {Change();};
	
	right.showLegend.WhenAction = [=] {Change();};
	
	right.unitsY.WhenAction = [=] {Change();};
	right.unitsX.WhenAction = [=] {Change();};

	right.name.WhenAction = [=] {Change();};
	right.primary.WhenAction = [=] {Change();};
	
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
//	if (IsNull(Upp::Color(~right.linecolor)))
//		scatter.SetLineColor(index, Upp::Color(~right.linecolor));
	
	scatter.MarkStyle(index, String(~right.markstyle));
	scatter.SetMarkColor(index, Upp::Color(~right.markcolor));
	scatter.SetMarkWidth(index, ~right.markwidth);
	scatter.ShowSeriesLegend(index, ~right.showLegend);
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
	
	right.showLegend <<= scatter.GetShowSeriesLegend(index);
	
	right.unitsY <<= scatter.GetUnitsY(index);
	right.unitsX <<= scatter.GetUnitsX(index);
	
	right.primary <<= scatter.IsDataPrimaryY(index);
}

void SeriesTab::OnMoveUp() 
{
	int index = left.list.GetCursor();
	if (index <= 0)
		return;
	
	ScatterCtrl &scatter = *pscatter;
	scatter.SwapSeries(index, index-1);
	
	Init0();
	
	left.list.SetCursor(index-1);
	
	scatter.SetModify();
	scatter.Refresh();
}

void SeriesTab::OnMoveDown() 
{
	int index = left.list.GetCursor();
	if (index < 0 || index == left.list.GetCount()-1)
		return;
	
	ScatterCtrl &scatter = *pscatter;
	if (!scatter.SwapSeries(index, index+1))
		return;
	
	Init0();
	
	left.list.SetCursor(index+1);
	
	scatter.SetModify();
	scatter.Refresh();
}

void SeriesTab::OnDelete()
{
	int index = left.list.GetCursor();
	if (index < 0)
		return;
	
	if (!PromptOKCancel(Format(t_("You are going to delete '%s' series.&Do you agree?"), DeQtf(String(left.list.Get(0))))))
		return;
	
	ScatterCtrl &scatter = *pscatter;
	if(!scatter.ScatterDraw::RemoveSeries(index))
		return;
	
	Init0();
	
	left.list.SetCursor(index);
	
	scatter.SetModify();
	scatter.Refresh();
}
	
}	