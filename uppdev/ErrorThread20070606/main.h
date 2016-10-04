#ifndef _ErrorThread_main_h_
#define _ErrorThread_main_h_

#include <CtrlLib/CtrlLib.h>
#include <Web/Web.h>
#include "displays.h"

using namespace Upp;

class App : public TopWindow
{
	typedef App CLASSNAME;

public:
	/* APP */
	App();
	
	/* MENU */
    MenuBar menu;
	void MenuContent(MenuBar& menu);
	void MenuContent_File(Bar& bar);

	/* TAB */
    TabCtrl tab;	
	void TabContent(TabCtrl& tab);
	
	/* CONTENT - DOWNLOAD */
	StaticRect contentDownload;
	void Content_Download(StaticRect& contentDownload);
	Button downloadButton;
	void ActionDownloadStart();
	Button cancelDownloadButton;
	void ActionDownloadStop(bool clearResult);
	DocEdit downloadResult;
	
	/* STATUS */
    StatusBar status;
    void StatusContent(StatusBar& status);
	void StatusUpdate();
	InfoCtrl statusInfo;
	
	/* STATUS PROGRESS */
	ProgressDisplayMy progressDisplay;
	bool ProgressCanceled(int pos, int size);
	bool isProgressCanceled;
    ProgressIndicator pi;

	/* ACTIONS */
	void ActionExit();
	virtual bool Key(dword key, int count);

public:
	Thread httpThread;
};

#endif
