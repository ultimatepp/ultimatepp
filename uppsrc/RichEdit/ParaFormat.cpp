#include "RichEdit.h"

NAMESPACE_UPP

struct ParaFormating : public WithParaLayout<StaticRect>
{
	DropList n[8];
	UnitEdit tabpos;
	DropList tabtype;
	DropList tabfill;
	bool     keepindent;
	Font     font;
	bool     modified;

	void                   EnableNumbering();
	RichPara::NumberFormat GetNumbering();
	bool                   IsNumbering();
	void                   SetupIndent();
	int                    ComputeIndent();
	void                   SetMod()                  { modified = true; }
	bool                   IsChanged()               { return IsModified() || modified; }

	void  Set(int unit, const RichText::FormatInfo& formatinfo);
	dword Get(RichText::FormatInfo& formatinfo);

	typedef ParaFormating CLASSNAME;

	ParaFormating();
};

RichPara::NumberFormat ParaFormating::GetNumbering()
{
	RichPara::NumberFormat f;
	f.before_number = before_number;
	f.after_number = after_number;
	f.reset_number = reset_number;
	for(int i = 0; i < 8; i++)
		f.number[i] = Nvl((int)~n[i]);
	return f;
}

bool ParaFormating::IsNumbering()
{
	if(!IsNull(before_number) || !IsNull(after_number) || reset_number)
		return true;
	for(int i = 0; i < 8; i++)
		if(!IsNull(n[i]))
		   return true;
	return false;
}

void ParaFormating::EnableNumbering()
{
	bool b = !IsNull(bullet) && !(int)~bullet;
	before_number.Enable(b);
	after_number.Enable(b);
	for(int i = 0; i < 8; i++)
		n[i].Enable(b);
}

int  ParaFormating::ComputeIndent()
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

void ParaFormating::SetupIndent()
{
	EnableNumbering();
	if(indent.IsModified() || keepindent) return;
	indent <<= ComputeIndent();
	indent.ClearModify();
}

void ParaFormating::Set(int unit, const RichText::FormatInfo& formatinfo)
{
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
		before_number = formatinfo.before_number.ToWString();
		after_number = formatinfo.after_number.ToWString();
		reset_number = formatinfo.reset_number;
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
	tabsize.Set(unit, RichText::TABSIZE & formatinfo.paravalid ? formatinfo.tabsize : Null);
	keepindent = formatinfo.indent != ComputeIndent();
	SetupIndent();
	ClearModify();
	modified = false;
}

dword ParaFormating::Get(RichText::FormatInfo& formatinfo)
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
	return v;
}

ParaFormating::ParaFormating()
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
	linespacing.Add(0, "1.0");
	linespacing.Add(-1, "1.5");
	linespacing.Add(-2, "2.0");
	bullet.Add(RichPara::BULLET_NONE, RichEditImg::NoneBullet());
	bullet.Add(RichPara::BULLET_ROUND, RichEditImg::RoundBullet());
	bullet.Add(RichPara::BULLET_ROUNDWHITE, RichEditImg::RoundWhiteBullet());
	bullet.Add(RichPara::BULLET_BOX, RichEditImg::BoxBullet());
	bullet.Add(RichPara::BULLET_BOXWHITE, RichEditImg::BoxWhiteBullet());
	bullet.Add(RichPara::BULLET_TEXT, RichEditImg::TextBullet());
	bullet.SetDisplay(CenteredHighlightImageDisplay());
	bullet.SetLineCy(18);
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
	EnableNumbering();
	rulerink.NullText("---");
}

struct ParaFormatDlg : public WithParaFormatLayout<TopWindow> {
	ParaFormating para;

	ParaFormatDlg() {
		CtrlLayoutOKCancel(*this, t_("Paragraph format"));
		ActiveFocus(para.before);
	}
};

void RichEdit::ParaFormat()
{
	ParaFormatDlg d;
	d.para.Set(unit, formatinfo);
	d.para.font = formatinfo;
	if(d.Execute() != IDOK || !d.para.IsChanged()) return;
	dword v = d.para.Get(formatinfo);
	if(v) ApplyFormat(0, v);
}

static int sCompareStyle(const Value& a, const Value& b)
{
	return CompareNoCase(String(a), String(b));
}

struct sCompareLess {
	bool operator()(const Value& a, const Value& b) const { return sCompareStyle(a, b) < 0; }
};

void RichEdit::ReadStyles()
{
	int i;
	style.Clear();
	Vector<Uuid> id;
	Vector<String> name;
	for(i = 0; i < text.GetStyleCount(); i++) {
		id.Add(text.GetStyleId(i));
		name.Add(text.GetStyle(i).name);
	}
	IndexSort(name, id, sCompareLess());
	for(i = 0; i < id.GetCount(); i++)
		style.Add(id[i], name[i]);
}

struct DisplayDefault : public Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
	                   Color ink, Color paper, dword style) const {
		String text = q;
		w.DrawRect(r, paper);
		DrawSmartText(w, r.left, r.top, r.Width(), text, StdFont().Bold(), ink);
	}
};

void RichEdit::SetStyle()
{
	if(!IsSelection()) {
		NextUndo();
		WithSetStyleLayout<TopWindow> d;
		CtrlLayoutOKCancel(d, t_("Set style"));
		d.newstyle <<= d.Breaker(IDYES);
		d.style.AddKey();
		d.style.AddColumn();
		d.style.NoHeader().NoGrid();
		for(int i = 0; i < text.GetStyleCount(); i++)
			d.style.Add(text.GetStyleId(i), text.GetStyle(i).name);
		d.style.Sort(1, sCompareStyle);
		int q = d.style.Find(RichStyle::GetDefaultId());
		if(q >= 0)
			d.style.SetDisplay(q, 0, Single<DisplayDefault>());
		d.style.FindSetCursor(formatinfo.styleid);
		RichStyle cs;
		cs.format = formatinfo;
		cs.format.sscript = 0;
		cs.format.link.Clear();
		cs.format.indexentry.Clear();
		cs.format.language = LNG_ENGLISH;
		cs.format.label.Clear();

		Uuid id;
		switch(d.Run()) {
		case IDCANCEL:
			return;
		case IDOK:
			if(d.style.IsCursor()) {
				id = d.style.GetKey();
				const RichStyle& st = text.GetStyle(id);
				cs.name = st.name;
				cs.next = st.next;
				SaveStyleUndo(id);
				break;
			}
			return;
		case IDYES:
			String newname;
			if(EditText(newname, Format(t_("New style no. %d"), text.GetStyleCount()),
			            "Name", CharFilterAscii128)) {
				cs.name = newname;
				id = Uuid::Create();
				cs.next = id;
				SaveStylesUndo();
				break;
			}
			return;
		}
		text.SetStyle(id, cs);
		ReadStyles();
		formatinfo.styleid = id;
		SaveFormat(GetCursor(), 0);
		text.ReStyle(GetCursor(), id);
		Finish();
	}
}

struct StyleManager : public WithStylesLayout<TopWindow> {
	ParaFormating             para;
	int                       unit;
	ArrayMap<Uuid, RichStyle> style;
	Index<Uuid>               dirty;
	FontHeight                height;
	EditString                name;

	void EnterStyle();
	void SaveStyle();
	void SetupFont();
	void SetupFont0();
	void Create();
	void ReloadNextStyles();
	void Remove();

	void GetFont(Font& font);

	void Menu(Bar& bar);

	typedef StyleManager CLASSNAME;

	StyleManager();
};

void StyleManager::EnterStyle()
{
	RichText::FormatInfo f;
	const RichStyle& s = style.Get(list.GetKey());
	f.Set(s.format);
	para.Set(unit, f);
	height <<= RichEdit::DotToPt(s.format.GetHeight());
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
		font.Height(RichEdit::PtToDot(~height));
	font.Bold(bold);
	font.Italic(italic);
	font.Underline(underline);
	font.Strikeout(strikeout);
}

void StyleManager::SetupFont0()
{
	Font font = Arial(120);
	GetFont(font);
	para.font = font;
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
		if(IsModified() || para.IsChanged()) {
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

StyleManager::StyleManager()
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
	face <<= height <<= italic <<= bold <<= underline <<= strikeout <<= THISBACK(SetupFont);
}

void RichEdit::Styles()
{
	NextUndo();
	StyleManager s;
	s.unit = unit;
	int i;
	for(i = 0; i < text.GetStyleCount(); i++)
		s.list.Add(text.GetStyleId(i), text.GetStyle(i).name);
	s.list.Sort(1, sCompareStyle);
	for(i = 0; i < text.GetStyleCount(); i++) {
		Uuid id = s.list.Get(i, 0);
		s.style.Add(id, text.GetStyle(id));
	}
	for(i = 0; fh[i]; i++)
		s.height.AddList(fh[i]);
	SetupFaceList(s.face);
	for(i = 0; i < face.GetCount(); i++)
		s.face.Add(face.GetKey(i));
	int q = s.list.Find(RichStyle::GetDefaultId());
	if(q >= 0)
		s.list.SetDisplay(q, 0, Single<DisplayDefault>());
	s.list.FindSetCursor(formatinfo.styleid);
	s.ReloadNextStyles();
	if(s.Execute() != IDOK)
		return;
	s.SaveStyle();
	SaveStylesUndo();
	for(i = 0; i < s.dirty.GetCount(); i++) {
		SetModify();
		modified = true;
		Uuid id = s.dirty[i];
		int q = s.style.Find(id);
		if(q >= 0)
			text.SetStyle(id, s.style.Get(id));
		else
			text.RemoveStyle(id);
	}
	ReadStyles();
	Finish();
}

void RichEdit::ApplyStylesheet(const RichText& r)
{
	NextUndo();
	SaveStylesUndo();
	text.OverrideStyles(r.GetStyles(), false, false);
	ReadStyles();
	Finish();
}

END_UPP_NAMESPACE
