#include "RichEdit.h"

namespace Upp {

Point RichEdit::GetPreedit()
{
	Rect r = GetCaretRect();
	if(formatinfo.sscript == 2) {
		Point p = r.BottomRight();
		p.y -= 3 * r.GetHeight() / 5;
		return p;
	}
	return r.TopRight();
}
	
Font RichEdit::GetPreeditFont()
{
	Font fnt = formatinfo;
	int h = abs(fnt.GetHeight());
	if(formatinfo.sscript)
		h = 3 * h / 5;
	return fnt(max(GetZoom() * abs(h), 1));
}

void RichEdit::ApplyFormat(dword charvalid, dword paravalid)
{
	if(IsReadOnly())
		return;
	RichText::FormatInfo f = formatinfo;
	f.charvalid = charvalid;
	f.paravalid = paravalid;
	if(objectpos >= 0) {
		ModifyFormat(objectpos, f, 1);
		Finish();
	}
	if(IsSelection()) {
		if(tablesel) {
			NextUndo();
			SaveTable(tablesel);
			text.ApplyTableFormatInfo(tablesel, cells, f);
		}
		else {
			int l = min(cursor, anchor);
			int h = max(cursor, anchor);
			RichPos rp = text.GetRichPos(h);
			if(rp.posinpara == 0 && h > l) {
				RichPos rp1 = text.GetRichPos(h - 1);
				if(InSameTxt(rp, rp1))
					h--;
			}
			ModifyFormat(l, f, h - l);
		}
		Finish();
	}
	else
	if(cursorp.paralen == 0) {
		ModifyFormat(cursor, f, 0);
		Finish();
	}
	else
	if(f.paravalid) {
		ModifyFormat(cursor, f, 0);
		Finish();
	}
	else
		RefreshBar();
	SetLastCharFormat(formatinfo);
}

void RichEdit::ApplyFormatInfo(const RichText::FormatInfo& fi)
{
	fi.ApplyTo(formatinfo);
	formatinfo.charvalid |= fi.charvalid;
	formatinfo.paravalid |= fi.paravalid;
	ApplyFormat(fi.charvalid, fi.paravalid);
}

void RichEdit::Bold()
{
	NextUndo();
	formatinfo.Bold(!(formatinfo.IsBold() && (formatinfo.charvalid & RichText::BOLD)));
	ApplyFormat(RichText::BOLD);
}

void RichEdit::Italic()
{
	NextUndo();
	formatinfo.Italic(!(formatinfo.IsItalic() && (formatinfo.charvalid & RichText::ITALIC)));
	ApplyFormat(RichText::ITALIC);
}

void RichEdit::Underline()
{
	NextUndo();
	formatinfo.Underline(!(formatinfo.IsUnderline() && (formatinfo.charvalid & RichText::UNDERLINE)));
	ApplyFormat(RichText::UNDERLINE);
}

void RichEdit::Strikeout()
{
	NextUndo();
	formatinfo.Strikeout(!(formatinfo.IsStrikeout() && (formatinfo.charvalid & RichText::STRIKEOUT)));
	ApplyFormat(RichText::STRIKEOUT);
}

void RichEdit::Capitals()
{
	NextUndo();
	formatinfo.capitals = !formatinfo.capitals && (formatinfo.charvalid & RichText::CAPITALS);
	ApplyFormat(RichText::CAPITALS);
}

void RichEdit::SetScript(int i)
{
	NextUndo();
	formatinfo.sscript = i;
	ApplyFormat(RichText::SSCRIPT);
}

void RichEdit::SetFace()
{
	NextUndo();
	formatinfo.Face(~face);
	ApplyFormat(RichText::FACE);
	SetFocus();
}

void RichEdit::SetHeight()
{
	NextUndo();
	formatinfo.Height(PtToDot(~height, unit));
	ApplyFormat(RichText::HEIGHT);
	SetFocus();
}

void RichEdit::SetInk()
{
	NextUndo();
	formatinfo.ink = ~ink;
	ApplyFormat(RichText::INK);
	SetFocus();
}

void RichEdit::SetPaper()
{
	NextUndo();
	formatinfo.paper = ~paper;
	ApplyFormat(RichText::PAPER);
	SetFocus();
}

void RichEdit::SetLanguage()
{
	NextUndo();
	formatinfo.language = (int)~language;
	ApplyFormat(RichText::LANGUAGE);
	SetFocus();
}

void RichEdit::Language()
{
	WithRichLanguageLayout<TopWindow> d;
	CtrlLayoutOKCancel(d, t_("Language"));
	d.lang <<= ~language;
	if(d.Run() != IDOK)
		return;
	formatinfo.language = (int)~d.lang;
	ApplyFormat(RichText::LANGUAGE);
	SetFocus();
	if(!language.HasKey((int)~d.lang)) {
		Vector<int> h;
		for(int i = 0; i < language.GetCount(); i++)
			h.Add(language.GetKey(i));
		h.Add(~d.lang);
		SetupLanguage(pick(h));
	}
}

void RichEdit::IndentMark()
{
	RichRuler::Marker m;
	int l = formatinfo.lm;
	int r = cursorc.textpage.Width() - formatinfo.rm;
	m.pos = l + formatinfo.indent;
	m.minpos = max(l, 0);
	m.maxpos = max(r - 120, 0);
	m.top = true;
	m.image = formatinfo.paravalid & RichText::INDENT ? RichEditImg::Indent()
	                                                  : RichEditImg::IndentMixed();
	ruler.Set(2, m);
}

void RichEdit::ReadFormat()
{
	if(objectpos >= 0)
		formatinfo = text.GetFormatInfo(objectpos, 1);
	else
	if(IsSelection())
		if(tablesel)
			formatinfo = text.GetTableFormatInfo(tablesel, cells);
		else
			formatinfo = text.GetFormatInfo(min(cursor, anchor), abs(cursor - anchor));
	else {
		RichPos p = cursorp;
		String l1 = p.format.link;
		bool clearlink = p.posinpara == 0 || p.posinpara == p.paralen;
		if(cursor && p.posinpara) {
			p = text.GetRichPos(cursor - 1);
			clearlink = clearlink || p.format.link.GetCount() && p.format.link != l1;
		}
		if(clearlink)
			p.format.link.Clear();
		formatinfo.Set(p.format);
	}
	ShowFormat();
}

void RichEdit::SetLastCharFormat()
{
	SetLastCharFormat(text.GetRichPos(max(cursor - 1, 0)).format);
}

void RichEdit::SetLastCharFormat(const RichPara::CharFormat& fmt)
{
	last_format = fmt;
	int isz = DPI(16);
	int fh = isz;
	Font fnt = last_format;
	while(fh >= 0 && fnt.Height(fh).GetCy() > isz)
		fh--;
	ImagePainter iw(isz, isz);
	String txt = "a";
	if(last_format.capitals) {
		txt = "A";
		fh = 5 * fh / 6;
	}
	if(last_format.sscript)
		fh = 4 * fh / 5;
	fnt.Height(fh);
	iw.Clear();
	iw.RoundedRectangle(0.5, 0.5, isz - 0.5, isz - 0.5, DPI(4));
	if(!IsNull(last_format.paper))
		iw.Fill(IsDarkContent() ? DarkTheme(last_format.paper) : last_format.paper);
	iw.Stroke(1, SLtBlue());
	Size tsz = GetTextSize(txt, fnt);
	iw.DrawText((isz - tsz.cx) / 2, last_format.sscript == 1 ? 0 : isz - fnt.GetCy(), txt, fnt,
	            IsDarkContent() ? DarkTheme(last_format.ink) : last_format.ink);
	last_format_img = iw;
	RefreshBar();
}

void RichEdit::LastCharFormat()
{
	RichText::FormatInfo fi;
	(RichPara::CharFormat&)fi = last_format;
	fi.paravalid = 0;
	fi.charvalid =
		RichText::BOLD|
		RichText::ITALIC|
		RichText::UNDERLINE|
		RichText::FACE|
		RichText::HEIGHT|
		RichText::INK|
		RichText::PAPER|
		RichText::SSCRIPT|
		RichText::CAPITALS|
		RichText::STRIKEOUT;
	auto bak = last_format;
	ApplyFormatInfo(fi);
	SetLastCharFormat(bak);
}

void RichEdit::ShowFormat()
{
	RefreshBar();

	if(formatinfo.charvalid & RichText::FACE)
		face <<= formatinfo.GetFace();
	else
		face <<= Null;

	if(formatinfo.charvalid & RichText::HEIGHT)
		height <<= DotToPt(formatinfo.GetHeight(), unit);
	else
		height <<= Null;

	if(formatinfo.charvalid & RichText::LINK)
		hyperlink <<= formatinfo.link;
	else
		hyperlink <<= Null;

	if(formatinfo.charvalid & RichText::INDEXENTRY)
		indexentry <<= formatinfo.indexentry;
	else
		indexentry <<= Null;

	if(formatinfo.charvalid & RichText::INK)
		ink <<= formatinfo.ink;
	else
		ink <<= Null;

	if(formatinfo.charvalid & RichText::PAPER)
		paper <<= formatinfo.paper;
	else
		paper <<= Null;

	if(formatinfo.charvalid & RichText::LANG)
		language <<= (int)formatinfo.language;
	else
		language <<= Null;

	if(IsSelection())
		label <<= Null;
	else
		label <<= formatinfo.label;

	int l = formatinfo.lm;
	int r = cursorc.textpage.Width() - formatinfo.rm;

	RichRuler::Marker m;
	m.pos = l;
	m.minpos = 0;
	m.maxpos = max(r - formatinfo.indent - 120, 0);
	m.image = formatinfo.paravalid & RichText::LM ? RichEditImg::Margin() : RichEditImg::MarginMixed();
	ruler.Set(0, m);

	m.pos = pixel_mode || !IsNull(floating_zoom) ? Null : r;
	m.minpos = max(l + formatinfo.indent + 120, 0);
	m.maxpos = cursorc.textpage.Width();
	m.image = formatinfo.paravalid & RichText::RM ? RichEditImg::Margin() : RichEditImg::MarginMixed();
	ruler.Set(1, m);

	IndentMark();

	int maxpos = 0;
	m.minpos = 0;
	m.deletable = true;
	if(formatinfo.paravalid & RichText::TABS) {
		for(int i = 0; i < formatinfo.tab.GetCount(); i++) {
			RichPara::Tab tab = formatinfo.tab[i];
			m.pos = tab.pos;
			if(tab.pos > maxpos)
				maxpos = tab.pos;
			switch(tab.align) {
			case ALIGN_LEFT:
				m.image = RichEditImg::LeftTab();
				break;
			case ALIGN_RIGHT:
				m.image = RichEditImg::RightTab();
				break;
			case ALIGN_CENTER:
				m.image = RichEditImg::CenterTab();
				break;
			}
			ruler.Set(i + 3, m);
		}
		ruler.SetTabs(maxpos, formatinfo.tabsize);
		ruler.SetCount(formatinfo.tab.GetCount() + 3);
	}
	else {
		ruler.SetTabs(INT_MAX / 2, 1);
		ruler.SetCount(3);
	}

	if(formatinfo.paravalid & RichText::STYLE)
		style <<= formatinfo.styleid;
	else
		style <<= Null;
	setstyle->Enable(!IsSelection());
}

void RichEdit::HighLightTab(int r)
{
	RichRuler::Marker m = ruler[r + 3];
	RichPara::Tab tab = formatinfo.tab[r];
	m.image = tab.align == ALIGN_RIGHT  ? RichEditImg::RightTabTrack() :
	          tab.align == ALIGN_CENTER ? RichEditImg::CenterTabTrack() :
	                                      RichEditImg::LeftTabTrack();
	ruler.Set(r + 3, m);
}

void RichEdit::Hyperlink()
{
	String s = formatinfo.link;
	if(!IsSelection() && !IsNull(s) && cursorp.format.link == s && text[cursor] != '\n') {
		int l = cursor - 1;
		while(l >= 0 && text[l] != '\n' && text.GetRichPos(l).format.link == s)
			l--;
		l++;
		int h = cursor;
		while(h < text.GetLength() && text[h] != '\n' && text.GetRichPos(h).format.link == s)
			h++;
		if(l < h)
			Select(l, h - l);
	}
	WString linktext;
	WhenHyperlink(s, linktext);
	if(s != formatinfo.link || linktext.GetLength()) {
		formatinfo.link = s;
		hyperlink <<= s;
		NextUndo();
		ApplyFormat(RichText::LINK);
		if(linktext.GetLength()) {
			RemoveSelection();
			RichPara p;
			p.format = formatinfo;
			p.Cat(linktext, formatinfo);
			RichText txt;
			txt.SetStyles(text.GetStyles());
			txt.Cat(p);
			Insert(cursor, txt, true);
			Move(cursor + linktext.GetCount(), false);
		}
	}
	SetFocus();
}

void RichEdit::Label()
{
	if(IsSelection()) return;
	String s = formatinfo.label;
	WhenLabel(s);
	if(s != formatinfo.label) {
		formatinfo.label = s;
		NextUndo();
		ApplyFormat(0, RichText::LABEL);
		SetFocus();
	}
}

void RichEdit::IndexEntry()
{
	String s = formatinfo.indexentry.ToString();
	String s0 = s;
	WhenIndexEntry(s);
	if(s != s0) {
		formatinfo.indexentry = s.ToWString();
		ApplyFormat(RichText::INDEXENTRY);
		NextUndo();
		SetFocus();
	}
}

void RichEdit::BeginRulerTrack()
{
	NextUndo();
	SaveFormat();
	int r = ruler.GetTrack();
	if(r < 0) return;
	RichRuler::Marker m = ruler[r];
	switch(r) {
	case 0:
	case 1:
		m.image = RichEditImg::MarginTrack();
		break;
	case 2:
		m.image = RichEditImg::IndentTrack();
		break;
	default:
		HighLightTab(r - 3);
		return;
	}
	ruler.Set(r, m);
}

void RichEdit::SetParaFormat(dword paravalid)
{
	RichText::FormatInfo f = formatinfo;
	f.charvalid = 0;
	f.paravalid = paravalid;
	if(IsSelection())
		if(tablesel)
			text.ApplyTableFormatInfo(tablesel, cells, f);
		else
			text.ApplyFormatInfo(min(cursor, anchor), f, abs(cursor - anchor));
	else
		text.ApplyFormatInfo(cursor, f, 0);
}

void RichEdit::RulerTrack()
{
	int r = ruler.GetTrack();
	if(r < 0) return;
	RichRuler::Marker m = ruler[r];
	switch(r) {
	case 0:
		formatinfo.lm = m.pos;
		SetParaFormat(RichText::LM);
		IndentMark();
		break;
	case 1:
		formatinfo.rm = cursorc.textpage.Width() - m.pos;
		SetParaFormat(RichText::RM);
		break;
	case 2:
		formatinfo.indent = m.pos - formatinfo.lm;
		SetParaFormat(RichText::INDENT);
		break;
	default:
		formatinfo.tab[r - 3].pos = m.pos;
		SetParaFormat(RichText::TABS);
		int maxpos = 0;
		for(int i = 0; i < formatinfo.tab.GetCount(); i++) {
			RichPara::Tab tab = formatinfo.tab[i];
			if(tab.pos > maxpos)
				maxpos = tab.pos;
		}
		ruler.SetTabs(maxpos, formatinfo.tabsize);
		break;
	}
	FinishNF();
}

void RichEdit::TabAdd(int align)
{
	RichPara::Tab tab;
	tab.pos = ruler.GetPos();
	tab.align = align;
	if(formatinfo.tab.GetCount() > 30000 || tab.pos < 0 || tab.pos >= cursorc.textpage.Width()) return;
	formatinfo.tab.Add(tab);
	SetParaFormat(RichText::TABS);
	Finish();
}

void RichEdit::AddTab()
{
	NextUndo();
	SaveFormat();
	TabAdd(ruler.GetNewTabAlign());
}

void RichEdit::TabMenu()
{
	NextUndo();
	int r = ruler.GetTrack() - 3;
	if(r >= 0)
		HighLightTab(r);
	CallbackArgTarget<int> align;
	CallbackArgTarget<int> fill;
	MenuBar menu;
	menu.Add(t_("Left"), RichEditImg::LeftTab(), align[ALIGN_LEFT]);
	menu.Add(t_("Right"), RichEditImg::RightTab(), align[ALIGN_RIGHT]);
	menu.Add(t_("Center"), RichEditImg::CenterTab(), align[ALIGN_CENTER]);
	if(r >= 0) {
		int f = formatinfo.tab[r].fillchar;
		menu.Separator();
		menu.Add(t_("No fill"), fill[0])
		    .Radio(f == 0);
		menu.Add(t_("Fill with ...."), fill[1])
		    .Radio(f == 1);
		menu.Add(t_("Fill with ----"), fill[2])
		    .Radio(f == 2);
		menu.Add(t_("Fill with __"), fill[3])
		    .Radio(f == 3);
		menu.Separator();
		menu.Add(t_("Remove"), fill[-1]);
	}
	menu.Execute();
	if(!IsNull(align)) {
		SaveFormat();
		if(r >= 0) {
			formatinfo.tab[r].align = (int)align;
			SetParaFormat(RichText::TABS);
		}
		else
			TabAdd(align);
	}
	if(!IsNull(fill) && r >= 0) {
		SaveFormat();
		if(r >= 0) {
			if(fill == -1)
				formatinfo.tab[r].pos = Null;
			else
				formatinfo.tab[r].fillchar = (int)fill;
			SetParaFormat(RichText::TABS);
		}
	}
	Finish();
}

void RichEdit::AlignLeft()
{
	NextUndo();
	formatinfo.align = ALIGN_LEFT;
	ApplyFormat(0, RichText::ALIGN);
}

void RichEdit::AlignRight()
{
	NextUndo();
	formatinfo.align = ALIGN_RIGHT;
	ApplyFormat(0, RichText::ALIGN);
}

void RichEdit::AlignCenter()
{
	NextUndo();
	formatinfo.align = ALIGN_CENTER;
	ApplyFormat(0, RichText::ALIGN);
}

void RichEdit::AlignJustify()
{
	NextUndo();
	formatinfo.align = ALIGN_JUSTIFY;
	ApplyFormat(0, RichText::ALIGN);
}

void  RichEdit::SetBullet(int bullet)
{
	NextUndo();
	if((formatinfo.paravalid & RichText::BULLET) && formatinfo.bullet == bullet) {
		formatinfo.bullet = RichPara::BULLET_NONE;
		formatinfo.indent = formatinfo.paravalid & RichText::STYLE ?
		                       text.GetStyle(formatinfo.styleid).format.indent : 0;
	}
	else {
		formatinfo.bullet = bullet;
		formatinfo.indent = bullet_indent;
	}
	ApplyFormat(0, RichText::INDENT|RichText::BULLET);
}

void RichEdit::Style()
{
	NextUndo();
	SaveFormat(cursor, 0);
	formatinfo.Set(text.GetStyle((Uuid)~style).format);
	ApplyFormat(0, RichText::STYLE);
	SetFocus();
	Finish();
}

void RichEdit::AdjustObjectSize()
{
	NextUndo();
	RichObject obj = cursorp.object;
	if(!obj) return;
	WithObjectSizeLayout<TopWindow> d;
	CtrlLayoutOKCancel(d, t_("Object position"));
	Size sz = obj.GetSize();
	Size psz = GetPhysicalSize(obj);
	if(psz.cx == 0) psz.cx = 2000;
	if(psz.cy == 0) psz.cy = 2000;
	d.width.Set(unit, sz.cx);
	d.height.Set(unit, sz.cy);
	d.widthp.SetInc(5).Pattern("%.1f");
	d.widthp <<= 100.0 * sz.cx / psz.cx;
	d.heightp.SetInc(5).Pattern("%.1f");
	d.heightp <<= 100.0 * sz.cy / psz.cy;
	d.keepratio = obj.IsKeepRatio();
	d.width <<= d.height <<= d.widthp <<= d.heightp <<= d.Breaker(IDYES);
	d.ydelta.WithSgn().Set(unit, obj.GetYDelta());
	d.keepratio <<= d.Breaker(IDNO);
	for(;;) {
		switch(d.Run()) {
		case IDCANCEL:
			return;
		case IDYES:
			if(d.width.HasFocus() && !IsNull(d.width)) {
				d.widthp <<= 100 * (double)~d.width / psz.cx;
				if(d.keepratio) {
					d.height <<= psz.cy * (double)~d.width / psz.cx;
					d.heightp <<= ~d.widthp;
				}
			}
			if(d.height.HasFocus() && !IsNull(d.height)) {
				d.heightp <<= 100 * (double)~d.height / psz.cy;
				if(d.keepratio) {
					d.width <<= psz.cx * (double)~d.height / psz.cy;
					d.widthp <<= ~d.heightp;
				}
			}
			if(d.widthp.HasFocus() && !IsNull(d.widthp)) {
				d.width <<= psz.cx * (double)~d.widthp / 100;
				if(d.keepratio) {
					d.height <<= psz.cy * (double)~d.width / psz.cx;
					d.heightp <<= ~d.widthp;
				}
			}
			if(d.heightp.HasFocus() && !IsNull(d.heightp)) {
				d.height <<= psz.cy * (double)~d.heightp / 100;
				if(d.keepratio) {
					d.width <<= psz.cx * (double)~d.height / psz.cy;
					d.widthp <<= ~d.heightp;
				}
			}
			break;
		case IDNO:
			if(d.keepratio && !IsNull(d.width)) {
				d.widthp <<= 100 * (double)~d.width / psz.cx;
				if(d.keepratio) {
					d.height  <<= psz.cy * (double)~d.width / psz.cx;
					d.heightp <<= ~d.widthp;
				}
			}
			break;
		case IDOK:
			if(!IsNull(d.width) && (int)~d.width > 0)
				sz.cx = ~d.width;
			if(!IsNull(d.height) && (int)~d.height > 0)
				sz.cy = ~d.height;
			obj.SetSize(sz);
			if(!IsNull(d.ydelta))
				obj.SetYDelta(~d.ydelta);
			obj.KeepRatio(d.keepratio);
			ReplaceObject(obj);
			return;
		}
	}
}

}
