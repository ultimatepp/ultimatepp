#include "CocoMM.h"

#define LLOG(x)

NSAutoreleasePool *main_coco_pool;

void Upp::CocoInit(int argc, const char **argv, const char **envptr)
{
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
}

void Upp::CocoExit()
{
	[main_coco_pool release];
}

bool Upp::Ctrl::IsWaitingEvent()
{
	Upp::AutoreleasePool __;
	bool b = [NSApp nextEventMatchingMask:NSEventMaskAny
	                untilDate:[NSDate date]
	                inMode:NSDefaultRunLoopMode dequeue:NO];
	return b;
}

bool Upp::Ctrl::ProcessEvent(bool *)
{
	ASSERT(IsMainThread());

	Upp::AutoreleasePool __;

	ONCELOCK {
		[NSApp finishLaunching];
	}
	
	NSEvent *event = [NSApp nextEventMatchingMask:NSEventMaskAny
	                  untilDate:[NSDate distantFuture]
	                  inMode:NSDefaultRunLoopMode
	                  dequeue:YES];
	
	[NSApp sendEvent:event];
	[NSApp updateWindows];

	return true;
}

void SweepMkImageCache();

bool Upp::Ctrl::ProcessEvents(bool *quit)
{
	if(ProcessEvent(quit)) {
		while(ProcessEvent(quit) && (!LoopCtrl || LoopCtrl->InLoop())); // LoopCtrl-MF 071008
		TimerProc(GetTickCount());
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
	LLOG("Entering event loop at level " << LoopLevel << BeginIndent);
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
		GuiSleep(1000);
//		if(EndSession()) break;
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / ProcessEvents");
		ProcessEvents(&quit);
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / after ProcessEvents");
	}

	if(ctrl)
		LoopCtrl = ploop;
	LoopLevel--;
	LLOG(EndIndent << "Leaving event loop ");
}

void Upp::Ctrl::GuiSleep(int ms)
{
	GuiLock __;
	Sleep(ms); // TODO!
/*	ASSERT(IsMainThread());
	ELOG("GuiSleep");
	if(EndSession())
		return;
	ELOG("GuiSleep 2");
	int level = LeaveGMutexAll();
#if !defined(flagDLL) && !defined(PLATFORM_WINCE)
	if(!OverwatchThread) {
		DWORD dummy;
		OverwatchThread = CreateThread(NULL, 0x100000, Win32OverwatchThread, NULL, 0, &dummy);
		ELOG("ExitLoopEventWait 1");
		ExitLoopEvent.Wait();
	}
	HANDLE h[1];
	*h = ExitLoopEvent.GetHandle();
	ELOG("ExitLoopEventWait 2 " << (void *)*h);
	MsgWaitForMultipleObjects(1, h, FALSE, ms, QS_ALLINPUT);
#else
	MsgWaitForMultipleObjects(0, NULL, FALSE, ms, QS_ALLINPUT);
#endif
	EnterGMutex(level);*/
}
