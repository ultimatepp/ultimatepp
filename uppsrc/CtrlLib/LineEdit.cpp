#include "CtrlLib.h"

NAMESPACE_UPP

#define LLOG(x) //  LOG(x)

LineEdit::LineEdit() {
	isdrag = false;
	nohbar = false;
	showtabs = false;
	tabsize = 4;
	font = Courier(16);
	SetFrame(ViewFrame());
	sb.NoBox();
	AddFrame(sb);
	sb.WhenScroll = THISBACK(Scroll);
	cutline = true;
	bordercolumn = -1;
	bordercolor = Null;
	overwrite = false;
	filter = NULL;
	showspaces = false;
}

LineEdit::~LineEdit() {}

void LineEdit::MouseWheel(Point, int zdelta, dword keyflags) {
	if(keyflags & K_SHIFT)
		sb.WheelX(zdelta);
	else
		sb.WheelY(zdelta);
}

void   LineEdit::Clear() {
	gcolumn = 0;
	TextCtrl::Clear();
	sb.SetTotal(0, 0);
	sb.Set(0, 0);
	NewScrollPos();
	PlaceCaret(0, false);
}

LineEdit& LineEdit::TabSize(int n) {
	tabsize = n;
	PlaceCaret0(GetColumnLine(cursor));
	Refresh();
	return *this;
}

LineEdit& LineEdit::BorderColumn(int col, Color c)
{
	bordercolumn = col;
	bordercolor = c;
	Refresh();
	return *this;
}

LineEdit& LineEdit::SetFont(Font f) {
	font = f;
	Layout();
	TabSize(tabsize);
	SetSb();
	return *this;
}

Size LineEdit::GetFontSize() const {
	FontInfo fi = font.Info();
	return Size(max(fi['M'], fi['W']), fi.GetHeight());
}

void   LineEdit::Paint0(Draw& w) {
	int sell, selh;
	GetSelection(sell, selh);
	if(!IsEnabled())
		sell = selh = 0;
	Size sz = GetSize();
	Size fsz = GetFontSize();
	Point sc = sb;
	int ll = min(line.GetCount(), sz.cy / fsz.cy + sc.y + 1);
	int  y = 0;
	cpos = GetPos(sc.y);
	cline = sc.y;
	sell -= cpos;
	selh -= cpos;
	int pos = cpos;
	Vector<int> dx, dx2;
	int fascent = font.Info().GetAscent();
	for(int i = sc.y; i < ll; i++) {
		WString tx = line[i];
		int len = tx.GetLength();
		if(w.IsPainting(0, y, sz.cx, fsz.cy)) {
			Highlight ih;
			ih.ink = color[IsShowEnabled() ? INK_NORMAL : INK_DISABLED];
			ih.paper = color[IsReadOnly() || !IsShowEnabled() ? PAPER_READONLY : PAPER_NORMAL];
			if(nobg)
				ih.paper = Null;
			ih.font = font;
			ih.chr = 0;
			Vector<Highlight> hl;
			hl.SetCount(len + 1, ih);
			for(int q = 0; q < tx.GetCount(); q++)
				hl[q].chr = tx[q];
			HighlightLine(i, hl, pos);
			int ln = hl.GetCount() - 1;
			int l = max(sell, 0);
			int h = selh > len ? len : selh;
			if(l < h)
				for(int i = l; i < h; i++) {
					hl[i].paper = color[PAPER_SELECTED];
					hl[i].ink = color[INK_SELECTED];
				}
			if(sell <= len && selh > len)
				for(int i = len; i < hl.GetCount(); i++) {
					hl[i].paper = color[PAPER_SELECTED];
					hl[i].ink = color[INK_SELECTED];
				}
			Buffer<wchar> txt(ln);
			for(int i = 0; i < ln; i++)
				txt[i] = hl[i].chr;
			for(int pass = 0; pass < 2; pass++) {
				int gp = 0;
				int scx = fsz.cx * sc.x;
				if(ln >= 0) {
					int q = 0;
					while(q < ln) {
						Highlight& h = hl[q];
						if(txt[q] == '\t') {
							int ngp = (gp + tabsize) / tabsize * tabsize;
							int l = ngp - gp;
							LLOG("Highlight -> tab[" << q << "] paper = " << h.paper);
							if(pass == 0) {
								w.DrawRect(gp * fsz.cx - scx, y, fsz.cx * l, fsz.cy, h.paper);
								if(showtabs && h.paper != SColorHighlight && q < tx.GetLength()) {
									Color c = Blend(SColorLight, SColorHighlight);
									w.DrawRect(gp * fsz.cx - scx + 2, y + fsz.cy / 2,
									           l * fsz.cx - 4, 1, c);
									w.DrawRect(ngp * fsz.cx - scx - 3, y + 3,
									           1, fsz.cy - 6, c);
								}
								if(bordercolumn > 0 && bordercolumn >= gp && bordercolumn < gp + l)
									w.DrawRect((bordercolumn - sc.x) * fsz.cx, y, 1, fsz.cy, bordercolor);
							}
							q++;
							gp = ngp;
						}
						else
						if(txt[q] == ' ') {
						    LLOG("Highlight -> space[" << q << "] paper = " << h.paper);
						    if(pass == 0) {
						        w.DrawRect(gp * fsz.cx - scx, y, fsz.cx, fsz.cy, h.paper);
						        if(showspaces && h.paper != SColorHighlight && q < tx.GetLength()) {
						            Color c = Blend(SColorLight, SColorHighlight);
						            w.DrawRect(gp * fsz.cx - scx + fsz.cx / 2, y + fsz.cy / 2,
						                       2, 2, c);
						        }
						        if(bordercolumn > 0 && bordercolumn >= gp && bordercolumn < gp + 1)
						            w.DrawRect((bordercolumn - sc.x) * fsz.cx, y, 1, fsz.cy, bordercolor);
						    }
						    q++;
						    gp++;
						}
						else {
							bool cjk = IsCJKIdeograph(txt[q]);
							int p = q + 1;
							while(p < len && h == hl[p] && txt[p] != '\t' && txt[p] != ' ' && IsCJKIdeograph(txt[p]) == cjk && p - q < 128)
								p++;
							int l = p - q;
							int ll = cjk ? 2 * l : l;
							LLOG("Highlight -> paper[" << q << "] = " << h.paper);
							int x = gp * fsz.cx - scx;
							int xx = x + (gp + ll) * fsz.cx;
							if(max(x, 0) < min(xx, sz.cx))
								if(pass == 0) {
									w.DrawRect(x, y, fsz.cx * ll, fsz.cy, h.paper);
									if(bordercolumn > 0 && bordercolumn >= gp && bordercolumn < gp + ll)
										w.DrawRect((bordercolumn - sc.x) * fsz.cx, y, 1, fsz.cy, bordercolor);
								}
								else {
									if(cjk)
										dx2.At(l, 2 * fsz.cx);
									else
										dx.At(l, fsz.cx);
									w.DrawText(x,
									           y + fascent - h.font.Info().GetAscent(),
									           ~txt + q, h.font, h.ink, l, cjk ? dx2 : dx);
								}
							q = p;
							gp += ll;
							if(x > sz.cx)
								break;
						}
					}
				}
				if(pass == 0) {
					int gpx = gp * fsz.cx - scx;
					w.DrawRect(gpx, y, sz.cx - gpx, fsz.cy, hl.Top().paper);
					if(bordercolumn > 0 && bordercolumn >= gp)
						w.DrawRect((bordercolumn - sc.x) * fsz.cx, y, 1, fsz.cy, bordercolor);
				}
			}
		}
		y += fsz.cy;
		sell -= len + 1;
		selh -= len + 1;
		pos += len + 1;
	}
	w.DrawRect(0, y, sz.cx, sz.cy - y, color[IsReadOnly() || !IsShowEnabled() ? PAPER_READONLY : PAPER_NORMAL]);
	DrawTiles(w, DropCaret(), CtrlImg::checkers());
}

void LineEdit::Paint(Draw& w)
{
	Paint0(w);
	scroller.Set(sb);
}

struct LineEdit::RefreshDraw : public NilDraw {
	Ctrl  *ctrl;
	bool (*chars)(int c);
	Size   fsz;
	virtual void DrawTextOp(int x, int y, int angle, const wchar *text, Font,
	                        Color, int n, const int *dx) {
		if(dx)
			while(n > 0) {
				if((*chars)(*text))
					ctrl->Refresh(x, y, fsz.cx, fsz.cy);
				text++;
				x += *dx++;
				n--;
			}
	}
	bool IsPaintingOp(const Rect& r) const {
		return true;
	}
};

void LineEdit::RefreshChars(bool (*chars)(int c))
{
	RefreshDraw rw;
	rw.ctrl = this;
	rw.fsz = GetFontSize();
	rw.chars = chars;
	Paint(rw);
}

void   LineEdit::Layout() {
	Size sz = sb.GetReducedViewSize();
	if(nohbar || isdrag)
		sz.cy = GetSize().cy;
	sb.SetPage(sz / GetFontSize());
	SetHBar();
}

int   LineEdit::GetGPos(int ln, int cl) const {
	ln = minmax(ln, 0, line.GetCount() - 1);
	WString txt = line[ln];
	const wchar *b = txt;
	const wchar *e = txt.End();
	const wchar *s = b;
	int gl = 0;
	while(s < e) {
		if(*s == '\t')
			gl = (gl + tabsize) / tabsize * tabsize;
		else
			gl += 1 + IsCJKIdeograph(*s);
		if(cl < gl) break;
		s++;
	}
	return GetPos(ln, int(s - b));
}

Point LineEdit::GetColumnLine(int pos) const {
	Point p;
	if(pos > total) pos = total;
	p.y = GetLinePos(pos);
	p.x = 0;
	WString txt = line[p.y];
	const wchar *s = txt;
	while(pos--) {
		if(*s == '\t')
			p.x = (p.x + tabsize) / tabsize * tabsize;
		else
			p.x += 1 + IsCJKIdeograph(*s);
		s++;
	}
	return p;
}

Point LineEdit::GetIndexLine(int pos) const
{
	Point p;
	if(pos > total) pos = total;
	p.y = GetLinePos(pos);
	p.x = minmax(pos, 0, line[p.y].GetLength());
	return p;
}

int LineEdit::GetIndexLinePos(Point pos) const
{
	if(pos.y < 0)
		return 0;
	if(pos.y >= GetLineCount())
		return total;
	return GetPos(pos.y, minmax(pos.x, 0, line[pos.y].GetLength()));
}

void LineEdit::RefreshLine(int i) {
	Size sz = GetSize();
	int fcy = GetFontSize().cy;
	Refresh(0, (i - sb.Get().y) * fcy, sz.cx, fcy);
}

Rect LineEdit::GetLineScreenRect(int line) const {
	int fcy = GetFontSize().cy;
	Rect r = RectC(0, (line - sb.Get().y) * fcy, GetSize().cx, fcy);
	r.Offset(GetScreenView().TopLeft());
	return r;
}

void LineEdit::SetSb() {
	sb.SetTotalY(line.GetCount());
	SetHBar();
}

void LineEdit::NewScrollPos() {}
void LineEdit::HighlightLine(int line, Vector<Highlight>& h, int pos) {}

void LineEdit::AlignChar() {
	int c = GetCursor();
	if(c == 0)
		return;
	Point pos = GetColumnLine(c);
	if(pos.x == 0)
		return;
	for(int d = 1; d <= pos.y && d < 100; d++) {
		int lny = pos.y - d;
		WString above = GetWLine(lny);
		int offset = GetGPos(lny, pos.x) - GetPos(lny);
		int end = offset;
		char ch = GetChar(c - 1);
		if(ch == ' ')
		{
			offset++;
			while(end < above.GetLength() && above[end] != ' ')
				end++;
			while(end < above.GetLength() && above[end] == ' ')
				end++;
		}
		else
			while(end < above.GetLength() && above[end] != ch)
				end++;
		if(end < above.GetLength()) {
			int count = end - offset + 1;
			WString s(' ', count);
			Insert(c - 1, s, true);
			SetCursor(c + count);
			return;
		}
	}
}

void LineEdit::PlaceCaret0(Point p) {
	Size fsz = GetFontSize();
	p -= sb;
	caretpos = Point(p.x * fsz.cx, p.y * fsz.cy);
	if(overwrite)
		SetCaret(caretpos.x, caretpos.y + fsz.cy - 2, fsz.cx, 2);
	else
		SetCaret(caretpos.x, caretpos.y, 2, fsz.cy);
}

int LineEdit::PlaceCaretNoG(int newcursor, bool sel) {
	if(newcursor > total) newcursor = total;
	Point p = GetColumnLine(newcursor);
	if(sel) {
		if(anchor < 0) {
			anchor = cursor;
		}
		RefreshLines(p.y, GetLine(cursor));
	}
	else
		if(anchor >= 0) {
			RefreshLines(GetLine(cursor), GetLine(anchor));
			anchor = -1;
		}
	cursor = newcursor;
	ScrollIntoCursor();
	PlaceCaret0(p);
	SelectionChanged();
	WhenSel();
	if(IsSelection())
		SetSelectionSource(ClipFmtsText());
	return p.x;
}

void LineEdit::PlaceCaret(int newcursor, bool sel) {
	gcolumn = PlaceCaretNoG(newcursor, sel);
}

void LineEdit::TopCursor(int lines)
{
	sb.SetY(max(0, GetLine(cursor) - lines));
}

void LineEdit::CenterCursor() {
	int cy = sb.GetPage().cy;
	if(cy > 4)
		sb.SetY(max(min(GetLine(cursor) - cy / 2, line.GetCount() - cy), 0));
}

void LineEdit::Scroll() {
	PlaceCaret0(GetColumnLine(cursor));
	scroller.Scroll(*this, GetSize(), sb.Get(), GetFontSize());
	SetHBar();
	NewScrollPos();
}

int LineEdit::GetMousePos(Point p) const {
	Size fsz = GetFontSize();
	p = (p + fsz.cx / 2 + fsz * (Size)sb.Get()) / fsz;
	return GetGPos(p.y, p.x);
}

void LineEdit::LeftDown(Point p, dword flags) {
	mpos = GetMousePos(p);
	int l, h;
	if(GetSelection(l, h) && mpos >= l && mpos < h) {
		selclick = true;
		return;
	}
	PlaceCaret(mpos, flags & K_SHIFT);
	SetWantFocus();
	SetCapture();
}

void LineEdit::LeftUp(Point p, dword flags)
{
	if(!HasCapture() && selclick) {
		mpos = GetMousePos(p);
		PlaceCaret(mpos, flags & K_SHIFT);
		SetWantFocus();
	}
	selclick = false;
}

void LineEdit::RightDown(Point p, dword flags)
{
	mpos = GetMousePos(p);
	SetWantFocus();
	int l, h;
	if(!GetSelection(l, h) || mpos < l || mpos >= h)
		PlaceCaret(mpos, false);
	MenuBar::Execute(WhenBar);
}

void LineEdit::LeftDouble(Point, dword)
{
	int l, h;
	if(GetWordSelection(cursor, l, h))
		SetSelection(l, h);
}

void LineEdit::LeftTriple(Point, dword)
{
	int q = cursor;
	int i = GetLinePos(q);
	q = cursor - q;
	SetSelection(q, q + GetLineLength(i) + 1);
}

void LineEdit::MouseMove(Point p, dword flags) {
	if((flags & K_MOUSELEFT) && HasFocus() && HasCapture()) {
		int c = GetMousePos(p);
		PlaceCaret(c, mpos != c || HasCapture());
	}
}

void LineEdit::LeftRepeat(Point p, dword flags) {
	if(HasCapture()) {
		int c = GetMousePos(p);
		if(mpos != c)
			PlaceCaret(c, true);
	}
}

Image LineEdit::CursorImage(Point, dword) {
	return Image::IBeam();
}

void LineEdit::MoveUpDown(int n, bool sel) {
	int cl = cursor;
	int ln = GetLinePos(cl);
	ln = minmax(ln + n, 0, line.GetCount() - 1);
	PlaceCaretNoG(GetGPos(ln, gcolumn), sel);
}

void LineEdit::MoveLeft(bool sel) {
	if(cursor)
		PlaceCaret(cursor - 1, sel);
}

void LineEdit::MoveRight(bool sel) {
	if(cursor < total)
		PlaceCaret(cursor + 1, sel);
}

void LineEdit::MoveUp(bool sel) {
	MoveUpDown(-1, sel);
}

void LineEdit::MoveDown(bool sel) {
	MoveUpDown(1, sel);
}

void LineEdit::MovePage(int dir, bool sel) {
	int n = dir * max(GetSize().cy / GetFontSize().cy - 2, 2);
	sb.SetY(Point(sb).y + n);
	MoveUpDown(n, sel);
}

void LineEdit::MovePageUp(bool sel) {
	MovePage(-1, sel);
}

void LineEdit::MovePageDown(bool sel) {
	MovePage(1, sel);
}

inline bool sTabSpace(int c) { return c == '\t' || c == ' '; }

void LineEdit::MoveHome(bool sel) {
	int cl = cursor;
	int li = GetLinePos(cl);
	int i = 0;
	WString l = line[li];
	while(sTabSpace(l[i]))
		i++;
	PlaceCaret(GetPos(li, cl == i ? 0 : i), sel);
}

void LineEdit::MoveEnd(bool sel) {
	int i = GetLine(cursor);
	PlaceCaret(GetPos(i, line[i].GetLength()), sel);
}

void LineEdit::MoveTextBegin(bool sel) {
	PlaceCaret(0, sel);
}

void LineEdit::MoveTextEnd(bool sel) {
	PlaceCaret(total, sel);
}

bool LineEdit::InsertChar(dword key, int count, bool canow) {
	if(key == K_TAB && !processtab)
		return false;
	if(filter && key >= 32 && key < 65535)
		key = (*filter)(key);
	if(!IsReadOnly() && (key >= 32 && key < 65536 || key == '\t' || key == '\n' ||
	   key == K_ENTER && processenter || key == K_SHIFT_SPACE)) {
		if(key >= 128 && key < 65536 && charset != CHARSET_UNICODE
		   && FromUnicode((wchar)key, charset) == DEFAULTCHAR)
			return true;
		if(!RemoveSelection() && overwrite && key != '\n' && key != K_ENTER && canow) {
			int q = cursor;
			int i = GetLinePos(q);
			if(q + count - 1 < GetLineLength(i))
				Remove(cursor, count);
		}
		WString text(key == K_ENTER ? '\n' : key == K_SHIFT_SPACE ? ' ' : key, count);
		Insert(cursor, text, true);
		PlaceCaret(cursor + count);
		Action();
		return true;
	}
	return false;
}

void LineEdit::DeleteChar() {
	if(IsReadOnly() || RemoveSelection()) {
		Action();
		return;
	}
	if(cursor < total) {
		Remove(cursor, 1);
		Action();
	}
}

void LineEdit::Backspace() {
	if(IsReadOnly() || RemoveSelection() || cursor == 0) return;
	MoveLeft();
	DeleteChar();
	Action();
}

void LineEdit::DeleteLine()
{
	int b, e;
	if(GetSelection(b, e) && GetLine(b) != GetLine(e)) {
		RemoveSelection();
		return;
	}
	int i = GetLine(cursor);
	int p = GetPos(i);
	Remove(p, line[i].GetLength() + 1);
	PlaceCaret(p);
	Action();
}

void LineEdit::CutLine()
{
	if(IsReadOnly()) return;
	int b, e;
	if(GetSelection(b, e) && GetLine(b) != GetLine(e)) {
		Cut();
		return;
	}
	int i = GetLine(cursor);
	int p = GetPos(i);
	WString txt = Get(p, line[i].GetLength() + 1).ToWString();
	WriteClipboardUnicodeText(txt);
	AppendClipboardText(txt.ToString());
	ClearSelection();
	DeleteLine();
}

void LineEdit::EditPos::Serialize(Stream& s) {
	int version = 0;
	s / version;
	s % sby % cursor;
}

LineEdit::EditPos LineEdit::GetEditPos() const {
	EditPos pos;
	pos.sby = sb.Get().y;
	pos.cursor = cursor;
	return pos;
}

void LineEdit::SetEditPos(const LineEdit::EditPos& pos) {
	sb.SetY(minmax(pos.sby, 0, line.GetCount() - 1));
	SetCursor(pos.cursor);
}

void LineEdit::SetEditPosSb(const LineEdit::EditPos& pos) {
	SetCursor(pos.cursor);
	sb.SetY(minmax(pos.sby, 0, line.GetCount() - 1));
}

void LineEdit::SetHBar()
{
	int mpos = 0;
	if(!nohbar && !isdrag) {
		int m = min(sb.y + sb.GetPage().cy + 2, line.GetCount());
		for(int i = sb.y; i < m; i++) {
			int pos = 0;
			WString l = line[i];
			const wchar *s = l;
			const wchar *e = l.End();
			while(s < e) {
				if(*s == '\t')
					pos = (pos + tabsize) / tabsize * tabsize;
				else
					pos += 1 + IsCJKIdeograph(*s);
				s++;
			}
			mpos = max(mpos, pos);
		}
	}
	sb.SetTotalX(mpos + 1);
}

void LineEdit::ScrollIntoCursor()
{
	Point p = GetColumnLine(GetCursor());
	sb.ScrollInto(p);
	SetHBar();
	sb.ScrollInto(p);
}

bool LineEdit::Key(dword key, int count) {
	NextUndo();
	switch(key) {
	case K_CTRL_UP:
		ScrollUp();
		return true;
	case K_CTRL_DOWN:
		ScrollDown();
		return true;
	case K_INSERT:
		OverWriteMode(!IsOverWriteMode());
		break;
	}
	bool sel = key & K_SHIFT;
	switch(key & ~K_SHIFT) {
	case K_CTRL_LEFT:
		{
			PlaceCaret(GetPrevWord(cursor), sel);
			break;
		}
	case K_CTRL_RIGHT:
		{
			PlaceCaret(GetNextWord(cursor), sel);
			break;
		}
	case K_LEFT:
		MoveLeft(sel);
		break;
	case K_RIGHT:
		MoveRight(sel);
		break;
	case K_HOME:
		MoveHome(sel);
		break;
	case K_END:
		MoveEnd(sel);
		break;
	case K_UP:
		MoveUp(sel);
		break;
	case K_DOWN:
		MoveDown(sel);
		break;
	case K_PAGEUP:
		MovePageUp(sel);
		break;
	case K_PAGEDOWN:
		MovePageDown(sel);
		break;
	case K_CTRL_PAGEUP:
	case K_CTRL_HOME:
		MoveTextBegin(sel);
		break;
	case K_CTRL_PAGEDOWN:
	case K_CTRL_END:
		MoveTextEnd(sel);
		break;
	case K_CTRL_C:
	case K_CTRL_INSERT:
		Copy();
		break;
	case K_CTRL_A:
		SelectAll();
		break;
	default:
		if(IsReadOnly())
			return MenuBar::Scan(WhenBar, key);
		switch(key) {
		case K_DELETE:
			DeleteChar();
			break;
		case K_BACKSPACE:
		case K_SHIFT|K_BACKSPACE:
			Backspace();
			break;
	   	case K_SHIFT_TAB:
			AlignChar();
			break;
		case K_CTRL_Y:
		case K_CTRL_L:
			if(cutline) {
				CutLine();
				break;
			}
		default:
			if(InsertChar(key, count, true))
				return true;
			return MenuBar::Scan(WhenBar, key);
		}
		return true;
	}
	Sync();
	return true;
}

void LineEdit::DragAndDrop(Point p, PasteClip& d)
{
	if(IsReadOnly()) return;
	int c = GetMousePos(p);
	if(AcceptText(d)) {
		NextUndo();
		int a = sb.y;
		int sell, selh;
		if(GetSelection(sell, selh)) {
			if(c >= sell && c < selh) {
				RemoveSelection();
				if(IsDragAndDropSource())
					d.SetAction(DND_COPY);
				c = sell;
			}
			else
			if(d.GetAction() == DND_MOVE && IsDragAndDropSource()) {
				if(c > sell)
					c -= selh - sell;
				RemoveSelection();
				d.SetAction(DND_COPY);
			}
		}
		int count = Insert(c, GetWString(d));
		sb.y = a;
		SetFocus();
		SetSelection(c, c + count);
		Action();
		return;
	}
	if(!d.IsAccepted()) return;
	if(!isdrag) {
		isdrag = true;
		ScrollIntoCursor();
	}
	Point dc = Null;
	if(c >= 0)
		dc = GetColumnLine(c);
	if(dc != dropcaret) {
		RefreshDropCaret();
		dropcaret = dc;
		RefreshDropCaret();
	}
}

Rect LineEdit::DropCaret()
{
	if(IsNull(dropcaret))
		return Rect(0, 0, 0, 0);
	Size fsz = GetFontSize();
	Point p = dropcaret - sb;
	p = Point(p.x * fsz.cx, p.y * fsz.cy);
	return RectC(p.x, p.y, 1, fsz.cy);
}

void LineEdit::RefreshDropCaret()
{
	Refresh(DropCaret());
}

void LineEdit::DragRepeat(Point p)
{
	sb.y = (int)sb.y + GetDragScroll(this, p, 1).y;
}

void LineEdit::DragLeave()
{
	RefreshDropCaret();
	dropcaret = Null;
	isdrag = false;
	Layout();
}

void LineEdit::LeftDrag(Point p, dword flags)
{
	int c = GetMousePos(p);
	int l, h;
	if(!HasCapture() && GetSelection(l, h) && c >= l && c < h) {
		WString sample = GetW(l, min(h - l, 3000));
		Size sz = StdSampleSize();
		ImageDraw iw(sz);
		iw.DrawRect(sz, Black());
		iw.Alpha().DrawRect(sz, Black());
		DrawTLText(iw.Alpha(), 0, 0, 9999, sample, Courier(10), White());
		NextUndo();
		if(DoDragAndDrop(ClipFmtsText(), iw) == DND_MOVE) {
			RemoveSelection();
			Action();
		}
	}
}

END_UPP_NAMESPACE
