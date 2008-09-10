#include "RichEdit.h"

NAMESPACE_UPP

void RichEdit::StyleTool(Bar& bar, int width)
{
	bar.Add(!IsReadOnly(), style, width);
}

void RichEdit::FaceTool(Bar& bar, int width)
{
	bar.Add(!IsReadOnly(), face, width);
}

void RichEdit::HeightTool(Bar& bar, int width)
{
	bar.Add(!IsReadOnly(), height, width);
}

void RichEdit::BoldTool(Bar& bar, dword key)
{
	bar.Add(!IsReadOnly(), t_("Bold"),
	        formatinfo.charvalid & RichText::BOLD ? RichEditImg::Bold() : RichEditImg::BoldMixed(),
	        THISBACK(Bold))
	   .Check(formatinfo.IsBold() && (formatinfo.charvalid & RichText::BOLD))
	   .Key(key);
}

void RichEdit::ItalicTool(Bar& bar, dword key)
{
	bar.Add(!IsReadOnly(), t_("Italic"),
            formatinfo.charvalid & RichText::ITALIC ? RichEditImg::Italic() : RichEditImg::ItalicMixed(),
	        THISBACK(Italic))
	   .Check(formatinfo.IsItalic() && (formatinfo.charvalid & RichText::ITALIC))
	   .Key(key);
}

void RichEdit::UnderlineTool(Bar& bar, dword key)
{
	bar.Add(!IsReadOnly(), t_("Underline"),
	        formatinfo.charvalid & RichText::UNDERLINE ? RichEditImg::Underline()
	                                               : RichEditImg::UnderlineMixed(),
	        THISBACK(Underline))
	   .Check(formatinfo.IsUnderline() && (formatinfo.charvalid & RichText::UNDERLINE))
	   .Key(key);
}

void RichEdit::StrikeoutTool(Bar& bar, dword key)
{
	bar.Add(!IsReadOnly(), t_("Strikeout"),
	        formatinfo.charvalid & RichText::STRIKEOUT ? RichEditImg::Strikeout()
	                                               : RichEditImg::StrikeoutMixed(),
	        THISBACK(Strikeout))
	   .Check(formatinfo.IsStrikeout() && (formatinfo.charvalid & RichText::STRIKEOUT))
	   .Key(key);
}


void RichEdit::CapitalsTool(Bar& bar, dword key)
{
	bar.Add(!IsReadOnly(), t_("Capitals"),
	        formatinfo.charvalid & RichText::CAPITALS  ? RichEditImg::Capitals()
	                                               : RichEditImg::CapitalsMixed(),
	        THISBACK(Capitals))
	   .Check(formatinfo.capitals && (formatinfo.charvalid & RichText::CAPITALS));
}

void RichEdit::SuperscriptTool(Bar& bar, dword key)
{
	int i = formatinfo.charvalid & RichText::SSCRIPT ? formatinfo.sscript : 0;
	bar.Add(!IsReadOnly(), t_("Superscript"),
	        formatinfo.charvalid & RichText::SSCRIPT ? RichEditImg::SuperScript()
	                                             : RichEditImg::SuperScriptMixed(),
			THISBACK1(SetScript, i == 1 ? 0 : 1))
	   .Check(i == 1);
}

void RichEdit::SubscriptTool(Bar& bar, dword key)
{
	int i = formatinfo.charvalid & RichText::SSCRIPT ? formatinfo.sscript : 0;
	bar.Add(!IsReadOnly(), t_("Subscript"),
	        formatinfo.charvalid & RichText::SSCRIPT ? RichEditImg::SubScript()
	                                             : RichEditImg::SubScriptMixed(),
			THISBACK1(SetScript, i == 2 ? 0 : 2))
	   .Check(i == 2);
}

void RichEdit::FontTools(Bar& bar)
{
	FaceTool(bar);
	bar.Gap(4);
	HeightTool(bar);
	bar.Gap();
	BoldTool(bar);
	ItalicTool(bar);
	UnderlineTool(bar);
	StrikeoutTool(bar);
	CapitalsTool(bar);
	SuperscriptTool(bar);
	SubscriptTool(bar);
}

void RichEdit::InkTool(Bar& bar)
{
	bar.Add(!IsReadOnly(), ink);
}

void RichEdit::PaperTool(Bar& bar)
{
	bar.Add(!IsReadOnly(), paper);
}

void RichEdit::LanguageTool(Bar& bar, int width)
{
	if(!fixedlang)
		bar.Add(!IsReadOnly(), language, width);
}

void RichEdit::SpellCheckTool(Bar& bar)
{
	bar.Add(t_("Show spelling errors"), RichEditImg::SpellCheck(), THISBACK(SpellCheck))
	   .Check(spellcheck);
}

String PlusKeyDesc(const char *text, dword key)
{
	String r = text;
	if(key)
		r << ' ' << GetKeyDesc(key);
	return r;
}

void Setup(DataPusher& b, const char *tip, const char *dtip, dword key)
{
	const char *s = tip ? tip : dtip;
	b.Tip(PlusKeyDesc(s, key));
	b.NullText(s, StdFont().Italic(), SColorDisabled());
}

void RichEdit::IndexEntryTool(Bar& bar, int width, dword key, const char *tip)
{
	bar.Add(!IsReadOnly(), indexentry, width);
	Setup(indexentry, tip, t_("Index entry"), key);
	bar.AddKey(key, THISBACK(IndexEntry));
}

void RichEdit::HyperlinkTool(Bar& bar, int width, dword key, const char *tip)
{
	bar.Add(!IsReadOnly(), hyperlink, width);
	Setup(hyperlink, tip, t_("Hyperlink"), key);
	bar.AddKey(key, THISBACK(Hyperlink));
}

void RichEdit::LabelTool(Bar& bar, int width, dword key, const char *tip)
{
	bar.Add(!IsReadOnly(), label, width);
	Setup(label, tip, t_("Paragraph label"), key);
	bar.AddKey(key, THISBACK(Label));
}

void RichEdit::LeftTool(Bar& bar, dword key)
{
	int a = formatinfo.paravalid & RichText::ALIGN ? formatinfo.align : Null;
	bar.Add(!IsReadOnly(), t_("Left"), RichEditImg::Left(), THISBACK(AlignLeft))
	   .Check(a == ALIGN_LEFT)
	   .Key(key);
}

void RichEdit::RightTool(Bar& bar, dword key)
{
	int a = formatinfo.paravalid & RichText::ALIGN ? formatinfo.align : Null;
	bar.Add(!IsReadOnly(), t_("Right"), RichEditImg::Right(), THISBACK(AlignRight))
	   .Check(a == ALIGN_RIGHT)
	   .Key(key);
}

void RichEdit::CenterTool(Bar& bar, dword key)
{
	int a = formatinfo.paravalid & RichText::ALIGN ? formatinfo.align : Null;
	bar.Add(!IsReadOnly(), t_("Center"), RichEditImg::Center(), THISBACK(AlignCenter))
	   .Check(a == ALIGN_CENTER)
	   .Key(key);
}

void RichEdit::JustifyTool(Bar& bar, dword key)
{
	int a = formatinfo.paravalid & RichText::ALIGN ? formatinfo.align : Null;
	bar.Add(!IsReadOnly(), t_("Justify"), RichEditImg::Justify(), THISBACK(AlignJustify))
	   .Check(a == ALIGN_JUSTIFY)
	   .Key(key);
}

void  RichEdit::RoundBulletTool(Bar& bar, dword key)
{
	int a = formatinfo.paravalid & RichText::BULLET ? formatinfo.bullet : Null;
	bar.Add(!IsReadOnly(), t_("Round bullet"), RichEditImg::RoundBullet(),
	        THISBACK1(SetBullet, RichPara::BULLET_ROUND))
	   .Check(a == RichPara::BULLET_ROUND)
	   .Key(key);
}

void  RichEdit::RoundWhiteBulletTool(Bar& bar, dword key)
{
	int a = formatinfo.paravalid & RichText::BULLET ? formatinfo.bullet : Null;
	bar.Add(!IsReadOnly(), t_("White round bullet"), RichEditImg::RoundWhiteBullet(),
	        THISBACK1(SetBullet, RichPara::BULLET_ROUNDWHITE))
	   .Check(a == RichPara::BULLET_ROUNDWHITE)
	   .Key(key);
}

void  RichEdit::BoxBulletTool(Bar& bar, dword key)
{
	int a = formatinfo.paravalid & RichText::BULLET ? formatinfo.bullet : Null;
	bar.Add(!IsReadOnly(), t_("Box bullet"), RichEditImg::BoxBullet(),
	        THISBACK1(SetBullet, RichPara::BULLET_BOX))
	   .Check(a == RichPara::BULLET_BOX)
	   .Key(key);
}

void  RichEdit::BoxWhiteBulletTool(Bar& bar, dword key)
{
	int a = formatinfo.paravalid & RichText::BULLET ? formatinfo.bullet : Null;
	bar.Add(!IsReadOnly(), t_("White box bullet"), RichEditImg::BoxWhiteBullet(),
	        THISBACK1(SetBullet, RichPara::BULLET_BOXWHITE))
	   .Check(a == RichPara::BULLET_BOXWHITE)
	   .Key(key);
}

void  RichEdit::TextBulletTool(Bar& bar, dword key)
{
	int a = formatinfo.paravalid & RichText::BULLET ? formatinfo.bullet : Null;
	bar.Add(!IsReadOnly(), t_("Text bullet"), RichEditImg::TextBullet(),
	        THISBACK1(SetBullet, RichPara::BULLET_TEXT))
	   .Check(a == RichPara::BULLET_TEXT)
	   .Key(key);
}

void RichEdit::ParaFormatTool(Bar& bar, dword key)
{
	bar.Add(!IsReadOnly(), t_("Paragraph format.."), RichEditImg::ParaFormat(), THISBACK(ParaFormat))
	   .Key(key);
}

void RichEdit::ToParaTool(Bar& bar, dword key)
{
	bar.Add(!IsReadOnly() && IsSelection() && !tablesel, t_("To single paragraph"),
	        RichEditImg::ToPara(), THISBACK(ToPara))
	   .Key(key);
}

void RichEdit::ParaTools(Bar& bar)
{
	LeftTool(bar);
	CenterTool(bar);
	RightTool(bar);
	JustifyTool(bar);
	bar.Gap();
	RoundBulletTool(bar);
	RoundWhiteBulletTool(bar);
	BoxBulletTool(bar);
	BoxWhiteBulletTool(bar);
	TextBulletTool(bar);
	bar.Gap();
	ToParaTool(bar);
	bar.Gap();
	ParaFormatTool(bar);
}

void RichEdit::UndoTool(Bar& bar, dword key)
{
	bar.Add(!IsReadOnly() && undo.GetCount(), t_("Undo"), CtrlImg::undo(), THISBACK(Undo))
	   .Repeat()
	   .Key(K_ALT_BACKSPACE)
	   .Key(key);
}

void RichEdit::RedoTool(Bar& bar, dword key)
{
	bar.Add(!IsReadOnly() && redo.GetCount(), t_("Redo"), CtrlImg::redo(), THISBACK(Redo))
	   .Repeat()
	   .Key(K_SHIFT|K_ALT_BACKSPACE)
	   .Key(key);
}

void RichEdit::CutTool(Bar& bar, dword key)
{
	bar.Add(!IsReadOnly() && IsSelection() || objectpos >= 0, t_("Cut"), CtrlImg::cut(), THISBACK(Cut))
	   .Key(K_SHIFT_DELETE)
	   .Key(key);
}

void RichEdit::CopyTool(Bar& bar, dword key)
{
	bar.Add(!IsReadOnly() && IsSelection() || objectpos >= 0,
	        t_("Copy"), CtrlImg::copy(), THISBACK(Copy))
	   .Key(K_CTRL_INSERT)
	   .Key(key);
}

void RichEdit::PasteTool(Bar& bar, dword key)
{
	bar.Add(!IsReadOnly(), t_("Paste"), CtrlImg::paste(), THISBACK(Paste))
	   .Key(K_SHIFT_INSERT)
	   .Key(key);
}

void RichEdit::ObjectTool(Bar& bar, dword key)
{
	for(int i = 0; i < RichObject::GetTypeCount(); i++) {
		String cn = RichObject::GetType(i).GetCreateName();
		if(!IsNull(cn))
			bar.Add(cn, THISBACK1(InsertObject, i));
	}
}

void RichEdit::PrintTool(Bar& bar, dword key)
{
	bar.Add(t_("Print"), CtrlImg::print(), THISBACK(DoPrint))
	   .Key(key);
}

void RichEdit::FindReplaceTool(Bar& bar, dword key)
{
	bar.Add(!IsReadOnly(), t_("Find/Replace"), RichEditImg::FindReplace(), THISBACK(OpenFindReplace))
	   .Key(key);
}

void RichEdit::EditTools(Bar& bar)
{
	CutTool(bar);
	CopyTool(bar);
	PasteTool(bar);
	bar.Gap();
	UndoTool(bar);
	RedoTool(bar);
	bar.Gap();
	FindReplaceTool(bar);
}

void RichEdit::InsertTableTool(Bar& bar, dword key)
{
	bar.Add(!IsSelection() && !IsReadOnly(),
	        t_("Insert table.."), RichEditImg::InsertTable(), THISBACK(InsertTable))
	   .Key(key);
}

void RichEdit::TablePropertiesTool(Bar& bar, dword key)
{
	bar.Add(!IsSelection() && cursorp.table && !IsReadOnly(),
	        t_("Table properties.."), RichEditImg::TableProperties(),
	        THISBACK(TableProps))
	   .Key(key);
}

void RichEdit::InsertTableRowTool(Bar& bar, dword key)
{
	bar.Add(!IsSelection() && cursorp.table && !IsReadOnly(),
	        t_("Insert row"), RichEditImg::TableInsertRow(),
	        THISBACK(TableInsertRow))
	   .Key(key);
}

void RichEdit::RemoveTableRowTool(Bar& bar, dword key)
{
	bar.Add(!IsSelection() && cursorp.table && !IsReadOnly(),
	        t_("Remove row"), RichEditImg::TableRemoveRow(), THISBACK(TableRemoveRow))
	   .Key(key);
}

void RichEdit::InsertTableColumnTool(Bar& bar, dword key)
{
	bar.Add(!IsSelection() && cursorp.table && !IsReadOnly(),
	        t_("Insert column"), RichEditImg::TableInsertColumn(),
	        THISBACK(TableInsertColumn))
	   .Key(key);
}

void RichEdit::RemoveTableColumnTool(Bar& bar, dword key)
{
	bar.Add(!IsSelection() && cursorp.table && !IsReadOnly(),
	        t_("Remove column"), RichEditImg::TableRemoveColumn(), THISBACK(TableRemoveColumn))
	   .Key(key);
}

void RichEdit::SplitJoinCellTool(Bar& bar, dword key)
{
	if(tablesel)
		bar.Add(!IsReadOnly(), t_("Join cells"), RichEditImg::JoinCell(), THISBACK(JoinCell))
		   .Key(key);
	else
		bar.Add(!IsSelection() && cursorp.table && !IsReadOnly(), t_("Split cell.."),
		        RichEditImg::SplitCell(), THISBACK(SplitCell))
		   .Key(key);
}

void RichEdit::CellPropertiesTool(Bar& bar, dword key)
{
	bar.Add(cursorp.table && (!IsSelection() || tablesel) && !IsReadOnly(),
	        t_("Cell properties.."), RichEditImg::CellProperties(), THISBACK(CellProperties))
	   .Key(key);
}

void RichEdit::TableTools(Bar& bar)
{
	InsertTableTool(bar);
	TablePropertiesTool(bar);
	InsertTableRowTool(bar);
	RemoveTableRowTool(bar);
	InsertTableColumnTool(bar);
	RemoveTableColumnTool(bar);
	SplitJoinCellTool(bar);
	CellPropertiesTool(bar);
}

void RichEdit::DefaultBar(Bar& bar, bool extended)
{
	EditTools(bar);
	bar.Gap();
	PrintTool(bar);
	bar.Gap();
	FontTools(bar);
	bar.Gap();
	InkTool(bar);
	PaperTool(bar);
	bar.Gap();
	LanguageTool(bar);
	SpellCheckTool(bar);
	if(extended) {
		bar.Gap();
		IndexEntryTool(bar, INT_MAX);
	}
	bar.Break();
	StyleTool(bar);
	bar.Gap();
	ParaTools(bar);
	bar.Gap();
	if(extended) {
		LabelTool(bar);
		bar.Gap();
	}
	TableTools(bar);
	if(extended) {
		bar.Gap();
		HyperlinkTool(bar, INT_MAX);
	}
}

END_UPP_NAMESPACE
