#include "CocoMM.h"

#ifdef GUI_COCO

#define LLOG(x)

void DoMouseEvent(CocoView *view, NSEvent *e, int event)
{
	DLOG("MOUSE");
	NSPoint p = [view convertPoint:[e locationInWindow] fromView:nil];
	int zd = 0; // TODO: MouseWheel
	view->ctrl->CocoDispatchMouse__(event, Upp::Point(p.x, p.y), zd);
}

void SyncRect(CocoView *view)
{
	NSWindow *win = [view window];
	view->ctrl->NewRect__(MakeRect([win contentRectForFrameRect: [win frame]]));
}

@implementation CocoView
-(void)drawRect:(NSRect)rect {
	Upp::SystemDraw w([[NSGraphicsContext currentContext] CGContext], [self bounds].size.height);
	DLOG("CocoDraw");
    ctrl->Paint(w); // TODO!!!
}

- (void)mouseDown:(NSEvent *)e { DoMouseEvent(self, e, Upp::Ctrl::LEFTDOWN); }
- (void)mouseUp:(NSEvent *)e { DoMouseEvent(self, e, Upp::Ctrl::LEFTUP); }
- (void)mouseMoved:(NSEvent *)e { DoMouseEvent(self, e, Upp::Ctrl::MOUSEMOVE); }
- (void)mouseDragged:(NSEvent *)e { DoMouseEvent(self, e, Upp::Ctrl::MOUSEMOVE); } // TODO?
- (void)rightMouseDown:(NSEvent*)e { DoMouseEvent(self, e, Upp::Ctrl::RIGHTDOWN); }
- (void)rightMouseUp:(NSEvent*)e { DoMouseEvent(self, e, Upp::Ctrl::RIGHTUP); }

- (void)keyDown:(NSEvent *)e {
//	ctrl->Text("keyDown flag: " + AsString(e.modifierFlags) + ", characters: " +
//	             ToString((CFStringRef)(e.characters)) + ", keycode: " + AsString(e.keyCode));
}

- (void)keyUp:(NSEvent *)e {
//	ctrl->Text("keyUp ");
}

- (void)windowDidResize:(NSNotification *)notification { SyncRect(self); }
- (void)windowDidMove:(NSNotification *)notification { SyncRect(self); }
//TODO: more layout changes

- (void)windowDidBecomeKey:(NSNotification *)notification
{
	// THIS IS "GOT FOCUS"
}

- (BOOL)acceptsFirstResponder {	return YES; }
- (BOOL)canBecomeKeyView { return YES; }

@end

#endif