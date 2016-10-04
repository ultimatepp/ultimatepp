#ifndef _UApplication_MainWorkSpace_h_
#define _UApplication_MainWorkSpace_h_

#include <CtrlLib/CtrlLib.h>

namespace UA{

using namespace UPP;

#define IMAGECLASS UAImg
#define IMAGEFILE <UApplication/UApplication.iml>
#include <Draw/iml_header.h>

#define LAYOUTFILE <UApplication/UApplication.lay>
#include <CtrlCore/lay.h>

#define TFILE <UApplication/UApplication.t>
#include <Core/t.h>

class MainWorkSpace : public WithUApplicationLayout<TopWindow> {
public:
	typedef MainWorkSpace CLASSNAME;
	MainWorkSpace();
	void Exit();
	void Options();
	void About();
#ifdef PLATFORM_WIN32	
	// Overriding default WindowProc
	virtual LRESULT  WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
#endif		
private:
	enum { LAUNCH };
	MenuBar menu;
	StatusBar status;
	int currentLanguage;
	ArrayMap<int, WString> suportedLanguagies;
	void mainMenu(Bar& CurrentBar);	
		void menuFile(Bar& CurrentBar);
		void menuTools(Bar& CurrentBar);
		void menuHelp(Bar& CurrentBar);
	void trayMenu(Bar& CurrentBar);
	void refreshData();
	void hide();
	void launch();
	void perform(int NewState);
	TrayIcon trayicon;
	bool trayiconvisible;
	bool trayiconAlwaysShowTrayIcon;
	bool hideInTrayIconOnMinimize;
	bool hideInTrayIconOnClose;
	bool isHidden;
	Callback whenMinimize;
	void onMinimize();
	void onClose();
};
};

#endif
