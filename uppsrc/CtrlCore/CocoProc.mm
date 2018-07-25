#include "CocoMM.h"

#ifdef PLATFORM_COCOA

#define LLOG(x)

static Upp::Point coco_mouse_pos;
static bool       coco_mouse_left;
static bool       coco_mouse_right;
static int        coco_flags;

namespace Upp {
bool  GetShift() { return coco_flags & NSEventModifierFlagShift; }
bool  GetCtrl() { return coco_flags & NSEventModifierFlagCommand; }
bool  GetAlt() { return coco_flags & NSEventModifierFlagControl; }
bool  GetOption() { return coco_flags & NSEventModifierFlagOption; }
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
		Upp::Point p(np.x, view->ctrl->GetRect().GetHeight() - np.y);
		int zd = 0; // TODO: MouseWheel
		coco_mouse_pos = p + view->ctrl->GetRect().TopLeft();
		view->ctrl->DispatchMouse(event, p, zd);
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

	static void KeyEvent(Upp::Ctrl *ctrl, NSEvent *e, int up) {
		Flags(e);
		Upp::dword k = e.keyCode|K_DELTA|up;
		if(GetCtrl())
			k |= K_CTRL;
		if(GetShift())
			k |= K_SHIFT;
		if(GetAlt())
			k |= K_ALT;
		if(GetOption()) // TODO
			k |= K_OPTION;
		ctrl->DispatchKey(k, 1);
		if(!up && !(k & (K_CTRL|K_ALT))) {
			WString x = ToWString((CFStringRef)(e.characters));
			for(wchar c : x)
				if(c < 0xF700 && c >= 32 && c != 127)
					ctrl->DispatchKey(c, 1);
		}
	}
	
	static void GotFocus(Upp::Ctrl *ctrl)
	{
		ctrl->ActivateWnd();
	}
	
	static void DoClose(Upp::Ctrl *ctrl)
	{
		DLOG("DoClose");
		ctrl->MMClose();
	}
};

};

@implementation CocoView
-(void)drawRect:(NSRect)r {
	int h = ctrl->GetRect().GetHeight();
	Upp::SystemDraw w([[NSGraphicsContext currentContext] CGContext], h);
	Upp::MMImp::Paint(ctrl, w, Upp::RectC(r.origin.x, h - r.origin.y - r.size.height,
	                                      r.size.width, r.size.height));
}

- (void)mouseDown:(NSEvent *)e { Upp::MMImp::MouseEvent(self, e, Upp::Ctrl::LEFTDOWN); coco_mouse_left = true; }
- (void)mouseUp:(NSEvent *)e { Upp::MMImp::MouseEvent(self, e, Upp::Ctrl::LEFTUP); coco_mouse_left = false; }
- (void)mouseMoved:(NSEvent *)e { Upp::MMImp::MouseEvent(self, e, Upp::Ctrl::MOUSEMOVE); }
- (void)mouseDragged:(NSEvent *)e { Upp::MMImp::MouseEvent(self, e, Upp::Ctrl::MOUSEMOVE); } // TODO?
- (void)rightMouseDown:(NSEvent*)e { Upp::MMImp::MouseEvent(self, e, Upp::Ctrl::RIGHTDOWN); coco_mouse_right = true; }
- (void)rightMouseUp:(NSEvent*)e { Upp::MMImp::MouseEvent(self, e, Upp::Ctrl::RIGHTUP); coco_mouse_right = false; }

- (void)keyDown:(NSEvent *)e { Upp::MMImp::KeyEvent(ctrl, e, 0); }
- (void)keyUp:(NSEvent *)e { Upp::MMImp::KeyEvent(ctrl, e, Upp::K_KEYUP); }

- (BOOL)windowShouldClose:(NSWindow *)sender { DLOG("SHOULD CLOSE"); Upp::MMImp::DoClose(ctrl); return NO; }

- (void)windowDidResize:(NSNotification *)notification { Upp::MMCtrl::SyncRect(self); }
- (void)windowDidMove:(NSNotification *)notification { Upp::MMCtrl::SyncRect(self); }
//TODO: more layout changes

- (void)windowDidBecomeKey:(NSNotification *)notification
{
	DLOG("DidBecomeKey");
	Upp::MMImp::GotFocus(ctrl);
}

- (BOOL)acceptsFirstResponder {	return YES; }
- (BOOL)canBecomeKeyView { return YES; }

@end

#endif