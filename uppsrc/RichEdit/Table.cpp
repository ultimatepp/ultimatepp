#include "RichEdit.h"

namespace Upp {

void RichEdit::SaveTableFormat(int table)
{
	AddUndo(MakeOne<UndoTableFormat>(text, table));
}

void RichEdit::SaveTable(int table)
{
	AddUndo(MakeOne<UndoTable>(text, table));
}

void RichEdit::InsertTable()
{
	if(IsSelection())
		return;
	WithCreateTableLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, t_("Insert table"));
	dlg.header = false;
	dlg.columns <<= 2;
	dlg.columns.MinMax(1, 20);
	dlg.ActiveFocus(dlg.columns);
	if(dlg.Run() != IDOK)
		return;
	RichTable::Format fmt;
	if(pixel_mode) {
		fmt.grid = 1;
		fmt.frame = 2;
	}
	int nx = minmax((int)~dlg.columns, 1, 20);
	for(int q = nx; q--;)
		fmt.column.Add(1);
	if(dlg.header)
		fmt.header = 1;
	RichTable table;
	table.SetFormat(fmt);
	for(int i = 0; i < (dlg.header ? 2 : 1); i++)
		for(int j = 0; j < nx; j++) {
			RichText h;
			h.SetStyles(text.GetStyles());
			RichPara p;
			p.format = formatinfo;
			p.format.firstonpage = p.format.newpage = false;
			p.format.label.Clear();
			h.Cat(p);
			table.SetPick(i, j, pick(h));
			if(pixel_mode) {
				RichCell::Format fmt;
				fmt.margin.left = fmt.margin.right = 4;
				fmt.margin.top = fmt.margin.bottom = 3;
				table.SetFormat(i, j, fmt);
			}
		}
	NextUndo();
	if(cursorp.posinpara)
		InsertLine();
	if(text.GetRichPos(cursor).paralen) {
		InsertLine();
		cursor = anchor = cursor - 1;
		begtabsel = false;
	}
	SaveFormat(cursor, 0);
	AddUndo(MakeOne<UndoCreateTable>(text.SetTable(cursor, table)));
	Finish();
}

template <class T>
struct CtrlRetrieveItemValueNN : public CtrlRetriever::Item {
	Ctrl& ctrl;
	T&    value;

	virtual void Retrieve() {
		if(!IsNull(ctrl))
			value = ~ctrl;
	}

	CtrlRetrieveItemValueNN(Ctrl& ctrl, T& value) : ctrl(ctrl), value(value) {}
};

template <class T>
void Advn(CtrlRetriever& r, Ctrl& ctrl, T& value) {
	ctrl <<= value;
	r.Put(MakeOne<CtrlRetrieveItemValueNN<T>>(ctrl, value));
}

void RichEdit::DestroyTable()
{
	AddUndo(MakeOne<UndoDestroyTable>(text, cursorp.table));
	int c = text.GetCellPos(cursorp.table, 0, 0).pos;
	text.DestroyTable(cursorp.table);
	Move(c);
}

int CharFilterEqualize(int c)
{
	return IsDigit(c) || c == ':' ? c : 0;
}

struct RichEditTableProperties : WithTablePropertiesLayout<TopWindow> {
	String header_qtf, footer_qtf;
	bool     dark = false;
	bool     allow_dark = false;

	void EditHdrFtr()
	{
		EditRichHeaderFooter(header_qtf, footer_qtf, allow_dark, dark);
	}
	
	void NewHdrFtr()
	{
		SyncHdrFtr();
		if(newhdrftr)
			EditHdrFtr();
	}
	
	void SyncHdrFtr()
	{
		hdrftr.Enable(newhdrftr && newhdrftr.IsEnabled());
	}
	
	typedef RichEditTableProperties CLASSNAME;

	RichEditTableProperties() {
		CtrlLayoutOKCancel(*this, t_("Table properties"));
		newhdrftr <<= THISBACK(NewHdrFtr);
		hdrftr <<= THISBACK(EditHdrFtr);
		SyncHdrFtr();
	}
};

void RichEdit::TableProps()
{
	if(IsSelection() || cursorp.table == 0)
		return;
	RichEditTableProperties dlg;
	SetupDark(dlg.framecolor);
	SetupDark(dlg.gridcolor);
	dlg.allow_dark = allow_dark_content;
	dlg.dark = dark_content;
	dlg.Breaker(dlg.destroy, IDNO);
	RichTable::Format fmt = text.GetTableFormat(cursorp.table);
	String ratios;
	for(int i = 0; i < fmt.column.GetCount(); i++) {
		if(i)
			ratios << ':';
		ratios << "1";
	}
	dlg.ratios.SetFilter(CharFilterEqualize);
	dlg.ratios <<= ratios;
	CtrlRetriever r;
	Advn(r, dlg.before.SetUnit(unit), fmt.before);
	Advn(r, dlg.after.SetUnit(unit), fmt.after);
	Advn(r, dlg.lm.SetUnit(unit), fmt.lm);
	Advn(r, dlg.rm.SetUnit(unit), fmt.rm);
	Advn(r, dlg.frame.SetUnit(unit), fmt.frame);
	r(dlg.framecolor, fmt.framecolor);
	Advn(r, dlg.grid.SetUnit(unit), fmt.grid);
	Advn(r, dlg.header, fmt.header);
	Advn(r, dlg.keep, fmt.keep);
	Advn(r, dlg.newpage, fmt.newpage);
	Advn(r, dlg.newhdrftr, fmt.newhdrftr);
	dlg.header_qtf = fmt.header_qtf;
	dlg.footer_qtf = fmt.footer_qtf;
	r(dlg.gridcolor, fmt.gridcolor);
	dlg.SyncHdrFtr();
	dlg.newhdrftr.Enable(cursorp.level == 1);
	dlg.hdrftr.Enable(cursorp.level == 1);
	for(;;) {
		switch(dlg.Run()) {
		case IDCANCEL:
			return;
		case IDNO:
			NextUndo();
			DestroyTable();
			return;
		default:
			r.Retrieve();
			if(dlg.newhdrftr) {
				fmt.header_qtf = dlg.header_qtf;
				fmt.footer_qtf = dlg.footer_qtf;
			}
			else
				fmt.header_qtf = fmt.footer_qtf = Null;
			const RichTable& tbl = text.GetConstTable(cursorp.table);
			bool valid = true;
			Point violator(0, 0);
			int vspan = 0;
			for(int rw = 0; valid && rw < fmt.header && rw < tbl.GetRows(); rw++)
				for(int co = 0; valid && co < tbl.GetColumns(); co++)
					if(tbl(rw, co) && (vspan = tbl[rw][co].vspan) + rw > fmt.header) {
						valid = false;
						violator.x = co;
						violator.y = rw;
						break;
					}
			if(!valid) {
				Exclamation(Format(t_("Invalid header row count %d, cell at rw %d, co %d has vspan = %d."),
					fmt.header, violator.y + 1, violator.x + 1, vspan));
				continue;
			}
			NextUndo();
			SaveTableFormat(cursorp.table);
			if(dlg.equalize) {
				Vector<String> r = Split((String)~dlg.ratios, ':');
				for(int i = 0; i < fmt.column.GetCount(); i++)
					fmt.column[i] = i < r.GetCount() ? max(atoi(r[i]), 1) : 1;
			}
			text.SetTableFormat(cursorp.table, fmt);
			Finish();
			return;
		}
	}
}

bool RichEdit::RemoveSpecial(int ll, int hh, bool back)
{
	NextUndo();
	int l = min(ll, hh);
	int h = max(ll, hh);
	RichPos p1 = text.GetRichPos(l);
	RichPos p2 = text.GetRichPos(h);
	if(InSameTxt(p1, p2))
		return false;
	if(p1.paralen == 0 && p2.posintab == 0 && text.CanRemoveParaSpecial(p2.table, true)) {
		AddUndo(MakeOne<UndoRemoveParaSpecial>(text, p2.table, true));
		text.RemoveParaSpecial(p2.table, true);
		Move(cursor - back);
	}
	else
	if(p2.paralen == 0 && p1.posintab == p1.tablen && text.CanRemoveParaSpecial(p1.table, false)) {
		AddUndo(MakeOne<UndoRemoveParaSpecial>(text, p1.table, false));
		text.RemoveParaSpecial(p1.table, false);
		Move(cursor - back);
	}
	return true;
}

bool RichEdit::InsertLineSpecial()
{
	NextUndo();
	if(cursorp.table) {
		RichPara::Format fmt;
		fmt = formatinfo;
		fmt.firstonpage = fmt.newpage = false;
		fmt.label.Clear();
		if(cursorp.posintab == 0 && text.ShouldInsertParaSpecial(cursorp.table, true)) {
			AddUndo(MakeOne<UndoInsertParaSpecial>(cursorp.table, true));
			text.InsertParaSpecial(cursorp.table, true, fmt);
			Move(cursor + 1);
			return true;
		}
		if(cursorp.posintab == cursorp.tablen && text.ShouldInsertParaSpecial(cursorp.table, false)) {
			AddUndo(MakeOne<UndoInsertParaSpecial>(cursorp.table, false));
			text.InsertParaSpecial(cursorp.table, false, fmt);
			Move(cursor + 1);
			return true;
		}
	}
	return false;
}

void RichEdit::TableInsertRow()
{
	if(IsSelection() || !cursorp.table)
		return;
	NextUndo();
	SaveTable(cursorp.table);
	Point p = cursorp.cell;
	if(cursorp.posintab == cursorp.tablen) {
		p.y++;
		p.x = 0;
	}
	text.InsertTableRow(cursorp.table, p.y);
	Move(text.GetCellPos(cursorp.table, text.GetMasterCell(cursorp.table, p)).pos);
}

void RichEdit::TableRemoveRow()
{

	if(IsSelection() || !cursorp.table)
		return;
	NextUndo();
	if(cursorp.tabsize.cy <= 1)
		DestroyTable();
	else {
		SaveTable(cursorp.table);
		text.RemoveTableRow(cursorp.table, cursorp.cell.y);
		Move(text.GetCellPos(cursorp.table, text.GetMasterCell(cursorp.table, cursorp.cell)).pos);
	}
}

void RichEdit::TableInsertColumn()
{
	if(IsSelection() || !cursorp.table)
		return;
	NextUndo();
	SaveTable(cursorp.table);
	Point p = cursorp.cell;
	if(cursorp.cell.x == cursorp.tabsize.cx - 1 && cursorp.posincell == cursorp.celllen)
		p.x++;
	text.InsertTableColumn(cursorp.table, p.x);
	Move(text.GetCellPos(cursorp.table, text.GetMasterCell(cursorp.table, p)).pos);
}

void RichEdit::TableRemoveColumn()
{

	if(IsSelection() || !cursorp.table)
		return;
	NextUndo();
	if(cursorp.tabsize.cx <= 1)
		DestroyTable();
	else {
		SaveTable(cursorp.table);
		text.RemoveTableColumn(cursorp.table, cursorp.cell.x);
		Move(text.GetCellPos(cursorp.table, text.GetMasterCell(cursorp.table, cursorp.cell)).pos);
	}
}

void RichEdit::SplitCell()
{
	if(IsSelection() || !cursorp.table)
		return;
	WithSplitCellLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, t_("Split cell"));
	dlg.cx.MinMax(1, 20).NotNull();
	dlg.cx <<= 1;
	dlg.cy.MinMax(1, 20).NotNull();
	dlg.cy <<= 1;
	if(dlg.Execute() != IDOK)
		return;
	NextUndo();
	SaveTable(cursorp.table);
	text.SplitCell(cursorp.table, cursorp.cell, Size(~dlg.cx, ~dlg.cy));
	Finish();
}

void RichEdit::CellProperties()
{
	if(!(tablesel || cursorp.table && !IsSelection()))
		return;
	WithCellPropertiesLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, t_("Cell properties"));
	SetupDark(dlg.color);
	SetupDark(dlg.border);
	int  tab;
	Rect a;
	if(tablesel) {
		tab = tablesel;
		a = cells;
	}
	else {
		tab = cursorp.table;
		a = Rect(cursorp.cell, Size(0, 0));
	}
	RichCell::Format fmt = text.GetCellFormat(tab, a);
	CtrlRetriever r;
	r
		(dlg.leftb.SetUnit(unit), fmt.border.left)
		(dlg.rightb.SetUnit(unit), fmt.border.right)
		(dlg.topb.SetUnit(unit), fmt.border.top)
		(dlg.bottomb.SetUnit(unit), fmt.border.bottom)
		(dlg.leftm.SetUnit(unit), fmt.margin.left)
		(dlg.rightm.SetUnit(unit), fmt.margin.right)
		(dlg.topm.SetUnit(unit), fmt.margin.top)
		(dlg.bottomm.SetUnit(unit), fmt.margin.bottom)
		(dlg.align, fmt.align)
		(dlg.minheight.SetUnit(unit), fmt.minheight)
		(dlg.color, fmt.color)
		(dlg.border, fmt.bordercolor)
		(dlg.keep, fmt.keep)
		(dlg.round, fmt.round)
	;
	dlg.align.Set(0, ALIGN_TOP);
	dlg.align.Set(1, ALIGN_CENTER);
	dlg.align.Set(2, ALIGN_BOTTOM);
	dlg.color.WithVoid().VoidText(t_("(no change)"));
	dlg.border.WithVoid().VoidText(t_("(no change)"));
	if(tablesel) {
		dlg.keep.ThreeState();
		dlg.keep <<= Null;
		dlg.round.ThreeState();
		dlg.round <<= Null;
	}
	if(dlg.Run() != IDOK)
		return;
	r.Retrieve();
	NextUndo();
	SaveTable(tab);
	text.SetCellFormat(tab, a, fmt, !tablesel || !IsNull(dlg.keep), !tablesel || !IsNull(dlg.round));
	Finish();
}

void RichEdit::JoinCell()
{
	if(!tablesel)
		return;
	NextUndo();
	SaveTable(tablesel);
	text.JoinCell(tablesel, cells);
	Move(text.GetCellPos(tablesel, cells.TopLeft()).pos);
}

}
