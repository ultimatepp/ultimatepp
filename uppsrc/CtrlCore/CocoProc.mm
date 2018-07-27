#include "CocoMM.h"

#ifdef PLATFORM_COCOA

#define LLOG(x)

static Upp::Point coco_mouse_pos;
static bool       coco_mouse_left;
static bool       coco_mouse_right;
static int        coco_flags;
static Upp::Ptr<Upp::Ctrl> coco_capture;

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

void Ctrl::WndEnable(bool)
{
	GuiLock __;
	// empty - we are testing the status in events....
}

bool Ctrl::SetWndCapture()
{
	GuiLock __;
	coco_capture = this;
	return true; // TODO: Always?
}

bool Ctrl::HasWndCapture() const
{
	GuiLock __;
	return coco_capture == this;
}

bool Ctrl::ReleaseWndCapture()
{
	GuiLock __;
	coco_capture = NULL;
	return true;
}

struct MMImp {
	static bool MouseEvent(CocoView *view, NSEvent *e, int event, double zd = 0)
	{
		NSPoint np = [view convertPoint:[e locationInWindow] fromView:nil];
		Rect r = view->ctrl->GetRect();
		Upp::Point p(np.x, r.GetHeight() - np.y);
		coco_mouse_pos = p + r.TopLeft();
		if(view->ctrl->IsEnabled() && (view->ctrl->HasWndCapture() || r.Contains(coco_mouse_pos)))
			view->ctrl->DispatchMouse(event, p, 120 * sgn(zd));
		return false;
	}
	static bool MouseDownEvent(CocoView *view, NSEvent *e, int button)
	{
		static int clicktime = msecs() - 100000;
		bool b = MouseEvent(view, e, button|(msecs(clicktime) < 250 ? Upp::Ctrl::DOUBLE : Upp::Ctrl::DOWN));
		clicktime = msecs();
		return b;
	}
	
	static void Flags(NSEvent *e)
	{
		coco_flags = [e modifierFlags];
	}
	
	static void Paint(Upp::Ctrl *ctrl, Upp::SystemDraw& w, const Rect& r)
	{
		ctrl->fullrefresh = false;
		ctrl->UpdateArea(w, r);
	}

	static bool KeyEvent(Upp::Ctrl *ctrl, NSEvent *e, int up) {
		Flags(e);
		if(!ctrl->IsEnabled())
			return false;
		Upp::dword k = e.keyCode|K_DELTA|up;
		if(GetCtrl())
			k |= K_CTRL;
		if(GetShift())
			k |= K_SHIFT;
		if(GetAlt())
			k |= K_ALT;
		if(GetOption()) // TODO
			k |= K_OPTION;
		
		if(e.keyCode == kVK_Help) // TODO: This is Insert key, but all this is dubious
			ctrl->DispatchKey(k & ~K_KEYUP, 1);

		ctrl->DispatchKey(k, 1);
		if(!up && !(k & (K_CTRL|K_ALT))) {
			WString x = ToWString((CFStringRef)(e.characters));
			for(wchar c : x)
				if(c < 0xF700 && c >= 32 && c != 127)
					ctrl->DispatchKey(c, 1);
		}
		return true;
	}
	
	static void BecomeKey(Upp::Ctrl *ctrl)
	{
		ctrl->ActivateWnd();
	}

	static void ResignKey(Upp::Ctrl *ctrl)
	{
		ctrl->KillFocusWnd();
	}
	
	static void DoClose(Upp::Ctrl *ctrl)
	{
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

- (void)mouseDown:(NSEvent *)e {
	coco_mouse_left = true;
	if(!Upp::MMImp::MouseDownEvent(self, e, Upp::Ctrl::LEFT))
		[super mouseDown:e];
}

- (void)mouseUp:(NSEvent *)e {
	coco_mouse_left = false;
	if(!Upp::MMImp::MouseEvent(self, e, Upp::Ctrl::LEFTUP))
		[super mouseUp:e];
}

- (void)mouseMoved:(NSEvent *)e {
	if(!Upp::MMImp::MouseEvent(self, e, Upp::Ctrl::MOUSEMOVE))
		[super mouseMoved:e];
}

- (void)mouseDragged:(NSEvent *)e {
	if(!Upp::MMImp::MouseEvent(self, e, Upp::Ctrl::MOUSEMOVE))
		[super mouseDragged:e];
}

- (void)rightMouseDown:(NSEvent*)e {
	coco_mouse_right = true;
	if(!Upp::MMImp::MouseDownEvent(self, e, Upp::Ctrl::RIGHT))
		[super rightMouseDown:e];
}

- (void)rightMouseUp:(NSEvent*)e {
	coco_mouse_right = false;
	if(!Upp::MMImp::MouseEvent(self, e, Upp::Ctrl::RIGHTUP))
		[super rightMouseUp:e];
}

- (void)scrollWheel:(NSEvent *)e {
	if(!Upp::MMImp::MouseEvent(self, e, Upp::Ctrl::MOUSEWHEEL, [e deltaY]))
		[super scrollWheel:e];
}

- (void)keyDown:(NSEvent *)e {
	if(!Upp::MMImp::KeyEvent(ctrl, e, 0))
		[super keyDown:e];
}

- (void)keyUp:(NSEvent *)e {
	if(!Upp::MMImp::KeyEvent(ctrl, e, Upp::K_KEYUP))
		[super keyUp:e];
}

- (BOOL)windowShouldClose:(NSWindow *)sender {
	Upp::MMImp::DoClose(ctrl);
	return NO;
}

- (void)windowDidResize:(NSNotification *)notification { Upp::MMCtrl::SyncRect(self); }
- (void)windowDidMove:(NSNotification *)notification { Upp::MMCtrl::SyncRect(self); }
//TODO: more layout changes

- (void)windowDidBecomeKey:(NSNotification *)notification {
	Upp::MMImp::BecomeKey(ctrl);
}

- (void)windowDidResignKey:(NSNotification *)notification {
	Upp::MMImp::ResignKey(ctrl);
}

- (BOOL)acceptsFirstResponder {
	return YES;
}

- (BOOL)canBecomeKeyView {
	return ctrl->IsEnabled();
}

@end

#endif