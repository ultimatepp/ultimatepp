#include "MathTools.h"


int r2Compare(const Vector<Value>& v1, const Vector<Value>& v2) {return double(v1[2]) > double(v2[2]);}
	
void TabRegression::Init() {
	CtrlLayout(up);
	CtrlLayout(down);
	Add(splitter.SizePos());
	splitter.Vert(up.SizePos(), down.SizePos());
	HSizePos().VSizePos();

	down.gridDef.Clipboard().Editing().Appending().Removing().Sorting(false).ResizeColMode(0);
	down.gridDef.AddColumn("", 40).Edit(xCell);
	down.gridDef.AddColumn("", 40).Edit(xFrom);
	down.gridDef.AddColumn("", 40).Edit(xTo);
	down.gridDef.AddColumn("", 40).Edit(yCell);
	
	if (!LoadFromJsonFile(*this) || !notFirstTime) {
		down.yBesideX = true;
		down.firstCellIsName = false;
		notFirstTime = true;
		down.minR2 = 0;
		down.coefficients = 0;
	}

	up.scatter.SetMouseHandling(true, true).ShowContextMenu().ShowPropertiesDlg().ShowProcessDlg();

	const int totalCols = 50;
	
	editGrid.SetCount(totalCols);
	for(int i = 0; i < totalCols; i++) {
		String name = Format("Column %d", i);
		down.grid.AddColumn(name).Width(70).Edit(editGrid[i]);
	}
		
	down.grid.SelectRow(false);
	down.grid.SetRowCount(200);
	down.grid.ResizePaintMode(1);
	down.grid.Absolute().Clipboard().Editing().Sorting(false);
	down.grid.WhenPaste = THISBACK(OnAutoset);
	
	if (down.gridDef.GetRowCount() == 0) 
		down.gridDef.SetRowCount(1);
	
	down.switchColsRows = 0;
	OnButtons();
			
	down.gridTrend.AddColumn("Type", 10);
	down.gridTrend.AddColumn("Equation", 40);
	down.gridTrend.AddColumn("R2", 5);
	down.gridTrend.SetLineCy(EditField::GetStdHeight()).MultiSelect();
	down.gridTrend.WhenBar = THISBACK(OnArrayBar);
	down.gridTrend.Sort(r2Compare);
	     
	for (int i = 0; i < ExplicitEquation::GetEquationCount(); ++i) {
		ExplicitEquation &equation = equationTypes.Add(ExplicitEquation::Create(i));
		if (equation.GetNumCoeff(i) > 5)
			equationTypes.Remove(equationTypes.GetCount() - 1);
	}
	userEquation = new UserEquation;
	equationTypes.Add(userEquation);
	
	down.switchColsRows.WhenAction = THISBACK(OnButtons);
	down.yBesideX.WhenAction = THISBACK(OnButtons);
	down.firstCellIsName.WhenAction = THISBACK(OnButtons);
	     
	down.butFit.WhenPush = THISBACK(Fit);
	down.butClear.WhenPush = THISBACK(OnClear);
	down.butAutoset.WhenPush = THISBACK(OnAutoset);
	down.butUpdate.WhenPush = THISBACK(OnUpdate);
}

void TabRegression::End() {
	StoreAsJsonFile(*this, NULL, true);
}

void TabRegression::OnClear() {
	int nRows = down.grid.GetRowCount();
	down.grid.SetRowCount(0);
	down.grid.SetRowCount(nRows);
	down.grid.SetCursor(Point(0, 0));
	down.gridDef.Clear();
	up.scatter.RemoveAllSeries();
	down.gridTrend.Clear();
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
	if (down.switchColsRows == 0) {		// Columnas
		int fRow = down.firstCellIsName ? 1 : 0;
		
		int c;
		for (c = 0; c < down.grid.GetColumnCount(); ++c) {
			if (IsNumber2(down.grid.Get(fRow, c))) {
				down.gridDef.Set(0, 0, c);
				down.gridDef.Set(0, 3, c + 1);
				down.gridDef.Set(0, 1, fRow);
				break;
			}
		}
		if (c == down.grid.GetColumnCount()) {
			Exclamation(t_("Problem in Autoset"));
			return;
		}
		int r;
		for (r = fRow; r < down.grid.GetRowCount(); ++r) {
			if (!IsNumber2(down.grid.Get(r, c)))
				break;
		}
		down.gridDef.Set(0, 2, r - 1);
	} else {
		Exclamation("Not implemented");
	}
	
	OnUpdate();
}

void TabRegression::OnUpdate() {
	up.scatter.RemoveAllSeries();
	int idX = down.gridDef.Get(0, 0);
	int idY = down.gridDef.Get(0, 3);
	bool useCols = down.switchColsRows == 0;
	int beginData = down.gridDef.Get(0, 1);
	int numData = int(down.gridDef.Get(0, 2)) - beginData + 1;
	ds.Init(down.grid, idY, idX, useCols, beginData, numData);
	up.scatter.AddSeries(ds).Legend("Series").NoMark();
	down.gridTrend.Clear();
	up.scatter.ZoomToFit(true, true);
}

void TabRegression::OnButtons() {
	if (down.switchColsRows == 0) {
		down.gridDef.GetColumn(0).Name("X Col");
		down.gridDef.GetColumn(1).Name("Row From");
		down.gridDef.GetColumn(2).Name("Row To");
		down.gridDef.GetColumn(3).Name("Y Col");
	} else {
		down.gridDef.GetColumn(0).Name("X Row");
		down.gridDef.GetColumn(1).Name("Col From");
		down.gridDef.GetColumn(2).Name("Col To");	
		down.gridDef.GetColumn(3).Name("Y Row");
	}
	down.gridDef.GetColumn(3).Width(down.yBesideX ? 0 : 40);
	down.gridDef.Refresh();
}

void TabRegression::Fit() {
	int idX = down.gridDef.Get(0, 0);
	int idY = down.gridDef.Get(0, 3);
	bool useCols = down.switchColsRows == 0;
	int beginData = down.gridDef.Get(0, 1);
	int numData = int(down.gridDef.Get(0, 2)) - beginData + 1;
	ds.Init(down.grid, idY, idX, useCols, beginData, numData);
	
	userEquation->Init("User", down.formula);
	
	Array<double> r2;
	r2.SetCount(equationTypes.GetCount());
	
	for (int i = 0; i < equationTypes.GetCount(); ++i) {
		equationTypes[i].GuessCoeff(ds);
		equationTypes[i].Fit(ds, r2[i]);
	}
	up.scatter.RemoveAllSeries();
	up.scatter.AddSeries(ds).Legend("Series").NoMark();
	for (int i = 0; i < equationTypes.GetCount(); ++i) {
		double kk = r2[i];
		if (r2[i] >= down.minR2)
			up.scatter.AddSeries(equationTypes[i]).Legend(equationTypes[i].GetFullName()).NoMark().Stroke(2);
	}
	up.scatter.ZoomToFit(true, true);
	
	int numDecimals = 3;
	switch (down.coefficients) {
	case 1:	numDecimals = 40;	break;
	case 2:	numDecimals = Null;	break;
	}
	down.gridTrend.Clear();
	for (int i = 0; i < equationTypes.GetCount(); ++i) 
		down.gridTrend.Add(equationTypes[i].GetFullName(), equationTypes[i].GetEquation(numDecimals), r2[i]);
	down.gridTrend.SetSortColumn(2, true);
}


void TabRegression::ArrayCopy() {
	down.gridTrend.SetClipboard(true, true);
}

void TabRegression::ArraySelect() {
	down.gridTrend.Select(0, down.gridTrend.GetCount(), true);
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