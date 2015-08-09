#include "HexView.h"

NAMESPACE_UPP

inline int FormatHexDigit(int c) {
	return c < 10 ? c + '0' : c - 10 + 'a';
}

void FormatHex(char *buffer, int64 number, int n) {
	buffer[n] = '\0';
	while(n) {
		buffer[--n] = FormatHexDigit((byte)number & 0x0f);
		number >>= 4;
	}
}

void HexViewInfo::PrintValue(Draw& w, int x, int y, int bytes, bool be)
{
	dword d = 0;
	Size fsz = GetTextSize("X", font);
	for(int i = 0; i < bytes; i++) {
		int b = data[be ? i : bytes - i - 1];
		if(b < 0) {
			w.DrawText(x, y, String('?', 2 * bytes), font, SColorHighlight);
			x += 2 * bytes * fsz.cx;
			w.DrawText(x, y, "=", font);
			x += fsz.cx;
			w.DrawText(x, y, "?", font, Red);
			return;
		}
		d = (d << 8) | (byte) b;
	}
	w.DrawText(x, y, FormatIntHex(d, 2 * bytes), font, SColorHighlight);
	x += 2 * bytes * fsz.cx;
	w.DrawText(x, y, "=", font);
	x += fsz.cx;
	String txt = FormatUnsigned(d);
	w.DrawText(x, y, txt, font, Red);
	x += GetTextSize(txt, font).cx;
	w.DrawText(x, y, "=", font);
	x += fsz.cx;
	int q = d;
	if(bytes == 1)
		q = (int8) d;
	else
	if(bytes == 2)
		q = (int16) d;
	w.DrawText(x, y, FormatInt(q), font, Magenta);
}

void HexViewInfo::Paint(Draw& w)
{
	Size sz = GetSize();
	w.DrawRect(sz, SColorLtFace);
	if(mode < 1)
		return;
	Size fsz = GetTextSize("X", font);
	char h[17];
	FormatHex(h, pos, longmode ? 16 : 8);
	int xx = 0;
	w.DrawText(xx, 0, h, font, SColorHighlight);
	xx += (longmode ? 16 : 8) * fsz.cx;
	w.DrawText(xx, 0, "=", font);
	xx += fsz.cx;
	w.DrawText(xx, 0, Format64(pos), font, Red);
	xx += (longmode ? 22 : 12) * fsz.cx;
	int y = 0;
	int x;
	for(int q = 0; q < mode; q++) {
		x = xx;
		if(q < 1)
			PrintValue(w, x, y, 1, q);
		x += 12 * fsz.cx;
		PrintValue(w, x, y, 2, q);
		x += 18 * fsz.cx;
		PrintValue(w, x, y, 4, q);
		x += 32 * fsz.cx;
		y += fsz.cy;
	}
	wchar wh[40];
	memset(wh, 0, sizeof(wh));
	int i = 0;
	for(i = 0; i < 40; i++) {
		if(data[2 * i] < 0 || data[2 * i + 1] < 0)
			break;
		wh[i] = MAKEWORD(data[2 * i], data[2 * i + 1]);
	}
	w.DrawText(x, 0, wh, font, Cyan, i);
	if(mode < 2)
		return;
	char sh[80];
	memset(sh, 0, sizeof(sh));
	for(i = 0; i < 80; i++) {
		if(data[i] < 0)
			break;
		sh[i] = data[i];
	}
	WString ws = FromUtf8(sh, i);
	w.DrawText(x, fsz.cy, ws, font, Cyan, i);
	String txt;
	String ftxt;
	i = 0;
	for(;;) {
		if(data[i] < 0) {
			if((unsigned)i < sizeof(float))
				ftxt = "?";
			txt = "?";
			break;
		}
		if((unsigned)i >= sizeof(double)) {
			double h;
			memcpy(&h, sh, sizeof(double));
			txt = Sprintf("%.8g", h);
			break;
		}
		if(i == sizeof(float)) {
			float h;
			memcpy(&h, sh, sizeof(float));
			ftxt = Sprintf("%.6g", h);
		}
		sh[i] = data[i];
		i++;
	}
	w.DrawText(0, fsz.cy, txt, font, Red);
	w.DrawText(18 * fsz.cx, fsz.cy, ftxt, font, Red);
}

void HexViewInfo::SetMode(int _mode)
{
	mode = _mode;
	Height(mode * GetTextSize("X", CourierZ(12)).cy + 3);
	Show(mode);
}

HexViewInfo::HexViewInfo()
{
	SetMode(0);
	AddFrame(TopSeparatorFrame());
	AddFrame(RightSeparatorFrame());
	font = CourierZ(12);
}

int HexView::Byte(int64 adr)
{
	return 0;
}

void HexView::Paint(Draw& w)
{
	Size sz = GetSize();
	w.DrawRect(sz, SColorPaper);
	int y = 0;
	int64 adr = sc;
	while(y < sz.cy) {
		char h[17];
		FormatHex(h, adr, IsLongMode() ? 16 : 8);
		w.DrawText(0, y, h, font);
		int x = (IsLongMode() ? 17 : 9) * fsz.cx;
		int tx = x + columns * fcx3;
		for(int q = columns; q--;) {
			if(adr >= total)
				return;
			if(adr == cursor) {
				w.DrawRect(x, y, fsz.cx * 2, fsz.cy, LtCyan);
				w.DrawRect(tx, y, fsz.cx, fsz.cy, LtCyan);
			}
			int b = Byte(adr++);
			if(b < 0) {
				w.DrawText(x, y, "??", font, Brown);
				w.DrawText(tx, y, "?", font, Brown);
			}
			else {
				h[0] = FormatHexDigit((b & 0xf0) >> 4);
				h[1] = FormatHexDigit(b & 0x0f);
				h[2] = '\0';
				w.DrawText(x, y, h, font, SColorText);
				Color color = SColorMark;
				switch(b) {
				case '\a': *h = 'a'; break;
				case '\b': *h = 'b'; break;
				case '\t': *h = 't'; break;
				case '\f': *h = 'f'; break;
				case '\r': *h = 'r'; break;
				case '\n': *h = 'n'; break;
				case '\v': *h = 'v'; break;
				case '\0': *h = '0'; break;
				default:
					if(b >= 32) {
						*h = b;
						color = SColorText;
					}
					else {
						*h = '.';
						color = SColorDisabled;
					}
				}
				h[1] = '\0';
				w.DrawText(tx, y, h, charset, font, color);
			}
			tx += fsz.cx;
			x += fcx3;
		}
		y += fsz.cy;
	}
}

void HexView::MouseWheel(Point, int zdelta, dword)
{
	sb.Wheel(zdelta);
}

void HexView::SetSb()
{
	sbm = 0;
	while((total >> sbm) > (1 << 30))
		sbm++;
	sb.SetTotal(int(total >> sbm) / columns + 2);
	sb.SetPage(int(rows >> sbm));
	sb.Set(int(sc >> sbm) / columns + 1);
}

void HexView::Layout()
{
	Size sz = GetSize();
	columns = fixed ? fixed : max(4, (sz.cx - (IsLongMode() ? 18 : 10) * fsz.cx) / (4 * fsz.cx));
	rows = max(1, sz.cy / fsz.cy);
	bytes = columns * rows;
	SetSb();
}

void HexView::SetTotal(int64 _total)
{
	total = _total;
	Layout();
	SetSb();
}

void HexView::SetSc(int64 address)
{
	sc = minmax(address, (int64)0, total);
	SetSb();
	Refresh();
}

void HexView::Scroll()
{
	int64 q = (int64)(int)sb << sbm;
	if(q == 0)
		sc = 0;
	else
		sc = (q - 1) * columns + sc % columns;
	Refresh();
}

void HexView::SetCursor(int64 _cursor)
{
	cursor = _cursor;
	if(cursor > total)
		cursor = total - 1;
	if(cursor < 0)
		cursor = 0;
	int q = int(sc % columns);
	if(cursor >= sc + bytes)
		sc = cursor - bytes + columns;
	if(cursor < sc) {
		sc = cursor;
	}
	if(sc > q)
		sc = (sc - q) / columns * columns + q;
	if(sc >= total)
		sc = total - 1;
	if(sc < 0)
		sc = 0;
	SetSb();
	Refresh();
	info.SetPos(cursor, IsLongMode());
	for(int i = 0; i < 80; i++)
		info.Set(i, Byte(cursor + i));
}

void HexView::LeftDown(Point p, dword)
{
	int rowi = p.y / fsz.cy;
	int x = (IsLongMode() ? 17 : 9) * fsz.cx;
	int tx = x + columns * fcx3;
	if(p.x >= x && p.x < tx) {
		x = p.x - x;
		int q = x / fcx3;
		if(x - q * fcx3 < 2 * fsz.cx && q < columns) {
			int64 c = sc + rowi * columns + q;
			if(c < total)
				SetCursor(c);
		}
	}
	else
	if(p.x >= tx) {
		int q = (p.x - tx) / fsz.cx;
		if(q >= 0 && q < columns) {
			int64 c = sc + rowi * columns + q;
			if(c < total)
				SetCursor(c);
		}
	}
	SetFocus();
}

bool HexView::Key(dword key, int)
{
	int pg = max(columns, bytes - columns);
	int q = int(sc % columns);
	switch(key) {
	case K_LEFT:
		SetCursor(cursor - 1);
		return true;
	case K_RIGHT:
		SetCursor(cursor + 1);
		return true;
	case K_UP:
		SetCursor(cursor - columns);
		return true;
	case K_DOWN:
		SetCursor(cursor + columns);
		return true;
	case K_PAGEUP:
		SetSc(sc - pg);
		SetCursor(cursor - pg);
		return true;
	case K_PAGEDOWN:
		SetSc(sc + pg);
		SetCursor(cursor + pg);
		return true;
	case K_CTRL_LEFT:
		SetSc(sc - 1);
		break;
	case K_CTRL_RIGHT:
		SetSc(sc + 1);
		break;
	case K_CTRL_UP:
		SetSc(sc - columns);
		break;
	case K_CTRL_DOWN:
		SetSc(sc + columns);
		break;
	case K_HOME:
		SetCursor((cursor - q) / columns * columns + q);
		break;
	case K_END:
		SetCursor((cursor - q) / columns * columns + q + columns - 1);
		break;
	case K_CTRL_HOME:
	case K_CTRL_PAGEUP:
		SetCursor(0);
		break;
	case K_CTRL_END:
	case K_CTRL_PAGEDOWN:
		SetCursor(total - 1);
		break;
	}
	return MenuBar::Scan(WhenBar, key);
}

void HexView::SetColumns(int x)
{
	FixedColumns(x);
}

void HexView::SetCharset(int chr)
{
	Charset(chr);
}

void HexView::StdGoto(const String& s)
{
	CParser p(s);
	int n = 10;
	if(p.Char2('0', 'x') || p.Char('$') || p.Char('#'))
		n = 16;
	if(p.IsNumber(n)) {
		int64 a = p.ReadNumber(n);
		if(a >= 0 && a < total) {
			SetCursor(a);
			SetSc(a);
			return;
		}
	}
	Exclamation("Invalid position!");
}

void HexView::Goto()
{
	if(go.Execute() == IDOK)
		WhenGoto((String)~go.text);
}

void HexView::ColumnsMenu(Bar& bar)
{
	bar.Add("Auto", THISBACK1(SetColumns, 0))
	   .Radio(fixed == 0);
	bar.Add("8", THISBACK1(SetColumns, 8))
	   .Radio(fixed == 8);
	bar.Add("16", THISBACK1(SetColumns, 16))
	   .Radio(fixed == 16);
	bar.Add("32", THISBACK1(SetColumns, 32))
	   .Radio(fixed == 32);
}

void HexView::SetInfo(int m)
{
	info.SetMode(m);
}

void HexView::InfoMenu(Bar& bar)
{
	bar.Add("None", THISBACK1(SetInfo, 0))
	   .Check(info.GetMode() == 0);
	bar.Add("Standard", THISBACK1(SetInfo, 1))
	   .Check(info.GetMode() == 1);
	bar.Add("Extended", THISBACK1(SetInfo, 2))
	   .Check(info.GetMode() == 2);
}

void HexView::CharsetMenu(Bar& bar)
{
	for(int i = 1; i < CharsetCount(); i++)
		bar.Add(CharsetName(i), THISBACK1(SetCharset, i))
		   .Radio(charset == i);
}

void HexView::StdMenu(Bar& bar)
{
	bar.Add("Go to..", THISBACK(Goto))
	   .Key(K_CTRL_G);
	bar.Add("Columns", THISBACK(ColumnsMenu));
	bar.Add("Charset", THISBACK(CharsetMenu));
	bar.Add("Position info", THISBACK(InfoMenu));
}

void HexView::RightDown(Point p, dword w)
{
	LeftDown(p, w);
	MenuBar::Execute(WhenBar);
}

HexView& HexView::SetFont(Font fnt)
{
	font = fnt;
	fsz = GetTextSize("X", font);
	fcx3 = 3 * fsz.cx;
	Layout();
	Refresh();
	SetSb();
	return *this;
}

void  HexView::SerializeSettings(Stream& s)
{
	int version = 0;
	s / version;
	s / fixed;
	s % charset;
	int mode = info.GetMode();
	s / mode;
	info.SetMode(mode);
	go.text.SerializeList(s);
}

HexView::HexView()
{
	SetFont(CourierZ(12));
	BackPaint();
	charset = CHARSET_WIN1252;
	sb <<= THISBACK(Scroll);
	SetFrame(InsetFrame());
	AddFrame(sb);
	cursor = sc = 0;
	total = 0;
	fixed = 0;
	SetSc(0);
	SetCursor(0);
	AddFrame(info);
	info.SetMode(1);
	WhenBar = THISBACK(StdMenu);
	CtrlLayoutOKCancel(go, "Go to");
	WhenGoto = THISBACK(StdGoto);
}

END_UPP_NAMESPACE
