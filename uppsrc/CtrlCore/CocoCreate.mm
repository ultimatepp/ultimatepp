#include "CocoMM.h"

#ifdef PLATFORM_COCOA

#define LLOG(x)

@interface CocoWindow : NSWindow
{
	@public
	Upp::Ptr<Upp::Ctrl> ctrl;
	bool active;
}
@end

@implementation CocoWindow

- (void)becomeKeyWindow {
	[super becomeKeyWindow];
}

- (BOOL)canBecomeKeyWindow {
    return active && ctrl && ctrl->IsEnabled();
}

- (BOOL)canBecomeMainWindow {
	LLOG("canBecomeMainWindow " << Upp::Name(ctrl) << ", owner " << Upp::Name(ctrl->GetOwner()));
	return active && ctrl && ctrl->IsEnabled() && dynamic_cast<Upp::TopWindow *>(~ctrl) && !ctrl->GetOwner();
}

@end

static Upp::Vector<Upp::Ptr<Upp::Ctrl>> mmtopctrl; // should work without Ptr, but let us be defensive....

Upp::Ctrl *Upp::Ctrl::GetOwner()
{
	GuiLock __;
	return top && top->coco ? top->coco->owner : NULL;
}

Upp::Ctrl *Upp::Ctrl::GetActiveCtrl()
{
	GuiLock __;
	for(Ctrl *p : mmtopctrl)
		if(p && p->top && p->top->coco && p->top->coco->window.keyWindow)
			return p;
	return NULL;
}

bool Upp::Ctrl::SetWndFocus()
{
	GuiLock __;
	if(top && top->coco) {
		[top->coco->window orderFront:top->coco->window];
		if([top->coco->window canBecomeKeyWindow])
			[top->coco->window makeKeyWindow];
		if(dynamic_cast<TopWindow *>(this) && [top->coco->window canBecomeMainWindow])
			[top->coco->window makeMainWindow];
	}
	return true;
}

bool Upp::Ctrl::HasWndFocus() const
{
	GuiLock __;
	return GetActiveCtrl() == this;
}


void Upp::Ctrl::SetWndForeground()
{
	GuiLock __;
	SetWndFocus();
}

bool Upp::Ctrl::IsWndForeground() const
{
	GuiLock __;
	return HasWndFocus();
}

NSRect DesktopRect(const Upp::Rect& r)
{
	double scalei = Upp::Ctrl::InvScale();
	return NSMakeRect(scalei * r.left,
	                  scalei * (Upp::Ctrl::GetPrimaryScreenArea().GetHeight() - r.top - r.GetHeight()),
	                  scalei * r.GetWidth(), scalei * r.GetHeight());
}

void *Upp::Ctrl::GetNSWindow() const
{
	return top && top->coco ? top->coco->window : NULL;
}

void *Upp::Ctrl::GetNSView() const
{
	return top && top->coco ? top->coco->view : NULL;
}

void Upp::Ctrl::Create(Ctrl *owner, dword style, bool active)
{
	if(owner)
		owner = owner->GetTopCtrl();

	top = new Top;
	top->coco = new CocoTop;
	top->coco->owner = owner;

	NSRect frame = DesktopRect(GetRect());
	CocoWindow *window = [[CocoWindow alloc] initWithContentRect:frame styleMask: style
	                                         backing:NSBackingStoreBuffered defer:false];
	top->coco->window = window;
	if(owner && owner->top && owner->top->coco)
		[owner->top->coco->window addChildWindow:window ordered:NSWindowAbove];

	window->ctrl = this;
	window->active = active;
	window.backgroundColor = [NSColor clearColor];
		
	CocoView *view = [[[CocoView alloc] initWithFrame:frame] autorelease];
	view->ctrl = this;
	top->coco->view = view;
	[window setContentView:view];
	[window setDelegate:view];
	[window setAcceptsMouseMovedEvents:YES];
	[window makeFirstResponder:view];
	[window makeKeyAndOrderFront:view];

	MMCtrl::SyncRect(view);
	isopen = true;
	mmtopctrl.Add(this);
}

void Upp::Ctrl::WndDestroy()
{
	LLOG("WndDestroy " << Name());
	if(!top)
		return;
	Ptr<Ctrl> owner = GetOwner();
	[top->coco->window close];
	delete top->coco;
	delete top;
	top = NULL;
	popup = isopen = false;
	int ii = FindIndex(mmtopctrl, this);
	if(ii >= 0)
		mmtopctrl.Remove(ii);
	if(owner && owner->IsOpen())
		owner->SetWndFocus();
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
	LLOG("Invalidate Rect " << r);
	if(top)
		[top->coco->view setNeedsDisplayInRect:(NSRect)MakeCGRect(r)];
}

void Upp::Ctrl::WndScrollView(const Rect& r, int dx, int dy)
{
	GuiLock __;
	LLOG("Scroll View " << r);
	WndInvalidateRect(r);
}

bool Upp::Ctrl::IsWndOpen() const {
	GuiLock __;
	return top;
}

void Upp::Ctrl::PopUp(Ctrl *owner, bool savebits, bool activate, bool dropshadow, bool topmost)
{
	Create(owner ? owner->GetTopCtrl() : GetActiveCtrl(), NSWindowStyleMaskBorderless, 0);
	popup = true;
	if(activate && IsEnabled())
		SetFocus();
	#if 0
		NSWindow *window = top->coco->window;
		[window setBackgroundColor:[NSColor clearColor]];
		[window setOpaque:NO];
		[window setStyleMask:NSResizableWindowMask | NSTitledWindowMask | NSFullSizeContentViewWindowMask];
		[window setMovableByWindowBackground:YES];
		[window setTitlebarAppearsTransparent:YES];
		[window setTitleVisibility:NSWindowTitleHidden];
		[window setShowsToolbarButton:NO];
		[window standardWindowButton:NSWindowFullScreenButton].hidden = YES;
		[window standardWindowButton:NSWindowMiniaturizeButton].hidden = YES;
		[window standardWindowButton:NSWindowCloseButton].hidden = YES;
		[window standardWindowButton:NSWindowZoomButton].hidden = YES;
		[window makeKeyWindow];
	//	[window setHasShadow:YES];
		ActivateWnd();
	}
	#endif
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

void Upp::TopWindow::Open(Ctrl *owner)
{
	GuiLock __;
	SetupRect(owner);
	if((owner && center == 1) || center == 2)
		SetRect((center == 1 ? owner->GetRect() : owner ? owner->GetWorkArea()
		                                                : GetPrimaryWorkArea())
		        .CenterRect(GetRect().GetSize()));
	Create(owner, GetMMStyle(), true);
	ActivateWnd();
	SyncCaption();
	SyncSizeHints();
	PlaceFocus();
	if(state == MAXIMIZED)
		Maximize();
	if(state == MINIMIZED)
		Minimize();
//	if(top)
//		top->placefocus = true;
}

void Upp::TopWindow::Open()
{
	Open(GetActiveCtrl());
}

void Upp::TopWindow::OpenMain()
{
	Open(NULL);
}

void Upp::TopWindow::SyncTitle()
{
	GuiLock __;
	if(top) {
		LLOG("SyncTitle " << title);
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

CGSize MMFrameSize(Upp::Size sz, Upp::dword style)
{
	return [NSWindow frameRectForContentRect:(NSRect)CGRectMake(100, 100, sz.cx, sz.cy) styleMask:style].size;
}

void Upp::TopWindow::SyncSizeHints()
{
	GuiLock __;
	if(top) {
		NSWindow *window = top->coco->window;
		Upp::dword style = GetMMStyle();
		Size sz = GetRect().GetSize();
		[window setMinSize:MMFrameSize(sizeable ? GetMinSize() : sz, style)];
		[window setMaxSize:MMFrameSize(sizeable ? GetMaxSize() : sz, style)];
	}
}

Upp::Rect Upp::Ctrl::GetWndScreenRect() const
{ // THIS IS NOT NEEDED
	GuiLock __;
	Rect r = GetRect();
	return r;
}

void Upp::Ctrl::WndSetPos(const Upp::Rect& rect)
{
	GuiLock __;
	if(top)
		[top->coco->window setFrame:
			[top->coco->window frameRectForContentRect:DesktopRect(rect)]
			display:YES];
}

void Upp::TopWindow::SerializePlacement(Stream& s, bool reminimize)
{
	GuiLock __;
	int version = 0;
	s / version;
	Rect rect = GetRect();
	s % rect;
	if(s.IsLoading())
		SetRect(rect);
}

void Upp::TopWindow::Maximize(bool effect)
{
	state = MAXIMIZED;
	if(top && top->coco && top->coco->window && !top->coco->window.zoomed) {
		if(effect)
			[top->coco->window performZoom:top->coco->window];
		else
			[top->coco->window zoom:top->coco->window];
	}
}

void Upp::TopWindow::Minimize(bool effect)
{
	state = MINIMIZED;
	if(top && top->coco && top->coco->window && !top->coco->window.miniaturized) {
		if(effect)
			[top->coco->window performMiniaturize:top->coco->window];
		else
			[top->coco->window miniaturize:top->coco->window];
	}
}

void Upp::TopWindow::Overlap(bool effect)
{
	state = OVERLAPPED;
	if(top && top->coco && top->coco->window && top->coco->window.zoomed)
		[top->coco->window zoom:top->coco->window];
	if(top && top->coco && top->coco->window && top->coco->window.miniaturized)
		[top->coco->window deminiaturize:top->coco->window];
}


#endif
