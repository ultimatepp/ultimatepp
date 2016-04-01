#include "ScatterCtrl.h"

NAMESPACE_UPP

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
	
	butUpdate.WhenAction = THISBACK(Change);
	
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
	ScatterCtrl &scatter = *pscatter;
	
    scatter.SetXYMin(xMin, yMin, yMin2);
    //scatter.SetMinUnits(xMin, yMin);
	scatter.SetRange(xMax - xMin, yMax - yMin, yMax2 - yMin2);

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
	
	scatter.SetTitle(title);
    scatter.SetLabels(xLabel, yLabel, yLabel2);
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
	
	Title(t_("Scatter processing"));
	
	list.Reset();
	list.SetLineCy(EditField::GetStdHeight());
	list.AddColumn(t_("Name"));
	for(int i = 0; i < scatter.GetCount(); i++) {
		list.Add(scatter.GetLegend(i));
		ProcessingTab& tab = tabs.Add();
		tab.Init(scatter);
		CtrlLayout(tab);
		Add(tab.HSizePosZ(128, 4).VSizePosZ(4, 28));
	}
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
	tabs[index].UpdateField(~list.Get(0), index);
}


ProcessingTab::ProcessingTab() 
{
	CtrlLayout(*this);
	
	CtrlLayout(tabFit);
	CtrlLayout(tabFreq);
	CtrlLayout(tabOp);
	tab.Add(tabFit.SizePos(), t_("Data fit"));
	tab.Add(tabFreq.SizePos(), t_("Frequency"));
	tab.Add(tabOp.SizePos(), t_("Operations"));
	tab.WhenSet = THISBACK(OnSet);
	
	tabFreq.butFFT.WhenAction = THISBACK(OnFFT);
	
	tabFit.opSeries = true;
	tabFit.opSeries.WhenAction = THISBACK(OnOp);
	tabFit.opAverage.WhenAction = THISBACK(OnOp);
	tabFit.opLinear.WhenAction = THISBACK(OnOp);
	tabFit.opCuadratic.WhenAction = THISBACK(OnOp);
	tabFit.opCubic.WhenAction = THISBACK(OnOp);
	tabFit.opSinus.WhenAction = THISBACK(OnOp);
	tabFit.opSpline.WhenAction = THISBACK(OnOp);
	tabFit.opMax.WhenAction = THISBACK(OnOp);
	tabFit.opMin.WhenAction = THISBACK(OnOp);
	tabFit.opMovAvg.WhenAction = THISBACK(OnOp);
	tabFit.width.WhenLostFocus = THISBACK(OnUpdateSensitivity);
	tabFit.width.WhenAction = THISBACK(OnUpdateSensitivity);
	
	tabFit.numDecimals <<= 3;
	tabFit.numDecimals.WhenAction = THISBACK(UpdateEquations);
	tabFit.showEquation.WhenAction = THISBACK(OnShowEquation);
	
	tabOp.xGreater.WhenLostFocus = THISBACK(OnOperation);
	tabOp.xLower.WhenLostFocus = THISBACK(OnOperation);
	
	tabFreqFirst = tabOpFirst = true;
	avgFirst = linearFirst = cuadraticFirst = cubicFirst = sinusFirst = splineFirst = true;
	
	exclamationOpened = false;
	newWidthMax = newWidthMin = newWidthMovAvg-1;
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
	tabFit.scatter.ScatterDraw::Show(6, tabFit.opSpline);
	tabFit.scatter.ScatterDraw::Show(7, tabFit.opMax);
	tabFit.scatter.ScatterDraw::Show(8, tabFit.opMin);
	tabFit.scatter.ScatterDraw::Show(9, tabFit.opMovAvg);
	
	UpdateEquations();
	OnShowEquation();
}

void ProcessingTab::OnOperation()
{
	if (exclamationOpened)	// To avoid WhenLostFocus to be called when Exclamation() is opened
		return;
	exclamationOpened = true;
	if (!IsNull(tabOp.xGreater) && !IsNull(tabOp.xLower)) {
		if (tabOp.xGreater >= tabOp.xLower) {
			Exclamation(t_("'x >' has to be lower than 'x <'"));
			exclamationOpened = false;
			return;
		}
	}
	exclamationOpened = false;
	dataSetCond.Init(pscatter->GetSeries(id), tabOp.xGreater, tabOp.xLower);
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
	
	tabFit.scatter.SetDataColor(0, pscatter->GetDataColor(id));
	tabFit.scatter.SetFillColor(0, pscatter->GetFillColor(id));
	tabFit.scatter.Dash(0, pscatter->GetDash(id));
	tabFit.scatter.SetDataThickness(0, pscatter->GetDataThickness(id));
	tabFit.scatter.MarkStyle(0, pscatter->GetMarkStyleName(id));
	tabFit.scatter.SetMarkColor(0, pscatter->GetMarkColor(id));
	tabFit.scatter.SetMarkWidth(0, pscatter->GetMarkWidth(id));
	tabFit.scatter.SetLegendAnchor(ScatterDraw::LEGEND_ANCHOR_RIGHT_TOP).SetLegendFillColor(Null);
	
	tabFit.scatter.Units(0, pscatter->GetUnitsX(id), pscatter->GetUnitsY(id));
	
	bool primary = pscatter->IsDataPrimaryY(id);
    tabFit.scatter.SetRange(pscatter->GetRangeX(), primary ? pscatter->GetRangeY() : pscatter->GetRangeY2());
	tabFit.scatter.SetMajorUnits(pscatter->GetMajorUnitsX(), primary ? pscatter->GetMajorUnitsY() : pscatter->GetMajorUnitsY2());
	tabFit.scatter.SetXYMin(pscatter->GetXMin(), primary ? pscatter->GetYMin() : pscatter->GetY2Min());
	
	tabFit.scatter.SetMouseHandling(true, true).ShowInfo();
	
	DataSource &data = tabFit.scatter.GetSeries(0);
	
	if (!data.IsParam() && !data.IsExplicit()) {	
		double avg = data.AvgY();
		tabFit.eAverage = avg;
		tabFit.eStdDev = data.StdDevY(avg);
		int64 idmx;
		double val;
		val = data.MaxY(idmx);
		if (!IsNull(val))
			tabFit.eMax = Format("(%f,%f)", data.x(idmx), val);
		val = data.MinY(idmx);
		if (!IsNull(val))
			tabFit.eMin = Format("(%f,%f)", data.x(idmx), val);
		
		tabFit.width <<= pscatter->GetRangeX()/15.;
		tabFit.width.SetInc(pscatter->GetRangeX()/15./2.);
		
		tabFit.scatter.AddSeries(average).NoMark().SetDataThickness(1.5);
		tabFit.scatter.AddSeries(linear).NoMark().SetDataThickness(1.5);
		tabFit.scatter.AddSeries(cuadratic).NoMark().SetDataThickness(1.5);
		tabFit.scatter.AddSeries(cubic).NoMark().SetDataThickness(1.5);		
		tabFit.scatter.AddSeries(sinus).NoMark().SetDataThickness(1.5);
		tabFit.scatter.AddSeries(spline).NoMark().Dash(LINE_SOLID).SetDataThickness(1.5);
		tabFit.scatter.AddSeries(upperEnvelope).Legend(pscatter->GetLegend(id) + String("-") + t_("Max"))
						.NoMark().Dash(LINE_DASHED).SetDataThickness(1.5).SetSequentialX(true);
		tabFit.scatter.AddSeries(lowerEnvelope).Legend(pscatter->GetLegend(id) + String("-") + t_("Min"))
						.NoMark().Dash(LINE_DASHED).SetSequentialX(true);
		tabFit.scatter.AddSeries(movAvg).SetDataThickness(1.5).Legend(pscatter->GetLegend(id) + String("-") + t_("MovAvg")).NoMark();		
					
		OnOp();
	} else {
		tabFit.opSeries.Enable(false);
		tabFit.opAverage.Enable(false);
		tabFit.opLinear.Enable(false);
		tabFit.opCuadratic.Enable(false);
		tabFit.opCubic.Enable(false);
		tabFit.opSinus.Enable(false);
		tabFit.opSpline.Enable(false);
		tabFit.opMax.Enable(false);
		tabFit.opMin.Enable(false);
		tabFit.opMovAvg.Enable(false);
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
			if ((maxdT - mindT)/maxdT > 0.00001) {
				tabFreq.comments.SetText(Format(t_("Impossible to set sampling time. It changes from %f to %f"), mindT, maxdT));
				tabFreq.samplingTime = Null;
			} else {
				tabFreq.comments.SetText("");
				tabFreq.samplingTime = (maxdT + mindT)/2.;
			}
		}
	} else if (tabOpFirst && tab.Get() == 2) {
		tabOpFirst = false; 
		
		tabOp.scatter.RemoveAllSeries();
		String legend = pscatter->GetLegend(id);
		dataSetCond.Init(pscatter->GetSeries(id), pscatter->GetSeries(id).MinX(), pscatter->GetSeries(id).MaxX());
		tabOp.xGreater <<= pscatter->GetSeries(id).MinX();
		tabOp.xLower <<= pscatter->GetSeries(id).MaxX();
		tabOp.scatter.AddSeries(dataSetCond).SetSequentialX(pscatter->GetSequentialX())
					   .Legend(legend + String("-") + t_("Processed")).NoMark().SetDataThickness(8)
					   .SetDataColor(Upp::Color(115, 214, 74));				   
		tabOp.scatter.AddSeries(pscatter->GetSeries(id)).SetSequentialX(pscatter->GetSequentialX())
					   .Legend(legend).NoMark().SetDataThickness(2).SetDataColor(Blue());				   
		tabOp.scatter.SetFastViewX(pscatter->GetFastViewX());
	
		tabOp.scatter.SetLegendAnchor(ScatterDraw::LEGEND_ANCHOR_RIGHT_TOP).SetLegendFillColor(Null);
		
		tabOp.scatter.Units(0, pscatter->GetUnitsX(id), pscatter->GetUnitsY(id));
		
		bool primary = pscatter->IsDataPrimaryY(id);
		tabOp.scatter.SetRange(pscatter->GetRangeX(), primary ? pscatter->GetRangeY() : pscatter->GetRangeY2());
		tabOp.scatter.SetMajorUnits(pscatter->GetMajorUnitsX(), primary ? pscatter->GetMajorUnitsY() : pscatter->GetMajorUnitsY2());
		tabOp.scatter.SetXYMin(pscatter->GetXMin(), primary ? pscatter->GetYMin() : pscatter->GetY2Min());
		
		tabOp.scatter.SetMouseHandling(true, true).ShowInfo();	
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
	tabFit.scatter.Legend(6, pscatter->GetLegend(id) + String("-") + String(t_("Spline")));
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
	
	resampledSeries.Clear();
	resampledSeries << orderedSeries[0].y;
	double nextSample = orderedSeries[0].x + samplingTime;
	for (int i = 0; i < orderedSeries.GetCount() - 1;) {
		double x0 = orderedSeries[i].x;
		double x1 = orderedSeries[i + 1].x;
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
	fft = series.FFTY(samplingTime);
	if (fft.IsEmpty()) {
		tabFreq.comments.SetText(errText);
		Exclamation(t_("Error obtaining FFT"));
		return;
	}
	
	String legend = tabFit.scatter.GetLegend(0) + String("-") + t_("FFT");
	tabFreq.scatter.AddSeries(fft).Legend(legend);
	tabFreq.scatter.SetMouseHandling(true, true).ShowInfo();
	tabFreq.scatter.SetLabelX(t_("Period [sec]"));
	tabFreq.scatter.SetLabelY(legend);
	tabFreq.scatter.ZoomToFit(true, true);
	tabFreq.comments.SetText(errText);
}

END_UPP_NAMESPACE