#include "RichEdit.h"

namespace Upp {

RichPara::NumberFormat ParaFormatting::GetNumbering()
{
	RichPara::NumberFormat f;
	f.before_number = ~before_number;
	f.after_number = ~after_number;
	f.reset_number = ~reset_number;
	for(int i = 0; i < 8; i++)
		f.number[i] = Nvl((int)~n[i]);
	return f;
}

bool ParaFormatting::IsNumbering()
{
	if(!IsNull(before_number) || !IsNull(after_number) || reset_number)
		return true;
	for(int i = 0; i < 8; i++)
		if(!IsNull(n[i]))
		   return true;
	return false;
}

void ParaFormatting::EnableNumbering()
{
	bool b = !IsNull(bullet) && !(int)~bullet;
	before_number.Enable(b);
	after_number.Enable(b);
	for(int i = 0; i < 8; i++)
		n[i].Enable(b);
}

int  ParaFormatting::ComputeIndent()
{
	if(!IsNull(bullet) && (int)~bullet)
		return 150;
	if(IsNumbering()) {
		RichPara::NumberFormat f = GetNumbering();
		RichPara::Number n;
		static byte n0[] = { 0, 37, 38, 48, 48, 37, 37 };
		static byte n1[] = { 0, 17, 18, 12, 12, 17, 17 };
		if(f.number[0] && f.number[0] < 8)
			n.n[0] = n0[f.number[0]];
		if(f.number[1] && f.number[1] < 8)
			n.n[1] = n1[f.number[1]];
		for(int i = 2; i < 8; i++) {
			static byte nn[] = { 0,  7,  8,  1,  1,  7,  7 };
			if(f.number[i] && f.number[i] < 8)
				n.n[i] = nn[f.number[i]];
		}
		String s = n.AsText(f);
		s.Cat(' ');
		return GetTextSize(s, font).cx;
	}
	return 0;
}

void ParaFormatting::SetupIndent()
{
	EnableNumbering();
	if(indent.IsModified() || keepindent) return;
	indent <<= ComputeIndent();
	indent.ClearModify();
}

void ParaFormatting::EditHdrFtr()
{
	if(EditRichHeaderFooter(header_qtf, footer_qtf, allow_dark, dark))
		modified = true;
}

void ParaFormatting::NewHdrFtr()
{
	SyncHdrFtr();
	if(newhdrftr)
		EditHdrFtr();
}

void ParaFormatting::SyncHdrFtr()
{
	hdrftr.Enable(newhdrftr && newhdrftr.IsEnabled());
}

void ParaFormatting::Set(int unit, const RichText::FormatInfo& formatinfo, bool baselevel)
{
	newhdrftr.Enable(baselevel);
	hdrftr.Enable(baselevel);
	font = formatinfo;
	ruler.Set(unit, RichText::RULER & formatinfo.paravalid ? formatinfo.ruler : Null);
	before.Set(unit, RichText::BEFORE & formatinfo.paravalid ? formatinfo.before : Null);
	lm.Set(unit, RichText::LM & formatinfo.paravalid ? formatinfo.lm : Null);
	indent.Set(unit, RichText::INDENT & formatinfo.paravalid ? formatinfo.indent : Null);
	rm.Set(unit, RichText::RM & formatinfo.paravalid ? formatinfo.rm : Null);
	after.Set(unit, RichText::AFTER & formatinfo.paravalid ? formatinfo.after : Null);
	if(RichText::ALIGN & formatinfo.paravalid)
		align <<= formatinfo.align == ALIGN_LEFT    ? 0 :
		          formatinfo.align == ALIGN_CENTER  ? 1 :
		          formatinfo.align == ALIGN_RIGHT   ? 2 :
		                                            3;
	if(RichText::NEWHDRFTR & formatinfo.paravalid) {
		newhdrftr = formatinfo.newhdrftr;
		header_qtf = formatinfo.header_qtf;
		footer_qtf = formatinfo.footer_qtf;
	}
	else {
		newhdrftr = Null;
		newhdrftr.ThreeState();
	}
	if(RichText::FIRSTONPAGE & formatinfo.paravalid)
		firstonpage = formatinfo.firstonpage;
	else {
		firstonpage = Null;
		firstonpage.ThreeState();
	}
	if(RichText::NEWPAGE & formatinfo.paravalid)
		page = formatinfo.newpage;
	else {
		page = Null;
		page.ThreeState();
	}
	if(RichText::KEEP & formatinfo.paravalid)
		keep = formatinfo.keep;
	else {
		keep = Null;
		keep.ThreeState();
	}
	if(RichText::KEEPNEXT & formatinfo.paravalid)
		keepnext = formatinfo.keepnext;
	else {
		keepnext = Null;
		keepnext.ThreeState();
	}
	if(RichText::ORPHAN & formatinfo.paravalid)
		orphan = formatinfo.orphan;
	else {
		orphan = Null;
		orphan.ThreeState();
	}
	if(RichText::RULERINK & formatinfo.paravalid)
		rulerink <<= formatinfo.rulerink;
	else
		rulerink <<= Null;
	if(RichText::RULERSTYLE & formatinfo.paravalid)
		rulerstyle <<= formatinfo.rulerstyle;
	else
		rulerstyle <<= Null;
	tabpos.SetUnit(unit);
	if(RichText::BULLET & formatinfo.paravalid)
		bullet <<= formatinfo.bullet;
	else
		bullet <<= Null;
	if(RichText::SPACING & formatinfo.paravalid)
		linespacing <<= formatinfo.linespacing;
	else
		linespacing <<= Null;
	if(RichText::NUMBERING & formatinfo.paravalid) {
		before_number <<= formatinfo.before_number.ToWString();
		after_number <<= formatinfo.after_number.ToWString();
		reset_number <<= formatinfo.reset_number;
		for(int i = 0; i < 8; i++)
			n[i] = formatinfo.number[i];
	}
	else {
		before_number <<= Null;
		after_number <<= Null;
		reset_number <<= Null;
		reset_number.ThreeState();
		for(int i = 0; i < 8; i++)
			n[i] = Null;
	}
	tabs.Clear();
	if(RichText::TABS & formatinfo.paravalid)
		for(int i = 0; i < formatinfo.tab.GetCount(); i++)
			tabs.Add(formatinfo.tab[i].pos, formatinfo.tab[i].align, formatinfo.tab[i].fillchar);
	if(unit == UNIT_PIXELMODE)
		tabs.Disable();
	tabsize.Set(unit, RichText::TABSIZE & formatinfo.paravalid ? formatinfo.tabsize : Null);
	keepindent = formatinfo.indent != ComputeIndent();
	SetupIndent();
	ClearModify();
	SyncHdrFtr();
	modified = false;
}

dword ParaFormatting::Get(RichText::FormatInfo& formatinfo)
{
	dword v = 0;
	if(!IsNull(before)) {
		formatinfo.before = ~before;
		v |= RichText::BEFORE;
	}
	if(!IsNull(lm)) {
		formatinfo.lm = ~lm;
		v |= RichText::LM;
	}
	if(!IsNull(indent)) {
		formatinfo.indent = ~indent;
		v |= RichText::INDENT;
	}
	if(!IsNull(rm)) {
		formatinfo.rm = ~rm;
		v |= RichText::RM;
	}
	if(!IsNull(after)) {
		formatinfo.after = ~after;
		v |= RichText::AFTER;
	}
	if(!IsNull(align)) {
		static int sw[] = { ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT, ALIGN_JUSTIFY };
		formatinfo.align = sw[(int)~align];
		v |= RichText::ALIGN;
	}
	if(!IsNull(page)) {
		formatinfo.newpage = page;
		v |= RichText::NEWPAGE;
	}
	if(!IsNull(firstonpage)) {
		formatinfo.firstonpage = firstonpage;
		v |= RichText::FIRSTONPAGE;
	}
	if(!IsNull(newhdrftr)) {
		formatinfo.newhdrftr = newhdrftr;
		v |= RichText::NEWHDRFTR;
		if(formatinfo.newhdrftr) {
			formatinfo.header_qtf = header_qtf;
			formatinfo.footer_qtf = footer_qtf;
		}
		else
			formatinfo.header_qtf = formatinfo.footer_qtf = Null;
	}
	if(!IsNull(keep)) {
		formatinfo.keep = keep;
		v |= RichText::KEEP;
	}
	if(!IsNull(keepnext)) {
		formatinfo.keepnext = keepnext;
		v |= RichText::KEEPNEXT;
	}
	if(!IsNull(orphan)) {
		formatinfo.orphan = orphan;
		v |= RichText::ORPHAN;
	}
	if(!IsNull(bullet)) {
		formatinfo.bullet = ~bullet;
		v |= RichText::BULLET;
	}
	if(!IsNull(linespacing)) {
		formatinfo.linespacing = ~linespacing;
		v |= RichText::SPACING;
	}
	if(IsNumbering()) {
		(RichPara::NumberFormat&)formatinfo = GetNumbering();
		v |= RichText::NUMBERING;
	}
	if((RichText::TABS & formatinfo.paravalid) || tabs.GetCount()) {
		formatinfo.tab.Clear();
		for(int i = 0; i < tabs.GetCount(); i++) {
			RichPara::Tab tab;
			tab.pos = tabs.Get(i, 0);
			tab.align = (int)tabs.Get(i, 1);
			tab.fillchar = (int)tabs.Get(i, 2);
			formatinfo.tab.Add(tab);
		}
		v |= RichText::TABS;
	}
	if(!IsNull(tabsize)) {
		formatinfo.tabsize = ~tabsize;
		v |= RichText::TABSIZE;
	}
	if(!IsNull(ruler)) {
		formatinfo.ruler = ~ruler;
		v |= RichText::RULER;
	}
	if(!IsNull(rulerink)) {
		formatinfo.rulerink = ~rulerink;
		v |= RichText::RULERINK;
	}
	if(!IsNull(rulerstyle)) {
		formatinfo.rulerstyle = ~rulerstyle;
		v |= RichText::RULERSTYLE;
	}
	return v;
}

struct RulerStyleDisplay : Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
	{
		w.DrawRect(r, paper);
		if(!IsNull(q))
			RichPara::DrawRuler(w, r.left, (r.top + r.bottom) / 2 - 1, r.Width(), 2, ink, q);
	}
};

ParaFormatting::ParaFormatting(const RichEdit& e)
{
	CtrlLayout(*this);
	tabtype.Add(ALIGN_LEFT, t_("Left"));
	tabtype.Add(ALIGN_RIGHT, t_("Right"));
	tabtype.Add(ALIGN_CENTER, t_("Centered"));
	tabfill.Add(0, t_("None"));
	tabfill.Add(1, t_("...."));
	tabfill.Add(2, t_("----"));
	tabfill.Add(3, t_("__"));
	tabs.AddColumn(t_("Tab position"), 2).Edit(tabpos).SetConvert(tabpos);
	tabs.AddColumn(t_("Type"), 2).Edit(tabtype).SetConvert(tabtype).InsertValue(ALIGN_LEFT);
	tabs.AddColumn(t_("Fill"), 1).Edit(tabfill).SetConvert(tabfill).InsertValue(0);
	tabs.ColumnWidths("103 89 78");
	tabs.Appending().Removing().NoAskRemove();
	tabs.WhenAcceptEdit = tabs.WhenArrayAction = THISBACK(SetMod);
	linespacing.Add(RichPara::LSP10, "1.0");
	linespacing.Add(RichPara::LSP115, "1.15");
	linespacing.Add(RichPara::LSP15, "1.5");
	linespacing.Add(RichPara::LSP20, "2.0");
	bullet.Add(RichPara::BULLET_NONE, RichEditImg::NoneBullet());
	bullet.Add(RichPara::BULLET_ROUND, RichEditImg::RoundBullet());
	bullet.Add(RichPara::BULLET_ROUNDWHITE, RichEditImg::RoundWhiteBullet());
	bullet.Add(RichPara::BULLET_BOX, RichEditImg::BoxBullet());
	bullet.Add(RichPara::BULLET_BOXWHITE, RichEditImg::BoxWhiteBullet());
	bullet.Add(RichPara::BULLET_TEXT, RichEditImg::TextBullet());
	bullet.SetDisplay(CenteredHighlightImageDisplay());
	bullet.SetLineCy(RichEditImg::RoundBullet().GetHeight() + Zy(2));
	for(int i = 0; i < 8; i++) {
		DropList& list = n[i];
		list.Add(Null);
		list.Add(RichPara::NUMBER_NONE, " - ");
		list.Add(RichPara::NUMBER_1, "1, 2, 3");
		list.Add(RichPara::NUMBER_0, "0, 1, 2");
		list.Add(RichPara::NUMBER_a, "a, b, c");
		list.Add(RichPara::NUMBER_A, "A, B, C");
		list.Add(RichPara::NUMBER_i, "i, ii, iii");
		list.Add(RichPara::NUMBER_I, "I, II, III");
		list <<= THISBACK(SetupIndent);
	}
	before_number <<=
	after_number <<=
	reset_number <<=
	bullet <<= THISBACK(SetupIndent);
	
	newhdrftr <<= THISBACK(NewHdrFtr);
	hdrftr <<= THISBACK(EditHdrFtr);
	SyncHdrFtr();
	
	EnableNumbering();
	rulerink.NullText("---");
	rulerstyle.SetDisplay(Single<RulerStyleDisplay>());
	rulerstyle.Add(Null);
	rulerstyle.Add(RichPara::RULER_SOLID);
	rulerstyle.Add(RichPara::RULER_DOT);
	rulerstyle.Add(RichPara::RULER_DASH);

	e.SetupDark(rulerink);
	allow_dark = e.allow_dark_content;
	dark = e.dark_content;
}

void StyleManager::EnterStyle()
{
	RichText::FormatInfo f;
	const RichStyle& s = style.Get(list.GetKey());
	f.Set(s.format);
	para.Set(unit, f);
	height <<= RichEdit::DotToPt(s.format.GetHeight(), unit);
	face <<= s.format.GetFace();
	bold = s.format.IsBold();
	italic = s.format.IsItalic();
	underline = s.format.IsUnderline();
	strikeout = s.format.IsStrikeout();
	capitals = s.format.capitals;
	ink <<= s.format.ink;
	paper <<= s.format.paper;
	next <<= s.next;
	ClearModify();
	SetupFont0();
	para.EnableNumbering();
}

void StyleManager::GetFont(Font& font)
{
	if(!IsNull(face))
		font.Face(~face);
	if(!IsNull(height))
		font.Height(RichEdit::PtToDot(~height, unit));
	font.Bold(bold);
	font.Italic(italic);
	font.Underline(underline);
	font.Strikeout(strikeout);
}

void StyleManager::SetupFont0()
{
	Font font = Arial(120);
	GetFont(font);
	para.SetFont(font);
}

void StyleManager::SetupFont()
{
	SetupFont0();
	para.SetupIndent();
}

void StyleManager::SaveStyle()
{
	if(list.IsCursor()) {
		Uuid id = list.GetKey();
		RichStyle& s = style.Get(list.GetKey());
		if(Ctrl::IsModifiedDeep() || para.IsChanged()) {
			dirty.FindAdd(id);
			RichText::FormatInfo f;
			para.Get(f);
			s.format = f;
			GetFont(s.format);
			s.format.capitals = capitals;
			s.format.ink = ~ink;
			s.format.paper = ~paper;
		}
		if(String(list.Get(1)) != s.name) {
			dirty.FindAdd(id);
			s.name = list.Get(1);
		}
		if((Uuid)~next != s.next) {
			dirty.FindAdd(id);
			s.next = ~next;
		}
	}
}

void StyleManager::Create()
{
	Uuid id = Uuid::Create();
	style.Add(id, style.Get(list.GetKey()));
	style.Top().next = id;
	dirty.FindAdd(id);
	list.Add(id);
	list.GoEnd();
	list.StartEdit();
}

void StyleManager::Remove()
{
	if(list.GetCount() > 1 && (Uuid)list.GetKey() != RichStyle::GetDefaultId()) {
		dirty.FindAdd(list.GetKey());
		style.Remove(list.GetCursor());
		list.Remove(list.GetCursor());
	}
}

void StyleManager::Menu(Bar& bar)
{
	bar.Add(t_("Create new style.."), THISBACK(Create))
	   .Key(K_INSERT);
	bar.Add(t_("Remove style"), THISBACK(Remove))
	   .Key(K_DELETE);
	bar.Add(t_("Rename.."), callback(&list, &ArrayCtrl::DoEdit))
	   .Key(K_CTRL_ENTER);
}

void StyleManager::ReloadNextStyles()
{
	next.ClearList();
	for(int i = 0; i < list.GetCount(); i++)
		next.Add(list.Get(i, 0), list.Get(i, 1));
}

void RichEdit::DisplayDefault::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	String text = q;
	w.DrawRect(r, paper);
	DrawSmartText(w, r.left, r.top, r.Width(), text, StdFont().Bold(), ink);
}

void StyleManager::Set(const RichText& text, const Uuid& current)
{
	list.Clear();
	int i;
	for(i = 0; i < text.GetStyleCount(); i++)
		list.Add(text.GetStyleId(i), text.GetStyle(i).name);
	list.Sort(1, RichEdit::CompareStyle);
	for(i = 0; i < text.GetStyleCount(); i++) {
		Uuid id = list.Get(i, 0);
		style.Add(id, text.GetStyle(id));
	}
	int q = list.Find(RichStyle::GetDefaultId());
	if(q >= 0)
		list.SetDisplay(q, 0, Single<RichEdit::DisplayDefault>());
	ReloadNextStyles();
	list.FindSetCursor(current);
}

void StyleManager::Set(const char *qtf)
{
	RichText txt = ParseQTF(qtf);
	Set(txt);
}

bool StyleManager::IsChanged() const
{
	return dirty.GetCount() || IsModifiedDeep();
}

void StyleManager::Get(RichText& text)
{
	SaveStyle();
	for(int i = 0; i < dirty.GetCount(); i++) {
		Uuid id = dirty[i];
		int q = style.Find(id);
		if(q >= 0)
			text.SetStyle(id, style.Get(id));
		else
			text.RemoveStyle(id);
	}
}

RichText StyleManager::Get()
{
	RichText output;
	output.SetStyles(style);
	return output;
}

String StyleManager::GetQTF()
{
	return AsQTF(Get());
}

void StyleManager::Setup(const Vector<int>& faces, int aunit)
{
	unit = aunit;
	height.Clear();
	for(int i = 0; RichEdit::fh[i]; i++)
		height.AddList(RichEdit::fh[i]);
	face.ClearList();
	SetupFaceList(face);
	for(int i = 0; i < faces.GetCount(); i++)
		face.Add(faces[i]);
}

StyleManager::StyleManager(const RichEdit& e)
:	para(e)
{
	CtrlLayoutOKCancel(*this, t_("Styles"));
	list.NoHeader().NoGrid();
	list.AddKey();
	list.AddColumn().Edit(name);
	list.WhenEnterRow = THISBACK(EnterStyle);
	list.WhenKillCursor = THISBACK(SaveStyle);
	list.WhenBar = THISBACK(Menu);
	list.WhenAcceptEdit = THISBACK(ReloadNextStyles);
	ink.NotNull();
	e.SetupDark(ink);
	e.SetupDark(paper);
	face <<= height <<= italic <<= bold <<= underline <<= strikeout <<= THISBACK(SetupFont);
	Vector<int> ffs;
	Vector<int> ff;
	ff.Add(Font::ARIAL);
	ff.Add(Font::ROMAN);
	ff.Add(Font::COURIER);
	Setup(ff, UNIT_DOT);
	para.page.Disable();
}

}
