#include "CtrlLib.h"

NAMESPACE_UPP

static struct {
	const char  *name;
	ColorF color;
}
s_colors[] = {
	{ "Black", &Black },
	{ "Red", &Red },
	{ "Green", &Green },
	{ "Brown", &Brown },
	{ "Blue", &Blue },
	{ "Magenta", &Magenta },
	{ "Cyan", &Cyan },
	{ "Gray", &Gray },
	{ "LtGray", &LtGray },
	{ "LtRed", &LtRed },
	{ "LtGreen", &LtGreen },
	{ "LtYellow", &LtYellow },
	{ "LtBlue", &LtBlue },
	{ "LtMagenta", &LtMagenta },
	{ "LtCyan", &LtCyan },
	{ "Yellow", &Yellow },
	{ "WhiteGray", &WhiteGray },
	{ "White", &White },

//deprecated: (TODO)
	{ "SBlack", &Black },
	{ "SRed", &Red },
	{ "SGreen", &Green },
	{ "SBrown", &Brown },
	{ "SBlue", &Blue },
	{ "SMagenta", &Magenta },
	{ "SCyan", &Cyan },
	{ "SGray", &Gray },
	{ "SLtGray", &LtGray },
	{ "SLtRed", &LtRed },
	{ "SLtGreen", &LtGreen },
	{ "SLtYellow", &LtYellow },
	{ "SLtBlue", &LtBlue },
	{ "SLtMagenta", &LtMagenta },
	{ "SLtCyan", &LtCyan },
	{ "SYellow", &Yellow },
	{ "SWhiteGray", &WhiteGray },
	{ "SWhite", &White },
};

Color ColorPopUp::hint[18];

void ColorPopUp_InitHint()
{
	for(int i = 0; i < 18; i++)
		ColorPopUp::hint[i] = LtGray;
}

INITBLOCK {
	ColorPopUp_InitHint();
}

void ColorPopUp::Hint(Color c)
{
	if(IsNull(c) || c == VoidColor)
		return;
	for(int i = 0; i < 17; i++)
		if(hint[i] == c) {
			memmove(&hint[i], &hint[i + 1], (17 - i) * sizeof(Color));
			hint[17] = LtGray;
		}
	memmove(&hint[1], &hint[0], 17 * sizeof(Color));
	hint[0] = c;
}

String FormatColor(Color c)
{
	if(IsNull(c))
		return "Null";
	for(int i = 0; i < __countof(s_colors); i++)
		if((*s_colors[i].color)() == c)
			return s_colors[i].name;
	return Format("Color(%d, %d, %d)", c.GetR(), c.GetG(), c.GetB());
}

Color ReadColor(CParser& p)
{
	for(int i = 0; i < __countof(s_colors); i++)
		if(p.Id(s_colors[i].name))
			return (*s_colors[i].color)();
	p.PassId("Color");
	p.PassChar('(');
	int r = p.ReadInt();
	p.PassChar(',');
	int g = p.ReadInt();
	p.PassChar(',');
	int b = p.ReadInt();
	p.PassChar(')');
	return Color(minmax(r, 0, 255), minmax(g, 0, 255), minmax(b, 0, 255));
}


static int sCharFilterNoDigit(int c)
{
	return IsDigit(c) ? 0 : c;
}

static int sCharFilterHex(int c)
{
	return c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F' || IsDigit(c) ? c : 0;
}

Color ColorFromText(const char *s)
{
	Vector<String> h = Split(s, sCharFilterNoDigit);
	if(h.GetCount() == 3 && (strchr(s, ',') || strchr(s, ';') || strchr(s, '.'))) {
		int r = atoi(h[0]);
		int g = atoi(h[1]);
		int b = atoi(h[2]);
		if(r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255)
			return Color(r, g, b);
	}
	String hex = Filter(s, sCharFilterHex);
	if(hex.GetCount() == 6 || hex.GetCount() == 8) {
		dword w = (dword)ScanInt64(~hex, NULL, 16);
		return Color(byte(w >> 16), byte(w >> 8), byte(w));
	}
	return Null;
}

ColorPopUp::~ColorPopUp() {}

int ColorPopUp::GetColorCount() const
{
	return 18 + scolors * 18 + 2 * 18 + hints * 18 + 216;
}

Color ColorPopUp::GetColor(int i) const
{
	if(!scolors)
		i += 18;
	if(i < 36)
		return *s_colors[i].color;
	i -= 36;
	if(i < 18)
		return GrayColor(255 * (i + 1) / 20);
	if(hints) {
		i -= 18;
		if(i < 18)
			return hint[i];
	}
	i -= 18;
	int q = i % 18;
	i /= 18;
	return Color(255 * (q < 9 ? q + 1 : 18 - q) / 9,
                 255 * (i < 6 ? i + 1 : 12 - i) / 6,
	             q < 9 ? i < 6 ? 0 : 200 : i < 6 ? 150 : 255);
}

int ColorPopUp::GetCy()
{
	return ((GetColorCount() + 17) / 18) * 16 +
			(norampwheel ? 0 : 2) +
	        (notnull ? 0 : StdFont().Info().GetHeight() + 3 + 2) +
	        (withvoid ? StdFont().Info().GetHeight() + 3 + 2 : 0);
}

void ColorPopUp::DrawFilledFrame(Draw &w, int x, int y, int cx, int cy, Color fcol, Color bcol)
{
	DrawFrame(w, x, y, cx, cy, fcol);
	w.DrawRect(x + 1, y + 1, cx - 2, cy - 2, bcol);
}

void ColorPopUp::DrawFilledFrame(Draw &w, Rect &r, Color fcol, Color bcol)
{
	DrawFrame(w, r.left, r.top, r.Width(), r.Height(), fcol);
	w.DrawRect(r.left + 1, r.top + 1, r.Width() - 2, r.Height() - 2, bcol);
}

void ColorPopUp::Paint(Draw& w)
{
	Size sz = GetSize();
	int cy = GetCy();

	w.DrawRect(sz, SColorMenu);

	int y = 1;

	if(withvoid) {
		Size fsz = GetTextSize(nulltext, StdFont());
		Rect r(1, y, sz.cx - 1, fsz.cy + y + 2);
		DrawFrame(w, r, SColorText);
		w.DrawText((sz.cx - fsz.cx) / 2, y, voidtext, StdFont(), SColorText());
		y = r.bottom + 3;
		if(colori == 997)
		{
			r.Inflate(1);
			if(GetMouseLeft())
				DrawFrame(w, r, SColorShadow, SColorLight);
			else
				DrawFrame(w, r, GUI_GlobalStyle() >= GUISTYLE_XP ? SColorText : SColorHighlight);
		}
	}

	if(!notnull) {
		Size fsz = GetTextSize(nulltext, StdFont());
		Rect r(1, y, sz.cx - 1, fsz.cy + y + 2);
		DrawFrame(w, r, SColorText);
		w.DrawText((sz.cx - fsz.cx) / 2, y, nulltext, StdFont(), SColorText());
		y = r.bottom + 3;
		if(colori == 998)
		{
			r.Inflate(1);
			if(GetMouseLeft())
				DrawFrame(w, r, SColorShadow, SColorLight);
			else
				DrawFrame(w, r, GUI_GlobalStyle() >= GUISTYLE_XP ? SColorText : SColorHighlight);
		}
	}

	int i = 0;
	for(;;) {
		for(int x = 0; x < 18 * 16; x += 16) {
			if(i >= GetColorCount()) {
				if(!norampwheel) {
					Rect r(8 * 16 + 1, cy + 4, 10 * 16 - 1, sz.cy - 4);
					DrawFilledFrame(w, r, SColorText, color);

					r.Inflate(1);
					if(colori == 999) {
						if(GetMouseLeft())
							DrawFrame(w, r, SColorShadow, SColorLight);
						else
							DrawFrame(w, r, GUI_GlobalStyle() >= GUISTYLE_XP ? SColorText : SColorHighlight);
					}
				}
				return;
			}

			DrawFilledFrame(w, x + 1, y, 14, 14, SColorText, GetColor(i));
			if(i < 18 && scolors)
				DrawFrame(w, x + 2, y + 1, 12, 12, Blend(SColorLight, SColorHighlight));

			if(i == colori) {
				if(GetMouseLeft())
					DrawFrame(w, x, y - 1, 16, 16, SColorShadow, SColorLight);
				else
					DrawFrame(w, x, y - 1, 16, 16, GUI_GlobalStyle() >= GUISTYLE_XP ? SColorText : SColorHighlight);
			}
			i++;
		}
		y += 16;
	}
}

int ColorPopUp::Get(Point p)
{
	if(p.y >= GetCy())
		return 999;
	if(withvoid) {
		int y0 = StdFont().Info().GetHeight() + 4;
		if(p.y < y0)
			return 997;
		p.y -= y0;
	}
	if(!notnull) {
		int y0 = StdFont().Info().GetHeight() + 4;
		if(p.y < y0)
			return 998;
		p.y -= y0;
	}
	Size sz = GetSize();
	if(p.x >= 0 && p.x < sz.cx && p.y >= 0)
		return p.x / 16 + p.y / 16 * 18;
	return -1;
}

void ColorPopUp::MouseMove(Point p, dword)
{
	int ci = Get(p);
	if(ci != colori) {
		colori = ci;
		Refresh();
		WhenAction();
	}
}

void ColorPopUp::MouseLeave()
{
	colori = -1;
	Refresh();
}

void ColorPopUp::Finish()
{
	popup.Clear();
	if(colori >= 0)
		WhenSelect();
	else
		WhenCancel();
}

void ColorPopUp::LeftDown(Point p, dword)
{
	Refresh();
}

void ColorPopUp::LeftUp(Point p, dword)
{
	Finish();
}

bool ColorPopUp::Key(dword key, int count)
{
	if(key == K_ESCAPE)	{
		Close();
		WhenCancel();
	}
	return true;
}

void ColorPopUp::Ramp()
{
	color = wheel <<= ~ramp;
	WhenAction();
	Refresh();
}

void ColorPopUp::Wheel()
{
	color = ramp <<= ~wheel;
	WhenAction();
	Refresh();
}

Color ColorPopUp::Get() const
{
	if(colori >= 0 && colori < GetColorCount())
		return GetColor(colori);
	else
	if(colori == 999)
		return color;
	else
	if(colori == 997)
		return VoidColor();
	else
		return Null;
}

void ColorPopUp::PopupDeactivate() {
	if(popup && popup->IsOpen() && !animating && open) {
		popup.Clear();
		IgnoreMouseClick();
		WhenCancel();
	}
}

void ColorPopUp::PopUp(Ctrl *owner, Color c)
{
	int cy = norampwheel ? 0 : 110;
	Size sz = AddFrameSize(18 * 16, GetCy() + cy);
	Rect wr = GetWorkArea();
	Rect r = owner->GetScreenRect();
	int x = r.left;
	int y = r.bottom;
	BottomPos(0, sz.cy).RightPos(0, sz.cx);
	Point start(x, y);
	if(x + sz.cx >= wr.right) {
		x = r.right - sz.cx;
		start.x = r.right;
		LeftPos(0, sz.cx);
	}
	if(y + sz.cy >= wr.bottom) {
		y = r.top - sz.cy;
		start.y = r.top;
		TopPos(0, sz.cy);
	}

	Rect rt = RectC(x, y, sz.cx, sz.cy);

	open = false;
	popup.Create();
	popup->color = this;
	popup->Add(*this);
	popup->SetRect(RectC(start.x, start.y, 3, 3));

	if(!norampwheel) {
		ramp.LeftPos(0, 18*7).VSizePos(GetCy(), 0);
		wheel.LeftPos(18*9 - 1, 18*7).VSizePos(GetCy(), 0);
	}
	ramp <<= c;
	wheel <<= c;
	color = c;
	colori = -1;

	if(GUI_PopUpEffect()) {
		animating = true;
		popup->PopUp(owner, true, true, GUI_GlobalStyle() >= GUISTYLE_XP);
		SetFocus();
		Ctrl::ProcessEvents();
		Animate(*popup, rt, GUIEFFECT_SLIDE);
		animating = false;
	}

	popup->SetRect(rt);
	if(!popup->IsOpen())
		popup->PopUp(owner, true, true, true);
	SetFocus();
	open = true;
}

void ColorPopUp::Select()
{
	colori = 999;
	Finish();
}

ColorPopUp::ColorPopUp()
{
	norampwheel = false;
	notnull = false;
	withvoid = false;
	scolors = false;
	animating = false;
	hints = false;
	open = false;
	SetFrame(MenuFrame());
	Add(ramp);
	Add(wheel);
	ramp <<= THISBACK(Ramp);
	wheel <<= THISBACK(Wheel);
	ramp.WhenLeftDouble = wheel.WhenLeftDouble = THISBACK(Select);
	BackPaint();
	nulltext = t_("(transparent)");
	voidtext = t_("(none)");
}

END_UPP_NAMESPACE
