#include "ScatterCtrl.h"

void ScatterCtrl::DoProcessing() 
{
	ProcessingDlg dlg;
	dlg.Init(*this);
	dlg.Run();
}

void PropertiesDlg::Init(ScatterCtrl& scatter) 
{
	CtrlLayoutExit(*this, t_("Scatter properties"));
	Sizeable().Zoomable();

	this->pscatter = &scatter;
	
	tab.Add(measures, t_("Measures"));
	tab.Add(texts, t_("Texts"));
	tab.Add(legend, t_("Legend"));
	tab.Add(series, t_("Series"));
	tab.Add(general, t_("General"));
	OnTab();
	
	tab <<= THISBACK(OnTab);
}

void PropertiesDlg::Set(int itab)
{
	tab.Set(itab);
	OnTab();
}

void PropertiesDlg::OnTab()
{
	if (tab.IsAt(measures))
		measures.Init(*pscatter);
	else if (tab.IsAt(texts))
		texts.Init(*pscatter);
	else if (tab.IsAt(legend))
		legend.Init(*pscatter);
	else if (tab.IsAt(series))
		series.Init(*pscatter);
	else if (tab.IsAt(general))
		general.Init(*pscatter);
}

void ProcessingDlg::Init(ScatterCtrl& scatter) 
{
	Title(Nvl(scatter.GetTitle(), "Data") + " processing");
	Acceptor(right.exit, IDEXIT);
	right.exit.Exit();
	Add(splitter.SizePos());
	CtrlLayout(list);
	CtrlLayout(right);
	splitter.Horz(list.SizePos(), right.SizePos());
	splitter.SetPos(1500, 0);
	Sizeable().Zoomable();
	
	this->pscatter = &scatter;
	
	list.list.Reset();
	list.list.SetLineCy(EditField::GetStdHeight());
	list.list.AddColumn(t_("Name"));
	list.list.AddColumn(t_("Id"));
	list.list.ColumnWidths("1 0");
	for(int i = 0; i < scatter.GetCount(); i++) {
		if (scatter.ScatterDraw::IsVisible(i)) {
			list.list.Add(scatter.GetLegend(i), i);
			ProcessingTab& tab = tabs.Add();
			tab.Init(scatter);
			CtrlLayout(tab);
			right.rect.Add(tab.SizePos());
		}
	}
	if (list.list.GetCount() > 0)
		list.list.SetCursor(0);
	list.list.WhenSel = THISBACK(UpdateFields);
	UpdateFields();
}

void ProcessingDlg::UpdateFields() 
{
	int index = list.list.GetCursor();
	if (index < 0)
		return;
	
	for (int i = 0; i < list.list.GetCount(); ++i)
		tabs[i].Hide();
	tabs[index].UpdateField(~list.list.Get(0), int(list.list.Get(1)));
}

int r2Compare(const Vector<Value>& v1, const Vector<Value>& v2) {return double(v1[2]) > double(v2[2]);}

ProcessingTab::ProcessingTab() 
{
	CtrlLayout(*this);
	
	CtrlLayout(tabFitLeft);
	CtrlLayout(tabFitRight);
	splitterTabFit.Horz(tabFitLeft.SizePos(), tabFitRight.SizePos());
	splitterTabFit.SetPos(7000, 0);
	CtrlLayout(tabFreqLeft);
	CtrlLayout(tabFreqRight);
	splitterTabFreq.Horz(tabFreqLeft.SizePos(), tabFreqRight.SizePos());
	splitterTabFreq.SetPos(8000, 0);
	CtrlLayout(tabOpLeft);
	CtrlLayout(tabOpRight);
	splitterTabOp.Horz(tabOpLeft.SizePos(), tabOpRight.SizePos());
	splitterTabOp.SetPos(8500, 0);
	CtrlLayout(tabBestFitLeft);
	CtrlLayout(tabBestFitRight);
	splitterTabBestFit.Horz(tabBestFitLeft.SizePos(), tabBestFitRight.SizePos());
	splitterTabBestFit.SetPos(6000, 0);
	tab.Add(splitterTabFit.SizePos(), t_("Processing"));
	tab.Add(splitterTabFreq.SizePos(), t_("Frequency"));
	tab.Add(splitterTabOp.SizePos(), t_("Operations"));
	tab.Add(splitterTabBestFit.SizePos(), t_("Best fit"));
	tab.WhenSet = THISBACK(OnSet);
	
	tabFreqRight.butFFT.WhenAction = THISBACK(OnFFT);
	tabFreqRight.opXAxis = 0;
	tabFreqRight.opXAxis.WhenAction = THISBACK(OnFFT);
	tabFreqRight.type.WhenAction = THISBACK(OnFFT);
	tabFreqRight.type = 0;
	
	for (int i = 0; i < DataSource::GetFFTWindowCount(); ++i)
		tabFreqRight.window.Add(InitCaps(DataSource::GetFFTWindowStr(i)));
	tabFreqRight.window.SetIndex(0);
	tabFreqRight.window.WhenAction = THISBACK(OnFFT);
	tabFreqRight.num <<= 1;
	tabFreqRight.overlapping <<= 0.1;
	
	tabFitRight.opSeries = true;
	tabFitRight.opSeries.WhenAction = THISBACK(OnOp);
	tabFitRight.opAverage.WhenAction = THISBACK(OnOp);
	tabFitRight.opLinear.WhenAction = THISBACK(OnOp);
	tabFitRight.opCuadratic.WhenAction = THISBACK(OnOp);
	tabFitRight.opCubic.WhenAction = THISBACK(OnOp);
	tabFitRight.opSinus.WhenAction = THISBACK(OnOp);
	tabFitRight.opSinusTend.WhenAction = THISBACK(OnOp);
	tabFitRight.opSpline.WhenAction = THISBACK(OnOp);
	tabFitRight.opMax.WhenAction = THISBACK(OnOp);
	tabFitRight.opMin.WhenAction = THISBACK(OnOp);
	tabFitRight.opMovAvg.WhenAction = THISBACK(OnOp);
	tabFitRight.opSecAvg.WhenAction = THISBACK(OnOp);
	tabFitRight.opCumAvg.WhenAction = THISBACK(OnOp);
	tabFitRight.butAutoSensSector.WhenAction = THISBACK(OnAutoSensSector);
	tabFitRight.width.WhenLostFocus = THISBACK(OnUpdateSensitivity);
	tabFitRight.width.WhenAction = THISBACK(OnUpdateSensitivity);
	
	tabFitRight.numDecimals <<= 3;
	tabFitRight.numDecimals.WhenAction = THISBACK(UpdateEquations);
	tabFitRight.showEquation.WhenAction = THISBACK(OnShowEquation);
	
	tabOpRight.xLow.WhenLostFocus = THISBACK(OnOperation);
	tabOpRight.xHigh.WhenLostFocus = THISBACK(OnOperation);
	
	tabBestFitRight.coefficients = 0;
	tabBestFitRight.coefficients.Tip(t_("To show real equation coefficients with different precisions or just in text"));
	tabBestFitRight.minR2 = 0.6;
	tabBestFitRight.minR2.Tip(t_("Min. R2 to plot the equation"));
	tabBestFitRight.userFormula <<= "c0 + c1*x^2; c0=0; c1=1";
	tabBestFitRight.userFormula.Tip(t_("User suppled equation. Initial guess values separated with ';'"));
	tabBestFitRight.gridTrend.AddColumn(t_("Type"), 10);
	tabBestFitRight.gridTrend.AddColumn(t_("Equation"), 30);
	tabBestFitRight.gridTrend.AddColumn(t_("R2"), 5);
	tabBestFitRight.gridTrend.SetLineCy(EditField::GetStdHeight()).MultiSelect();
	tabBestFitRight.gridTrend.WhenBar = THISBACK(OnArrayBar);
	tabBestFitRight.gridTrend.Sort(r2Compare);
	for (int i = 0; i < ExplicitEquation::GetEquationCount(); ++i) 
		equationTypes.Add(ExplicitEquation::Create(i));
	userEquation = new UserEquation;
	equationTypes.Add(userEquation);
	
	tabBestFitRight.butFit.Tip(t_("It tries to fit the series with the supported equations"));
	tabBestFitRight.butFit.WhenPush = THISBACK(OnFit);
	
	tabFreqFirst = tabOpFirst = tabBestFitFirst = true;
	avgFirst = linearFirst = cuadraticFirst = cubicFirst = sinusFirst = sinusTendFirst = splineFirst = true;

	exclamationOpened = false;
	newWidthMax = newWidthMin = newWidthMovAvg-1;
}

void ProcessingTab::ArrayCopy() {
	tabBestFitRight.gridTrend.SetClipboard(true, true);
}

void ProcessingTab::ArraySelect() {
	tabBestFitRight.gridTrend.Select(0, tabBestFitRight.gridTrend.GetCount(), true);
}

void ProcessingTab::OnArrayBar(Bar &menu) {
	menu.Add(t_("Select all"), Null, THISBACK(ArraySelect)).Key(K_CTRL_A).Help(t_("Select all rows"));
	menu.Add(t_("Copy"), ScatterImgP::Copy(), THISBACK(ArrayCopy)).Key(K_CTRL_C).Help(t_("Copy selected rows"));
}

void ProcessingTab::OnFit() {
	WaitCursor waitcursor;
	
	DataSource &ds = pscatter->GetSeries(id);
	
	userEquation->Init("User", ~tabBestFitRight.userFormula, "x");
	
	Array<double> r2;
	r2.SetCount(equationTypes.GetCount());
	
	for (int i = 0; i < equationTypes.GetCount(); ++i) {
		equationTypes[i].GuessCoeff(ds);
		equationTypes[i].Fit(ds, r2[i]);
	}
	tabBestFitLeft.scatter.RemoveAllSeries();
	tabBestFitLeft.scatter.AddSeries(ds).Legend("Series").NoMark();
	for (int i = 0; i < equationTypes.GetCount(); ++i) {
		if (r2[i] >= tabBestFitRight.minR2)
			tabBestFitLeft.scatter.AddSeries(equationTypes[i]).Legend(equationTypes[i].GetFullName()).NoMark().Stroke(2);
	}
	tabBestFitLeft.scatter.ZoomToFit(true, true);
	
	int numDecimals = 3;
	switch (tabBestFitRight.coefficients) {
	case 1:	numDecimals = 40;	break;
	case 2:	numDecimals = Null;	break;
	}
	tabBestFitRight.gridTrend.Clear();
	for (int i = 0; i < equationTypes.GetCount(); ++i) 
		tabBestFitRight.gridTrend.Add(equationTypes[i].GetFullName(), equationTypes[i].GetEquation(numDecimals), r2[i]);
	tabBestFitRight.gridTrend.SetSortColumn(2, true);
}

void ProcessingTab::OnOp() 
{
	DataSource &data = tabFitLeft.scatter.GetSeries(0);
	
	if (data.IsParam() || data.IsExplicit()) 
		return;
	
	if (tabFitRight.opAverage && avgFirst) {	
		double r2;
		average.Fit(data, r2);
		avgFirst = false;
	}
	if (tabFitRight.opLinear && linearFirst) {	
		if (linear.Fit(data, r2Linear) < 0) {
			tabFitRight.opLinear = false;
			tabFitRight.opLinear.Enable(false);
		} else
			linearFirst = false;
	}
	if (tabFitRight.opCuadratic && cuadraticFirst) {		
		cuadratic.GuessCoeff(data);
		if (cuadratic.Fit(data, r2Cuadratic) < 0) {
			tabFitRight.opCuadratic = false;
			tabFitRight.opCuadratic.Enable(false);
		} else
			cuadraticFirst = false;
	}
	if (tabFitRight.opCubic && cubicFirst) {		
		cubic.GuessCoeff(data);
		if (cubic.Fit(data, r2Cubic) < 0) {
			tabFitRight.opCubic = false;
			tabFitRight.opCubic.Enable(false);
		} else
			cubicFirst = false;
	}
	if (tabFitRight.opSinus && sinusFirst) {		
		sinus.GuessCoeff(data);
		if (sinus.Fit(data, r2Sinus) < 0) {
			tabFitRight.opSinus = false;
			tabFitRight.opSinus.Enable(false);
		} else
			sinusFirst = false;
	}
	if (tabFitRight.opSinusTend && sinusTendFirst) {
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
			tabFitRight.opSinusTend = false;
			tabFitRight.opSinusTend.Enable(false);
		} else {
			splineFirst = false;
			r2SinusTend = r2SinusTendBest;
			sinusTend = sinusTendBest;
		}
	} 
	if (tabFitRight.opSpline && splineFirst) {		
		if (spline.Fit(data) < 0) {
			tabFitRight.opSpline = false;
			tabFitRight.opSpline.Enable(false);
		} else
			splineFirst = false;
	}
	OnUpdateSensitivity();
	
	tabFitLeft.scatter.ScatterDraw::Show(0, tabFitRight.opSeries);
	tabFitLeft.scatter.ScatterDraw::Show(1, tabFitRight.opAverage);
	tabFitLeft.scatter.ScatterDraw::Show(2, tabFitRight.opLinear);
	tabFitLeft.scatter.ScatterDraw::Show(3, tabFitRight.opCuadratic);
	tabFitLeft.scatter.ScatterDraw::Show(4, tabFitRight.opCubic);
	tabFitLeft.scatter.ScatterDraw::Show(5, tabFitRight.opSinus);
	tabFitLeft.scatter.ScatterDraw::Show(6, tabFitRight.opSinusTend);
	tabFitLeft.scatter.ScatterDraw::Show(7, tabFitRight.opSpline);
	tabFitLeft.scatter.ScatterDraw::Show(8, tabFitRight.opMax);
	tabFitLeft.scatter.ScatterDraw::Show(9, tabFitRight.opMin);
	tabFitLeft.scatter.ScatterDraw::Show(10,tabFitRight.opMovAvg);
	tabFitLeft.scatter.ScatterDraw::Show(11,tabFitRight.opSecAvg);
	tabFitLeft.scatter.ScatterDraw::Show(12,tabFitRight.opCumAvg);
	
	UpdateEquations();
	OnShowEquation();
}


void ProcessingTab::OnAutoSensSector() 
{
	DataSource &data = tabFitLeft.scatter.GetSeries(0);
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
			tabFitRight.width <<= width;
			return;
		}
	}
	tabFitLeft.scatter.Refresh();
}

void ProcessingTab::OnOperation()
{
	if (exclamationOpened)	// To avoid WhenLostFocus to be called when Exclamation() is opened
		return;
	exclamationOpened = true;
	if (!IsNull(tabOpRight.xLow) && !IsNull(tabOpRight.xHigh)) {
		if (tabOpRight.xLow >= tabOpRight.xHigh) {
			Exclamation(t_("'x >' has to be lower than 'x <'"));
			exclamationOpened = false;
			return;
		}
	}
	exclamationOpened = false;
	dataXRange.Init(pscatter->GetSeries(id), tabOpRight.xLow, tabOpRight.xHigh);
	tabOpLeft.scatter.Refresh();
}

void ProcessingTab::UpdateField(const String _name, int _id) 
{
	id = _id;
	name.SetText(_name);
	
	tabFitLeft.scatter.RemoveAllSeries();
	tabFitLeft.scatter.AddSeries(pscatter->GetSeries(id)).SetSequentialX(pscatter->GetSequentialX())
				   .Legend(pscatter->GetLegend(id));
	tabFitLeft.scatter.SetFastViewX(pscatter->GetFastViewX());
	
	tabFitLeft.scatter.SetFillColor(0, pscatter->GetFillColor(id));
	tabFitLeft.scatter.Dash(0, pscatter->GetDash(id));
	
	Upp::Color color;
	double thickness;
	pscatter->GetStroke(0, thickness, color);
	tabFitLeft.scatter.Stroke(0, thickness, color);
	tabFitLeft.scatter.MarkStyle(0, pscatter->GetMarkStyleName(id));
	tabFitLeft.scatter.SetMarkColor(0, pscatter->GetMarkColor(id));
	tabFitLeft.scatter.SetMarkWidth(0, pscatter->GetMarkWidth(id));
	tabFitLeft.scatter.MarkStyle(0, pscatter->GetMarkStyleName(id));
	tabFitLeft.scatter.SetLegendAnchor(ScatterDraw::RIGHT_TOP).SetLegendFillColor(Null);
	
	tabFitLeft.scatter.Units(0, pscatter->GetUnitsY(id), pscatter->GetUnitsX(id));
	tabFitLeft.scatter.SetLabelX(pscatter->GetLabelX());
	
	bool primary = pscatter->IsDataPrimaryY(id);
    tabFitLeft.scatter.SetRange(pscatter->GetXRange(), primary ? pscatter->GetYRange() : pscatter->GetY2Range());
	tabFitLeft.scatter.SetMajorUnits(pscatter->GetMajorUnitsX(), primary ? pscatter->GetMajorUnitsY() : pscatter->GetMajorUnitsY2());
	tabFitLeft.scatter.SetXYMin(pscatter->GetXMin(), primary ? pscatter->GetYMin() : pscatter->GetY2Min());
	
	tabFitLeft.scatter.ShowInfo().ShowContextMenu().ShowProcessDlg().ShowPropertiesDlg().SetMouseHandlingLinked(true, true);
	
	DataSource &data = tabFitLeft.scatter.GetSeries(0);
	
	if (!data.IsParam()/* && !data.IsExplicit()*/) {	
		double avg = data.AvgY();
		tabFitRight.eAverage = avg;
		tabFitRight.eRMS = data.RMSY();
		tabFitRight.eStdDev = data.StdDevY(avg);
		int64 idmx;
		double val;
		val = data.MaxY(idmx);
		if (!IsNull(val)) {
			tabFitRight.eMax = Format("(%f,%f)", data.x(idmx), val);
			Pointf p = data.MaxSubDataImpY(idmx, 3);
			if (!IsNull(p))
				tabFitRight.eMaxImp = Format("(%f,%f)", p.x, p.y);
			val = data.MinY(idmx);
			if (!IsNull(val))
				tabFitRight.eMin = Format("(%f,%f)", data.x(idmx), val);
		}
	}
	if (!data.IsParam() && !data.IsExplicit()) {	
		tabFitRight.width <<= pscatter->GetXRange()/15.;
		tabFitRight.width.SetInc(pscatter->GetXRange()/15./2.);
		
		tabFitLeft.scatter.AddSeries(average).NoMark().Stroke(1.5);
		tabFitLeft.scatter.AddSeries(linear).NoMark().Stroke(1.5);
		tabFitLeft.scatter.AddSeries(cuadratic).NoMark().Stroke(1.5);
		tabFitLeft.scatter.AddSeries(cubic).NoMark().Stroke(1.5);		
		tabFitLeft.scatter.AddSeries(sinus).NoMark().Stroke(1.5);
		tabFitLeft.scatter.AddSeries(sinusTend).NoMark().Stroke(1.5);
		tabFitLeft.scatter.AddSeries(spline).NoMark().Dash(LINE_SOLID).Stroke(1.5);
		tabFitLeft.scatter.AddSeries(upperEnvelope).Legend(pscatter->GetLegend(id) + String("-") + t_("Max"))
						.NoMark().Dash(LINE_DASHED).Stroke(1.5).SetSequentialX(true);
		tabFitLeft.scatter.AddSeries(lowerEnvelope).Legend(pscatter->GetLegend(id) + String("-") + t_("Min"))
						.NoMark().Dash(LINE_DASHED).SetSequentialX(true);
		tabFitLeft.scatter.AddSeries(movAvg).Stroke(1.5).Legend(pscatter->GetLegend(id) + String("-") + t_("MovAvg")).NoMark();		
		tabFitLeft.scatter.AddSeries(secAvg).Stroke(1.5).Legend(pscatter->GetLegend(id) + String("-") + t_("SecAvg")).NoMark();		
		tabFitLeft.scatter.AddSeries(cumAvg).Stroke(1.5).Legend(pscatter->GetLegend(id) + String("-") + t_("CumAvg")).NoMark();		
					
		OnOp();
	} else {
		tabFitRight.opSeries.Enable(false);
		tabFitRight.opAverage.Enable(false);
		tabFitRight.opLinear.Enable(false);
		tabFitRight.opCuadratic.Enable(false);
		tabFitRight.opCubic.Enable(false);
		tabFitRight.opSinus.Enable(false);
		tabFitRight.opSinusTend.Enable(false);
		tabFitRight.opSpline.Enable(false);
		tabFitRight.opMax.Enable(false);
		tabFitRight.opMin.Enable(false);
		tabFitRight.opMovAvg.Enable(false);
		tabFitRight.opSecAvg.Enable(false);
		tabFitRight.opCumAvg.Enable(false);
	}
	
	Show();	
}

void ProcessingTab::OnUpdateSensitivity() 
{
	DataSource &data = tabFitLeft.scatter.GetSeries(0);
	
	bool refresh = false;
	if (tabFitRight.opMax && newWidthMax != tabFitRight.width) {
		newWidthMax = tabFitRight.width;
		
		upperEnvelope.Clear();
		Vector<int64> idsUpper = data.UpperEnvelopeY(tabFitRight.width);
		for (int i = 0; i < idsUpper.GetCount(); ++i) 
			upperEnvelope << Pointf(data.x(idsUpper[i]), data.y(idsUpper[i]));
		refresh = true;
	}
	if (tabFitRight.opMin && newWidthMin != tabFitRight.width) {
		newWidthMin = tabFitRight.width;
		
		lowerEnvelope.Clear();
		Vector<int64> idsLower = data.LowerEnvelopeY(tabFitRight.width);
		for (int i = 0; i < idsLower.GetCount(); ++i) 
			lowerEnvelope << Pointf(data.x(idsLower[i]), data.y(idsLower[i]));
		refresh = true;
	}
	if (tabFitRight.opMovAvg && newWidthMovAvg != tabFitRight.width) {
		newWidthMovAvg = tabFitRight.width;
		
		movAvg = data.MovingAverageY(tabFitRight.width);
		refresh = true;
	}
	if (tabFitRight.opSecAvg && newWidthMovAvg != tabFitRight.width) {
		newWidthMovAvg = tabFitRight.width;
		
		secAvg = data.SectorAverageY(tabFitRight.width);
		refresh = true;
	}
	if (tabFitRight.opCumAvg) {
		cumAvg = data.CumulativeAverageY();
		refresh = true;
	}
			
	if (refresh)
		tabFitLeft.scatter.Refresh();
}

void ProcessingTab::OnSet() 
{
	DataSource &data = tabFitLeft.scatter.GetSeries(0);
	
	if (tabFreqFirst && tab.Get() == 1) {
		tabFreqFirst = false;
		if (data.IsParam() || data.IsExplicit()) {
			tabFreqLeft.comments.SetText(t_("Impossible to calculate FFT from a not sampled series"));
			tabFreqRight.butFFT.Enable(false);
		} else if (data.GetCount() < 5) {
			tabFreqLeft.comments.SetText(t_("Insufficient data to calculate FFT"));
			tabFreqRight.butFFT.Enable(false);
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
				tabFreqLeft.comments.SetText(Format(t_("Sampling time changes from %f to %f"), mindT, maxdT));
			else 
				tabFreqLeft.comments.SetText("");
			tabFreqRight.samplingTime = (maxdT + mindT)/2.;
		}
	} else if (tabOpFirst && tab.Get() == 2) {
		tabOpFirst = false; 
		
		tabOpLeft.scatter.RemoveAllSeries();
		String legend = pscatter->GetLegend(id);
		double xLow = pscatter->GetSeries(id).MinX();
		if (IsNull(xLow))
			xLow = pscatter->GetXMin();
		tabOpRight.xLow <<= xLow;
		double xHigh = pscatter->GetSeries(id).MaxX();
		if (IsNull(xHigh))
			xHigh = pscatter->GetXMin() + pscatter->GetXRange();
		tabOpRight.xHigh <<= xHigh;
		dataXRange.Init(pscatter->GetSeries(id), xLow, xHigh);
		tabOpLeft.scatter.AddSeries(dataXRange).SetSequentialX(pscatter->GetSequentialX())
					   .Legend(legend + String("-") + t_("Processed")).NoMark()
					   .Stroke(8, Upp::Color(115, 214, 74));				   
		tabOpLeft.scatter.AddSeries(pscatter->GetSeries(id)).SetSequentialX(pscatter->GetSequentialX())
					   .Legend(legend).NoMark().Stroke(2, Blue());				   
		tabOpLeft.scatter.SetFastViewX(pscatter->GetFastViewX());
	
		tabOpLeft.scatter.SetLegendAnchor(ScatterDraw::RIGHT_TOP).SetLegendFillColor(Null);
		
		tabOpLeft.scatter.Units(0, pscatter->GetUnitsY(id), pscatter->GetUnitsX(id));
		tabOpLeft.scatter.SetLabelX(pscatter->GetLabelX());
		
		bool primary = pscatter->IsDataPrimaryY(id);
		tabOpLeft.scatter.SetRange(pscatter->GetXRange(), primary ? pscatter->GetYRange() : pscatter->GetY2Range());
		tabOpLeft.scatter.SetMajorUnits(pscatter->GetMajorUnitsX(), primary ? pscatter->GetMajorUnitsY() : pscatter->GetMajorUnitsY2());
		tabOpLeft.scatter.SetXYMin(pscatter->GetXMin(), primary ? pscatter->GetYMin() : pscatter->GetY2Min());
		
		tabOpLeft.scatter.ShowInfo().ShowContextMenu().ShowProcessDlg().ShowPropertiesDlg().SetMouseHandlingLinked(true, true);	
	} else if (tabBestFitFirst && tab.Get() == 3) {
		tabBestFitFirst = false; 
		
		tabBestFitLeft.scatter.RemoveAllSeries();
		String legend = pscatter->GetLegend(id);
		
		tabBestFitLeft.scatter.AddSeries(pscatter->GetSeries(id)).SetSequentialX(pscatter->GetSequentialX())
					   .Legend(legend).NoMark().Stroke(2);				   
		tabBestFitLeft.scatter.SetFastViewX(pscatter->GetFastViewX());
	
		tabBestFitLeft.scatter.SetLegendAnchor(ScatterDraw::RIGHT_TOP).SetLegendFillColor(Null);
		
		tabBestFitLeft.scatter.Units(0, pscatter->GetUnitsY(id), pscatter->GetUnitsX(id));
		tabBestFitLeft.scatter.SetLabelX(pscatter->GetLabelX());
		
		bool primary = pscatter->IsDataPrimaryY(id);
		tabBestFitLeft.scatter.SetRange(pscatter->GetXRange(), primary ? pscatter->GetYRange() : pscatter->GetY2Range());
		tabBestFitLeft.scatter.SetMajorUnits(pscatter->GetMajorUnitsX(), primary ? pscatter->GetMajorUnitsY() : pscatter->GetMajorUnitsY2());
		tabBestFitLeft.scatter.SetXYMin(pscatter->GetXMin(), primary ? pscatter->GetYMin() : pscatter->GetY2Min());
		
		tabBestFitLeft.scatter.ShowInfo().ShowContextMenu().ShowProcessDlg().ShowPropertiesDlg().SetMouseHandlingLinked(true, true);	
	}
}

void ProcessingTab::UpdateEquations()
{
	tabFitRight.eqAverage = tabFitRight.opAverage ? average.GetEquation(tabFitRight.numDecimals) : "";
	tabFitRight.eqLinear = tabFitRight.opLinear ? linear.GetEquation(tabFitRight.numDecimals) : "";
	tabFitRight.r2Linear = tabFitRight.opLinear ? r2Linear : Null;
	tabFitRight.eqCuadratic = tabFitRight.opCuadratic ? cuadratic.GetEquation(tabFitRight.numDecimals) : "";
	tabFitRight.r2Cuadratic = tabFitRight.opCuadratic ? r2Cuadratic : Null;
	tabFitRight.eqCubic = tabFitRight.opCubic ? cubic.GetEquation(tabFitRight.numDecimals) : "";
	tabFitRight.r2Cubic = tabFitRight.opCubic ? r2Cubic : Null;
	tabFitRight.eqSinus = tabFitRight.opSinus ? sinus.GetEquation(tabFitRight.numDecimals) : "";
	tabFitRight.r2Sinus = tabFitRight.opSinus ? r2Sinus : Null;
	tabFitRight.eqSinusTend = tabFitRight.opSinusTend ? sinusTend.GetEquation(tabFitRight.numDecimals) : "";
	tabFitRight.r2SinusTend = tabFitRight.opSinusTend ? r2SinusTend : Null;
}

void ProcessingTab::OnShowEquation()
{
	bool show = tabFitRight.showEquation;
	tabFitLeft.scatter.Legend(1, pscatter->GetLegend(id) + String("-") + 
						(show && tabFitRight.opAverage ? average.GetEquation(tabFitRight.numDecimals) : String(t_("Average"))));
	tabFitLeft.scatter.Legend(2, pscatter->GetLegend(id) + String("-") + 
						(show && tabFitRight.opLinear ? linear.GetEquation(tabFitRight.numDecimals) : String(t_("Linear"))));
	tabFitLeft.scatter.Legend(3, pscatter->GetLegend(id) + String("-") + 
						(show && tabFitRight.opCuadratic ? cuadratic.GetEquation(tabFitRight.numDecimals) : String(t_("Cuadratic"))));
	tabFitLeft.scatter.Legend(4, pscatter->GetLegend(id) + String("-") + 
						(show && tabFitRight.opCubic ? cubic.GetEquation(tabFitRight.numDecimals) : String(t_("Cubic"))));
	tabFitLeft.scatter.Legend(5, pscatter->GetLegend(id) + String("-") + 
						(show && tabFitRight.opSinus ? sinus.GetEquation(tabFitRight.numDecimals) : String(t_("Sinusoidal"))));
	tabFitLeft.scatter.Legend(6, pscatter->GetLegend(id) + String("-") + 
						(show && tabFitRight.opSinusTend ? sinusTend.GetEquation(tabFitRight.numDecimals) : String(t_("Sinusoidal tend"))));
	tabFitLeft.scatter.Legend(7, pscatter->GetLegend(id) + String("-") + String(t_("Spline")));
	tabFitLeft.scatter.Refresh();
}

void ProcessingTab::OnFFT() 
{
	String errText;
	tabFreqLeft.scatter.RemoveAllSeries();
	double samplingTime = tabFreqRight.samplingTime;
	if (samplingTime < 0) {
		Exclamation(t_("Incorrect sampling time"));
		return;
	}
	int64 idMaxFFT;
	{
		WaitCursor waitcursor;
		
		DataSource &data = tabFitLeft.scatter.GetSeries(0);
		
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
		fft = series.FFTY(samplingTime, tabFreqRight.opXAxis == 1, tabFreqRight.type, 
							tabFreqRight.window.GetIndex(), tabFreqRight.num, tabFreqRight.overlapping);
		VectorPointf fftData(fft);
		fftData.MaxY(idMaxFFT);
		Pointf p = fftData.MaxSubDataImpY(idMaxFFT, 3);
		if (!IsNull(p))
			tabFreqRight.eMax = Format("(%f,%f)", p.x, p.y);
		
		if (tabFreqRight.type == DataSource::T_PSD) {
			double m_1, m0, m1, m2;
			fftData.GetSpectralMomentsY(tabFreqRight.opXAxis == 1, m_1, m0, m1, m2);
			tabFreqRight.m_1 <<= FormatDouble(m_1);
			tabFreqRight.m0  <<= FormatDouble(m0);
			tabFreqRight.m1  <<= FormatDouble(m1);
			tabFreqRight.m2  <<= FormatDouble(m2);
		} else {
			tabFreqRight.m_1 <<= "";
			tabFreqRight.m0  <<= "";
			tabFreqRight.m1  <<= "";
			tabFreqRight.m2  <<= "";
		}
	}
	if (fft.IsEmpty()) {
		tabFreqLeft.comments.SetText(errText);
		Exclamation(t_("Error obtaining FFT"));
		return;
	}
	
	String strtype;
	switch(tabFreqRight.type) {
	case DataSource::T_FFT:		strtype = t_("FFT");					break;
	case DataSource::T_PHASE:	strtype = t_("FFT-phase [rad]");		break;
	case DataSource::T_PSD:		strtype = t_("Power Spectral Density");	break;
	}
	String legend = tabFitLeft.scatter.GetLegend(0) + String("-") + strtype;
	tabFreqLeft.scatter.AddSeries(fft).Legend(legend);
	tabFreqLeft.scatter.ShowInfo().ShowContextMenu().ShowProcessDlg().ShowPropertiesDlg().SetMouseHandlingLinked(true, true);
	tabFreqLeft.scatter.SetLabelX(tabFreqRight.opXAxis == 1 ? t_("Frequency [Hz]") : t_("Period [sec]"));
	tabFreqLeft.scatter.SetLabelY(legend);
	tabFreqLeft.scatter.ZoomToFit(true, true);
	if (idMaxFFT > 0 && fft[int(idMaxFFT)].x < (fft[fft.GetCount() - 1].x)/2)
		tabFreqLeft.scatter.SetRange(fft[int(idMaxFFT)].x*2, Null);
	
	tabFreqLeft.comments.SetText(errText);
}
