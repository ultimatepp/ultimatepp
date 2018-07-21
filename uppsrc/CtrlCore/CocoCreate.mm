#include "CocoMM.h"

#ifdef GUI_COCO

#define LLOG(x)

@interface CocoWindow : NSWindow
{
}
@end

@implementation CocoWindow
- (BOOL)canBecomeKeyWindow {
    return YES;
}
@end

void Upp::MMCtrl::SyncRect(CocoView *view)
{
	NSWindow *win = [view window];
	view->ctrl->SetWndRect(MakeRect([win contentRectForFrameRect: [win frame]]));
}

void Upp::Ctrl::Create(const Upp::Rect& r, const char *title, bool popup)
{
	NSRect frame = NSMakeRect(r.left, r.top, r.GetWidth(), r.GetHeight());
	
	Upp::dword style = popup ? NSWindowStyleMaskBorderless :
	                    NSWindowStyleMaskTitled | NSWindowStyleMaskClosable
	                             | NSWindowStyleMaskMiniaturizable
	                             | NSWindowStyleMaskResizable;
	
	NSWindow *window = [[CocoWindow alloc] initWithContentRect:frame styleMask: style
	                                       backing:NSBackingStoreBuffered defer:false];
	
	// TODO: Move title
	CFRef<CFStringRef> s = CFStringCreateWithCString(NULL, title, kCFStringEncodingUTF8);
	[window setTitle:(NSString *)~s];

	window.backgroundColor = nil;
		
	CocoView *view = [[[CocoView alloc] initWithFrame:frame] autorelease];
	view->ctrl = this;
	[window setContentView:view];
	[window setDelegate:view];
	[window setAcceptsMouseMovedEvents:YES];
	[window makeFirstResponder:view];
	[window makeKeyAndOrderFront:view];

	top = new Top;
	top->coco = new CocoTop;
	top->coco->window = window;
	top->coco->view = view;
	MMCtrl::SyncRect(view);
	isopen = true;
}

void Upp::Ctrl::WndDestroy()
{
	// TODO: Destroy window...
	if(!top)
		return;
	delete top->coco;
	delete top;
	top = NULL;
	isopen = false;
}

void Upp::Ctrl::WndInvalidateRect(const Rect& r)
{
	GuiLock __;
	DLOG("InvalidateRect " << r);
	[top->coco->view setNeedsDisplay:YES];
	// - (BOOL)needsToDrawRect:(NSRect)rect;
}

bool Upp::Ctrl::IsWndOpen() const {
	GuiLock __;
	return top;
}

#endif
