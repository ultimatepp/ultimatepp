#include "CtrlLib.h"

namespace Upp {

void DocEdit::MouseWheel(Point p, int zdelta, dword keyflags)
{
	sb.Wheel(zdelta);
}

void DocEdit::ClearLines()
{
	para.Clear();
	ASSERT(GetLineCount() == para.GetCount());
}

void DocEdit::InsertLines(int line, int count)
{
	para.Insert(line, Para(), count);
	ASSERT(GetLineCount() == para.GetCount());
}

void DocEdit::RemoveLines(int line, int count)
{
	para.Remove(line, count);
	ASSERT(GetLineCount() == para.GetCount());
}

DocEdit::Fmt DocEdit::Format(const WString& text) const
{
	FontInfo fi = font.Info();
	Fmt fmt;
	int tcx = fi['x'] * 4;
	fmt.len = text.GetLength();
	fmt.text.Alloc(text.GetLength());
	memcpy(fmt.text, text, text.GetLength() * sizeof(wchar));
	fmt.width.Alloc(text.GetLength());
	fmt.line.Add(0);
	int *w = fmt.width;
	int x = 0;
	const wchar *space = NULL;
	int spacex = 0;
	for(wchar *s = fmt.text; s < fmt.text + fmt.len; s++) {
		int cw;
		if(*s == '\t')
			cw = (x + tcx) / tcx * tcx - x;
		else
			cw = fi[*s];
		*w++ = cw;
		if(*s == ' ' || *s == '\t') {
			space = s;
			spacex = x + cw;
			*s = ' ';
		}
		x += cw;
		if(x > cx) {
			if(space && space <= s) {
				space++;
				fmt.line.Add(int(space - fmt.text));
				space = NULL;
				x -= spacex;
			}
			else {
				fmt.line.Add(int(s - fmt.text));
				x = cw;
			}
		}
	}
	fmt.fi = fi;
	return fmt;
}

int  DocEdit::GetHeight(int i) {
	Para& p = para[i];
	if(p.cx == cx) return p.cy;
	Fmt fmt = Format(GetWLine(i));
	p.cx = cx;
	p.cy = fmt.line.GetCount() * (fmt.fi.GetHeight()) + after;
	return p.cy;
}

int DocEdit::GetY(int parai) {
	int y = 1;
	for(int i = 0; i < parai; i++)
		y += GetHeight(i);
	return y;
}

void DocEdit::InvalidateLine(int i)
{
	para[i].cx = -1;
}

void DocEdit::RefreshLine(int i) {
	int q = para[i].cx >= 0 ? para[i].cy : -1;
	Refresh(1, GetY(i) - sb, cx, GetHeight(i));
	if(q < 0 || q != para[i].cy)
		Refresh();
}

int sSum(const int *w, int n)
{
	int m = 0;
	while(n--)
		m += *w++;
	return m;
}

void DocEdit::Paint(Draw& w) {
	Size sz = GetSize();
	Color bg =  color[IsShowEnabled() && !IsReadOnly() ? PAPER_NORMAL : PAPER_READONLY];
	if(nobg)
		bg = Null;
	int y = -sb + 1;
	int pos = 0;
	int sell, selh;
	GetSelection32(sell, selh);
	for(int i = 0; i < para.GetCount() && y < sz.cy; i++) {
		int h = GetHeight(i);
		if(y + h >= 0) {
			WString text = GetWLine(i);
			Fmt fmt = Format(text);
			int p = pos;
			for(int i = 0; i < fmt.line.GetCount(); i++) {
				int n = fmt.LineEnd(i) - fmt.line[i];
				int a = minmax(sell - p, 0, n);
				int b = minmax(selh - p, 0, n) - a;
				int c = n - a - b;
				int *wa = fmt.width + fmt.line[i];
				int *wb = fmt.width + fmt.line[i] + a;
				int *wc = fmt.width + fmt.line[i] + a + b;
				int acx = sSum(wa, a);
				int bcx = sSum(wb, b);
				int ccx = sSum(wc, c);
				w.DrawRect(1, y, acx, fmt.fi.GetHeight(), bg);
				w.DrawText(1, y, ~fmt.text + fmt.line[i], font,
				           IsShowEnabled() ? color[INK_NORMAL] : color[INK_DISABLED], a, wa);
				w.DrawRect(1 + acx, y, bcx, fmt.fi.GetHeight(), color[PAPER_SELECTED]);
				w.DrawText(1 + acx, y, ~fmt.text + fmt.line[i] + a, font, color[INK_SELECTED], b, wb);
				w.DrawRect(1 + acx + bcx, y, ccx, fmt.fi.GetHeight(), bg);
				w.DrawText(1 + acx + bcx, y, ~fmt.text + fmt.line[i] + a + b, font, color[INK_NORMAL], c, wc);
				p += n;
				w.DrawRect(1 + acx + bcx + ccx, y, cx - (acx + bcx + ccx), fmt.fi.GetHeight(),
				           p >= sell && p < selh ? color[PAPER_SELECTED] : bg);
				y += fmt.fi.GetHeight();
			}
			w.DrawRect(1, y, cx, after, color[PAPER_NORMAL]);
			y += after;
		}
		else
			y += h;
		pos += GetLineLength(i) + 1;
	}
	w.DrawRect(0, -sb, sz.cx, 1, bg);
	w.DrawRect(0, 0, 1, sz.cy, bg);
	w.DrawRect(sz.cx - 1, 0, 1, sz.cy, bg);
	if(eofline)
		w.DrawRect(1, y++, cx, 1, SColorShadow);
	if(y < sz.cy)
		w.DrawRect(1, y, cx, sz.cy - y, bg);
	DrawTiles(w, DropCaret(), CtrlImg::checkers());
}

void DocEdit::SetSb()
{
	Size sz = GetSize();
	cx = max(Draw::GetStdFontCy(), sz.cx - 2);
	sb.SetPage(GetSize().cy);
	sb.SetTotal(GetY(para.GetCount()) + 2);
	PlaceCaret(false);
}

void DocEdit::Layout()
{
	SetSb();
	Invalidate();
}

Point DocEdit::GetCaret(int pos) {
	int i = GetLinePos32(pos);
	Fmt fmt = Format(GetWLine(i));
	int l;
	for(l = 0; l < fmt.line.GetCount(); l++)
		if(pos < fmt.line[l])
			break;
	l--;
	const int *w = fmt.width + fmt.line[l];
	pos -= fmt.line[l];
	int x = 0;
	while(pos-- > 0)
		x += *w++;
	return Point(x, GetY(i) + l * fmt.fi.GetHeight());
}

int  DocEdit::GetCursorPos(Point p) {
	int pos = 0;
	for(int i = 0; i < para.GetCount(); i++) {
		int h = GetHeight(i);
		if(p.y < h) {
			WString text = GetWLine(i);
			Fmt fmt = Format(text);
			int x = 0;
			int l = p.y / fmt.fi.GetHeight();
			if(l < 0)
				return pos;
			if(l >= fmt.line.GetCount())
				return pos + text.GetLength();
			const int *w = fmt.width + fmt.line[l];
			const int *e = fmt.width + fmt.LineEnd(l);
			while(w < e) {
				if(p.x < x + *w / 2)
					return int(w - fmt.width) + pos;
				x += *w++;
			}
			int p = int(e - fmt.width);
			if(p > 0 && text[p - 1] == ' ' && l < fmt.line.GetCount() - 1)
			   p--;
			return p + pos;
		}
		p.y -= h;
		pos += GetLineLength(i) + 1;
	}
	return GetLength32();
}

Rect DocEdit::GetCaret() const
{
	return caret;
}

void DocEdit::PlaceCaret(bool scroll) {
	Point cr = GetCaret((int)cursor);
	int fy = font.Info().GetLineHeight();
	if(scroll) {
		if(cursor == GetLength32())
			sb.End();
		else
			sb.ScrollInto(cr.y, fy + 2);
	}
	caret = RectC(cr.x + 1, cr.y - sb, 1, fy);
	WhenSel();
}

void DocEdit::PlaceCaret(int64 newpos, bool select) {
	if(newpos > GetLength32())
		newpos = GetLength32();
	int z = GetLine(newpos);
	if(select) {
		if(anchor < 0)
			anchor = cursor;
		RefreshLines(z, GetLine(cursor));
	}
	else
		if(anchor >= 0) {
			RefreshLines(GetLine(cursor), GetLine(anchor));
			anchor = -1;
		}
	cursor = newpos;
	PlaceCaret(true);
	SelectionChanged();
	if(IsSelection())
		SetSelectionSource(ClipFmtsText());
	CancelMyPreedit();
}

int DocEdit::GetMousePos(Point p)
{
	return GetCursorPos(Point(p.x - 1, p.y + sb - 1));
}

void DocEdit::LeftDown(Point p, dword flags) {
	SetFocus();
	int c = GetMousePos(p);
	int l, h;
	if(GetSelection32(l, h) && c >= l && c < h) {
		selclick = true;
		return;
	}
	PlaceCaret(c, flags & K_SHIFT);
	SetCapture();
}

void DocEdit::LeftUp(Point p, dword flags)
{
	WhenLeftUp();
	if(!HasCapture() && selclick) {
		int c = GetMousePos(p);
		PlaceCaret(c, flags & K_SHIFT);
		SetFocus();
	}
	selclick = false;
	ReleaseCapture();
}

void DocEdit::MouseMove(Point p, dword flags) {
	if(!HasCapture()) return;
	PlaceCaret(GetMousePos(p), true);
}

void DocEdit::LeftDouble(Point, dword)
{
	int64 l, h;
	if(GetWordSelection(cursor, l, h))
		SetSelection(l, h);
}

void DocEdit::LeftTriple(Point, dword)
{
	int q = (int)cursor;
	int i = GetLinePos32(q);
	q = (int)cursor - q;
	SetSelection(q, q + GetLineLength(i) + 1);
}

Image DocEdit::CursorImage(Point, dword) {
	return Image::IBeam();
}

void DocEdit::GotFocus() {
	Refresh();
}

void DocEdit::LostFocus() {
	Refresh();
}

void DocEdit::VertMove(int delta, bool select, bool scs) {
	int hy = GetY(para.GetCount());
	Point p = GetCaret((int)cursor);
	int yy = p.y;
	for(;;) {
		p.y += delta;
		if(p.y > hy) p.y = hy - 1;
		if(p.y < 0) p.y = 0;
		int q = GetCursorPos(p);
		if(q >= 0 && q != cursor && (delta < 0) == (q < cursor) && GetCaret(q).y != yy) {
			PlaceCaret(q, select);
			break;
		}
		if(p.y == 0 || p.y >= hy - 1) {
			PlaceCaret(delta > 0 ? GetLength32() : 0, select);
			break;
		}
		delta = sgn(delta) * 4;
	}
	if(scs)
		sb = GetCaret((int)cursor).y - (yy - sb);
	PlaceCaret(true);
}

void DocEdit::HomeEnd(int x, bool select) {
	Point p = GetCaret((int)cursor);
	p.x = x;
	PlaceCaret(GetCursorPos(p), select);
}

bool DocEdit::Key(dword key, int cnt)
{
	NextUndo();
	bool h;
	int q;
	bool select = key & K_SHIFT;
	int pgsk = max(8, 6 * GetSize().cy / 8);
	switch(key & ~K_SHIFT) {
	case K_CTRL_LEFT:
		PlaceCaret(GetPrevWord(cursor), select);
		break;
	case K_CTRL_RIGHT:
		PlaceCaret(GetNextWord(cursor), select);
		break;
	case K_HOME:
		HomeEnd(0, select);
		break;
	case K_END:
		HomeEnd(cx, select);
		break;
	case K_CTRL_HOME:
	case K_CTRL_PAGEUP:
		PlaceCaret(0, select);
		break;
	case K_CTRL_END:
	case K_CTRL_PAGEDOWN:
		PlaceCaret(GetLength32(), select);
		break;
	case K_UP:
		if(GetCursor() == 0)
			return !updownleave;
		VertMove(-8, select, false);
		return true;
	case K_DOWN:
		if(GetCursor32() == GetLength32())
			return !updownleave;
		VertMove(8, select, false);
		return true;
	case K_PAGEUP:
		VertMove(-pgsk, select, true);
		return true;
	case K_PAGEDOWN:
		VertMove(pgsk, select, true);
		return true;
	case K_LEFT:
		if(cursor)
			PlaceCaret(cursor - 1, select);
		break;
	case K_RIGHT:
		if(cursor < GetLength32())
			PlaceCaret(cursor + 1, select);
		break;
	default:
		if(IsReadOnly()) return MenuBar::Scan(WhenBar, key);
		switch(key) {
		case K_BACKSPACE:
		case K_SHIFT|K_BACKSPACE:
			if(RemoveSelection()) break;
			if(cursor == 0) return true;
			cursor--;
			Remove((int)cursor, 1);
			break;
		case K_CTRL_BACKSPACE:
			if(RemoveSelection()) break;
			if(cursor <= 0) return true;
			q = (int)cursor - 1;
			h = IsLetter(GetChar(q));
			while(q > 0 && IsLetter(GetChar(q - 1)) == h) q--;
			Remove(q, (int)cursor - q);
			SetCursor(q);
			break;
		case K_DELETE:
			if(RemoveSelection()) break;
			if(cursor >= GetLength32()) return true;
			if(cursor < GetLength32())
				Remove((int)cursor, 1);
			break;
		case K_CTRL_DELETE:
			if(RemoveSelection()) break;
			if(cursor >= GetLength32()) return true;
			q = (int)cursor;
			h = IsLetter(GetChar(q));
			while(IsLetter(GetChar(q)) == h && q < GetLength32()) q++;
			Remove((int)cursor, q - (int)cursor);
			break;
		case K_ENTER:
			if(!processenter)
				return true;
			key = '\n';
		default:
			if(filter && key >= 32 && key < 65535)
				key = (*filter)(key);
			if(key >= ' ' && key < K_CHAR_LIM || key == '\n' || key == '\t' || key == K_SHIFT_SPACE) {
				if(key == K_TAB && !processtab)
					return false;
				if(key >= 128 && key < K_CHAR_LIM && (charset != CHARSET_UTF8 && charset != CHARSET_UTF8_BOM)
				   && FromUnicode((wchar)key, charset) == DEFAULTCHAR)
					return true;
				RemoveSelection();
				Insert((int)cursor, WString(key == K_SHIFT_SPACE ? ' ' : key, cnt), true);
				cursor += cnt;
				break;
			}
			return MenuBar::Scan(WhenBar, key);
		}
		UpdateAction();
	}
	PlaceCaret(true);
	return true;
}

void DocEdit::Scroll()
{
	PlaceCaret(false);
	Refresh();
}

void DocEdit::Invalidate()
{
	for(int i = 0; i < para.GetCount(); i++)
		para[i].cx = -1;
	PlaceCaret(false);
}

void  DocEdit::RefreshStyle()
{
	cursor = 0;
	sb = 0;
	ClearSelection();
	Invalidate();
	Layout();
	Refresh();
}

void DocEdit::RightDown(Point p, dword w)
{
	SetFocus();
	int c = GetMousePos(p);
	int l, h;
	if(!GetSelection32(l, h) || c < l || c >= h)
		PlaceCaret(c, false);
	MenuBar::Execute(WhenBar);
}

DocEdit::DocEdit()
{
	updownleave = false;
	cx = 0;
	filter = NULL;
	after = 0;
	font = StdFont();
	AutoHideSb();
	SetFrame(ViewFrame());
	AddFrame(sb);
	sb.SetLine(8);
	sb.WhenScroll = THISBACK(Scroll);
	InsertLines(0, 1);
	eofline = true;
	PlaceCaret(false);
}

DocEdit::~DocEdit() {}

void DocEdit::DragAndDrop(Point p, PasteClip& d)
{
	if(IsReadOnly()) return;
	int c = GetMousePos(p);
	if(AcceptText(d)) {
		NextUndo();
		int a = sb;
		int sell, selh;
		WString txt = GetWString(d);
		if(GetSelection32(sell, selh)) {
			if(c >= sell && c < selh) {
				if(!IsReadOnly())
					RemoveSelection();
				if(IsDragAndDropSource())
					d.SetAction(DND_COPY);
				c = sell;
			}
			else
			if(d.GetAction() == DND_MOVE && IsDragAndDropSource()) {
				if(c > sell)
					c -= selh - sell;
				if(!IsReadOnly())
					RemoveSelection();
				d.SetAction(DND_COPY);
			}
		}
		int count = Insert(c, txt);
		sb = a;
		SetFocus();
		SetSelection(c, c + count);
		Action();
		return;
	}
	if(!d.IsAccepted()) return;
	Point dc = Null;
	if(c >= 0) {
		Point cr = GetCaret(c);
		dc = Point(cr.x + 1, cr.y);
	}
	if(dc != dropcaret) {
		RefreshDropCaret();
		dropcaret = dc;
		RefreshDropCaret();
	}
}

Rect DocEdit::DropCaret()
{
	if(IsNull(dropcaret))
		return Rect(0, 0, 0, 0);
	return RectC(dropcaret.x, dropcaret.y - sb, 1, font.Info().GetLineHeight());
}

void DocEdit::RefreshDropCaret()
{
	Refresh(DropCaret());
}

void DocEdit::DragRepeat(Point p)
{
	sb = (int)sb + GetDragScroll(this, p, 16).y;
}

void DocEdit::DragLeave()
{
	RefreshDropCaret();
	dropcaret = Null;
	isdrag = false;
	Layout();
}

void DocEdit::LeftDrag(Point p, dword flags)
{
	int c = GetMousePos(p);
	int l, h;
	if(!HasCapture() && GetSelection32(l, h) && c >= l && c < h) {
		WString sample = GetW(l, min(h - l, 3000));
		Size ssz = StdSampleSize();
		ImageDraw iw(ssz);
		iw.DrawRect(ssz, Black());
		iw.Alpha().DrawRect(ssz, Black());
		DrawTLText(iw.Alpha(), 0, 0, ssz.cx, sample, StdFont(), White());
		NextUndo();
		if(DoDragAndDrop(ClipFmtsText(), iw) == DND_MOVE && !IsReadOnly()) {
			RemoveSelection();
			Action();
		}
	}
}

TextCtrl::EditPos DocEdit::GetEditPos() const {
	EditPos pos;
	pos.sby = sb.Get();
	pos.cursor = cursor;
	return pos;
}

void DocEdit::SetEditPos(const TextCtrl::EditPos& pos) {
	sb.Set(minmax(pos.sby, 0, GetY(para.GetCount()) + 2));
	SetCursor(pos.cursor);
}

}
