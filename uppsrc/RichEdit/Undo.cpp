#include "RichEdit.h"

NAMESPACE_UPP

void RichEdit::UndoInsert::Apply(RichText& txt)
{
	txt.Remove(pos, length);
}

RichEdit::UndoRec *RichEdit::UndoInsert::GetRedo(const RichText& txt)
{
	return new UndoRemove(txt, pos, length);
}

RichEdit::UndoInsert::UndoInsert(int pos, int length, bool typing)
: pos(pos), length(length), typing(typing) {}

// -----------------------

void RichEdit::UndoRemove::Apply(RichText& txt)
{
	txt.Insert(pos, text);
}

RichEdit::UndoRec *RichEdit::UndoRemove::GetRedo(const RichText& txt)
{
	return new UndoInsert(pos, text.GetLength());
}

RichEdit::UndoRemove::UndoRemove(const RichText& txt, int pos, int length)
: pos(pos)
{
	text = txt.Copy(pos, length);
}

// -----------------------

void RichEdit::UndoFormat::Apply(RichText& txt)
{
	txt.RestoreFormat(pos, format);
}

RichEdit::UndoRec *RichEdit::UndoFormat::GetRedo(const RichText& txt)
{
	return new UndoFormat(txt, pos, length);
}

RichEdit::UndoFormat::UndoFormat(const RichText& txt, int pos, int length)
: pos(pos), length(length)
{
	format = txt.SaveFormat(pos, length);
}

// -----------------------

void RichEdit::UndoStyle::Apply(RichText& txt)
{
	txt.SetStyle(id, style);
}

RichEdit::UndoRec *RichEdit::UndoStyle::GetRedo(const RichText& txt)
{
	return new UndoStyle(txt, id);
}

RichEdit::UndoStyle::UndoStyle(const RichText& txt, const Uuid& id)
: id(id)
{
	style = txt.GetStyle(id);
}

// -----------------------

void RichEdit::UndoStyles::Apply(RichText& txt)
{
	txt.SetStyles(styles);
}

RichEdit::UndoRec *RichEdit::UndoStyles::GetRedo(const RichText& txt)
{
	return new UndoStyles(txt);
}

RichEdit::UndoStyles::UndoStyles(const RichText& txt)
{
	styles <<= txt.GetStyles();
}

END_UPP_NAMESPACE
