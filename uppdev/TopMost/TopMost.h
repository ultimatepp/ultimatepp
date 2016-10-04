#ifndef _TopMost_TopMost_h
#define _TopMost_TopMost_h

#include <CtrlLib/CtrlLib.h>

class TopMost : public TopWindow {
public:
	void SyncTitle()
	{
		Title(GetExStyle() & WS_EX_TOPMOST ? "TOPMOST" : "NORMAL");
	}
	virtual void LeftDown(Point, dword) { ExStyle(GetExStyle() ^ WS_EX_TOPMOST); SyncTitle(); }

	typedef TopMost CLASSNAME;
	TopMost();
};

#endif
