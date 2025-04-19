#include "CtrlLib.h"

namespace Upp {

void ColorPusher::Paint(Draw& w)
{
	Size sz = GetSize();
	w.DrawRect(sz, push ? SColorHighlight : SColorPaper);
	int ty = (sz.cy - StdFont().Info().GetHeight()) / 2;
	if(IsNull(color))
		w.DrawText(max(2, (sz.cx - GetTextSize(nulltext, StdFont()).cx) / 2), ty,
		           nulltext, StdFont(), SColorText());
	else
	if(color == VoidColor)
		w.DrawText(max(2, (sz.cx - GetTextSize(nulltext, StdFont()).cx) / 2), ty,
		           voidtext, StdFont(), SColorText());
	else {
		auto DrawColor = [&](int x, int y, int cx, int cy) {
			if(color.GetSpecial() >= 0 && color.GetSpecial() < 18) {
				Color c = RealizeColor(color);
				w.DrawRect(x, y, cx / 2, cy, c);
				w.DrawRect(x + cx / 2, y, cx - cx / 2, cy, DarkTheme(c));
			}
			else
				w.DrawRect(x, y, cx, cy, colors.IsDarkContent() ? DarkTheme(color) : color);
		};
		if(withtext || withhex) {
			DrawColor(2, 2, sz.cy - 4, sz.cy - 4);
			DrawFrame(w, 1, 1, sz.cy - 2, sz.cy - 2, SColorText);
			w.DrawText(sz.cy + 2, ty, withhex ? ColorToHtml(color) : FormatColor(color), StdFont(), SColorText());
		}
		else {
			DrawColor(2, 2, sz.cx - 4, sz.cy - 4);
			DrawFrame(w, 1, 1, sz.cx - 2, sz.cy - 2, SColorText);
		}
	}
	if(HasFocus())
		DrawFocus(w, GetSize());
}

void ColorPusher::LeftDown(Point p, dword)
{
	Drop();
}

bool ColorPusher::Key(dword key, int)
{
	if(key == K_SPACE) {
		Drop();
		return true;
	}
	return false;
}

void ColorPusher::Drop()
{
	if(push) return;
	push = true;
	Refresh();
	saved_color = color;
	Color c = ColorFromText(ReadClipboardText());
	if(!IsNull(c))
		ColorPopUp::Hint(c);
	colors.PopUp(this, color);
}

void ColorPusher::CloseColors()
{
	push = false;
	if(color != saved_color) {
		color = saved_color;
		UpdateAction();
	}
	Refresh();
}

void ColorPusher::AcceptColors()
{
	push = false;
	color = colors.Get();
	UpdateActionRefresh();
}

void ColorPusher::SetData(const Value& v)
{
	Color c = v;
	if(color != c) {
		color = c;
		Refresh();
	}
	ColorPopUp::Hint(c);
}

Value ColorPusher::GetData() const
{
	return color;
}

void ColorPusher::NewColor()
{
	if(track) {
		color = colors.Get();
		UpdateAction();
	}
}

ColorPusher::ColorPusher()
{
	nulltext = t_("(transparent)");
	voidtext = t_("(none)");
	color = Null;
	track = push = withtext = withhex = false;
	colors.WhenSelect = THISBACK(AcceptColors);
	colors.WhenCancel = THISBACK(CloseColors);
	colors.WhenAction = THISBACK(NewColor);
	colors.Hints();
	SetFrame(EditFieldFrame());
}

Size ColorButton::GetMinSize() const
{
	return DPI(Size(24, 24));
}

void ColorButton::Paint(Draw& w)
{
	Size sz = GetSize();
	Size isz = (IsNull(image) ? staticimage : image).GetSize();
	Point center = (sz - isz) / 2;
	if(GUI_GlobalStyle() >= GUISTYLE_XP)
		ChPaint(w, sz, style->look[!IsEnabled() ? CTRL_DISABLED : push ? CTRL_PRESSED
		                                        : HasMouse() ? CTRL_HOT : CTRL_NORMAL]);
	else {
		w.DrawRect(sz, SColorFace);
		if(push)
			DrawFrame(w, sz, SColorShadow, SColorLight);
		else
		if(HasMouse())
			DrawFrame(w, sz, SColorLight, SColorShadow);
	}
	if(IsEnabled()) {
		if(IsNull(color))
			w.DrawImage(center.x + push, center.y + push, nullimage);
		else
			w.DrawImage(center.x + push, center.y + push, image, colors.IsDarkContent() ? DarkTheme(color) : color);
		w.DrawImage(center.x + push, center.y + push, staticimage);
	}
	else
		w.DrawImage(center.x + push, center.y + push, staticimage, SColorDisabled());
}

void  ColorButton::MouseEnter(Point p, dword keyflags)
{
	Refresh();
}

void  ColorButton::MouseLeave()
{
	Refresh();
}

ColorButton::ColorButton()
{
	style = &ToolBar::StyleDefault().buttonstyle;
	SetFrame(NullFrame());
	NoTrack();
	Transparent();
}

}
