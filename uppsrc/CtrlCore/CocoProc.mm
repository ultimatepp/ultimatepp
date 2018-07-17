#include "CocoMM.h"

#ifdef GUI_COCO

#define LLOG(x)

static Upp::Point coco_mouse_pos;
static bool       coco_mouse_left;
static bool       coco_mouse_right;
static int        coco_flags;

namespace Upp {
bool  GetShift() { return coco_flags & NSEventModifierFlagShift; }
bool  GetCtrl() { return coco_flags & NSEventModifierFlagCommand; }
bool  GetAlt() { return coco_flags & NSEventModifierFlagControl; }
bool  GetCapsLock() { return coco_flags & NSEventModifierFlagCapsLock; }
/*
NSEventModifierFlagOption
NSEventModifierFlagNumericPad
NSEventModifierFlagHelp
NSEventModifierFlagFunction
NSEventModifierFlagDeviceIndependentFlagsMask
*/

bool  GetMouseLeft() { return coco_mouse_left; }
bool  GetMouseRight() { return coco_mouse_right; }
bool  GetMouseMiddle() { return false; } // TODO

Point GetMousePos() { return coco_mouse_pos; } // TODO: read it if no mouse events


struct MMImp {
	static void MouseEvent(CocoView *view, NSEvent *e, int event)
	{
		NSPoint np = [view convertPoint:[e locationInWindow] fromView:nil];
		Upp::Point p(np.x, np.y);
		int zd = 0; // TODO: MouseWheel
		coco_mouse_pos = p + view->ctrl->GetRect().TopLeft();
		view->ctrl->DispatchMouse(event, p, zd);
		DLOG("MOUSE " << Upp::Point(p.x, p.y));
	}
	
	static void Flags(NSEvent *e)
	{
		coco_flags = [e modifierFlags];
	}
	
	static void Paint(Upp::Ctrl *ctrl, Upp::SystemDraw& w, const Rect& r)
	{
		DLOG("MMPaint " << r);
		ctrl->fullrefresh = false;
		ctrl->UpdateArea(w, r);
	}
};

};

@implementation CocoView
-(void)drawRect:(NSRect)rect {
	Upp::SystemDraw w([[NSGraphicsContext currentContext] CGContext], [self bounds].size.height);
	Upp::MMImp::Paint(ctrl, w, MakeRect(rect));
}

- (void)mouseDown:(NSEvent *)e { Upp::MMImp::MouseEvent(self, e, Upp::Ctrl::LEFTDOWN); coco_mouse_left = true; }
- (void)mouseUp:(NSEvent *)e { Upp::MMImp::MouseEvent(self, e, Upp::Ctrl::LEFTUP); coco_mouse_left = false; }
- (void)mouseMoved:(NSEvent *)e { Upp::MMImp::MouseEvent(self, e, Upp::Ctrl::MOUSEMOVE); }
- (void)mouseDragged:(NSEvent *)e { Upp::MMImp::MouseEvent(self, e, Upp::Ctrl::MOUSEMOVE); } // TODO?
- (void)rightMouseDown:(NSEvent*)e { Upp::MMImp::MouseEvent(self, e, Upp::Ctrl::RIGHTDOWN); coco_mouse_right = true; }
- (void)rightMouseUp:(NSEvent*)e { Upp::MMImp::MouseEvent(self, e, Upp::Ctrl::RIGHTUP); coco_mouse_right = false; }

- (void)keyDown:(NSEvent *)e {
//	ctrl->Text("keyDown flag: " + AsString(e.modifierFlags) + ", characters: " +
//	             ToString((CFStringRef)(e.characters)) + ", keycode: " + AsString(e.keyCode));
}

- (void)keyUp:(NSEvent *)e {
//	ctrl->Text("keyUp ");
}

- (void)windowDidResize:(NSNotification *)notification { Upp::MMCtrl::SyncRect(self); }
- (void)windowDidMove:(NSNotification *)notification { Upp::MMCtrl::SyncRect(self); }
//TODO: more layout changes

- (void)windowDidBecomeKey:(NSNotification *)notification
{
	// THIS IS "GOT FOCUS"
}

- (BOOL)acceptsFirstResponder {	return YES; }
- (BOOL)canBecomeKeyView { return YES; }

@end

#endif