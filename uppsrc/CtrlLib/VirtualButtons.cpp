#include "CtrlLib.h"

namespace Upp {

int                  VirtualButtons::ButtonCount() const        { return 0; }
Rect                 VirtualButtons::ButtonRect(int i) const    { return Null; }
const Button::Style& VirtualButtons::ButtonStyle(int i) const   { return Button::StyleEdge(); }
Image                VirtualButtons::ButtonImage(int i) const   { return Null; }
bool                 VirtualButtons::ButtonEnabled(int i) const { return true; }
bool                 VirtualButtons::ButtonMono(int i) const    { return false; }

void VirtualButtons::ButtonPush(int i)   {}
void VirtualButtons::ButtonRepeat(int i) {}
void VirtualButtons::ButtonAction(int i) {}

int VirtualButtons::FindButton(Point p) const
{
	for(int i = 0; i < ButtonCount(); i++)
		if(ButtonRect(i).Contains(p))
			return i;
	return -1;
}

int VirtualButtons::ButtonVisualState(Ctrl *ctrl, int i)
{
	if(ButtonEnabled(i)) {
		if(i == pushi)
			return CTRL_PRESSED;
		if(ctrl->HasMouseIn(ButtonRect(i)))
			return CTRL_HOT;
		return CTRL_NORMAL;
	}
	return CTRL_DISABLED;
}

void VirtualButtons::PaintButtons(Draw& w, Ctrl *ctrl)
{
	for(int i = 0; i < ButtonCount(); i++)
		Button::PaintButton(w, ButtonRect(i), ButtonStyle(i), ButtonVisualState(ctrl, i), false,
		                    String(), StdFont(), ButtonImage(i),
		                    ButtonMono(i), 0, false, !ButtonEnabled(i));
}

void VirtualButtons::EndPush(Ctrl *ctrl)
{
	int i = pushi;
	pushi = -1;
	RefreshButton(ctrl, i);
}

void VirtualButtons::RefreshButton(Ctrl *ctrl, int i)
{
	if(i >= 0)
		ctrl->Refresh(ButtonRect(i));
}

bool VirtualButtons::ButtonsMouseEvent(Ctrl *ctrl, int event, Point p)
{
	if(event == Ctrl::CURSORIMAGE)
		return false;
	int i = event == Ctrl::MOUSELEAVE ? -1 : FindButton(p);
	if(i != mi) {
		RefreshButton(ctrl, mi);
		RefreshButton(ctrl, mi = i);
	}
	switch(event) {
	case Ctrl::LEFTDOWN:
		pushi = i;
		if(pushi >= 0) {
			ButtonPush(i);
			ctrl->SetCapture();
		}
		else
			EndPush(ctrl);
		RefreshButton(ctrl, pushi);
		break;
	case Ctrl::MOUSEMOVE:
		if(ctrl->HasCapture() && i != pushi) {
			RefreshButton(ctrl, pushi);
			pushi = i;
		}
		break;
	case Ctrl::LEFTREPEAT:
		if(i >= 0)
			ButtonRepeat(i);
		break;
	case Ctrl::LEFTUP:
		int ii = pushi;
		if(ii >= 0) {
			EndPush(ctrl);
			ReleaseCapture();
			ButtonAction(ii);
		}
		break;
	}
	return i >= 0;
}

};