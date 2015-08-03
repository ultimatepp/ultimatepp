#include "TextDiffCtrl.h"

namespace Upp {


inline Color HistoryBg() { return Color(255, 255, 0); }

TextCompareCtrl::TextCompareCtrl()
{
	letter = Size(1, 1);
	number_width = 0;
	number_yshift = 0;
	number_bg = WhiteGray();
	SetFrame(FieldFrame());
	AddFrame(scroll);
	SetFont(Courier(Zy(14)), Courier(Zy(10)));
	scroll.NoAutoHide();
	scroll.WhenScroll = THISBACK(SelfScroll);
	maxwidth = 0;
	tabsize = 4;
	gutter_width = 0;
	gutter_bg = Color(151, 190, 239);
	gutter_fg = SGreen;
	cursor = anchor = Null;
	gutter_capture = false;
}

int TextCompareCtrl::GetLineNo(int y, int& ii)
{
	ii = scroll.Get().y + y / letter.cy;
	return ii >= 0 && ii < lines.GetCount() ? lines[ii].number : Null;
}

void TextCompareCtrl::DoSelection(int y, bool shift)
{
	int ii;
	int i = GetLineNo(y, ii);
	if(!IsNull(i)) {
		if(shift)
			cursor = i;
		else
			cursor = anchor = i;
		Refresh();
		scroll.ScrollIntoY(ii);
	}
}

void TextCompareCtrl::LeftDown(Point pt, dword keyflags)
{
	Size sz = GetSize();
	if(pt.x > sz.cx - gutter_width || HasCapture() && gutter_capture) {
		if(!HasCapture())
			SetCapture();
		int line = (pt.y * lines.GetCount()) / sz.cy;
		int page_lines = sz.cy / letter.cy;
		scroll.SetY(line - page_lines / 2);
		gutter_capture = true;
	}
	else {
		DoSelection(pt.y, keyflags & K_SHIFT);
		SetCapture();
		gutter_capture = false;
	}
	SetWantFocus();
}

void TextCompareCtrl::LeftDouble(Point pt, dword keyflags)
{
	int ii;
	int i = GetLineNo(pt.y, ii);
	if(!IsNull(i))
		WhenLeftDouble(i - 1);
}

void TextCompareCtrl::MouseMove(Point pt, dword flags)
{
	if(HasCapture()) {
		if(gutter_capture)
			LeftDown(pt, flags);
		else
			DoSelection(pt.y, true);
	}
}

void TextCompareCtrl::LeftUp(Point pt, dword keyflags)
{
	ReleaseCapture();
}

void TextCompareCtrl::Copy()
{
	int sell, selh;
	if(GetSelection(sell, selh)) {
		String clip;
		for(int i = 0; i < lines.GetCount(); i++) {
			const Line& l = lines[i];
			if(l.number >= sell && l.number <= selh) {
				clip << l.text;
			#ifdef PLATFORM_WIN32
				clip << "\r\n";
			#else
				clip << "\n";
			#endif
			}
		}
		ClearClipboard();
		AppendClipboardText(clip);
	}
}

void TextCompareCtrl::RightDown(Point p, dword keyflags)
{
	MenuBar b;
	b.Add(!IsNull(cursor), t_("Copy"), CtrlImg::copy(), THISBACK(Copy)).Key(K_CTRL_C);
	b.Execute();
}

bool TextCompareCtrl::Key(dword key, int repcnt)
{
	Point pos = scroll, newpos = pos, page = scroll.GetPage();
	switch(key) {
		case K_LEFT:       newpos.x--; break;
		case K_RIGHT:      newpos.x++; break;
		case K_CTRL_LEFT:  newpos.x -= page.x >> 1; break;
		case K_CTRL_RIGHT: newpos.x += page.x >> 1; break;
		case K_UP:         newpos.y--; break;
		case K_DOWN:       newpos.y++; break;
		case K_PAGEUP:     newpos.y -= page.y; break;
		case K_PAGEDOWN:   newpos.y += page.y; break;
		case K_HOME:       newpos.x = 0; break;
		case K_END:        newpos.x = maxwidth - page.x; break;
		case K_CTRL_HOME:  newpos.y = 0; break;
		case K_CTRL_END:   newpos.y = lines.GetCount() - page.y; break;
		case K_CTRL_C:
			Copy();
			break;
		case K_F3: {
			bool found = false;
			int i = max(pos.y + 2, 0);
			while(i < lines.GetCount() && lines[i].level)
				i++;
			while(i < lines.GetCount())
				if(lines[i++].level) {
					newpos.y = i - 2;
					found = true;
					break;
				}
			if(!found) {
				BeepExclamation();
				return true;
			}
			break;
		}
		case K_SHIFT_F3: {
			bool found = false;
			int i = min(pos.y, lines.GetCount() - 1);
			while(i > 0 && lines[i].level)
				i--;
			while(i >= 0)
				if(lines[i--].level) {
					newpos.y = i;
					found = true;
					break;
				}
			if(!found) {
				BeepExclamation();
				return true;
			}
			break;
		}
		default: return false;
	}
	if(newpos != pos)
		scroll.Set(newpos);
	return true;
}

void TextCompareCtrl::Paint(Draw& draw)
{
	Point sc = scroll.Get();
	Size offset = (Size)sc * letter;
	Size sz = GetSize();

	int lcnt = lines.GetCount();
	int first_line = offset.cy / letter.cy;
	int last_line = min(idivceil(sz.cy + offset.cy, letter.cy), lines.GetCount() - 1);

	if(gutter_width > 0)
	{
		int t = 0, b = 0;
		int gx = sz.cx - gutter_width;
		for(int i = 0; i < lcnt; i++)
			if(lines[i].level > 1) {
				b = idivceil(sz.cy * i, lcnt);
				if(b >= t) {
					draw.DrawRect(gx, t, gutter_width, b - t, gutter_bg);
					draw.DrawRect(gx, b, gutter_width, 1, gutter_fg);
					t = b + 1;
				}
			}

		draw.DrawRect(gx, t, gutter_width, sz.cy - t, gutter_bg);

		int total = letter.cy * lcnt;
		if(total <= 0)
			total = 1;
		int page_height = (sz.cy * sz.cy) / total;
		int ty = max(0, (sz.cy * offset.cy) / total);
		int by = min(sz.cy, ty + page_height);
		draw.DrawRect(gx, ty, gutter_width, 2, Black);
		draw.DrawRect(gx, by - 2, gutter_width, 2, Black);
		draw.DrawRect(gx, ty, 2, by - ty, Black);
		draw.DrawRect(gx + gutter_width - 2, ty, 2, by - ty, Black);
	}

	for(int i = first_line; i <= last_line; i++) {
		const Line& l = lines[i];
		int y = i * letter.cy - offset.cy;
		draw.DrawRect(0, y, number_width, letter.cy, number_bg);
		if(!IsNull(l.number))
			draw.DrawText(0, y + number_yshift, FormatInt(l.number), number_font, l.diff ? LtBlue() : SColorText());
	}
	draw.Clip(number_width, 0, sz.cx - gutter_width - number_width, sz.cy);
	int sell, selh;
	GetSelection(sell, selh);
	for(int i = first_line; i <= last_line; i++) {
		const Line& l = lines[i];
		int y = i * letter.cy - offset.cy;
		Color ink = SColorText();
		Color paper = IsNull(l.number) ? SGray() : l.diff ? SColorInfo() : SColorPaper();
		bool sel = l.number >= sell && l.number <= selh;
		if(sel) {
			ink = SColorHighlightText;
			paper = SColorHighlight;
		}
		draw.DrawRect(0, y, sz.cx, letter.cy, paper);
		WString ln = l.text.ToWString();
		if(ln.GetCount() > 20000)
			ln.Trim(20000);
		ln = ExpandTabs(ln);
		Vector<LineEdit::Highlight> hln;
		hln.SetCount(ln.GetCount() + 1);
		for(int i = 0; i < ln.GetCount(); i++) {
			LineEdit::Highlight& h = hln[i];
			h.paper = paper;
			h.ink = ink;
			h.chr = ln[i];
			h.font = StdFont();
		}
		if(!sel)
			WhenHighlight(hln, ln);
		int x = 0;
		int a = StdFont().GetAscent();
		for(int i = 0; i < hln.GetCount(); i++) {
			Font fnt = font;
			LineEdit::Highlight& h = hln[i];
			fnt.Bold(h.font.IsBold());
			fnt.Italic(h.font.IsItalic());
			fnt.Underline(h.font.IsUnderline());
			draw.DrawText(number_width - offset.cx + x, y + a - fnt.GetAscent(), &h.chr, fnt, h.ink, 1);
			x += font[h.chr];
		}
	}
	int lcy = lcnt * letter.cy - offset.cy;
	draw.DrawRect(0, lcy, sz.cx, sz.cy - lcy, SGray());
	draw.End();
	draw.DrawRect(0, lcy, number_width, sz.cy - lcy, number_bg);
}

void TextCompareCtrl::TabSize(int t)
{
	tabsize = t;
	UpdateWidth();
	Layout();
}

void TextCompareCtrl::SetFont(Font f, Font nf)
{
	font = f;
	number_font = nf;
	FontInfo fi = f.Info();
	FontInfo ni = nf.Info();
	letter.cy = fi.GetHeight();
	letter.cx = fi.GetAveWidth();
	number_width = 5 * ni.GetAveWidth();
	number_yshift = (fi.GetHeight() - ni.GetHeight() + 2) >> 1;
	Layout();
}

void TextCompareCtrl::SetFont(Font f)
{
	SetFont(f, GetNumberFont());
}

void TextCompareCtrl::Layout()
{
	scroll.Set(scroll, (scroll.GetReducedViewSize() - Size(number_width + gutter_width, 0)) / letter, Size(maxwidth, lines.GetCount()));
	Refresh();
}

void TextCompareCtrl::MouseWheel(Point pt, int zdelta, dword keyflags)
{
	scroll.WheelY(zdelta);
}

void TextCompareCtrl::SetCount(int c)
{
	bool rl = (c < lines.GetCount());
	lines.SetCount(c);
	if(rl)
		UpdateWidth();
	Layout();
}

void TextCompareCtrl::AddCount(int c)
{
	lines.InsertN(lines.GetCount(), c);
	Layout();
}

void TextCompareCtrl::Set(int line, String text, bool diff, int number, int level)
{
	Line& l = lines.At(line);
	int tl = MeasureLength(text.ToWString());
	int lt = MeasureLength(l.text.ToWString());
	bool rl = (tl < lt && lt == maxwidth);
	l.number = number;
	l.text = text;
	l.diff = diff;
	l.level = level;
	if(rl)
		UpdateWidth();
	else if(tl > maxwidth) {
		maxwidth = tl;
		Layout();
	}
}

void TextCompareCtrl::SelfScroll()
{
	Refresh();
	WhenScroll();
}

void TextCompareCtrl::PairScroll(TextCompareCtrl *ctrl)
{
	scroll.Set(ctrl->scroll.Get());
}

void TextCompareCtrl::UpdateWidth()
{
	maxwidth = 0;
	for(int i = 0; i < lines.GetCount(); i++)
		maxwidth = max(maxwidth, MeasureLength(lines[i].text.ToWString()));
}

int TextCompareCtrl::MeasureLength(const wchar *text) const
{
	int pos = 0;
	while(*text)
		if(*text++ == '\t')
			pos += tabsize - pos % tabsize;
		else
			pos++;
	return pos;
}

bool TextCompareCtrl::GetSelection(int& l, int& h)
{
	if(IsNull(cursor)) {
		l = h = -1;
		return false;
	}
	l = min(cursor, anchor);
	h = max(cursor, anchor);
	return true;
}

WString TextCompareCtrl::ExpandTabs(const wchar *text) const
{
	WString out;
	for(wchar c; (c = *text++);)
		if(c == '\t')
			out.Cat(' ', tabsize - out.GetLength() % tabsize);
		else
			out.Cat(c);
	return out;
}

Point TextCompareCtrl::GetPos() const
{
	Point pos(0, 0);
	int ltop = minmax(scroll.Get().y, 0, lines.GetCount() - 1);
	while(ltop >= 0 && IsNull(lines[ltop].number)) {
		ltop--;
		pos.y++;
	}
	if(ltop >= 0)
		pos.x = lines[ltop].number;
	return pos;
}

void TextCompareCtrl::SetPos(Point pos)
{
	int l = FindFieldIndex(lines, &Line::number,pos.x);
	if(l < 0)
		l = 0;
	SetSb(l + pos.y);
}

};
