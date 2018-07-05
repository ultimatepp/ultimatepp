#include "Debuggers.h"

void DbgDisas::MouseWheel(Point, int zdelta, dword)
{
	sb.Wheel(zdelta);
}

Size DbgDisas::GetBox() const
{
	Size sz = GetTextSize("12345678", CourierZ(12));
	if(mode64)
		sz.cx *= 2;
	sz.cy = max(sz.cy, opfont.GetCy());
	return sz;
}

void DbgDisas::Layout()
{
	sb.SetPage(GetSize().cy / GetBox().cy);
}

void DbgDisas::Paint(Draw& w)
{
	Size sz = GetSize();
	Size box = GetBox();
	int i = sb;
	int y = 0;
	Font hexfont = CourierZ(12);
	int maxb = 0;
	for(int i = 0; i < inst.GetCount(); i++)
		maxb = max(inst[i].bytes.GetCount(), maxb);
	int bcx = GetTextSize("00", hexfont).cx + HorzLayoutZoom(2);
	Color lblink = HighlightSetup::GetHlStyle(HighlightSetup::INK_KEYWORD).color;
	Color lblpaper = HighlightSetup::GetHlStyle(HighlightSetup::PAPER_BRACKET).color;
	while(i < inst.GetCount() && y < sz.cy) {
		Inst& n = inst[i];
		Color ink = HasFocus() && i == cursor ? SColorPaper : SColorText;
		int x = 0;
		w.DrawRect(0, y, sz.cx, box.cy, i == cursor ? HasFocus() ? SColorHighlight : SColorFace
		                                            : i & 1 ? Blend(SColorMark, SColorPaper, 220) : SColorPaper());
		int hexy = y + (box.cy - hexfont.GetCy()) / 2;
		if(sz.cx > bcx * 15) {
			bool lbl = taddr.Find(addr[i]) >= 0;
			if(lbl && i != cursor)
				w.DrawRect(0, y, box.cx, box.cy, lblpaper);
			w.DrawText(0, hexy, mode64 ? Sprintf("%16llX", (uint64)addr[i]) : Sprintf("%08X", (uint32)addr[i]),
			           hexfont,
			           HasFocus() && i == cursor ? SColorPaper : lbl ? lblink : SColorText);
			x += box.cx;
		}
		if(i == ip)
			DrawHighlightImage(w, x + 2, y + (box.cy - HorzLayoutZoom(12)) / 2, ipimg);
		x += HorzLayoutZoom(18);
		if(sz.cx - x - (maxb + 1) * bcx > bcx * 12) {
			for(int i = 0; i < min(n.bytes.GetCount(), maxb); i++)
				w.DrawText(x + i * bcx, hexy, Sprintf("%02X", (byte)n.bytes[i]), hexfont, ink);
			x += (maxb + 1) * bcx;
		}
		int opy = y + (box.cy - opfont.GetCy()) / 2;
		w.DrawText(x, opy, n.code, opfont, ink);
		w.DrawText(x + codecx, opy, n.args, opfont, ink);
		y += box.cy;
		i++;
	}
	w.DrawRect(0, y, sz.cx, sz.cy, SColorPaper);
}

void DbgDisas::LeftDown(Point p, dword)
{
	SetFocus();
	cursor = minmax(p.y / GetBox().cy + sb, 0, addr.GetCount() - 1);
	WhenCursor();
	Refresh();
}

bool DbgDisas::Key(dword key, int)
{
	int p = GetSize().cy / GetBox().cy;
	switch(key) {
	case K_UP:
		cursor--;
		break;
	case K_DOWN:
		cursor++;
		break;
	case K_HOME:
		cursor = 0;
		break;
	case K_END:
		cursor = INT_MAX;
		break;
	case K_PAGEUP:
		cursor -= p;
		break;
	case K_PAGEDOWN:
		cursor += p;
		break;
	default:
		return false;
	}
	cursor = minmax(cursor, 0, addr.GetCount() - 1);
	sb.ScrollInto(cursor);
	Refresh();
	WhenCursor();
	return true;
}

void DbgDisas::Clear()
{
	low = 0xffffffff;
	high = 0;
	cursor = -1;
	inst.Clear();
	taddr.Clear();
	addr.Clear();
	opfont = StdFontZ(12);
	codecx = GetTextSize("movlmo", opfont).cx;
	
	Refresh();
}

void  DbgDisas::Add(adr_t adr, const String& code, const String& args, const String& bytes)
{
	if(adr < low)
		low = adr;
	if(adr > high)
		high = adr;
	addr.Add(adr);
	Inst& n = inst.Add();
	n.code = code;
	n.args = args;
	n.bytes = bytes;
	sb.SetTotal(inst.GetCount());
	Refresh();
	int cx = GetTextSize(n.code, opfont).cx;
	if(cx > codecx)
		codecx = cx;
}

void DbgDisas::Scroll()
{
	Refresh();
}

void DbgDisas::SetCursor(adr_t adr)
{
	cursor = addr.Find(adr);
	if(cursor >= 0)
		sb.ScrollInto(cursor);
	Refresh();
}

void DbgDisas::SetIp(adr_t adr, const Image& img)
{
	ip = addr.Find(adr);
	ipimg = img;
	Refresh();
}

void DbgDisas::GotFocus()
{
	WhenFocus();
	Refresh();
}

void DbgDisas::LostFocus()
{
	WhenFocus();
	Refresh();
}

void DbgDisas::WriteClipboard()
{
	String s;
	for(int i = 0; i < addr.GetCount(); i++)
		s << Format("%08X  ", (int)addr[i]) << inst[i].code << " " << inst[i].args << "\n";
	WriteClipboardText(s);
}

DbgDisas::DbgDisas()
{
	SetFrame(InsetFrame());
	AddFrame(sb);
	sb.WhenScroll = THISBACK(Scroll);
	low = high = 0;
	mode64 = false;
	Clear();
}
