#ifndef _Nil_Nil_h
#define _Nil_Nil_h

#include <CtrlLib/CtrlLib.h>

namespace Upp {
	
struct NilGUI : VirtualGui {
	virtual dword       GetOptions();
	virtual Size        GetSize();
	virtual dword       GetMouseButtons();
	virtual dword       GetModKeys();
	virtual bool        IsMouseIn();
	virtual bool        ProcessEvent(bool *quit);
	virtual void        WaitEvent(int ms);
	virtual bool        IsWaitingEvent();
	virtual void        WakeUpGuiThread();
	virtual void        SetMouseCursor(const Image& image);
	virtual SystemDraw& BeginDraw();
	virtual void        CommitDraw();
	virtual void        Quit();

	NilDraw       nildraw;
	SystemDraw    sysdraw;
};

}

#endif
