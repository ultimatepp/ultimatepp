#include "LayoutTest.h"

#define DECLARE_IP_TABLE(type, name) type *name##_ptr;
DECLARE_IP_TABLE(int, tab);
#define tab (*tab_ptr)

LayoutTest::LayoutTest()
{

	CtrlLayout(*this, "Window title");
	Sizeable();
	b1 <<= THISBACK(OnB1);
	b2 <<= THISBACK(Close);

	CtrlLayout(p, "Popup");
	CtrlLayout(c, "Embedded");
	Add(c.HSizePosZ(4, 4).VSizePosZ(4, 28));
}

GUI_APP_MAIN
{
	DDUMP(Image::SizeVert().GetSerialId());
	LayoutTest().Run();
}

PopupWindow::PopupWindow()
{
	AddFrame(OutsetFrame());  // TODO: Replace with proper window frame
	AddFrame(InsetFrame());
	AddFrame(titlebar);
	moving = false;
	sizedir = -1;
	minsz = Size(0, 0);
	BackPaint();
}

void PopupWindow::ChildMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags)
{
	return;
	if (child == &titlebar) {
		if(event == LEFTDOWN) {
			titlebar.SetFocusLook(true);
			SetCapture();
			mstart = GetMousePos();
			if (GetParent())
				mstart -= GetParent()->GetRect().TopLeft();
			rstart = GetRect();
			moving = true;
			//OverrideCursor(Image::SizeAll());
			LOG("Move Start 1!");
			WhenMoveStart();
		}
		else if (event == RIGHTDOWN)
			titlebar.SetFocusLook(true);
	}
}

Image PopupWindow::FrameMouseEvent(int event, Point p, int zdelta, dword keyflags)
{
	if (HasCapture()) return CursorImage(p, keyflags);
	rstart = GetRect();
	mstart = GetMousePos();
	if (GetParent())
		mstart -= GetParent()->GetRect().TopLeft();
	Rect r = rstart.Deflated(8);

	moving = false;
	sizedir = -1;
	if (r.left > mstart.x)
		sizedir = 0;
	else if (r.top > mstart.y)
		sizedir = 1;
	else if (r.right < mstart.x)
		sizedir = 2;
	else if (r.bottom < mstart.y)
		sizedir = 3;

	if (sizedir >= 0) {
		Image cursor = (sizedir & 1) ? Image::SizeVert() : Image::SizeHorz();
		if (event == LEFTDOWN) {
			//OverrideCursor(cursor);
			LOG("Move Start!");
			SetCapture();
		}
		return cursor;
	}
	return Image::Arrow();
}

void PopupWindow::MouseMove(Point p, dword keyflags)
{
	if (HasCapture()) {
		Point pt = GetMousePos();
		if (GetParent())
			pt -= GetParent()->GetRect().TopLeft();
		if (moving) {
			LOG("Moving");
			SetRect(rstart.Offseted(pt - mstart));
			WhenMove();
		}
		else {
			Rect r = rstart;
			switch (sizedir) {
				case 0:
					r.left = minmax(r.left + (pt.x - mstart.x), r.right - GetMaxSize().cx, r.right - minsz.cx);
					break;
				case 1:
					r.top = minmax(r.top + (pt.y - mstart.y), r.bottom - GetMaxSize().cy, r.bottom - minsz.cy);
					break;
				case 2:
					r.right = minmax(r.right + (pt.x - mstart.x), r.left + minsz.cx, r.left + GetMaxSize().cx);
					break;
				case 3:
					r.bottom = minmax(r.bottom + (pt.y - mstart.y), r.top + minsz.cy, r.top + GetMaxSize().cy);
					break;
			}
			SetRect(r);
		}
	}
}

Image PopupWindow::CursorImage(Point p, dword keyflags)
{
	return Image::SizeVert();
	if (HasCapture()) {
		if (moving)
			return Image::SizeAll();
		else if (sizedir < 0)
			return (sizedir & 1) ? Image::SizeVert() : Image::SizeHorz();;
	}
	if (sizedir < 0)
			return (sizedir & 1) ? Image::SizeVert() : Image::SizeHorz();;
	return Image::Arrow();
}

void PopupWindow::Open(Ctrl *owner)
{
	Rect r = GetRect();
	r.SetSize(minmax(r.GetSize(), minsz, GetMaxSize()));
	SetRect(r);
	PopUp(owner, false, true, false, false);
}

void PopupWindow::EndMoveResize()
{
	if (HasCapture()) {
		ReleaseCapture();
		if (moving)
			WhenMoveEnd();
		moving = false;
		sizedir = -1;
	}
}
