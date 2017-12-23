#include "ScatterCtrl.h"


#define IMAGECLASS ScatterImgP
#define IMAGEFILE <ScatterCtrl/ScatterCtrl.iml>
#include <Draw/iml.h>

#define  LAYOUTFILE <ScatterCtrl/ScatterCtrl.lay>
#include <CtrlCore/lay.h>

#include "Properties.h"	

void ScatterCtrl::DoShowEditDlg(int itab) 
{
	PropertiesDlg(*this, itab).Run(true);
}

void ScatterCtrl::DoShowData() 
{
	DataDlg(*this).Run(true);
}

void ScatterCtrl::DoProcessing() 
{
	ProcessingDlg(*this).Run(true);
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
    else
        NEVER();
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

	marktype.Clear();

	markstyle.Add(t_("No mark"));
	for(int i = 0; i < MarkPlot::GetCount(); i++)
		markstyle.Add(MarkPlot::TypeName(i));
	
	markstyle.SetIndex(0);
	
	for(int i = 0; i < DashStyle::GetCount(); i++)
		dashStyle.Add(DashStyle::TypeName(i));
	
	UpdateFields();

	linecolor <<= THISBACK(Change);
	fillcolor <<= THISBACK(Change);
	visible <<= THISBACK(Change);
	dashStyle.WhenAction = THISBACK(Change);
	linethickness <<= THISBACK(Change);

	markstyle.WhenAction = THISBACK(Change);
	markcolor <<= THISBACK(Change);
	markwidth <<= THISBACK(Change);
	marktype.WhenAction = THISBACK(Change);
	
	unitsY <<= THISBACK(Change);
	unitsX <<= THISBACK(Change);

	name <<= THISBACK(Change);
	primary <<= THISBACK(Change);
	
	name.SetFocus();
}

void SeriesTab::ChangeMark() {
	int index = list.GetCursor();
	if (index < 0)
		return;
	
	ScatterCtrl &scatter = *pscatter;
	
	int id = MarkPlot::TypeIndex(~markstyle);
	
	marktype.Clear();
	
	if (id >= 0) {
		for (int i = 0; i < MarkPlot::GetTypeCount(id); ++i)
			marktype.Add(MarkPlot::TypeString(id, i));
	}
	if (marktype.GetCount() > 0)
		marktype.SetIndex(0);
	int idStyle = scatter.GetMarkStyleType(index);
	if (idStyle >= 0 && idStyle < marktype.GetCount())
		marktype.SetIndex(idStyle);
}

void SeriesTab::Change() 
{
	int index = list.GetCursor();
	if (index < 0)
		return;
	
	ScatterCtrl &scatter = *pscatter;
	
	scatter.SetFillColor(index, ~fillcolor);
	scatter.ScatterDraw::Show(index, ~visible);
	scatter.Dash(index, DashStyle::Style(DashStyle::TypeIndex(~dashStyle)));	
	scatter.Stroke(index, ~linethickness, Upp::Color(~linecolor));
	
	scatter.MarkStyle(index, String(~markstyle));
	scatter.SetMarkColor(index, Upp::Color(~markcolor));
	scatter.SetMarkWidth(index, ~markwidth);
	ChangeMark();
	
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
	
	fillcolor <<= scatter.GetFillColor(index);
	visible <<= scatter.ScatterDraw::IsVisible(index);
	int id = DashStyle::StyleIndex(scatter.GetDash(index));
	if (id < 0) {
		id = DashStyle::Register(Format(t_("Dash \"%s\""), scatter.GetDash(index)), scatter.GetDash(index));
		dashStyle.Add(DashStyle::TypeName(id));
	}
	dashStyle <<= DashStyle::TypeName(id);
	
	Upp::Color color;
	double thickness;
	
	scatter.GetStroke(index, thickness, color);
	linethickness <<= thickness;
	linecolor <<= color;
	markstyle <<= scatter.GetMarkStyleName(index);
	markcolor <<= scatter.GetMarkColor(index);
	markwidth <<= scatter.GetMarkWidth(index);
	ChangeMark();
	
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
	for(int c = 0; c < scatter.GetCount(); c++) { 
		if (!IsNull(scatter.GetCount(c))) {
			WithDataSeries <StaticRect> &dataseries = series.Add();
			CtrlLayout(dataseries);
			dataseries.scatterIndex.Hide();
			dataseries.scatterIndex = c;
			tab.Add(dataseries.SizePos(), scatter.GetLegend(c));
		}
	}
	if (tab.GetCount() < 1)
		return;
	OnTab(); 
	
	bool addedAll = false;
	DataSource &serie0 = scatter.GetSeries(0);
	for(int c = 1; c < scatter.GetCount(); c++) {
		if (!IsNull(scatter.GetCount(c))) {
			DataSource &serie = scatter.GetSeries(c);
			if (serie0.SameX(serie)) {
				if (!addedAll) {
					addedAll = true;
					WithDataSeries <StaticRect> &dataseries = series.Add();
					CtrlLayout(dataseries);
					dataseries.scatterIndex.Hide();
					dataseries.scatterIndex = -1;
					tab.Add(dataseries.SizePos(), t_("All"));
				}
			}
		}
	}
	
	tab <<= THISBACK(OnTab);
}

Value DataDlg::DataSourceX::Format(const Value& q) const 
{
	Value ret = pscatter->GetStringX(index, q);
	if (ret.Is<String>()) {
		String sret = ret;
		sret.Replace("\n", " ");
		return sret;
	}
	return ret;
}

Value DataDlg::DataSourceY::Format(const Value& q) const 
{
	Value ret = pscatter->GetStringY(index, q);
	if (ret.Is<String>()) {
		String sret = ret;
		sret.Replace("\n", " ");
		return sret;
	}
	return ret;
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
	dataSourceYArr.Clear();
	data.MultiSelect().SetLineCy(EditField::GetStdHeight());
	if (scatterIndex >= 0) {
		data.SetVirtualCount(int(scatter.GetCount(scatterIndex)));
		DataSourceY &dataSourceY = dataSourceYArr.Add();
		dataSourceX.pscatter = dataSourceY.pscatter = pscatter;
		dataSourceX.index = dataSourceY.index = scatterIndex;
		String textX = pscatter->GetLabelX() + 
					   (pscatter->GetUnitsX(scatterIndex).IsEmpty() ? "" : " [" + pscatter->GetUnitsX(scatterIndex) + "]"); 
		String textY = pscatter->GetLegend(scatterIndex) + 
					   (pscatter->GetUnitsY(scatterIndex).IsEmpty() ? "" : " [" + pscatter->GetUnitsY(scatterIndex) + "]"); 
		data.AddRowNumColumn(textX).SetConvert(dataSourceX);
		data.AddRowNumColumn(textY).SetConvert(dataSourceY);
	} else {
		data.SetVirtualCount(int(scatter.GetCount(0)));
		dataSourceX.pscatter = pscatter;
		dataSourceX.index = 0;
		String textX = pscatter->GetLabelX() + 
					   (pscatter->GetUnitsX(0).IsEmpty() ? "" : " [" + pscatter->GetUnitsX(0) + "]"); 
		data.AddRowNumColumn(textX).SetConvert(dataSourceX);
		for (int c = 0; c < scatter.GetCount(); ++c) {
			if (!IsNull(scatter.GetCount(c))) {
				DataSourceY &dataY = dataSourceYArr.Add();
				dataY.pscatter = pscatter;
				dataY.index = c;
				String textY = pscatter->GetLegend(c) + 
					   (pscatter->GetUnitsY(c).IsEmpty() ? "" : " [" + pscatter->GetUnitsY(c) + "]"); 		
				data.AddRowNumColumn(textY).SetConvert(dataY);	
			}
		}
	}
	data.WhenBar = THISBACK(OnArrayBar);
}

void DataDlg::ArrayCopy() {
	int index = tab.Get();
	if (index < 0)
		return;
	
	ArrayCtrl &data = series[index].data;
	data.SetClipboard(true, true);
}

void DataDlg::ArraySaveToFile(String fileName) {
	int index = tab.Get();
	if (index < 0)
		return;
	
	FileSel fileToSave;
	GuiLock __;
	if (IsNull(fileName)) {
		String name = pscatter->GetTitle() + " " + pscatter->GetLegend(series[index].scatterIndex);
		if (name.IsEmpty())
			name = t_("Scatter plot data");
		fileToSave.PreSelect(ForceExt(name, ".csv"));
		fileToSave.ClearTypes();
		fileToSave.Type(Format(t_("%s file"), t_("Comma separated values (.csv)")), "*.csv");
	    if(!fileToSave.ExecuteSaveAs(t_("Saving plot data"))) {
	        Exclamation(t_("Plot data has not been saved"));
	        return;
	    }
        fileName = fileToSave;
	} 
	ArrayCtrl &data = series[index].data;
	SaveFileBOMUtf8(fileName, data.AsText(AsString, true, pscatter->GetDefaultCSVSeparator(), 
								"\r\n", pscatter->GetDefaultCSVSeparator(), "\r\n"));
}


void DataDlg::ArraySelect() 
{
	int index = tab.Get();
	if (index < 0)
		return;
	
	ArrayCtrl &data = series[index].data;
	data.Select(0, data.GetCount(), true);
}

void DataDlg::OnArrayBar(Bar &menu) 
{
	int index = tab.Get();
	if (index < 0)
		return;
	
	menu.Add(t_("Select all"), Null, THISBACK(ArraySelect)).Key(K_CTRL_A)
								.Help(t_("Select all rows"));
								
	ArrayCtrl &data = series[index].data;							
	int count = data.GetSelectCount();
	if (count == 0)
		menu.Add(t_("No row selected"), Null, Null).Enable(false).Bold(true);
	else {
		menu.Add(Format(t_("Selected %d rows"), count), Null, Null).Enable(false).Bold(true);
		menu.Add(t_("Copy"), ScatterImgP::Copy(), THISBACK(ArrayCopy)).Key(K_CTRL_C)
									.Help(t_("Copy selected rows to clipboard"));
		menu.Add(t_("Save to file"), ScatterImgP::Save(), THISBACK1(ArraySaveToFile, Null)).Key(K_CTRL_S)
									.Help(t_("Save to .csv file"));
	}
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

void PropertiesDlg::OnClose() 
{
	measures.Change();
	
	RejectBreak(IDOK);
	Close();	Close();
}

ProcessingDlg::ProcessingDlg(ScatterCtrl& scatter) : scatter(scatter) 
{
	CtrlLayout(*this);
	Sizeable().Zoomable();
	
	String title;
	if (scatter.GetTitle().IsEmpty())
		title = t_("Data processing");
	else
		title = Format(t_("%s processing"), scatter.GetTitle());
	
	Title(title);
	
	list.Reset();
	list.SetLineCy(EditField::GetStdHeight());
	list.AddColumn(t_("Name"));
	list.AddColumn(t_("Id"));
	list.ColumnWidths("1 0");
	for(int i = 0; i < scatter.GetCount(); i++) {
		if (scatter.ScatterDraw::IsVisible(i)) {
			list.Add(scatter.GetLegend(i), i);
			ProcessingTab& tab = tabs.Add();
			tab.Init(scatter);
			CtrlLayout(tab);
			Add(tab.HSizePosZ(128, 4).VSizePosZ(4, 28));
		}
	}
	if (list.GetCount() > 0)
		list.SetCursor(0);
	list.WhenSel = THISBACK(UpdateFields);
	UpdateFields();
	
	close <<= THISBACK(OnClose);
	close.Exit();
}

void ProcessingDlg::OnClose() 
{
	RejectBreak(IDOK);
	Close();	Close();
}

void ProcessingDlg::UpdateFields() 
{
	int index = list.GetCursor();
	if (index < 0)
		return;
	
	for (int i = 0; i < list.GetCount(); ++i)
		tabs[i].Hide();
	tabs[index].UpdateField(~list.Get(0), int(list.Get(1)));
}

int r2Compare(const Vector<Value>& v1, const Vector<Value>& v2) {return double(v1[2]) > double(v2[2]);}

ProcessingTab::ProcessingTab() 
{
	CtrlLayout(*this);
	
	CtrlLayout(tabFit);
	CtrlLayout(tabFreq);
	CtrlLayout(tabOp);
	CtrlLayout(tabBestFit);
	tab.Add(tabFit.SizePos(), t_("Data fit"));
	tab.Add(tabFreq.SizePos(), t_("Frequency"));
	tab.Add(tabOp.SizePos(), t_("Operations"));
	tab.Add(tabBestFit.SizePos(), t_("Best fit"));
	tab.WhenSet = THISBACK(OnSet);
	
	tabFreq.butFFT.WhenAction = THISBACK(OnFFT);
	tabFreq.opXAxis = 0;
	tabFreq.opXAxis.WhenAction = THISBACK(OnFFT);
	tabFreq.type.WhenAction = THISBACK(OnFFT);
	tabFreq.type = 0;
	
	for (int i = 0; i < DataSource::GetFFTWindowCount(); ++i)
		tabFreq.window.Add(InitCaps(DataSource::GetFFTWindowStr(i)));
	tabFreq.window.SetIndex(0);
	tabFreq.window.WhenAction = THISBACK(OnFFT);
	tabFreq.num <<= 1;
	tabFreq.overlapping <<= 0.1;
	
	tabFit.opSeries = true;
	tabFit.opSeries.WhenAction = THISBACK(OnOp);
	tabFit.opAverage.WhenAction = THISBACK(OnOp);
	tabFit.opLinear.WhenAction = THISBACK(OnOp);
	tabFit.opCuadratic.WhenAction = THISBACK(OnOp);
	tabFit.opCubic.WhenAction = THISBACK(OnOp);
	tabFit.opSinus.WhenAction = THISBACK(OnOp);
	tabFit.opSinusTend.WhenAction = THISBACK(OnOp);
	tabFit.opSpline.WhenAction = THISBACK(OnOp);
	tabFit.opMax.WhenAction = THISBACK(OnOp);
	tabFit.opMin.WhenAction = THISBACK(OnOp);
	tabFit.opMovAvg.WhenAction = THISBACK(OnOp);
	tabFit.opSecAvg.WhenAction = THISBACK(OnOp);
	tabFit.butAutoSensSector.WhenAction = THISBACK(OnAutoSensSector);
	tabFit.width.WhenLostFocus = THISBACK(OnUpdateSensitivity);
	tabFit.width.WhenAction = THISBACK(OnUpdateSensitivity);
	
	tabFit.numDecimals <<= 3;
	tabFit.numDecimals.WhenAction = THISBACK(UpdateEquations);
	tabFit.showEquation.WhenAction = THISBACK(OnShowEquation);
	
	tabOp.xLow.WhenLostFocus = THISBACK(OnOperation);
	tabOp.xHigh.WhenLostFocus = THISBACK(OnOperation);
	
	tabBestFit.coefficients = 0;
	tabBestFit.minR2 = 0.6;
	tabBestFit.userFormula <<= "c0 + c1*x^2";
	tabBestFit.gridTrend.AddColumn("Type", 10);
	tabBestFit.gridTrend.AddColumn("Equation", 40);
	tabBestFit.gridTrend.AddColumn("R2", 5);
	tabBestFit.gridTrend.SetLineCy(EditField::GetStdHeight()).MultiSelect();
	tabBestFit.gridTrend.WhenBar = THISBACK(OnArrayBar);
	tabBestFit.gridTrend.Sort(r2Compare);
	for (int i = 0; i < ExplicitEquation::GetEquationCount(); ++i) 
		/*ExplicitEquation &equation = */equationTypes.Add(ExplicitEquation::Create(i));
	userEquation = new UserEquation;
	equationTypes.Add(userEquation);
	
	tabBestFit.butFit.WhenPush = THISBACK(OnFit);
	
	tabFreqFirst = tabOpFirst = tabBestFitFirst = true;
	avgFirst = linearFirst = cuadraticFirst = cubicFirst = sinusFirst = sinusTendFirst = splineFirst = true;

	exclamationOpened = false;
	newWidthMax = newWidthMin = newWidthMovAvg-1;
}

void ProcessingTab::ArrayCopy() {
	tabBestFit.gridTrend.SetClipboard(true, true);
}

void ProcessingTab::ArraySelect() {
	tabBestFit.gridTrend.Select(0, tabBestFit.gridTrend.GetCount(), true);
}

void ProcessingTab::OnArrayBar(Bar &menu) {
	menu.Add(t_("Select all"), Null, THISBACK(ArraySelect)).Key(K_CTRL_A).Help(t_("Select all rows"));
	menu.Add(t_("Copy"), ScatterImgP::Copy(), THISBACK(ArrayCopy)).Key(K_CTRL_C).Help(t_("Copy selected rows"));
}

void ProcessingTab::OnFit() {
	//ds.Init(pscatter->GetSeries(id));
	DataSource &ds = pscatter->GetSeries(id);
	
	userEquation->Init("User", ~tabBestFit.userFormula, "x");
	
	Array<double> r2;
	r2.SetCount(equationTypes.GetCount());
	
	for (int i = 0; i < equationTypes.GetCount(); ++i) {
		equationTypes[i].GuessCoeff(ds);
		equationTypes[i].Fit(ds, r2[i]);
	}
	tabBestFit.scatter.RemoveAllSeries();
	tabBestFit.scatter.AddSeries(ds).Legend("Series").NoMark();
	for (int i = 0; i < equationTypes.GetCount(); ++i) {
		if (r2[i] >= tabBestFit.minR2)
			tabBestFit.scatter.AddSeries(equationTypes[i]).Legend(equationTypes[i].GetFullName()).NoMark().Stroke(2);
	}
	tabBestFit.scatter.ZoomToFit(true, true);
	
	int numDecimals = 3;
	switch (tabBestFit.coefficients) {
	case 1:	numDecimals = 40;	break;
	case 2:	numDecimals = Null;	break;
	}
	tabBestFit.gridTrend.Clear();
	for (int i = 0; i < equationTypes.GetCount(); ++i) 
		tabBestFit.gridTrend.Add(equationTypes[i].GetFullName(), equationTypes[i].GetEquation(numDecimals), r2[i]);
	tabBestFit.gridTrend.SetSortColumn(2, true);
}

void ProcessingTab::OnOp() 
{
	DataSource &data = tabFit.scatter.GetSeries(0);
	
	if (data.IsParam() || data.IsExplicit()) 
		return;
	
	if (tabFit.opAverage && avgFirst) {	
		double r2;
		average.Fit(data, r2);
		avgFirst = false;
	}
	if (tabFit.opLinear && linearFirst) {	
		if (linear.Fit(data, r2Linear) < 0) {
			tabFit.opLinear = false;
			tabFit.opLinear.Enable(false);
		} else
			linearFirst = false;
	}
	if (tabFit.opCuadratic && cuadraticFirst) {		
		cuadratic.GuessCoeff(data);
		if (cuadratic.Fit(data, r2Cuadratic) < 0) {
			tabFit.opCuadratic = false;
			tabFit.opCuadratic.Enable(false);
		} else
			cuadraticFirst = false;
	}
	if (tabFit.opCubic && cubicFirst) {		
		cubic.GuessCoeff(data);
		if (cubic.Fit(data, r2Cubic) < 0) {
			tabFit.opCubic = false;
			tabFit.opCubic.Enable(false);
		} else
			cubicFirst = false;
	}
	if (tabFit.opSinus && sinusFirst) {		
		sinus.GuessCoeff(data);
		if (sinus.Fit(data, r2Sinus) < 0) {
			tabFit.opSinus = false;
			tabFit.opSinus.Enable(false);
		} else
			sinusFirst = false;
	}
	if (tabFit.opSinusTend && sinusTendFirst) {
		DataXRange dataXRange;
		dataXRange.Init(data, Null, Null);		
		double r2SinusTendBest = Null;
		SinEquation sinusTendBest;
		for (int iLow = 9; iLow >= 0; iLow--) {
			double xLow = data.x(int64(data.GetCount()*iLow/10.));
			dataXRange.SetXLow(xLow);
			sinusTend.GuessCoeff(dataXRange);
			if (sinusTend.Fit(dataXRange, r2SinusTend) < 0)
				break;
			if (!IsNull(r2SinusTendBest) && r2SinusTendBest > r2SinusTend)
				break;
			r2SinusTendBest = r2SinusTend;
			sinusTendBest = sinusTend;
		}
		if (IsNull(r2SinusTendBest)) {
			tabFit.opSinusTend = false;
			tabFit.opSinusTend.Enable(false);
		} else {
			splineFirst = false;
			r2SinusTend = r2SinusTendBest;
			sinusTend = sinusTendBest;
		}
	} 
	if (tabFit.opSpline && splineFirst) {		
		if (spline.Fit(data) < 0) {
			tabFit.opSpline = false;
			tabFit.opSpline.Enable(false);
		} else
			splineFirst = false;
	}
	OnUpdateSensitivity();
	
	tabFit.scatter.ScatterDraw::Show(0, tabFit.opSeries);
	tabFit.scatter.ScatterDraw::Show(1, tabFit.opAverage);
	tabFit.scatter.ScatterDraw::Show(2, tabFit.opLinear);
	tabFit.scatter.ScatterDraw::Show(3, tabFit.opCuadratic);
	tabFit.scatter.ScatterDraw::Show(4, tabFit.opCubic);
	tabFit.scatter.ScatterDraw::Show(5, tabFit.opSinus);
	tabFit.scatter.ScatterDraw::Show(6, tabFit.opSinusTend);
	tabFit.scatter.ScatterDraw::Show(7, tabFit.opSpline);
	tabFit.scatter.ScatterDraw::Show(8, tabFit.opMax);
	tabFit.scatter.ScatterDraw::Show(9, tabFit.opMin);
	tabFit.scatter.ScatterDraw::Show(10,tabFit.opMovAvg);
	tabFit.scatter.ScatterDraw::Show(11,tabFit.opSecAvg);
	
	UpdateEquations();
	OnShowEquation();
}


void ProcessingTab::OnAutoSensSector() 
{
	DataSource &data = tabFit.scatter.GetSeries(0);
	Vector<Pointf> secAvg;
	double baseWidth;
	
	baseWidth = 0;
	for (int64 i = 1; i < data.GetCount(); ++i)
		baseWidth += (data.x(i) - data.x(i-1));
	baseWidth /= (data.GetCount() - 1);
	
	double rangeX = data.x(data.GetCount() - 1) - data.x(int64(0));
	
	for(double width = baseWidth; width < rangeX/10.; width += baseWidth) {
		secAvg = data.SectorAverageY(width);
		VectorPointf sector(secAvg);
		Vector<int64> ids;
		sector.MaxListY(ids, 10*baseWidth);
		if (ids.GetCount() < 5) {
			tabFit.width <<= width;
			return;
		}
	}
	tabFit.scatter.Refresh();
}

void ProcessingTab::OnOperation()
{
	if (exclamationOpened)	// To avoid WhenLostFocus to be called when Exclamation() is opened
		return;
	exclamationOpened = true;
	if (!IsNull(tabOp.xLow) && !IsNull(tabOp.xHigh)) {
		if (tabOp.xLow >= tabOp.xHigh) {
			Exclamation(t_("'x >' has to be lower than 'x <'"));
			exclamationOpened = false;
			return;
		}
	}
	exclamationOpened = false;
	dataXRange.Init(pscatter->GetSeries(id), tabOp.xLow, tabOp.xHigh);
	tabOp.scatter.Refresh();
}

void ProcessingTab::UpdateField(const String _name, int _id) 
{
	id = _id;
	name.SetText(_name);
	
	tabFit.scatter.RemoveAllSeries();
	tabFit.scatter.AddSeries(pscatter->GetSeries(id)).SetSequentialX(pscatter->GetSequentialX())
				   .Legend(pscatter->GetLegend(id));
	tabFit.scatter.SetFastViewX(pscatter->GetFastViewX());
	
	tabFit.scatter.SetFillColor(0, pscatter->GetFillColor(id));
	tabFit.scatter.Dash(0, pscatter->GetDash(id));
	
	Upp::Color color;
	double thickness;
	pscatter->GetStroke(0, thickness, color);
	tabFit.scatter.Stroke(0, thickness, color);
	tabFit.scatter.MarkStyle(0, pscatter->GetMarkStyleName(id));
	tabFit.scatter.SetMarkColor(0, pscatter->GetMarkColor(id));
	tabFit.scatter.SetMarkWidth(0, pscatter->GetMarkWidth(id));
	tabFit.scatter.MarkStyle(0, pscatter->GetMarkStyleName(id));
	tabFit.scatter.SetLegendAnchor(ScatterDraw::LEGEND_ANCHOR_RIGHT_TOP).SetLegendFillColor(Null);
	
	tabFit.scatter.Units(0, pscatter->GetUnitsX(id), pscatter->GetUnitsY(id));
	
	bool primary = pscatter->IsDataPrimaryY(id);
    tabFit.scatter.SetRange(pscatter->GetXRange(), primary ? pscatter->GetYRange() : pscatter->GetY2Range());
	tabFit.scatter.SetMajorUnits(pscatter->GetMajorUnitsX(), primary ? pscatter->GetMajorUnitsY() : pscatter->GetMajorUnitsY2());
	tabFit.scatter.SetXYMin(pscatter->GetXMin(), primary ? pscatter->GetYMin() : pscatter->GetY2Min());
	
	tabFit.scatter.ShowInfo().ShowContextMenu().ShowProcessDlg().ShowPropertiesDlg().SetMouseHandlingLinked(true, true);
	
	DataSource &data = tabFit.scatter.GetSeries(0);
	
	if (!data.IsParam()/* && !data.IsExplicit()*/) {	
		double avg = data.AvgY();
		tabFit.eAverage = avg;
		tabFit.eRMS = data.RMSY();
		tabFit.eStdDev = data.StdDevY(avg);
		int64 idmx;
		double val;
		val = data.MaxY(idmx);
		if (!IsNull(val))
			tabFit.eMax = Format("(%f,%f)", data.x(idmx), val);
		Pointf p = data.MaxSubDataImpY(idmx, 3);
		if (!IsNull(p))
			tabFit.eMaxImp = Format("(%f,%f)", p.x, p.y);
		val = data.MinY(idmx);
		if (!IsNull(val))
			tabFit.eMin = Format("(%f,%f)", data.x(idmx), val);
	}
	if (!data.IsParam() && !data.IsExplicit()) {	
		tabFit.width <<= pscatter->GetXRange()/15.;
		tabFit.width.SetInc(pscatter->GetXRange()/15./2.);
		
		tabFit.scatter.AddSeries(average).NoMark().Stroke(1.5);
		tabFit.scatter.AddSeries(linear).NoMark().Stroke(1.5);
		tabFit.scatter.AddSeries(cuadratic).NoMark().Stroke(1.5);
		tabFit.scatter.AddSeries(cubic).NoMark().Stroke(1.5);		
		tabFit.scatter.AddSeries(sinus).NoMark().Stroke(1.5);
		tabFit.scatter.AddSeries(sinusTend).NoMark().Stroke(1.5);
		tabFit.scatter.AddSeries(spline).NoMark().Dash(LINE_SOLID).Stroke(1.5);
		tabFit.scatter.AddSeries(upperEnvelope).Legend(pscatter->GetLegend(id) + String("-") + t_("Max"))
						.NoMark().Dash(LINE_DASHED).Stroke(1.5).SetSequentialX(true);
		tabFit.scatter.AddSeries(lowerEnvelope).Legend(pscatter->GetLegend(id) + String("-") + t_("Min"))
						.NoMark().Dash(LINE_DASHED).SetSequentialX(true);
		tabFit.scatter.AddSeries(movAvg).Stroke(1.5).Legend(pscatter->GetLegend(id) + String("-") + t_("MovAvg")).NoMark();		
		tabFit.scatter.AddSeries(secAvg).Stroke(1.5).Legend(pscatter->GetLegend(id) + String("-") + t_("SecAvg")).NoMark();		
					
		OnOp();
	} else {
		tabFit.opSeries.Enable(false);
		tabFit.opAverage.Enable(false);
		tabFit.opLinear.Enable(false);
		tabFit.opCuadratic.Enable(false);
		tabFit.opCubic.Enable(false);
		tabFit.opSinus.Enable(false);
		tabFit.opSinusTend.Enable(false);
		tabFit.opSpline.Enable(false);
		tabFit.opMax.Enable(false);
		tabFit.opMin.Enable(false);
		tabFit.opMovAvg.Enable(false);
		tabFit.opSecAvg.Enable(false);
	}
	
	Show();	
}

void ProcessingTab::OnUpdateSensitivity() 
{
	DataSource &data = tabFit.scatter.GetSeries(0);
	
	bool refresh = false;
	if (tabFit.opMax && newWidthMax != tabFit.width) {
		newWidthMax = tabFit.width;
		
		upperEnvelope.Clear();
		Vector<int64> idsUpper = data.UpperEnvelopeY(tabFit.width);
		for (int i = 0; i < idsUpper.GetCount(); ++i) 
			upperEnvelope << Pointf(data.x(idsUpper[i]), data.y(idsUpper[i]));
		refresh = true;
	}
	if (tabFit.opMin && newWidthMin != tabFit.width) {
		newWidthMin = tabFit.width;
		
		lowerEnvelope.Clear();
		Vector<int64> idsLower = data.LowerEnvelopeY(tabFit.width);
		for (int i = 0; i < idsLower.GetCount(); ++i) 
			lowerEnvelope << Pointf(data.x(idsLower[i]), data.y(idsLower[i]));
		refresh = true;
	}
	if (tabFit.opMovAvg && newWidthMovAvg != tabFit.width) {
		newWidthMovAvg = tabFit.width;
		
		movAvg = data.MovingAverageY(tabFit.width);
		refresh = true;
	}
	if (tabFit.opSecAvg && newWidthMovAvg != tabFit.width) {
		newWidthMovAvg = tabFit.width;
		
		secAvg = data.SectorAverageY(tabFit.width);
		refresh = true;
	}
		
	if (refresh)
		tabFit.scatter.Refresh();
}

void ProcessingTab::OnSet() 
{
	DataSource &data = tabFit.scatter.GetSeries(0);
	
	if (tabFreqFirst && tab.Get() == 1) {
		tabFreqFirst = false;
		if (data.IsParam() || data.IsExplicit()) {
			tabFreq.comments.SetText(t_("Impossible to calculate FFT from a not sampled series"));
			tabFreq.butFFT.Enable(false);
		} else if (data.GetCount() < 5) {
			tabFreq.comments.SetText(t_("Insufficient data to calculate FFT"));
			tabFreq.butFFT.Enable(false);
		} else {
			double mindT, maxdT = Null;
			mindT = -maxdT;
			for (int64 i = 1; i < data.GetCount(); ++i) {
				double d2 = data.x(i), d1 = data.x(i - 1);
				
				if (!IsNull(d1) && !IsNull(d2)) {
					double delta = (d2 - d1);
					mindT = min(delta, mindT);
					maxdT = max(delta, maxdT);
				}
			}
			if ((maxdT - mindT)/maxdT > 0.00001) 
				tabFreq.comments.SetText(Format(t_("Sampling time changes from %f to %f"), mindT, maxdT));
			else 
				tabFreq.comments.SetText("");
			tabFreq.samplingTime = (maxdT + mindT)/2.;
		}
	} else if (tabOpFirst && tab.Get() == 2) {
		tabOpFirst = false; 
		
		tabOp.scatter.RemoveAllSeries();
		String legend = pscatter->GetLegend(id);
		double xLow = pscatter->GetSeries(id).MinX();
		if (IsNull(xLow))
			xLow = pscatter->GetXMin();
		tabOp.xLow <<= xLow;
		double xHigh = pscatter->GetSeries(id).MaxX();
		if (IsNull(xHigh))
			xHigh = pscatter->GetXMin() + pscatter->GetXRange();
		tabOp.xHigh <<= xHigh;
		dataXRange.Init(pscatter->GetSeries(id), xLow, xHigh);
		tabOp.scatter.AddSeries(dataXRange).SetSequentialX(pscatter->GetSequentialX())
					   .Legend(legend + String("-") + t_("Processed")).NoMark()
					   .Stroke(8, Upp::Color(115, 214, 74));				   
		tabOp.scatter.AddSeries(pscatter->GetSeries(id)).SetSequentialX(pscatter->GetSequentialX())
					   .Legend(legend).NoMark().Stroke(2, Blue());				   
		tabOp.scatter.SetFastViewX(pscatter->GetFastViewX());
	
		tabOp.scatter.SetLegendAnchor(ScatterDraw::LEGEND_ANCHOR_RIGHT_TOP).SetLegendFillColor(Null);
		
		tabOp.scatter.Units(0, pscatter->GetUnitsX(id), pscatter->GetUnitsY(id));
		
		bool primary = pscatter->IsDataPrimaryY(id);
		tabOp.scatter.SetRange(pscatter->GetXRange(), primary ? pscatter->GetYRange() : pscatter->GetY2Range());
		tabOp.scatter.SetMajorUnits(pscatter->GetMajorUnitsX(), primary ? pscatter->GetMajorUnitsY() : pscatter->GetMajorUnitsY2());
		tabOp.scatter.SetXYMin(pscatter->GetXMin(), primary ? pscatter->GetYMin() : pscatter->GetY2Min());
		
		tabOp.scatter.ShowInfo().ShowContextMenu().ShowProcessDlg().ShowPropertiesDlg().SetMouseHandlingLinked(true, true);	
	} else if (tabBestFitFirst && tab.Get() == 3) {
		tabBestFitFirst = false; 
		
		tabBestFit.scatter.RemoveAllSeries();
		String legend = pscatter->GetLegend(id);
		
		tabBestFit.scatter.AddSeries(pscatter->GetSeries(id)).SetSequentialX(pscatter->GetSequentialX())
					   .Legend(legend).NoMark().Stroke(2);				   
		tabBestFit.scatter.SetFastViewX(pscatter->GetFastViewX());
	
		tabBestFit.scatter.SetLegendAnchor(ScatterDraw::LEGEND_ANCHOR_RIGHT_TOP).SetLegendFillColor(Null);
		
		tabBestFit.scatter.Units(0, pscatter->GetUnitsX(id), pscatter->GetUnitsY(id));
		
		bool primary = pscatter->IsDataPrimaryY(id);
		tabBestFit.scatter.SetRange(pscatter->GetXRange(), primary ? pscatter->GetYRange() : pscatter->GetY2Range());
		tabBestFit.scatter.SetMajorUnits(pscatter->GetMajorUnitsX(), primary ? pscatter->GetMajorUnitsY() : pscatter->GetMajorUnitsY2());
		tabBestFit.scatter.SetXYMin(pscatter->GetXMin(), primary ? pscatter->GetYMin() : pscatter->GetY2Min());
		
		tabBestFit.scatter.ShowInfo().ShowContextMenu().ShowProcessDlg().ShowPropertiesDlg().SetMouseHandlingLinked(true, true);	
	}
}

void ProcessingTab::UpdateEquations()
{
	tabFit.eqAverage = tabFit.opAverage ? average.GetEquation(tabFit.numDecimals) : "";
	tabFit.eqLinear = tabFit.opLinear ? linear.GetEquation(tabFit.numDecimals) : "";
	tabFit.r2Linear = tabFit.opLinear ? r2Linear : Null;
	tabFit.eqCuadratic = tabFit.opCuadratic ? cuadratic.GetEquation(tabFit.numDecimals) : "";
	tabFit.r2Cuadratic = tabFit.opCuadratic ? r2Cuadratic : Null;
	tabFit.eqCubic = tabFit.opCubic ? cubic.GetEquation(tabFit.numDecimals) : "";
	tabFit.r2Cubic = tabFit.opCubic ? r2Cubic : Null;
	tabFit.eqSinus = tabFit.opSinus ? sinus.GetEquation(tabFit.numDecimals) : "";
	tabFit.r2Sinus = tabFit.opSinus ? r2Sinus : Null;
	tabFit.eqSinusTend = tabFit.opSinusTend ? sinusTend.GetEquation(tabFit.numDecimals) : "";
	tabFit.r2SinusTend = tabFit.opSinusTend ? r2SinusTend : Null;
}

void ProcessingTab::OnShowEquation()
{
	bool show = tabFit.showEquation;
	tabFit.scatter.Legend(1, pscatter->GetLegend(id) + String("-") + 
						(show && tabFit.opAverage ? average.GetEquation(tabFit.numDecimals) : String(t_("Average"))));
	tabFit.scatter.Legend(2, pscatter->GetLegend(id) + String("-") + 
						(show && tabFit.opLinear ? linear.GetEquation(tabFit.numDecimals) : String(t_("Linear"))));
	tabFit.scatter.Legend(3, pscatter->GetLegend(id) + String("-") + 
						(show && tabFit.opCuadratic ? cuadratic.GetEquation(tabFit.numDecimals) : String(t_("Cuadratic"))));
	tabFit.scatter.Legend(4, pscatter->GetLegend(id) + String("-") + 
						(show && tabFit.opCubic ? cubic.GetEquation(tabFit.numDecimals) : String(t_("Cubic"))));
	tabFit.scatter.Legend(5, pscatter->GetLegend(id) + String("-") + 
						(show && tabFit.opSinus ? sinus.GetEquation(tabFit.numDecimals) : String(t_("Sinusoidal"))));
	tabFit.scatter.Legend(6, pscatter->GetLegend(id) + String("-") + 
						(show && tabFit.opSinusTend ? sinusTend.GetEquation(tabFit.numDecimals) : String(t_("Sinusoidal tend"))));
	tabFit.scatter.Legend(7, pscatter->GetLegend(id) + String("-") + String(t_("Spline")));
	tabFit.scatter.Refresh();
}

void ProcessingTab::OnFFT() 
{
	String errText;
	tabFreq.scatter.RemoveAllSeries();
	double samplingTime = tabFreq.samplingTime;
	if (samplingTime < 0) {
		Exclamation(t_("Incorrect sampling time"));
		return;
	}
	int64 idMaxFFT;
	{
		WaitCursor waitcursor;
		
		DataSource &data = tabFit.scatter.GetSeries(0);
		
		Vector<Pointf> orderedSeries;
		for (int64 i = 0; i < data.GetCount(); ++i) {		// Clean Nulls
			if (!IsNull(data.x(i)) && !IsNull(data.y(i)))
				orderedSeries << Pointf(data.x(i), data.y(i));
		}
		//if (orderedSeries.GetCount() != data.GetCount())
		//	errText << Format(t_("Removed %d Null points."), data.GetCount() - orderedSeries.GetCount());
		
		PointfLess less;
		Sort(orderedSeries, less);								
		
		Vector<double> resampledSeries;
		resampledSeries << orderedSeries[0].y;
		double nextSample = orderedSeries[0].x + samplingTime;
		for (int i = 0; i < orderedSeries.GetCount() - 1;) {
			if (orderedSeries[i].x == nextSample) {
				resampledSeries << orderedSeries[i].y;
				nextSample += samplingTime;
			} else if (orderedSeries[i].x < nextSample && orderedSeries[i + 1].x > nextSample) {	// Linear interpolation
				resampledSeries << (orderedSeries[i].y + (orderedSeries[i + 1].y - orderedSeries[i].y)*
								   (nextSample - orderedSeries[i].x)/(orderedSeries[i + 1].x - orderedSeries[i].x));
				nextSample += samplingTime;
			} else
				++i;
		}
		if (orderedSeries[orderedSeries.GetCount() - 1].x == nextSample) 
			resampledSeries << orderedSeries[orderedSeries.GetCount() - 1].y;
	
		VectorY<double> series(resampledSeries, 0, samplingTime);
		fft = series.FFTY(samplingTime, tabFreq.opXAxis == 1, tabFreq.type, 
							tabFreq.window.GetIndex(), tabFreq.num, tabFreq.overlapping);
		VectorPointf fftData(fft);
		fftData.MaxY(idMaxFFT);
		Pointf p = fftData.MaxSubDataImpY(idMaxFFT, 3);
		if (!IsNull(p))
			tabFreq.eMax = Format("(%f,%f)", p.x, p.y);
		
		if (tabFreq.type == DataSource::T_PSD) {
			double m_1, m0, m1, m2;
			fftData.GetSpectralMomentsY(tabFreq.opXAxis == 1, m_1, m0, m1, m2);
			tabFreq.m_1 <<= FormatDouble(m_1);
			tabFreq.m0  <<= FormatDouble(m0);
			tabFreq.m1  <<= FormatDouble(m1);
			tabFreq.m2  <<= FormatDouble(m2);
		} else {
			tabFreq.m_1 <<= "";
			tabFreq.m0  <<= "";
			tabFreq.m1  <<= "";
			tabFreq.m2  <<= "";
		}
	}
	if (fft.IsEmpty()) {
		tabFreq.comments.SetText(errText);
		Exclamation(t_("Error obtaining FFT"));
		return;
	}
	
	String strtype;
	switch(tabFreq.type) {
	case DataSource::T_FFT:		strtype = t_("FFT");					break;
	case DataSource::T_PHASE:	strtype = t_("FFT-phase [rad]");		break;
	case DataSource::T_PSD:		strtype = t_("Power Spectral Density");	break;
	}
	String legend = tabFit.scatter.GetLegend(0) + String("-") + strtype;
	tabFreq.scatter.AddSeries(fft).Legend(legend);
	tabFreq.scatter.ShowInfo().ShowContextMenu().ShowProcessDlg().ShowPropertiesDlg().SetMouseHandlingLinked(true, true);
	tabFreq.scatter.SetLabelX(tabFreq.opXAxis == 1 ? t_("Frequency [Hz]") : t_("Period [sec]"));
	tabFreq.scatter.SetLabelY(legend);
	tabFreq.scatter.ZoomToFit(true, true);
	if (idMaxFFT > 0 && fft[int(idMaxFFT)].x < (fft[fft.GetCount() - 1].x)/2)
		tabFreq.scatter.SetRange(fft[int(idMaxFFT)].x*2, Null);
	
	tabFreq.comments.SetText(errText);
}
