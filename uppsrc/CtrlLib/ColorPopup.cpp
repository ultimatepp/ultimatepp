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

ColorPopUp::~ColorPopUp() {}

int ColorPopUp::GetColorCount() const
{
	return scolors ? 216 + 36 + 18 : 216 + 18 + 18;
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
	        (notnull ? 0 : StdFont().Info().GetHeight() + 3 + 2);
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
					if(colori == 999)
						if(GetMouseLeft())
							DrawFrame(w, r, SColorShadow, SColorLight);
						else
							DrawFrame(w, r, GUI_GlobalStyle() >= GUISTYLE_XP ? SColorText : SColorHighlight);
				}
				return;
			}

			DrawFilledFrame(w, x + 1, y, 14, 14, SColorText, GetColor(i));
			if(i < 18 && scolors)
				DrawFrame(w, x + 2, y + 1, 12, 12, Blend(SColorLight, SColorHighlight));

			if(i == colori)
				if(GetMouseLeft())
					DrawFrame(w, x, y - 1, 16, 16, SColorShadow, SColorLight);
				else
					DrawFrame(w, x, y - 1, 16, 16, GUI_GlobalStyle() >= GUISTYLE_XP ? SColorText : SColorHighlight);
			i++;
		}
		y += 16;
	}
}

int ColorPopUp::Get(Point p)
{
	if(p.y >= GetCy())
		return 999;
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

void ColorPopUp::Deactivate() {
	if(IsOpen()) {
		Close();
		IgnoreMouseClick();
		WhenCancel();
	}
}

void ColorPopUp::Finish()
{
	Close();
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
		return Null;
}

void ColorPopUp::PopUp(Ctrl *owner, Color c)
{
	int cy = norampwheel ? 0 : 110;
	Size sz = AddFrameSize(18 * 16, GetCy() + cy);
	Rect wr = GetWorkArea();
	Rect r = owner->GetScreenRect();
	int x = r.left;
	int y = r.bottom;
	if(x + sz.cx >= wr.right)
		x = r.right - sz.cx;
	if(y + sz.cy >= wr.bottom)
		y = r.top - sz.cy;

	Rect rt = RectC(x, y, sz.cx, sz.cy);
	if(GUI_PopUpEffect()) {
		sPaintRedirectCtrl pb;
		pb.ctrl = this;
		Add(pb.BottomPos(0, rt.Height()).LeftPos(0, rt.Width()));
		SetRect(RectC(rt.left, rt.top, 1, 1));
		Ctrl::PopUp(owner, true, true, GUI_GlobalStyle() >= GUISTYLE_XP);
		SetFocus();
		Ctrl::ProcessEvents();
		Animate(*this, rt, GUIEFFECT_SLIDE);
		pb.Remove();
	}
	else {
		SetRect(rt);
		Ctrl::PopUp(owner, true, true, true);
	}

	SetFocus();

	if(!norampwheel) {
		ramp.LeftPos(0, 18*7).VSizePos(GetCy(), 0);
		wheel.LeftPos(18*9 - 1, 18*7).VSizePos(GetCy(), 0);
	}

	ramp <<= c;
	wheel <<= c;
	color = c;
	colori = -1;
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
	scolors = false;
	SetFrame(MenuFrame());
	Add(ramp);
	Add(wheel);
	ramp <<= THISBACK(Ramp);
	wheel <<= THISBACK(Wheel);
	ramp.WhenLeftDouble = wheel.WhenLeftDouble = THISBACK(Select);
	BackPaint();
	nulltext = t_("(transparent)");
}

END_UPP_NAMESPACE
