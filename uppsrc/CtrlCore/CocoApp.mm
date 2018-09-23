#include "CocoMM.h"

#ifdef PLATFORM_COCOA

#define LLOG(x) // DLOG(x)

int  Upp::Ctrl::WndCaretTime;
bool Upp::Ctrl::WndCaretVisible;

static NSAutoreleasePool *main_coco_pool;
static NSEvent           *current_event;

static NSEvent *GetNextEvent(NSDate *until)
{
	if(!current_event) {
		current_event = [NSApp nextEventMatchingMask:NSEventMaskAny
		                 untilDate:until
		                 inMode:NSDefaultRunLoopMode dequeue:YES];
		[current_event retain];
	}
	return current_event;
}

static void ReleaseCurrentEvent()
{
	if(current_event) {
		[current_event release];
		current_event = nil;
	}
}

void SyncPopupFocus(NSWindow *win)
{
	Upp::Ctrl *q = Upp::Ctrl::GetFocusCtrl();
	if(q) {
		q = q->GetTopCtrl();
		if(q->IsPopUp() && q->GetNSWindow() != win) {
			q = q->GetOwner();
			if(q) q->SetFocus();
		}
	}
}

namespace Upp {
	extern const char *sClipFmtsRTF;
};

void Upp::CocoInit(int argc, const char **argv, const char **envptr)
{
	Ctrl::GlobalBackBuffer();
    main_coco_pool = [NSAutoreleasePool new];

    [NSApplication sharedApplication];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    id menubar = [[NSMenu new] autorelease];
    id appMenuItem = [[NSMenuItem new] autorelease];
    [menubar addItem:appMenuItem];
    [NSApp setMainMenu:menubar];
    id appMenu = [[NSMenu new] autorelease];
    id appName = [[NSProcessInfo processInfo] processName];
    id quitTitle = [@"Quit " stringByAppendingString:appName];
    id quitMenuItem = [[[NSMenuItem alloc] initWithTitle:quitTitle
        action:@selector(terminate:) keyEquivalent:@"q"] autorelease];
    [appMenu addItem:quitMenuItem];
    [appMenuItem setSubmenu:appMenu];
    [NSApp activateIgnoringOtherApps:YES];
    
	NSFont *sysfont = [NSFont systemFontOfSize:0];
	Font::SetFace(0, Upp::ToString((CFStringRef)[sysfont familyName]), Font::TTF);
	
	Ctrl::SetUHDEnabled(true);

	for (NSScreen *screen in [NSScreen screens])
		if([screen backingScaleFactor] > 1 && Ctrl::IsUHDEnabled()) {
			SetUHDMode(true);
			break;
		}

	Font::SetDefaultFont(StdFont(fceil(DPI([sysfont pointSize]))));
	
	GUI_DblClickTime_Write(1000 * NSEvent.doubleClickInterval);

	[NSEvent addGlobalMonitorForEventsMatchingMask:(NSEventMaskLeftMouseDown)
	  handler:^(NSEvent *e) {
	      SyncPopupFocus(NULL);
    }];
	[NSEvent addLocalMonitorForEventsMatchingMask:(NSEventMaskLeftMouseDown)
	  handler:^NSEvent *(NSEvent *e) {
	      SyncPopupFocus([e window]);
	      return e;
    }];
    
    Upp::sClipFmtsRTF = "rtf";
    
    Upp::Ctrl::Csizeinit();
}

void Upp::CocoExit()
{
	ReleaseCurrentEvent();
	[main_coco_pool release];
}

int Upp::Ctrl::GetKbdDelay()
{
	GuiLock __;
	return int(1000 * NSEvent.keyRepeatDelay);
}

int Upp::Ctrl::GetKbdSpeed()
{
	GuiLock __;
	return int(1000 * NSEvent.keyRepeatInterval);
}

bool Upp::Ctrl::IsWaitingEvent()
{
	return GetNextEvent(nil);
}

bool Upp::Ctrl::ProcessEvent(bool *)
{
	ASSERT(IsMainThread());

	Upp::AutoreleasePool __;
	
	ONCELOCK {
		[NSApp finishLaunching];
	}
	
	NSEvent *event = GetNextEvent(nil);

	// DLOG("ProcessEvent " << ToString(event.description));

	if(!event)
		return false;
	
	current_event = nil;
	[NSApp sendEvent:event];
	[event release];

	return true;
}

void SweepMkImageCache();

bool Upp::Ctrl::ProcessEvents(bool *quit)
{
	if(ProcessEvent(quit)) {
		while(ProcessEvent(quit) && (!LoopCtrl || LoopCtrl->InLoop()));
		TimerProc(GetTickCount());
		AnimateCaret();
		[NSApp updateWindows];
		SweepMkImageCache();
		return true;
	}
	SweepMkImageCache();
	TimerProc(GetTickCount());
	return false;
}


void Upp::Ctrl::EventLoop(Ctrl *ctrl)
{
	GuiLock __;
	ASSERT(IsMainThread());
	ASSERT(LoopLevel == 0 || ctrl);
	LoopLevel++;
	LLOG("Entering event loop at level " << LoopLevel);
	Ptr<Ctrl> ploop;
	if(ctrl) {
		ploop = LoopCtrl;
		LoopCtrl = ctrl;
		ctrl->inloop = true;
	}

	bool quit = false;
	ProcessEvents(&quit);
	while(ctrl ? ctrl->IsOpen() && ctrl->InLoop() : GetTopCtrls().GetCount())
	{
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / GuiSleep");
		SyncCaret();
		AnimateCaret();
		GuiSleep(20);
//		if(EndSession()) break;
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / ProcessEvents");
		ProcessEvents(&quit);
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / after ProcessEvents");
	}

	if(ctrl)
		LoopCtrl = ploop;
	LoopLevel--;
	LLOG("Leaving event loop ");
}

static std::atomic<bool> sGuiSleep;

void Upp::Ctrl::GuiSleep(int ms)
{
	ASSERT(IsMainThread());
	sGuiSleep = true;
	GetNextEvent([NSDate dateWithTimeIntervalSinceNow:ms / 1000.0]);
	sGuiSleep = false;
}

namespace Upp {
void WakeUpGuiThread(void)
{
	if(sGuiSleep) {
		sGuiSleep = false;
		[NSApp postEvent:[NSEvent otherEventWithType:NSEventTypeApplicationDefined
		                          location:NSMakePoint(0, 0) modifierFlags:0 timestamp:0.0
		                          windowNumber:0 context:nil subtype: 0 data1:0 data2:0]
		       atStart:YES];
	}
}
};

void  Upp::Ctrl::AnimateCaret()
{
	GuiLock __;
	int v = !(((GetTickCount() - WndCaretTime) / 500) & 1);
	if(v != WndCaretVisible) {
		WndCaretVisible = v;
		RefreshCaret();
	}
}

void Upp::Ctrl::PaintCaret(SystemDraw& w)
{
	GuiLock __;
	LLOG("PaintCaret " << Name() << ", caretCtrl: " << caretCtrl << ", WndCaretVisible: " << WndCaretVisible);
	if(this == caretCtrl && WndCaretVisible)
		w.DrawRect(caretx, carety, caretcx, caretcy, InvertColor);
}

void Upp::Ctrl::SetCaret(int x, int y, int cx, int cy)
{
	GuiLock __;
	LLOG("SetCaret " << Name());
	if(this == caretCtrl)
		RefreshCaret();
	caretx = x;
	carety = y;
	caretcx = cx;
	caretcy = cy;
	if(this == caretCtrl) {
		WndCaretTime = GetTickCount();
		RefreshCaret();
		AnimateCaret();
	}
}

void Upp::Ctrl::SyncCaret() {
	GuiLock __;
//	LLOG("SyncCaret");
	if(focusCtrl != caretCtrl) {
		LLOG("SyncCaret DO " << Upp::Name(caretCtrl) << " -> " << Upp::Name(focusCtrl));
		RefreshCaret();
		caretCtrl = focusCtrl;
		RefreshCaret();
	}
}

Upp::Rect Upp::Ctrl::GetWorkArea() const
{
	return GetPrimaryWorkArea();
}

void Upp::Ctrl::GetWorkArea(Array<Rect>& rc)
{
	GuiLock __;
	rc.Add(GetPrimaryWorkArea());
}

Upp::Rect Upp::Ctrl::GetVirtualWorkArea()
{
	return GetPrimaryWorkArea();
}

Upp::Rect Upp::Ctrl::GetVirtualScreenArea()
{
	return GetPrimaryWorkArea();
}

Upp::Rect MakeScreenRect(NSScreen *screen, CGRect r)
{
	r.origin.y = [screen frame].size.height - r.origin.y - r.size.height;
	return MakeRect(r);
}

Upp::Rect Upp::Ctrl::GetPrimaryWorkArea()
{
	for (NSScreen *screen in [NSScreen screens])
		return DPI(1) * MakeScreenRect(screen, [screen visibleFrame]);
	return Rect(0, 0, 1024, 768);
}

Upp::Rect Upp::Ctrl::GetPrimaryScreenArea()
{
	for (NSScreen *screen in [NSScreen screens])
		return DPI(1) * MakeScreenRect(screen, [screen frame]);
	return Rect(0, 0, 1024, 768);
}

bool Upp::Ctrl::IsCompositedGui()
{
	return true;
}

Upp::Rect Upp::Ctrl::GetDefaultWindowRect()
{
	GuiLock __;
	Rect r  = GetPrimaryWorkArea();
	Size sz = r.GetSize();
	
	static int pos = min(sz.cx / 10, 50);
	pos += 10;
	int cx = sz.cx * 2 / 3;
	int cy = sz.cy * 2 / 3;
	if(pos + cx + 50 > sz.cx || pos + cy + 50 > sz.cy)
		pos = 0;
	return RectC(r.left + pos + 20, r.top + pos + 20, cx, cy);
}

void Upp::Ctrl::GuiPlatformGetTopRect(Rect& r) const
{
}

void Upp::MMCtrl::SyncRect(CocoView *view)
{
	NSWindow *win = [view window];
	view->ctrl->SetWndRect(DPI(1) *
	                       MakeScreenRect([win screen], [win contentRectForFrameRect: [win frame]]));
}

void Upp::AppendClipboardText(const String& s)
{
	CFRef<CFStringRef> cs = CFStringCreateWithCString(NULL, (const char *)~s.ToString(), kCFStringEncodingUTF8);
    [[NSPasteboard generalPasteboard] setString:(NSString *)~cs forType:NSPasteboardTypeString];
}

void Upp::AppendClipboardUnicodeText(const WString& s)
{
	AppendClipboardText(s.ToString());
}

Upp::ViewDraw::ViewDraw(Ctrl *ctrl)
{
	EnterGuiMutex();
	ASSERT(ctrl->top->coco);
	Rect tr = ctrl->GetTopCtrl()->GetScreenRect();
	Rect r = ctrl->GetScreenView();
	NSGraphicsContext *gc = [NSGraphicsContext graphicsContextWithWindow:ctrl->top->coco->window];
	Init([gc CGContext], NULL);

	CGContextTranslateCTM(cgHandle, 0, tr.GetHeight());
	CGContextScaleCTM(cgHandle, 1, -1);

	Clipoff(Rect(r.TopLeft() - tr.TopLeft(), r.GetSize()));
}

Upp::ViewDraw::~ViewDraw()
{
	End();
	CGContextFlush(cgHandle);
	LeaveGuiMutex();
}

#endif
