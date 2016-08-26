#include "RichText.h"

namespace Upp {

RichTable::Format RichText::GetTableFormat(int table) const
{
	Mutex::Lock __(mutex);
	return GetConstTable(table).GetFormat();
}

void RichText::SetTableFormat(int table, const RichTable::Format& fmt)
{
	RichTable& tab = GetUpdateTable(table);
	tab.SetFormat(fmt);
	tab.Normalize();
}

int  RichText::SetTable(int pos, const RichTable& table)
{
	RefreshAll();
	RichPos p = GetRichPos(pos);
	int bpos = pos;
	RichTxt& txt = GetUpdateText(pos);
	int pi = txt.FindPart(pos);
	ASSERT(pos == 0 && txt.GetPartLength(pi) == 0 && txt.IsPara(pi));
	RichTable pt(table, 1);
	txt.SetPick(pi, pick(pt));
	return GetRichPos(bpos).table;
}

RichTable RichText::CopyTable(int table) const
{
	Mutex::Lock __(mutex);
	RichTable tab(GetConstTable(table), 1);
	return tab;
}

void RichText::ReplaceTable(int table, const RichTable& tab)
{
	(GetUpdateTable(table) <<= tab).Normalize();
	RefreshAll();
}

void      RichText::DestroyTable(int table)
{
	int pi;
	RichTxt& txt = GetTableUpdateText(table, style, pi);
	RichPara p;
	txt.Set(pi, p, style);
	RefreshAll();
}

void RichText::RemoveParaSpecial(int table, bool before)
{
	int pi;
	RichTxt& txt = GetTableUpdateText(table, style, pi);
	if(before)
		pi--;
	else
		pi++;
	ASSERT(txt.IsPara(pi) && txt.GetPartLength(pi) == 0);
	txt.part.Remove(pi);
	RefreshAll();
}

bool RichText::CanRemoveParaSpecial(int table, bool before)
{
	int pi;
	if(!table)
		return false;
	RichTxt& txt = GetTableUpdateText(table, style, pi);
	if(before)
		pi--;
	else
		pi++;
	return pi >= 0 && pi < txt.GetPartCount() && txt.IsPara(pi) && txt.GetPartLength(pi) == 0;
}

bool RichText::ShouldInsertParaSpecial(int table, bool before)
{
	int pi;
	RichTxt& txt = GetTableUpdateText(table, style, pi);
	if(!before)
		pi++;
	return pi == 0 || pi >= txt.GetPartCount() || txt.IsTable(pi);
}


void RichText::InsertParaSpecial(int table, bool before, const RichPara::Format& fmt)
{
	int pi;
	RichTxt& txt = GetTableUpdateText(table, style, pi);
	if(!before)
		pi++;
	txt.RefreshAll();
	txt.part.Insert(pi);
	RichPara p;
	p.format = fmt;
	txt.Set(pi, p, style);
	RefreshAll();
}

RichTable RichText::CopyTable(int table, const Rect& sel) const
{
	Mutex::Lock __(mutex);
	return GetConstTable(table).Copy(sel);
}

Point RichText::GetMasterCell(int table, int row, int column)
{
	return GetConstTable(table).GetMasterCell(row, column);
}

void  RichText::PasteTable(int table, Point pos, const RichTable& tab)
{
	GetUpdateTable(table).Paste(pos, tab);
	RefreshAll();
}

void  RichText::InsertTableRow(int table, int row)
{
	GetUpdateTable(table).InsertRow(row, style);
	RefreshAll();
}

void  RichText::RemoveTableRow(int table, int row)
{
	GetUpdateTable(table).RemoveRow(row);
	RefreshAll();
}

void  RichText::InsertTableColumn(int table, int column)
{
	GetUpdateTable(table).InsertColumn(column, style);
	RefreshAll();
}

void  RichText::RemoveTableColumn(int table, int column)
{
	GetUpdateTable(table).RemoveColumn(column);
	RefreshAll();
}

void  RichText::SplitCell(int table, Point cell, Size sz)
{
	GetUpdateTable(table).SplitCell(cell, sz, style);
	RefreshAll();
}

void  RichText::JoinCell(int table, const Rect& sel)
{
	RichTable& tab = GetUpdateTable(table);
	RichCell& cell = tab[sel.top][sel.left];
	cell.vspan = sel.bottom - sel.top;
	cell.hspan = sel.right - sel.left;
	for(int i = sel.top; i <= sel.bottom; i++)
		for(int j = sel.left; j <= sel.right; j++) {
			if(tab(i, j) && (i != sel.top || j != sel.left)) {
				RichTxt& t = tab[i][j].text;
				for(int pi = 0; pi < t.GetPartCount(); pi++)
					if(t.IsTable(pi))
						cell.text.CatPick(pick(t.part[pi].Get<RichTable>()));
					else
					if(pi < t.GetPartCount() - 1 || t.GetPartLength(pi))
						cell.text.Cat(t.Get(pi, style), style);
				tab[i][j].ClearText();
			}
		}
	tab.Normalize();
	RefreshAll();
}

RichCell::Format RichText::GetCellFormat(int table, const Rect& sel) const
{
	Mutex::Lock __(mutex);
	return GetConstTable(table).GetCellFormat(sel);
}

void RichText::SetCellFormat(int table, const Rect& sel, const RichCell::Format& fmt, bool setkeep, bool setround)
{
	GetUpdateTable(table).SetCellFormat(sel, fmt, setkeep, setround);
	RefreshAll();
}

void RichText::ClearTable(int table, const Rect& sel)
{
	RichTable& tab = GetUpdateTable(table);
	for(int i = sel.top; i <= sel.bottom; i++)
		for(int j = sel.left; j <= sel.right; j++) {
			if(tab(i, j)) {
				tab.InvalidateRefresh(i, j);
				tab[i][j].ClearText(tab[i][j].text.GetFirstFormat(style), style);
			}
		}
	tab.Normalize();
	RefreshAll();
}

RichText::FormatInfo RichText::GetTableFormatInfo(int table, const Rect& sel) const
{
	Mutex::Lock __(mutex);
	const RichTable& tab = GetConstTable(table);
	bool first = true;
	FormatInfo fi;
	for(int i = sel.top; i <= sel.bottom; i++)
		for(int j = sel.left; j <= sel.right; j++) {
			if(tab(i, j)) {
				const RichTxt& txt = tab[i][j].text;
				txt.CombineFormat(fi, 0, txt.GetPartCount(), first, style);
			}
		}
	return fi;
}

void       RichText::ApplyTableFormatInfo(int table, const Rect& sel, const RichText::FormatInfo& fi)
{
	RichTable& tab = GetUpdateTable(table);
	for(int i = sel.top; i <= sel.bottom; i++)
		for(int j = sel.left; j <= sel.right; j++) {
			if(tab(i, j)) {
				tab.InvalidateRefresh(i, j);
				RichTxt& txt = tab[i][j].text;
				txt.ApplyFormat(fi, 0, txt.GetPartCount(), style);
			}
		}
	tab.Normalize();
	RefreshAll();
}

}
