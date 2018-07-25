#include "CocoMM.h"

#ifdef PLATFORM_COCOA

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

static Upp::Vector<Upp::Ptr<Upp::Ctrl>> mmtopctrl; // should work without Ptr, but let us be defensive....

void Upp::Ctrl::Create(dword style)
{
	Rect r = GetRect();
	
	NSRect frame = NSMakeRect(r.left, GetPrimaryScreenArea().GetHeight() - r.top - r.GetHeight(),
	                          r.GetWidth(), r.GetHeight());
		
	NSWindow *window = [[CocoWindow alloc] initWithContentRect:frame styleMask: style
	                                       backing:NSBackingStoreBuffered defer:false];
	
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
	mmtopctrl.Add(this);
}

void Upp::Ctrl::WndDestroy()
{
	if(!top)
		return;
	[top->coco->window release];
	delete top->coco;
	delete top;
	top = NULL;
	isopen = false;
	int ii = FindIndex(mmtopctrl, this);
	if(ii >= 0)
		mmtopctrl.Remove(ii);
}

Upp::Vector<Upp::Ctrl *> Upp::Ctrl::GetTopCtrls()
{
	Vector<Ctrl *> h;
	for(Ctrl *p : mmtopctrl)
		if(p) h.Add(p);
	return h;
}

void Upp::Ctrl::WndInvalidateRect(const Rect& r)
{
	GuiLock __;
	[top->coco->view
	      setNeedsDisplayInRect:(NSRect)CGRectMake(r.left, GetRect().GetHeight() - r.top - r.GetHeight(),
	                                               r.GetWidth(), r.GetHeight())];
}

bool Upp::Ctrl::IsWndOpen() const {
	GuiLock __;
	return top;
}


void Upp::Ctrl::PopUp(Ctrl *owner, bool savebits, bool activate, bool dropshadow, bool topmost)
{
	Create(NSWindowStyleMaskBorderless);
}

Upp::dword Upp::TopWindow::GetMMStyle() const
{
	Upp::dword style = NSWindowStyleMaskTitled|NSWindowStyleMaskClosable|NSWindowStyleMaskResizable;
	if(minimizebox)
		style |= NSWindowStyleMaskMiniaturizable;
//	if(maximizebox)
//		style |= ;
	return style;
}

void Upp::TopWindow::Open(Ctrl *owner_)
{
	GuiLock __;
	owner = owner_;
	SetupRect(owner);
	if((owner && center == 1) || center == 2)
		SetRect((center == 1 ? owner->GetRect() : owner ? owner->GetWorkArea()
		                                                : GetPrimaryWorkArea())
		        .CenterRect(GetRect().GetSize()));
	Create(GetMMStyle());
	SyncCaption();
	SyncSizeHints();
}

void Upp::TopWindow::Open()
{
	Open(NULL);
}

void Upp::TopWindow::OpenMain()
{
	Open(NULL);
}

void Upp::TopWindow::SyncTitle()
{
	GuiLock __;
	if(top) {
		DLOG("SyncTitle " << title);
		CFRef<CFStringRef> s = CFStringCreateWithCString(NULL, (const char *)~title.ToString(), kCFStringEncodingUTF8);
		[top->coco->window setTitle:(NSString *)~s];
	}
}

void Upp::TopWindow::SyncCaption()
{
	GuiLock __;
	if(top) {
		SyncTitle();
		NSWindow *window = top->coco->window;
		[[window standardWindowButton:NSWindowMiniaturizeButton] setHidden:!minimizebox];
		[[window standardWindowButton:NSWindowZoomButton] setHidden:!maximizebox];
	}
}

CGRect MMFrameRect(const Upp::Rect& r, Upp::dword style)
{// TODO: Revert Y
	return [NSWindow frameRectForContentRect:(NSRect)MakeCGRect(r, 1000) styleMask:style];
}

CGSize MMFrameSize(Upp::Size sz, Upp::dword style)
{
	return MMFrameRect(Upp::RectC(100, 100, sz.cx, sz.cy), style).size;
}

void Upp::TopWindow::SyncSizeHints()
{
	GuiLock __;
	if(top) {
		NSWindow *window = top->coco->window;
		Upp::dword style = GetMMStyle();
		Size sz0 = GetRect().GetSize();
		Size sz = sz0;
		if(sizeable)
			sz = GetMinSize();
		[window setMinSize:MMFrameSize(sz, style)];
		sz = sz0;
		if(sizeable)
			sz = GetMaxSize();
		[window setMaxSize:MMFrameSize(sz, style)];
	}
}

#endif
