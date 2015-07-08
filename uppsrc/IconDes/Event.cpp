#include "IconDes.h"

NAMESPACE_UPP

void IconDes::LeftDown(Point p, dword flags)
{
	SetFocus();
	if(!IsCurrent())
		return;
	SaveUndo();
	startpoint = GetPos(p);
	if(IsPasting()) {
		if(Rect(Current().pastepos, Current().paste_image.GetSize()).Contains(startpoint)) {
			startpoint -= Current().pastepos;
			SetCapture();
		}
		else
			FinishPaste();
		return;
	}
	SetCapture();
	Current().base_image = CurrentImage();
	if(flags & K_SHIFT) {
		ImageBuffer ib(CurrentImage());
		if(!doselection) {
			RGBA c = CurrentColor();
			c.r += 127;
			MaskFill(ib, c, 0);
		}
		FloodFill(ib, CurrentColor(), startpoint, ib.GetSize());
		SetCurrentImage(ib);
		if(!doselection)
			MaskSelection();
		return;
	}
	if(selectrect)
		EmptyRectTool(startpoint, flags);
	else
	if(tool)
		(this->*tool)(startpoint, flags);
}

void IconDes::MouseMove(Point p, dword keyflags)
{
	if(!HasCapture() || !IsCurrent())
		return;
	p = GetPos(p);
	if(IsPasting()) {
		Current().pastepos = p - startpoint;
		MakePaste();
		return;
	}
	if(selectrect)
		EmptyRectTool(p, keyflags);
	else
	if(tool)
		(this->*tool)(p, keyflags);
}

void IconDes::LeftUp(Point p, dword keyflags)
{
	if(!IsCurrent())
		return;
	if(IsPasting() && HasCapture())
		Refresh();
	else
	if(HasCapture() && selectrect)
		Move();
	else
		Current().base_image.Clear();
	SetBar();
	SyncShow();
}

void IconDes::RightDown(Point p, dword flags)
{
	p = GetPos(p);
	if(!InImage(p))
		return;
	if(tool == &IconDes::HotSpotTool) {
		if(p != Current().image.Get2ndSpot()) {
			ImageBuffer ib(Current().image);
			ib.Set2ndSpot(p);
			Current().image = ib;
			Refresh();
		}
		return;
	}
	RGBA ic = CurrentImage()[p.y][p.x];
	RGBA c = CurrentColor();
	if(flags & K_ALT) {
		c.a = ic.a;
		ic = c;
	}
	if(flags & K_CTRL)
		ic.a = c.a;
	rgbactrl.Set(ic);
	ColorChanged();
}

Image IconDes::CursorImage(Point p, dword flags)
{
	if(IsHotSpot())
		return Image::Arrow();
	if(IsPasting())
		return HasCapture() ? IconDesImg::MoveMove()
		       : Rect(Current().pastepos, Current().paste_image.GetSize()).Contains(GetPos(p)) ? IconDesImg::MoveCursor()
		       : IconDesImg::MoveOk();
	return flags & K_SHIFT ? fill_cursor : cursor_image;
}

END_UPP_NAMESPACE
