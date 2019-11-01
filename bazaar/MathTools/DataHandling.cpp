#include "MathTools.h"

	
void TabData::Init() {
	CtrlLayout(up);
	CtrlLayout(down);
	CtrlLayout(downL);
	CtrlLayout(downR);
	
	Add(splitter.SizePos());
	splitter.Vert(up.SizePos(), down.SizePos());
	down.splitterDown.Horz(downL.SizePos(), downR.SizePos());
	
	HSizePos().VSizePos();

	downL.gridDef.Clipboard().Editing().Appending().Removing().Sorting(false);
	downL.gridDef.AddColumn(t_("X Col"), 30).Edit(xCell);
	downL.gridDef.AddColumn(t_("From"), 40) .Edit(xFrom);
	downL.gridDef.AddColumn(t_("To"), 60)   .Edit(xTo);
	downL.gridDef.AddColumn(t_("Y Col"), 30).Edit(yCell);
	downL.gridDef.AddColumn(t_("Show"), 30) .Edit(option);
	downL.gridDef.AddColumn(t_("Name"), 60);
	
	option.WhenAction = THISBACK(OnUpdate);
	
	splitterPos = splitterDownPos = Null;
	bool initSplitter = false;
	if (!LoadFromJsonFile(*this) || !notFirstTime) {
		downL.nIsHeader <<= 0;
		down.butFitPM = true;
		notFirstTime = true;
		initSplitter = true;
	}
	if (initSplitter || IsNull(splitterPos)) {
		splitterPos = 5500;
		splitterDownPos = 2500;
	} 
	splitter.SetPos(splitterPos, 0);
	down.splitterDown.SetPos(splitterDownPos, 0);
	
	up.scatter.SetMouseHandling(true, true).ShowButtons().ShowContextMenu().ShowPropertiesDlg().ShowProcessDlg();
	up.scatter.SetMode(ScatterDraw::MD_ANTIALIASED);
	
	const int totalCols = 50;
	
	editGrid.SetCount(totalCols);
	for(int i = 0; i < totalCols; i++) {
		String name = Format(t_("Column %d"), i + 1);
		downR.grid.AddColumn(name).Width(70).Edit(editGrid[i]);
	}
		
	downR.grid.SelectRow(false);
	downR.grid.SetRowCount(200);
	downR.grid.ResizePaintMode(1);
	downR.grid.Absolute().Clipboard().Editing().Sorting(false);
	downR.grid.WhenPaste = THISBACK(OnAutoset);
	
//	if (downr.gridDef.GetRowCount() == 0) 
//		downr.gridDef.SetRowCount(1);
	
	downR.butClear.WhenPush = THISBACK(OnClear);
	downL.butClearDef.WhenPush = THISBACK(OnClearDef);
	downL.butAutoset.WhenPush = THISBACK(OnAutoset);
	downL.butUpdate.WhenPush = THISBACK(OnUpdate);
	downL.butShowAll.WhenAction = THISBACK(OnShowAll);
	downL.nIsHeader.WhenAction = THISBACK(OnNIsheader);
	
	openMenuButton.Type(t_("Comma separated file"), "*.csv")
			  	  .Type(t_("Binary file"), "*.mtb")
  			  	  .Type(t_("Text file"), "*.txt")
  			  	  .DefaultExt("txt");
	openMenuButton.Attach(down.editFile);	
	openMenuButton.PreSelect(~down.editFile);
	
	butDown.SetImage(CtrlImg::down_arrow());
	butDown <<= THISBACK1(OnChangeFile, 1);
	down.editFile.AddFrame(butDown);
	
	butUp.SetImage(CtrlImg::up_arrow());
	butUp <<= THISBACK1(OnChangeFile, -1);
	down.editFile.AddFrame(butUp);
	
	openButton.SetImage(CtrlImg::menu_check());
	openButton <<= THISBACK2(OnOpen, true, true);
	down.editFile.AddFrame(openButton);
		
	down.editFile.WhenAction = THISBACK2(OnOpen, false, true);
	
	OnOpen(true, true);
}

void TabData::End() {
	splitterPos = splitter.GetPos(0);
	splitterDownPos = down.splitterDown.GetPos(0);
	StoreAsJsonFile(*this, NULL, true);
}

void TabData::OnClear() {
	int nRows = downR.grid.GetRowCount();
	downR.grid.SetRowCount(0);
	downR.grid.SetRowCount(nRows);
	downR.grid.SetCursor(Point(0, 0));
	up.scatter.RemoveAllSeries();
}

void TabData::OnClearDef() {
	downL.gridDef.Clear();
	up.scatter.RemoveAllSeries();
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

void TabData::OnAutoset() {
	int titleRow = downL.nIsHeader - 1;
	
	downL.gridDef.Clear();
	
	int xcol = Null;
	for (int c = 0; c < downR.grid.GetColumnCount(); ++c) {
		if (IsNumber2(downR.grid.Get(titleRow+1, c))) {
			if (IsNull(xcol)) 
				xcol = c;
			else {
				String name = AsString(downR.grid.Get(titleRow, c));
				downL.gridDef.Add(xcol+1, titleRow+2, 0, c+1, true, name);	
			}
		}
	}
	if (downL.gridDef.GetRowCount() == 0) {
		Exclamation(t_("No data found&Maybe '# row is header' is not properly set"));
		return;
	}
	for (int rowdef = 0; rowdef < downL.gridDef.GetRowCount(); ++rowdef) {
		int c = int(downL.gridDef.Get(rowdef, 3))-1;
		int r;
		for (r = titleRow+1; r < downR.grid.GetRowCount(); ++r) {
			if (!IsNumber2(downR.grid.Get(r, c)))
				break;
		}
		downL.gridDef.Set(rowdef, 2, r);
	}
	OnUpdate();
}

void TabData::OnUpdate() {
	if (downR.grid.GetRowCount() == 0 || downL.gridDef.GetRowCount() == 0)
		return;
	
	for (int r = 0; r < downL.gridDef.GetRowCount(); ++r) {
		String name;
		
		int row = int(downL.gridDef.Get(r, 1)) - 1 - 1;
		if (row < 0)
			row = 0;
		int col = int(downL.gridDef.Get(r, 3)) - 1;
		name = AsString(downR.grid.Get(row, col));
		downL.gridDef.Set(r, 5, name);
	}
	
	up.scatter.RemoveAllSeries();
	ds.Clear();
	for (int grow = 0; grow < downL.gridDef.GetRowCount(); ++grow) {
		if (!downL.gridDef.Get(grow, 4))
			continue;
		int idX = int(downL.gridDef.Get(grow, 0))-1;
		int idY = int(downL.gridDef.Get(grow, 3))-1;
		int beginData = int(downL.gridDef.Get(grow, 1))-1;
		int numData = int(downL.gridDef.Get(grow, 2)) - beginData;
		GridCtrlSource &uds = ds.Add();
		uds.Init(downR.grid, idY, idX, true, beginData, numData);
		String serieName, xLegend;
		if (downL.nIsHeader > 0) {
			int row, col;
			row = int(~downL.nIsHeader) - 1;
			col = int(downL.gridDef.Get(grow, 0))-1;
			if (IsNull(col))
				return;
			xLegend = AsString(downR.grid.Get(row, col));
			col = int(downL.gridDef.Get(grow, 3))-1;
			if (IsNull(col))
				return;
			serieName = AsString(downR.grid.Get(row, col));
		} else
			serieName << t_("Series") << " " << grow;
		up.scatter.AddSeries(uds).Legend(serieName).NoMark().Stroke(2);
		if (!xLegend.IsEmpty()) {
			up.scatter.SetPlotAreaBottomMargin(45);
			up.scatter.SetLabelX(xLegend);
		} else {
			up.scatter.SetPlotAreaBottomMargin(30);
			up.scatter.SetLabelX("");
		}
	}
	if (down.butFitPM)
		up.scatter.ZoomToFit(true, true);
}

int GuessCsvSeparator(FileIn &in) {
	Vector<int> sep;
	sep << ',' << '.' << ';' << ':' << '\t' << ' ';
	Vector<String> lines;
	for (int i = 0; i < 20 && !in.IsEof(); ++i) 
		lines << in.GetLine();
	if (lines.GetCount() >= 10)
		lines.Remove(0, 10);
	Vector<int> sepCount;
	sepCount.SetCount(sep.GetCount(), 0);
	for (int i = 0; i < lines.GetCount(); ) {
		String &s =	lines[i];
		bool instring = false;
		for (int ic = 0; ic < s.GetCount(); ++ic) {
			int c = s[ic];
			if(c == '\"') {
				if(instring && (i < lines.GetCount()-1) && s[ic+1] == '\"') 
					i++;
				else
					instring = !instring;
			} else if (!IsLeNum(c)) {
				for (int is = 0; is < sep.GetCount(); ++is) {
					if (c == sep[is])
						sepCount[is]++;
				}
			}
			i++;
		}
	}
	int separator = Null;
	IndexSort(sepCount, sep, StdGreater<int>());
	int idMax = (sep[0] == '.') ? 1 : 0;
	if (sepCount[idMax] > 0)
		separator = sep[idMax];	
	in.Seek(0);
	return separator;
}
		
void TabData::OnOpen(bool force, bool updateButtons) {
	if (!force && (~openMenuButton != ~down.editFile))
		return;
	
	WaitCursor wait;
	if (updateButtons) {
		butUp.Show(false);
		butDown.Show(false);
	}

	String fileName = TrimBoth(~down.editFile);
	if (fileName.IsEmpty())
		return;
	FindFile ff(fileName);
	if(!ff || !ff.IsFile()) {
		Exclamation(Format(t_("File '%s' does not exist"), DeQtf(fileName)));
		return;
	}
	openMenuButton.PreSelect(fileName);
	
	if (GetFileExt(fileName) == ".mtb") {
		ScatterDraw data;
		
		if (!LoadFromFile(data, fileName)) {
			Exclamation(t_("Problem reading binary file"));
			return;
		}
		if (data.GetCount() == 0 || data.GetDataSource(0).GetCount() == 0) {
			Exclamation(t_("Data table not found"));
			return;
		}
		downR.grid.SetRowCount(0);
		/*for (int col = 0; col < saver.data[0].data.GetCount(); ++col) {
			for (int row = 0; row < saver.data[0].data[col].GetCount(); row++) 
				downR.grid.Set(row, col, saver.data[0].data[col][row]);	
		}*/
	} else {
		FileIn in(fileName);
		in.ClearError();
		
		int separator = GuessCsvSeparator(in);
		if (IsNull(separator))
			separator = ';';
		
		downR.grid.SetRowCount(0);
			
		byte charset = CHARSET_UTF8;
	
		for (int row = 0; ; row++) {
			String sline = in.GetLine();
			if (sline.IsVoid())
				break;
			StringStream line(sline);
			Vector<String> fields = GetCsvLine(line, separator, charset);	
			for (int col = 0; col < fields.GetCount(); ++col) {
				double d = ScanDouble(fields[col]);
				if (IsNull(d))
					downR.grid.Set(row, col, fields[col]);	
				else
					downR.grid.Set(row, col, d);	
			}
		}
	}
	if (down.butSetPM || downL.gridDef.GetRowCount() == 0)
		OnAutoset();
	else
		OnUpdate();
	up.scatter.SetTitle(GetFileName(fileName));
	if (updateButtons)
		OnChangeFile(0);
}

void TabData::OnChangeFile(int delta) {
	String fileName = ~down.editFile;
	
	String ext = GetFileExt(fileName);
	FindFile ff(AppendFileName(GetFileFolder(fileName), "*" + ext));
	Index<String> fileList;
	do {
		if (ff.IsFile()) 
			fileList << ff.GetPath();
	} while(ff.Next());
	if (fileList.IsEmpty()) {
		butUp.Show(false);
		butDown.Show(false);
		return;
	}
	int id = fileList.Find(fileName) + delta;
	butUp.Show(id != 0);
	butDown.Show(id != fileList.GetCount() - 1);
 	
 	if (delta == 0)
 		return;
	down.editFile <<= fileList[id];
	OnOpen(true, false);
}

void TabData::OnShowAll() {
	bool showAll = downL.butShowAll;
	for (int grow = 0; grow < downL.gridDef.GetRowCount(); ++grow) 
		downL.gridDef.Set(grow, 4, showAll);
	OnUpdate();
}

void TabData::OnNIsheader() {
	int n = downL.nIsHeader + 1;
	for (int grow = 0; grow < downL.gridDef.GetRowCount(); ++grow) 
		downL.gridDef.Set(grow, 1, n);
	OnUpdate();	
}

Tool *Construct1()
{
	static TabData tab;
	tab.name = t_("Data Handling");
	return &tab;
}

INITBLOCK {
	RegisterExample(Construct1, __FILE__);
}