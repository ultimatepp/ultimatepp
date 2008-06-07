#include "RichEdit.h"

NAMESPACE_UPP

void RichEdit::UndoTableFormat::Apply(RichText& txt)
{
	txt.SetTableFormat(table, format);
}

RichEdit::UndoRec *RichEdit::UndoTableFormat::GetRedo(const RichText& txt)
{
	return new UndoTableFormat(txt, table);
}

RichEdit::UndoTableFormat::UndoTableFormat(const RichText& txt, int table)
: table(table)
{
	format = txt.GetTableFormat(table);
}

// -----------------------


void RichEdit::UndoCreateTable::Apply(RichText& txt)
{
	txt.DestroyTable(table);
}

RichEdit::UndoRec *RichEdit::UndoCreateTable::GetRedo(const RichText& txt)
{
	return new UndoDestroyTable(txt, table);
}

// -----------------------

void RichEdit::UndoDestroyTable::Apply(RichText& txt)
{
	txt.SetTable(pos, table);
}

RichEdit::UndoRec *RichEdit::UndoDestroyTable::GetRedo(const RichText& txt)
{
	return new UndoCreateTable(txt.GetRichPos(pos).table + 1);
}

RichEdit::UndoDestroyTable::UndoDestroyTable(const RichText& txt, int tab)
{
	pos = txt.GetCellPos(tab, 0, 0).pos;
	table = txt.CopyTable(tab);
}

// -----------------------

void RichEdit::UndoInsertParaSpecial::Apply(RichText& txt)
{
	txt.RemoveParaSpecial(table, before);
	RichCellPos p = txt.GetCellPos(table, 0, 0);
}

RichEdit::UndoRec *RichEdit::UndoInsertParaSpecial::GetRedo(const RichText& txt)
{
	return new UndoRemoveParaSpecial(txt, table, before);
}

// -----------------------

void RichEdit::UndoRemoveParaSpecial::Apply(RichText& txt)
{
	txt.InsertParaSpecial(table, before, format);
}

RichEdit::UndoRec *RichEdit::UndoRemoveParaSpecial::GetRedo(const RichText& txt)
{
	return new UndoInsertParaSpecial(table, before);
}

RichEdit::UndoRemoveParaSpecial::UndoRemoveParaSpecial(const RichText& txt, int table, bool before)
: table(table), before(before)
{
	RichCellPos p = txt.GetCellPos(table, 0, 0);
	format = txt.GetRichPos(before ? p.pos - 1 : p.pos + p.tablen + 1).format;
}

// -----------------------

void RichEdit::UndoTable::Apply(RichText& txt)
{
	txt.ReplaceTable(table, copy);
}

RichEdit::UndoRec *RichEdit::UndoTable::GetRedo(const RichText& txt)
{
	return new UndoTable(txt, table);
}

RichEdit::UndoTable::UndoTable(const RichText& txt, int tab)
{
	table = tab;
	copy = txt.CopyTable(tab);
}

END_UPP_NAMESPACE
