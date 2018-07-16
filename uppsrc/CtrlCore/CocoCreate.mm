#include "CocoMM.h"

#ifdef GUI_COCO

#define LLOG(x)

void Upp::Ctrl::Create(const Upp::Rect& r, const char *title)
{
	NSRect frame = NSMakeRect(r.left, r.top, r.GetWidth(), r.GetHeight());
	NSWindow *window = [[NSWindow alloc] initWithContentRect:frame
	                   styleMask:NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask
	                             | NSWindowStyleMaskResizable
	                   backing:NSBackingStoreBuffered
	                   defer:false];
	
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
	SyncRect(view);
}

void Upp::Ctrl::WndDestroy()
{
	// TODO: Destroy window...
	if(!top)
		return;
	delete top->coco;
	delete top;
	top = NULL;
}

void Upp::Ctrl::WndInvalidateRect(const Rect& r)
{
	GuiLock __;
	[top->coco->view setNeedsDisplay:YES];
	// - (BOOL)needsToDrawRect:(NSRect)rect;
}

void Upp::Ctrl::NewRect__(const Rect& r)
{
	DLOG("NewRect " << r);
	SetWndRect(r);
}

/*
Upp::Size CocoWindow::GetSize() const
{
	return top ? top->ns_size : Size(0, 0);
}
*/

#endif
