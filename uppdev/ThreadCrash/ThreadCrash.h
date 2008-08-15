#ifndef _ThreadCrash_ThreadCrash_h
#define _ThreadCrash_ThreadCrash_h

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <ThreadCrash/ThreadCrash.lay>
#include <CtrlCore/lay.h>



class ThreadCrash : public WithThreadCrashLayout<TopWindow> {
public:
	typedef ThreadCrash CLASSNAME;
	ThreadCrash();
	ToolBar bar;
	Thread thr;
	ArrayCtrl arr;

	void MakeToolBar(Bar &bar);
	void RefreshOption();
	void RefreshOptionThr();
};

#endif
