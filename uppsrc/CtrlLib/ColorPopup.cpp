#include "CtrlLib.h"

namespace Upp {

static struct {
	const char  *name;
	Color        color;
}
s_colors[] = {
	{ "SBlack", Color::Special(0) },
	{ "SRed", Color::Special(1) },
	{ "SGreen", Color::Special(2) },
	{ "SBrown", Color::Special(3) },
	{ "SBlue", Color::Special(4) },
	{ "SMagenta", Color::Special(5) },
	{ "SCyan", Color::Special(6) },
	{ "SGray", Color::Special(7) },
	{ "SLtGray", Color::Special(8) },
	{ "SLtRed", Color::Special(9) },
	{ "SLtGreen", Color::Special(10) },
	{ "SLtYellow", Color::Special(11) },
	{ "SLtBlue", Color::Special(12) },
	{ "SLtMagenta", Color::Special(13) },
	{ "SLtCyan", Color::Special(14) },
	{ "SYellow", Color::Special(15) },
	{ "SWhiteGray", Color::Special(16) },
	{ "SWhite", Color::Special(17) },

	{ "Black", Black },
	{ "Red", Red },
	{ "Green", Green },
	{ "Brown", Brown },
	{ "Blue", Blue },
	{ "Magenta", Magenta },
	{ "Cyan", Cyan },
	{ "Gray", Gray },
	{ "LtGray", LtGray },
	{ "LtRed", LtRed },
	{ "LtGreen", LtGreen },
	{ "LtYellow", LtYellow },
	{ "LtBlue", LtBlue },
	{ "LtMagenta", LtMagenta },
	{ "LtCyan", LtCyan },
	{ "Yellow", Yellow },
	{ "WhiteGray", WhiteGray },
	{ "White", White },
};

Color ColorPopUp::hint[18];

ColorPopUp& ColorPopUp::DarkContent(bool b)
{
	wheel.DarkContent(b);
	ramp.DarkContent(b);
	Refresh();
	return *this;
}

ColorPopUp& ColorPopUp::AllowDarkContent(bool b)
{
	wheel.AllowDarkContent(b);
	ramp.AllowDarkContent(b);
	Refresh();
	return *this;
}

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

Color RealizeColor(Color c)
{
	int i = c.GetSpecial();
	return i >= 0 && i < 18 ? s_colors[i + 18].color : c;
}

String FormatColor(Color c)
{
	if(IsNull(c))
		return "Null";
	for(int i = 0; i < __countof(s_colors); i++)
		if(s_colors[i].color == c)
			return s_colors[i].name;
	return Format("Color(%d, %d, %d)", c.GetR(), c.GetG(), c.GetB());
}

Color ReadColor(CParser& p)
{
	if(p.Id("Null"))
		return Null;
	for(int i = 0; i < __countof(s_colors); i++)
		if(p.Id(s_colors[i].name))
			return s_colors[i].color;
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

int ColorPopUp::GetColorCount() const
{
	return 18 + scolors * 18 + 2 * 18 + hints * 18 + 216;
}

Color ColorPopUp::GetColor(int i) const
{
	if(!scolors)
		i += 18;
	if(i < 36)
		return s_colors[i].color;
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
	return ((GetColorCount() + 17) / 18) * DPI(16) +
			(norampwheel ? 0 : DPI(2)) +
	        (notnull ? 0 : StdFont().Info().GetHeight() + DPI(3 + 2)) +
	        (withvoid ? StdFont().Info().GetHeight() + DPI(3 + 2) : 0);
}

void ColorPopUp::DrawFilledFrame(Draw &w, int x, int y, int cx, int cy, Color fcol, Color bcol)
{
	DrawFrame(w, x, y, cx, cy, fcol);
	w.DrawRect(x + DPI(1), y + DPI(1), cx - DPI(2), cy - DPI(2), bcol);
}

void ColorPopUp::DrawFilledFrame(Draw &w, Rect &r, Color fcol, Color bcol)
{
	DrawFilledFrame(w, r.left, r.top, r.GetWidth(), r.GetHeight(), fcol, bcol);
}

void ColorPopUp::Paint(Draw& w)
{
	Size sz = GetSize();
	int cy = GetCy();

	w.DrawRect(sz, SColorFace());

	int y = DPI(1);

	if(withvoid) {
		Size fsz = GetTextSize(nulltext, StdFont());
		Rect r(1, y, sz.cx - DPI(1), fsz.cy + y + DPI(2));
		DrawFrame(w, r, SColorText);
		w.DrawText((sz.cx - fsz.cx) / 2, y, voidtext, StdFont(), SColorText());
		y = r.bottom + DPI(3);
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
		Rect r(1, y, sz.cx - DPI(1), fsz.cy + y + DPI(2));
		DrawFrame(w, r, SColorText);
		w.DrawText((sz.cx - fsz.cx) / 2, y, nulltext, StdFont(), SColorText());
		y = r.bottom + DPI(3);
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
	bool dark = IsDarkContent();
	for(;;) {
		for(int x = 0; x < 18 * DPI(16); x += DPI(16)) {
			if(i >= GetColorCount()) {
				if(!norampwheel) {
					Rect r(DPI(8 * 16 + 1), cy + DPI(4), DPI(10 * 16 - 1), sz.cy - DPI(4) - DPI(24));
					DrawFilledFrame(w, r, SColorText, dark ? DarkThemeCached(color) : color);

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

			Color c = RealizeColor(GetColor(i));
			DrawFilledFrame(w, x + DPI(1), y, DPI(14), DPI(14), SColorText, dark ? DarkThemeCached(c) : c);
			if(i < 18 && scolors)
				w.DrawRect(x + DPI(2) + DPI(6), y + DPI(1), DPI(6), DPI(12), dark ? c : DarkThemeCached(c));

			if(i == colori) {
				if(GetMouseLeft())
					DrawFrame(w, x, y - DPI(1), DPI(16), DPI(16), SColorShadow, SColorLight);
				else
					DrawFrame(w, x, y - DPI(1), DPI(16), DPI(16), GUI_GlobalStyle() >= GUISTYLE_XP ? SColorText : SColorHighlight);
			}
			i++;
		}
		y += DPI(16);
	}
}

int ColorPopUp::Get(Point p)
{
	if(p.y >= GetCy())
		return 999;
	if(withvoid) {
		int y0 = StdFont().Info().GetHeight() + DPI(4);
		if(p.y < y0)
			return 997;
		p.y -= y0;
	}
	if(!notnull) {
		int y0 = StdFont().Info().GetHeight() + DPI(4);
		if(p.y < y0)
			return 998;
		p.y -= y0;
	}
	Size sz = GetSize();
	if(p.x >= 0 && p.x < sz.cx && p.y >= 0)
		return p.x / DPI(16) + p.y / DPI(16) * 18;
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
	int cy = norampwheel ? 0 : DPI(110);
	Size sz = AddFrameSize(18 * DPI(16), GetCy() + cy);
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
		ramp.LeftPos(0, DPI(18*7)).VSizePos(GetCy(), 0);
		wheel.LeftPos(DPI(18*9 - 1), DPI(18*7)).VSizePos(GetCy(), 0);
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

void ColorPopUp::Layout()
{
	if(norampwheel)
		settext.Hide();
	else
		settext.LeftPos(DPI(8 * 16), DPI(2 * 16)).BottomPos(DPI(2), DPI(24));
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
	
	settext.SetImage(CtrlImg::color_edit());
	settext << [=] {
		String text;
		if(!IsNull(color) && color != VoidColor())
			text = ColorToHtml(color);
		EditText(text, "Set Color", "Color value");
		Color c = ColorFromText(text);
		if(IsNull(c))
			return;
		color = c;
		ramp <<= wheel <<= c;
		colori = 999;
		WhenAction();
		Refresh();
		Finish();
	};
	
	Add(settext);
}

}
