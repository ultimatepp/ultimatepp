#include "HexView.h"

namespace Upp {

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
	if(mode < 1 || empty)
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
	int x = 0;
	for(int q = 0; q < min(mode, 2); q++) {
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
	char sh[200];
	memset(sh, 0, sizeof(sh));
	int i;
	for(i = 0; i < 200; i++) {
		if(data[i] < 0)
			break;
		sh[i] = data[i];
	}
	if(mode == 3)
		w.DrawText(x, 0, ToUtf32(sh, i), font, Cyan, i);
	if(mode < 2)
		return;
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
	Height(min(mode, 2) * GetTextSize("X", CourierZ(12)).cy + 3);
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
	if(!total) {
		w.DrawText(Zx(10), Zx(10), "No data", ArialZ(20).Italic(), SRed());
		return;
	}
	int y = 0;
	uint64 adr = sc;
	Buffer<char> hex(2 * columns), text(columns), mark(columns);
	Buffer<int>  hex_dx(2 * columns), text_dx(columns, fsz.cx);
	while(y < sz.cy && adr < total) {
		char h[17];
		FormatHex(h, adr + start, IsLongMode() ? 16 : 8);
		w.DrawText(0, y, h, font);
		int x = (IsLongMode() ? 17 : 9) * fsz.cx;
		int tx = x + columns * fcx3;
		int hexi = 0;
		int texti = 0;
		uint64 adr0 = adr;
		for(int q = columns; q-- && adr < total;) {
			if(adr == cursor) {
				w.DrawRect(x + fsz.cx * 3 * int(adr - adr0), y, fsz.cx * 2, fsz.cy, LtCyan);
				w.DrawRect(tx + fsz.cx * int(adr - adr0), y, fsz.cx, fsz.cy, LtCyan);
			}
			int b = Byte(adr++);
			hex[hexi] = b < 0 ? '?' : FormatHexDigit((b & 0xf0) >> 4);
			hex_dx[hexi++] = fsz.cx;
			hex[hexi] = b < 0 ? '?' : FormatHexDigit(b & 0x0f);
			hex_dx[hexi++] = 2 * fsz.cx;
			text[texti] = b >= 32 && b <= 128 ? b : ' ';
			mark[texti++] = b > 128 ? '.'
			                        : b < 32 ? decode(b, '\a', 'a', '\b', 'b', '\t', 't', '\f', 'f',
			                                          '\r', 'r', '\n', 'n', '\v', 'v', '\0', '0',
			                                          '\1', '1', '.') : ' ';
		}
		w.DrawText(x, y, hex, font, SColorText(), hexi, hex_dx);
		w.DrawText(tx, y, text, font, SColorText(), texti, text_dx);
		w.DrawText(tx, y, mark, font, SColorMark(), texti, text_dx);
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
	uint64 sz = total;
	while((sz >> sbm) > (1 << 30))
		sbm++;
	sb.SetTotal(int(sz >> sbm) / columns + 2);
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

void HexView::SetStart(uint64 start_)
{
	start = start_;
	Layout();
	SetSb();
	Refresh();
	RefreshInfo();
}

void HexView::SetTotal(uint64 _total)
{
	total = _total;
	Layout();
	SetSb();
	Refresh();
	RefreshInfo();
}

void HexView::SetSc(uint64 address)
{
	sc = minmax(address, start, total);
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

void HexView::RefreshInfo()
{
	if(total && (int64)cursor >= 0) {
		info.SetPos(cursor + start, IsLongMode());
		for(int i = 0; i < 80; i++)
			info.Set(i, Byte(cursor + i));
	}
	else
		info.SetEmpty();
}

void HexView::SetCursor(uint64 _cursor)
{
	cursor = _cursor;
	
	if(cursor > INT64_MAX - INT_MAX)
		cursor = 0;
	if(cursor >= total)
		cursor = total - 1;
	int q = int(sc % columns);
	if(cursor >= sc + bytes)
		sc = cursor - bytes + columns;
	if(cursor < sc) {
		sc = cursor;
	}
	if(sc > (uint64)q)
		sc = (sc - q) / columns * columns + q;
	if(sc >= total)
		sc = total - 1;
	SetSb();
	Refresh();
	RefreshInfo();
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
			uint64 c = sc + rowi * columns + q;
			if(c < total)
				SetCursor(c);
		}
	}
	else
	if(p.x >= tx) {
		int q = (p.x - tx) / fsz.cx;
		if(q >= 0 && q < columns) {
			uint64 c = sc + rowi * columns + q;
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
		uint64 a = p.ReadNumber(n);
		if(a < total) {
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
	   .Radio(info.GetMode() == 0);
	bar.Add("Standard", THISBACK1(SetInfo, 1))
	   .Radio(info.GetMode() == 1);
	bar.Add("Extended", THISBACK1(SetInfo, 2))
	   .Radio(info.GetMode() == 2);
	bar.Add("Full", THISBACK1(SetInfo, 3))
	   .Radio(info.GetMode() == 3);
}

void HexView::CharsetMenu(Bar& bar)
{
	for(int i = 1; i < CharsetCount(); i++)
		bar.Add(CharsetName(i), THISBACK1(SetCharset, i))
		   .Radio(charset == i);
}

void HexView::StdMenu(Bar& bar)
{
	bar.Add("Go to..", [=] { WhenGotoDlg(); })
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
	WhenGotoDlg = THISBACK(Goto);
}

}
