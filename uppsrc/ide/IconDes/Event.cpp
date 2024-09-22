#include "IconDes.h"

void IconDes::LeftDown(Point p, dword flags)
{
	SetFocus();
	if(!IsCurrent())
		return;
	SaveUndo();
	startpoint = GetPos(p);
	startcolor = InImage(startpoint) ? CurrentImage()[startpoint.y][startpoint.x] : RGBAZero();

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
	if(flags & K_CTRL) {
		scroll_start = startpoint;
		scroll_base = sb;
		return;
	}
	if(flags & K_SHIFT) {
		if(Rect(CurrentImage().GetSize()).Contains(startpoint))
			DoFill(decode(fill_type, 0, 0, 1, 20, 2, 40, -1));
		return;
	}
	if(flags & K_ALT)
		Freehand(startpoint, 1);
	else
	if(selectrect)
		EmptyRectTool(startpoint, flags);
	else
	if(tool)
		(this->*tool)(startpoint, flags);
}

void IconDes::MouseMove(Point p, dword flags)
{
	SyncStatus();
	if(!HasCapture() || !IsCurrent())
		return;
	if(!IsNull(scroll_start)) {
		p = p / max(magnify, 1) + scroll_base;
		sb = scroll_base + scroll_start - p;
		return;
	}
	p = GetPos(p);
	if(IsPasting()) {
		Current().pastepos = p - startpoint;
		MakePaste();
		return;
	}
	if(flags & K_SHIFT)
		return;
	if(flags & K_ALT)
		Freehand(p, 1);
	else
	if(selectrect)
		EmptyRectTool(p, flags);
	else
	if(tool)
		(this->*tool)(p, flags);
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
	scroll_start = Null;
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
	if(!IsNull(scroll_start))
		return IconDesImg::MoveMove();
	if(flags & K_CTRL)
		return IconDesImg::MoveCursor();
	if(flags & K_SHIFT)
		return decode(fill_type, 0, fill_cursor, 1, fill_cursor2, 2, fill_cursor3, antifill_cursor);
	if(flags & K_ALT)
		return cursor_image_free;
	return cursor_image;
}

void IconDes::MouseWheel(Point pt, int zdelta, dword keyflags)
{
	if(keyflags & K_CTRL) {
		Point p = GetPos(pt);
		if(zdelta < 0)
			ZoomOut();
		else
			ZoomIn();
		sb = sb + p - GetPos(pt);
	}
	else
	if(keyflags & K_SHIFT)
		sb.WheelX(zdelta);
	else
		sb.WheelY(zdelta);
}

void IconDes::HorzMouseWheel(Point pt, int zdelta, dword keyflags)
{
	sb.WheelX(zdelta);
}
