#include "MathTools.h"


int r2Compare(const Vector<Value>& v1, const Vector<Value>& v2) {return double(v1[2]) > double(v2[2]);}
	
void TabRegression::Init() {
	CtrlLayout(*this);	
	HSizePos().VSizePos();

	gridDef.Clipboard().Editing().Appending().Removing().Sorting(false).ResizeColMode(0);
	gridDef.AddColumn("", 40).Edit(xCell);
	gridDef.AddColumn("", 40).Edit(xFrom);
	gridDef.AddColumn("", 40).Edit(xTo);
	gridDef.AddColumn("", 40).Edit(yCell);
	
	if (!LoadFromJsonFile(*this) || !notFirstTime) {
		yBesideX = true;
		firstCellIsName = false;
		notFirstTime = true;
		minR2 = 0;
		coefficients = 0;
	}

	scatter.SetMouseHandling(true, true).ShowContextMenu();

	const int totalCols = 50;
	
	editGrid.SetCount(totalCols);
	for(int i = 0; i < totalCols; i++) {
		String name = Format("Column %d", i);
		grid.AddColumn(name).Width(70).Edit(editGrid[i]);
	}
		
	grid.SelectRow(false);
	grid.SetRowCount(200);
	grid.ResizePaintMode(1);
	grid.Absolute().Clipboard().Editing().Sorting(false);
	grid.WhenPaste = THISBACK(OnAutoset);
	
	if (gridDef.GetRowCount() == 0) 
		gridDef.SetRowCount(1);
	
	switchColsRows = 0;
	OnButtons();
			
	gridTrend.AddColumn("Type", 10);
	gridTrend.AddColumn("Equation", 40);
	gridTrend.AddColumn("R2", 5);
	gridTrend.SetLineCy(EditField::GetStdHeight()).MultiSelect();
	gridTrend.WhenBar = THISBACK(OnArrayBar);
	gridTrend.Sort(r2Compare);
	     
	for (int i = 0; i < ExplicitEquation::GetEquationCount(); ++i) {
		ExplicitEquation &equation = equationTypes.Add(ExplicitEquation::Create(i));
		if (equation.GetNumCoeff(i) > 5)
			equationTypes.Remove(equationTypes.GetCount() - 1);
	}
	userEquation = new UserEquation;
	equationTypes.Add(userEquation);
	
	switchColsRows.WhenAction = THISBACK(OnButtons);
	yBesideX.WhenAction = THISBACK(OnButtons);
	firstCellIsName.WhenAction = THISBACK(OnButtons);
	     
	calc.WhenPush = THISBACK(Calculate);
	clear.WhenPush = THISBACK(OnClear);
	autoset.WhenPush = THISBACK(OnAutoset);
}

void TabRegression::End() {
	StoreAsJsonFile(*this, NULL, true);
}

void TabRegression::OnClear() {
	for (int r = 0; r < grid.GetRowCount(); ++r)
		for (int c = 0; c < grid.GetColumnCount(); ++c)	
			grid.Set(r, c, Null);
	grid.SetCursor(0);
	grid.GoBegin();
	gridDef.Clear();
	scatter.RemoveAllSeries();
	gridTrend.Clear();
}

bool IsNumber2(const Value &val) {
	if (IsNull(val))
		return false;
	if (IsNumber(val))
		return true;
	if (IsString(val)) {
		double d = ScanDouble(val.ToString());
		if (!IsNull(d))
			return true;
	}
	return false;
}

void TabRegression::OnAutoset() {
	if (switchColsRows == 0) {		// Columnas
		int fRow = firstCellIsName ? 1 : 0;
		
		int c;
		for (c = 0; c < grid.GetColumnCount(); ++c) {
			if (IsNumber2(grid.Get(fRow, c))) {
				gridDef.Set(0, 0, c);
				gridDef.Set(0, 3, c + 1);
				gridDef.Set(0, 1, fRow);
				break;
			}
		}
		if (c == grid.GetColumnCount()) {
			Exclamation(t_("Problem in Autoset"));
			return;
		}
		int r;
		for (r = fRow; r < grid.GetRowCount(); ++r) {
			if (!IsNumber2(grid.Get(r, c)))
				break;
		}
		gridDef.Set(0, 2, r - 1);
	} else {
		Exclamation("No implementado");
	}
	
	scatter.RemoveAllSeries();
	scatter.AddSeries(ds).Legend("Series").MarkStyle<RhombMarkPlot>().MarkWidth(10).NoPlot();
	gridTrend.Clear();
	scatter.ZoomToFit(true, true);
}

void TabRegression::OnButtons() {
	if (switchColsRows == 0) {
		gridDef.GetColumn(0).Name("X Col");
		gridDef.GetColumn(1).Name("Row From");
		gridDef.GetColumn(2).Name("Row To");
		gridDef.GetColumn(3).Name("Y Col");
	} else {
		gridDef.GetColumn(0).Name("X Row");
		gridDef.GetColumn(1).Name("Col From");
		gridDef.GetColumn(2).Name("Col To");	
		gridDef.GetColumn(3).Name("Y Row");
	}
	gridDef.GetColumn(3).Width(yBesideX ? 0 : 40);
	gridDef.Refresh();
}

void TabRegression::Calculate() {
	int idX = gridDef.Get(0, 0);
	int idY = gridDef.Get(0, 3);
	bool useCols = switchColsRows == 0;
	int beginData = gridDef.Get(0, 1);
	int numData = int(gridDef.Get(0, 2)) - beginData + 1;
	ds.Init(grid, idY, idX, useCols, beginData, numData);
	
	userEquation->Init("User", formula);
	
	Array<double> r2;
	r2.SetCount(equationTypes.GetCount());
	
	for (int i = 0; i < equationTypes.GetCount(); ++i) {
		equationTypes[i].GuessCoeff(ds);
		equationTypes[i].Fit(ds, r2[i]);
	}
	scatter.RemoveAllSeries();
	scatter.AddSeries(ds).Legend("Series").MarkStyle<RhombMarkPlot>().MarkWidth(10).NoPlot();
	for (int i = 0; i < equationTypes.GetCount(); ++i) {
		double kk = r2[i];
		if (r2[i] >= minR2)
			scatter.AddSeries(equationTypes[i]).Legend(equationTypes[i].GetFullName()).NoMark().Stroke(2);
	}
	scatter.ZoomToFit(true, true);
	
	int numDecimals = 3;
	switch (coefficients) {
	case 1:	numDecimals = 40;	break;
	case 2:	numDecimals = Null;	break;
	}
	gridTrend.Clear();
	for (int i = 0; i < equationTypes.GetCount(); ++i) 
		gridTrend.Add(equationTypes[i].GetFullName(), equationTypes[i].GetEquation(numDecimals), r2[i]);
	gridTrend.SetSortColumn(2, true);
}


void TabRegression::ArrayCopy() {
	gridTrend.SetClipboard(true, true);
}

void TabRegression::ArraySelect() {
	gridTrend.Select(0, gridTrend.GetCount(), true);
}

void TabRegression::OnArrayBar(Bar &menu) {
	menu.Add(t_("Select all"), Null, THISBACK(ArraySelect)).Key(K_CTRL_A).Help(t_("Select all rows"));
	menu.Add(t_("Copy"), MyImages::Copy(), THISBACK(ArrayCopy)).Key(K_CTRL_C).Help(t_("Copy selected rows"));
}


Tool *Construct1()
{
	static TabRegression tab;
	return &tab;
}

INITBLOCK {
	RegisterExample(t_("Regression"), Construct1, __FILE__);
}