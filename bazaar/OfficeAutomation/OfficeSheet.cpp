#include <Core/Core.h>

#ifndef PLATFORM_WIN32
#error Sorry: This platform is not supported!. Look for OfficeAutomation in Bazaar Upp Forum to search for info and new news
#endif

#include "OfficeAutomationBase.h"
#include "OfficeAutomation.h"


OfficeSheet::OfficeSheet() 	 {
	data = NULL;
	Ole::Init();
}

OfficeSheet::~OfficeSheet()	 {
	if (!data)
		return;
	if (type == SheetOPEN)
		delete (OPENSheet*)data;
	else if (type == SheetMS)
		delete (MSSheet*)data;		
	
	Ole::Close();
}

void OfficeSheet::CellToColRow(const char *cell, int &col, int &row) {
	String s_col;
	int i;
	int lenCell = strlen(cell);
	for (i = 0; (i < lenCell) && (cell[i] >= 'A') && (cell[i] <= 'Z'); ++i)
		s_col.Cat(cell[i]);
	
	row = atoi(cell + i);
	
	int num_az = 'Z' - 'A' + 1;
	col = 0;
	int len = s_col.GetCount();
	int p = (int)pow((double)num_az, len-1);
	for (int j = 0; j < s_col.GetCount(); ++j) {
		col += (s_col[j] - 'A' + 1)*p;
		p /= num_az;
	}
}

void OfficeSheet::CellToColRow(const char *cell, Cell &cellPos) {
	CellToColRow(cell, cellPos.col, cellPos.row);
}

String OfficeSheet::ColRowToCell(const int col, const int row) {
	String cell;
	int num_az = 'Z' - 'A' + 1;
	int firstLetter = col/num_az;
	int secondLetter = col - firstLetter*num_az;
	int len = firstLetter > 0 ? 2: 1;
		
	StringBuffer bCell(len);
	if (len == 2) {
		bCell[0] = 'A' + firstLetter - 1;
		bCell[1] = 'A' + secondLetter - 1;
	} else if (len == 1)
		bCell[0] = 'A' + secondLetter - 1;
	else
		return "Error";
	bCell.Cat(AsString(row));
	cell = bCell;
	
	return cell;
}

String OfficeSheet::ColRowToCell(const Cell &cellPos) {
	return ColRowToCell(cellPos.col, cellPos.row);
}		

bool OfficeSheet::IsAvailable(String strtype) {
	if (strtype == "Open" || strtype == "Libre")
		return OPENSheet::IsAvailable();
	else if (strtype == "Microsoft")
		return MSSheet::IsAvailable();
	else
		return false;
}

bool OfficeSheet::Init(String strtype) {
	if (data) {
		if (type == SheetOPEN)
			delete (OPENSheet*)data;
		else if (type == SheetMS)
			delete (MSSheet*)data;	
	}
	if (strtype == "Open" || strtype == "Libre") {
		type = SheetOPEN;
		data = new OPENSheet();
	} else if (strtype == "Microsoft") {
		type = SheetMS;
		data = new MSSheet();
	} else
		return false;
	return true;
}

bool OfficeSheet::AddSheet(bool visible) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->AddSheet(visible);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->AddSheet(visible);
	else
		return false;
}

bool OfficeSheet::OpenSheet(String fileName, bool visible) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->OpenSheet(fileName, visible);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->OpenSheet(fileName, visible);
	else
		return false;
}

bool OfficeSheet::SetValue(int col, int row, Value value) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetValue(col, row, value);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetValue(col, row, value);
	else
		return false;
}

bool OfficeSheet::SetValue(String cell, Value value) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetValue(cell, value);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetValue(cell, value);
	else
		return false;
}

bool OfficeSheet::SetValue(Value value) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetValue(value);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetValue(value);
	else
		return false;
}

Value OfficeSheet::GetValue(String cell) {
	if (!data)
		return Null;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->GetValue(cell);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->GetValue(cell);
	else
		return Null;
}

Value OfficeSheet::GetValue(int col, int row) {
	if (!data)
		return Null;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->GetValue(col, row);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->GetValue(col, row);
	else
		return Null;
}

bool OfficeSheet::Replace(Value search, Value replace) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->Replace(search, replace);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->Replace(search, replace);
	else
		return false;
}

bool OfficeSheet::SetBold(int col, int row, bool bold) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetBold(col, row, bold);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetBold(col, row, bold);
	else
		return false;
}

bool OfficeSheet::SetBold(String cell, bool bold) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetBold(cell, bold);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetBold(cell, bold);
	else
		return false;
}

bool OfficeSheet::SetBold(bool bold) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetBold(bold);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetBold(bold);
	else
		return false;
}

bool OfficeSheet::SetItalic(int col, int row, bool italic) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetItalic(col, row, italic);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetItalic(col, row, italic);
	else
		return false;
}

bool OfficeSheet::SetItalic(String cell, bool italic) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetItalic(cell, italic);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetItalic(cell, italic);
	else
		return false;
}

bool OfficeSheet::SetItalic(bool italic) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetItalic(italic);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetItalic(italic);
	else
		return false;
}

bool OfficeSheet::SetUnderline(int col, int row, bool underline) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetUnderline(col, row, underline);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetUnderline(col, row, underline);
	else
		return false;
}

bool OfficeSheet::SetUnderline(String cell, bool underline) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetUnderline(cell, underline);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetUnderline(cell, underline);
	else
		return false;
}

bool OfficeSheet::SetUnderline(bool underline) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetUnderline(underline);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetUnderline(underline);
	else
		return false;
}

bool OfficeSheet::SetFont(int col, int row, String name, int size) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetFont(col, row, name, size);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetFont(col, row, name, size);
	else
		return false;
}

bool OfficeSheet::SetFont(String cell, String name, int size) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetFont(cell, name, size);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetFont(cell, name, size);
	else
		return false;
}

bool OfficeSheet::SetFont(String name, int size) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetFont(name, size);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetFont(name, size);
	else
		return false;
}

bool OfficeSheet::SetColor(int col, int row, Color color) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetColor(col, row, color);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetColor(col, row, color);
	else
		return false;
}

bool OfficeSheet::SetColor(String cell, Color color) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetColor(cell, color);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetColor(cell, color);
	else
		return false;
}

bool OfficeSheet::SetColor(Color color) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetColor(color);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetColor(color);
	else
		return false;
}

bool OfficeSheet::SetBackColor(int col, int row, Color color) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetBackColor(col, row, color);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetBackColor(col, row, color);
	else
		return false;
}

bool OfficeSheet::SetBackColor(String cell, Color color) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetBackColor(cell, color);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetBackColor(cell, color);
	else
		return false;
}

bool OfficeSheet::SetBackColor(Color color) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetBackColor(color);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetBackColor(color);
	else
		return false;
}

bool OfficeSheet::SetRowHeight(int row, double height) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetRowHeight(row, height);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetRowHeight(row, height);
	else
		return false;
}	

bool OfficeSheet::SetColWidth(int col, double width) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetColWidth(col, width);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetColWidth(col, width);
	else
		return false;
}	

bool OfficeSheet::SetVertAlignment(int col, int row, int alignment) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetVertAlignment(col, row, alignment);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetVertAlignment(col, row, alignment);
	else
		return false;
}

bool OfficeSheet::SetVertAlignment(String cell, int alignment) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetVertAlignment(cell, alignment);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetVertAlignment(cell, alignment);
	else
		return false;
}
	
bool OfficeSheet::SetHorizAlignment(int col, int row, int alignment) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetHorizAlignment(col, row, alignment);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetHorizAlignment(col, row, alignment);
	else
		return false;
}

bool OfficeSheet::SetHorizAlignment(String cell, int alignment) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetHorizAlignment(cell, alignment);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetHorizAlignment(cell, alignment);
	else
		return false;
}	

bool OfficeSheet::SetBorder(int col, int row, int borderIndx, int lineStyle, int weight, Color color) {
	if (!data)
		return false;
//	if (type == SheetOPEN) 
//		return ((OPENSheet*)data)->SetBorder(col, row, borderIndx, lineStyle, weight, color);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetBorder(col, row, borderIndx, lineStyle, weight, color);  
	else
		return false;
}

bool OfficeSheet::SetBorder(int borderIndx, int lineStyle, int weight, Color color) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetBorder(borderIndx, lineStyle, weight, color);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetBorder(borderIndx, lineStyle, weight, color);  
	else
		return false;
}
	
bool OfficeSheet::Select(String range) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->Select(range);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->Select(range);
	else
		return false;
}

bool OfficeSheet::Select(int fromX, int fromY, int toX, int toY) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->Select(fromX, fromY, toX, toY);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->Select(fromX, fromY, toX, toY);
	else
		return false;
}

bool OfficeSheet::Select() {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->Select();  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->Select();
	else
		return false;
}
/*
void OfficeSheet::DefMatrix(int width, int height)
{
	if (!data)
		return;
	if (type == SheetOPEN) 
		((OPENSheet*)data)->DefMatrix(width, height);  
	else if (type == SheetMS) 
		((MSSheet*)data)->DefMatrix(width, height);
	else
		return;
}
bool OfficeSheet::FillSelectionMatrix()
{
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->FillSelectionMatrix();  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->FillSelectionMatrix();
	else
		return false;
}
void OfficeSheet::SetMatrixValue(int i, int j, ::Value value)
{
	if (!data)
		return;
	if (type == SheetOPEN) 
		((OPENSheet*)data)->SetMatrixValue(i, j, value);  
	else if (type == SheetMS) 
		((MSSheet*)data)->SetMatrixValue(i, j, value);
	else
		return;
}	
*/
bool OfficeSheet::SaveAs(String fileName, String _type) {
	if (!data)
		return false;
	if (type == SheetOPEN) {
		int ret;
		ret = ((OPENSheet*)data)->SaveAs(fileName, _type);  
		((OPENSheet*)data)->SetSaved(true);
		return ret;
	} else if (type == SheetMS) {
		((MSSheet*)data)->SetSaved(true);
		return ((MSSheet*)data)->SaveAs(fileName, _type);
	} else
		return false;
}

bool OfficeSheet::Quit() {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->Quit();  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->Quit();
	else
		return false;
}

bool OfficeSheet::Print() {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->Print();  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->Print();
	else
		return false;
}

bool OfficeSheet::InsertTab(String name) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->InsertTab(name);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->InsertTab(name);
	else
		return false;
}

bool OfficeSheet::ChooseTab(String name) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->ChooseTab(name);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->ChooseTab(name);
	else
		return false;
}

bool OfficeSheet::ChooseTab(int index) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->ChooseTab(index);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->ChooseTab(index);
	else
		return false;
}	

bool OfficeSheet::RemoveTab(String name) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->RemoveTab(name);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->RemoveTab(name);
	else
		return false;
}

bool OfficeSheet::RemoveTab(int index) {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->RemoveTab(index);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->RemoveTab(index);
	else
		return false;
}

int OfficeSheet::GetNumTabs() {
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->GetNumTabs();  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->GetNumTabs();
	else
		return false;
}	
