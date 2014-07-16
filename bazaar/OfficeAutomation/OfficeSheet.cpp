#include <Core/Core.h>

using namespace Upp;

#include <Functions4U/Functions4U.h>

#ifndef PLATFORM_WIN32
#error Sorry: This platform is not supported!. Look for OfficeAutomation in Bazaar Upp Forum to search for info and new news
#endif

#include "OfficeAutomation.h"
#include "OfficeAutomationBase.h"


OfficeSheet::OfficeSheet() 	 {
	Ole::Init();
}

OfficeSheet::~OfficeSheet()	 {
	Ole::Close();
}

void OfficeSheet::CellToColRow(const char *cell, int &col, int &row) {
	String s_col;
	int lenCell = int(strlen(cell));
	int i;
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

String OfficeSheet::Function(String name, String arg1, String arg2, String arg3, String arg4, String arg5, String arg6) {
	String sep;
	if (GetName() != "Microsoft")
		sep = ";";
	else
		sep = ",";

	String ret = "=" + name + "(";
	if (!arg1.IsEmpty())
		ret += arg1;
	if (!arg2.IsEmpty())
		ret += sep + arg2;
	if (!arg3.IsEmpty())
		ret += sep + arg3;
	if (!arg4.IsEmpty())
		ret += sep + arg4;
	if (!arg5.IsEmpty())
		ret += sep + arg5;
	if (!arg6.IsEmpty())
		ret += sep + arg6;
	ret += ")";
	return ret;
}

bool OfficeSheet::Init(const char *name) {
	return PluginInit(*this, name);
}

bool SheetPlugin::IsAvailable() {return false;}
bool OfficeSheet::IsAvailable(const char *_name) {
	for (int i = 0; i < Plugins().GetCount(); ++i) {
		if (Plugins()[i].name == _name && Plugins()[i].type == typeid(OfficeSheet).name()) {
			void *dat = Plugins()[i].New();
			if (!dat)
				return false;
			bool ret = (static_cast<SheetPlugin *>(dat))->IsAvailable();
			Plugins()[i].Delete(dat);
			return ret;
		}
	}
	return false;
}


bool SheetPlugin::AddSheet(bool visible) {return false;}
bool OfficeSheet::AddSheet(bool visible) {return (static_cast<SheetPlugin *>(GetData()))->AddSheet(visible);}

bool SheetPlugin::OpenSheet(String fileName, bool visible) {return false;}
bool OfficeSheet::OpenSheet(String fileName, bool visible) {return (static_cast<SheetPlugin *>(GetData()))->OpenSheet(fileName, visible);}

bool SheetPlugin::SetValue(int col, int row, Value value) {return false;}
bool OfficeSheet::SetValue(int col, int row, Value value) {return (static_cast<SheetPlugin *>(GetData()))->SetValue(col, row, value);}

bool SheetPlugin::SetValue(String cell, Value value) {return false;}
bool OfficeSheet::SetValue(String cell, Value value) {return (static_cast<SheetPlugin *>(GetData()))->SetValue(cell, value);}

bool SheetPlugin::SetValue(Value value) {return false;}
bool OfficeSheet::SetValue(Value value) {return (static_cast<SheetPlugin *>(GetData()))->SetValue(value);}

Value SheetPlugin::GetValue(String cell) {return Null;}
Value OfficeSheet::GetValue(String cell) {return (static_cast<SheetPlugin *>(GetData()))->GetValue(cell);}

Value SheetPlugin::GetValue(int col, int row) {return Null;}
Value OfficeSheet::GetValue(int col, int row) {return (static_cast<SheetPlugin *>(GetData()))->GetValue(col, row);}

Value SheetPlugin::GetText(String cell) {return Null;}
Value OfficeSheet::GetText(String cell) {return (static_cast<SheetPlugin *>(GetData()))->GetText(cell);}

Value SheetPlugin::GetText(int col, int row) {return Null;}
Value OfficeSheet::GetText(int col, int row) {return (static_cast<SheetPlugin *>(GetData()))->GetText(col, row);}

bool SheetPlugin::SetHyperlink(int col, int row, String address, String text) {return false;}
bool OfficeSheet::SetHyperlink(int col, int row, String address, String text) {return (static_cast<SheetPlugin *>(GetData()))->SetHyperlink(col, row, address, text);}

bool SheetPlugin::SetHyperlink(String cell, String address, String text) {return false;}
bool OfficeSheet::SetHyperlink(String cell, String address, String text) {return (static_cast<SheetPlugin *>(GetData()))->SetHyperlink(cell, address, text);}

bool SheetPlugin::SetHyperlink(String address, String text) {return false;}
bool OfficeSheet::SetHyperlink(String address, String text) {return (static_cast<SheetPlugin *>(GetData()))->SetHyperlink(address, text);}

bool SheetPlugin::Replace(Value search, Value replace) {return false;}
bool OfficeSheet::Replace(Value search, Value replace) {return (static_cast<SheetPlugin *>(GetData()))->Replace(search, replace);}

bool SheetPlugin::SetBold(int col, int row, bool bold) {return false;}
bool OfficeSheet::SetBold(int col, int row, bool bold) {return (static_cast<SheetPlugin *>(GetData()))->SetBold(col, row, bold);}

bool SheetPlugin::SetBold(String cell, bool bold) {return false;}
bool OfficeSheet::SetBold(String cell, bool bold) {return (static_cast<SheetPlugin *>(GetData()))->SetBold(cell, bold);}

bool SheetPlugin::SetBold(bool bold) {return false;}
bool OfficeSheet::SetBold(bool bold) {return (static_cast<SheetPlugin *>(GetData()))->SetBold(bold);}

bool SheetPlugin::SetItalic(int col, int row, bool italic) {return false;}
bool OfficeSheet::SetItalic(int col, int row, bool italic) {return (static_cast<SheetPlugin *>(GetData()))->SetBold(col, row, italic);}

bool SheetPlugin::SetItalic(String cell, bool italic) {return false;}
bool OfficeSheet::SetItalic(String cell, bool italic) {return (static_cast<SheetPlugin *>(GetData()))->SetBold(cell, italic);}

bool SheetPlugin::SetItalic(bool italic) {return false;}
bool OfficeSheet::SetItalic(bool italic) {return (static_cast<SheetPlugin *>(GetData()))->SetBold(italic);}

bool SheetPlugin::SetUnderline(int col, int row, bool underline) {return false;}
bool OfficeSheet::SetUnderline(int col, int row, bool underline) {return (static_cast<SheetPlugin *>(GetData()))->SetUnderline(col, row, underline);}

bool SheetPlugin::SetUnderline(String cell, bool underline) {return false;}
bool OfficeSheet::SetUnderline(String cell, bool underline) {return (static_cast<SheetPlugin *>(GetData()))->SetUnderline(cell, underline);}

bool SheetPlugin::SetUnderline(bool underline) {return false;}
bool OfficeSheet::SetUnderline(bool underline) {return (static_cast<SheetPlugin *>(GetData()))->SetUnderline(underline);}

bool SheetPlugin::SetFont(int col, int row, String name, int size) {return false;}
bool OfficeSheet::SetFont(int col, int row, String name, int size) {return (static_cast<SheetPlugin *>(GetData()))->SetFont(col, row, name, size);}

bool SheetPlugin::SetFont(String cell, String name, int size) {return false;}
bool OfficeSheet::SetFont(String cell, String name, int size) {return (static_cast<SheetPlugin *>(GetData()))->SetFont(cell, name, size);}

bool SheetPlugin::SetFont(String name, int size) {return false;}
bool OfficeSheet::SetFont(String name, int size) {return (static_cast<SheetPlugin *>(GetData()))->SetFont(name, size);}

bool SheetPlugin::SetColor(int col, int row, Color color) {return false;}
bool OfficeSheet::SetColor(int col, int row, Color color) {return (static_cast<SheetPlugin *>(GetData()))->SetColor(col, row, color);}

bool SheetPlugin::SetColor(String cell, Color color) {return false;}
bool OfficeSheet::SetColor(String cell, Color color) {return (static_cast<SheetPlugin *>(GetData()))->SetColor(cell, color);}

bool SheetPlugin::SetColor(Color color) {return false;}
bool OfficeSheet::SetColor(Color color) {return (static_cast<SheetPlugin *>(GetData()))->SetColor(color);}

bool SheetPlugin::SetBackColor(int col, int row, Color color) {return false;}
bool OfficeSheet::SetBackColor(int col, int row, Color color) {return (static_cast<SheetPlugin *>(GetData()))->SetBackColor(col, row, color);}

bool SheetPlugin::SetBackColor(String cell, Color color) {return false;}
bool OfficeSheet::SetBackColor(String cell, Color color) {return (static_cast<SheetPlugin *>(GetData()))->SetBackColor(cell, color);}

bool SheetPlugin::SetBackColor(Color color) {return false;}
bool OfficeSheet::SetBackColor(Color color) {return (static_cast<SheetPlugin *>(GetData()))->SetBackColor(color);}

bool SheetPlugin::SetRowHeight(int row, double height) {return false;}
bool OfficeSheet::SetRowHeight(int row, double height) {return (static_cast<SheetPlugin *>(GetData()))->SetRowHeight(row, height);}

bool SheetPlugin::SetColWidth(int col, double width) {return false;}
bool OfficeSheet::SetColWidth(int col, double width) {return (static_cast<SheetPlugin *>(GetData()))->SetColWidth(col, width);}

bool SheetPlugin::SetVertAlignment(int col, int row, int alignment) {return false;}
bool OfficeSheet::SetVertAlignment(int col, int row, int alignment) {return (static_cast<SheetPlugin *>(GetData()))->SetVertAlignment(col, row, alignment);}

bool SheetPlugin::SetVertAlignment(String cell, int alignment) {return false;}
bool OfficeSheet::SetVertAlignment(String cell, int alignment) {return (static_cast<SheetPlugin *>(GetData()))->SetVertAlignment(cell, alignment);}

bool SheetPlugin::SetHorizAlignment(int col, int row, int alignment) {return false;}
bool OfficeSheet::SetHorizAlignment(int col, int row, int alignment) {return (static_cast<SheetPlugin *>(GetData()))->SetHorizAlignment(col, row, alignment);}

bool SheetPlugin::SetHorizAlignment(String cell, int alignment) {return false;}
bool OfficeSheet::SetHorizAlignment(String cell, int alignment) {return (static_cast<SheetPlugin *>(GetData()))->SetHorizAlignment(cell, alignment);}
	
bool SheetPlugin::SetBorder(int col, int row, int borderIndx, int lineStyle, int weight, Color color) {return false;}
bool OfficeSheet::SetBorder(int col, int row, int borderIndx, int lineStyle, int weight, Color color) {return (static_cast<SheetPlugin *>(GetData()))->SetBorder(col, row, borderIndx, lineStyle, weight, color);}

bool SheetPlugin::SetBorder(int borderIndx, int lineStyle, int weight, Color color) {return false;}
bool OfficeSheet::SetBorder(int borderIndx, int lineStyle, int weight, Color color) {return (static_cast<SheetPlugin *>(GetData()))->SetBorder(borderIndx, lineStyle, weight, color);}

bool SheetPlugin::Select() {return false;}
bool OfficeSheet::Select() {return (static_cast<SheetPlugin *>(GetData()))->Select();}

bool SheetPlugin::Select(String range) {return false;}
bool OfficeSheet::Select(String range) {return (static_cast<SheetPlugin *>(GetData()))->Select(range);}

bool SheetPlugin::Select(int fromX, int fromY, int toX, int toY) {return false;}
bool OfficeSheet::Select(int fromX, int fromY, int toX, int toY) {return (static_cast<SheetPlugin *>(GetData()))->Select(fromX, fromY, toX, toY);}

bool SheetPlugin::EnableCommandVars(bool) {return false;}
bool OfficeSheet::EnableCommandVars(bool enable) {return (static_cast<DocPlugin *>(GetData()))->EnableCommandVars(enable);}
	
bool SheetPlugin::MatrixAllocate(int width, int height) {return false;}
bool OfficeSheet::MatrixAllocate(int width, int height) {return (static_cast<SheetPlugin *>(GetData()))->MatrixAllocate(width, height);}

bool SheetPlugin::MatrixDelete() {return false;}
bool OfficeSheet::MatrixDelete() {return (static_cast<SheetPlugin *>(GetData()))->MatrixDelete();}

bool SheetPlugin::MatrixFillSelection() {return false;}
bool OfficeSheet::MatrixFillSelection() {return (static_cast<SheetPlugin *>(GetData()))->MatrixFillSelection();}

bool SheetPlugin::MatrixFill(int fromX, int fromY, Vector<Vector<Value> > &data) {return false;}
bool OfficeSheet::MatrixFill(int fromX, int fromY, Vector<Vector<Value> > &data) {return (static_cast<SheetPlugin *>(GetData()))->MatrixFill(fromX, fromY, data);}

bool SheetPlugin::MatrixSetValue(int i, int j, ::Value value) {return false;}
bool OfficeSheet::MatrixSetValue(int i, int j, ::Value value) {return (static_cast<SheetPlugin *>(GetData()))->MatrixSetValue(i, j, value);}

bool SheetPlugin::SaveAs(String fileName, String _type) {return false;}
bool OfficeSheet::SaveAs(String fileName, String _type) {return (static_cast<SheetPlugin *>(GetData()))->SaveAs(fileName, _type);}

bool SheetPlugin::SetSaved(bool saved) {return false;}
bool OfficeSheet::SetSaved(bool saved) {return (static_cast<SheetPlugin *>(GetData()))->SetSaved(saved);}

bool SheetPlugin::Quit() {return false;}
bool OfficeSheet::Quit() {return (static_cast<SheetPlugin *>(GetData()))->Quit();}
	
bool SheetPlugin::Print() {return false;}
bool OfficeSheet::Print() {return (static_cast<SheetPlugin *>(GetData()))->Print();}
		
bool SheetPlugin::InsertTab(String name) {return false;}
bool OfficeSheet::InsertTab(String name) {return (static_cast<SheetPlugin *>(GetData()))->InsertTab(name);}

bool SheetPlugin::ChooseTab(String name) {return false;}
bool OfficeSheet::ChooseTab(String name) {return (static_cast<SheetPlugin *>(GetData()))->ChooseTab(name);}

bool SheetPlugin::ChooseTab(int index) {return false;}
bool OfficeSheet::ChooseTab(int index) {return (static_cast<SheetPlugin *>(GetData()))->ChooseTab(index);}

bool SheetPlugin::RemoveTab(String name) {return false;}
bool OfficeSheet::RemoveTab(String name) {return (static_cast<SheetPlugin *>(GetData()))->RemoveTab(name);}

bool SheetPlugin::RemoveTab(int index) {return false;}
bool OfficeSheet::RemoveTab(int index) {return (static_cast<SheetPlugin *>(GetData()))->RemoveTab(index);}

int SheetPlugin::GetNumTabs() {return false;}
int OfficeSheet::GetNumTabs() {return (static_cast<SheetPlugin *>(GetData()))->GetNumTabs();}
