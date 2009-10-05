#define LAYOUTFILE <SysInfo_demo_gui/SysInfo_demo_gui.lay>
#include <CtrlCore/lay.h>

#define IMAGEFILE <SysInfo_demo_gui/SysInfo_demo_gui.iml>
#include <Draw/iml_header.h>

struct SpecialFolders : WithSpecialFolders<StaticRect> {

	typedef SpecialFolders CLASSNAME;

	SpecialFolders()
	{
		CtrlLayout(*this);
	}
	void Fill();
	void ButInstalledSoftware_Push();
};
struct SystemInfo : WithSystemInfo<StaticRect> {

	typedef SystemInfo CLASSNAME;

	SystemInfo()
	{
		CtrlLayout(*this);
	}
	void Fill();
	void ButUpdate_Push();
};
struct WindowsList_ : WithWindowsList<StaticRect> {

	typedef WindowsList_ CLASSNAME;

	WindowsList_()
	{
		CtrlLayout(*this);
	}
	void Fill();
	void ButUpdate_Push();
};
struct ProcessList : WithProcessList<StaticRect> {

	typedef ProcessList CLASSNAME;

	ProcessList()
	{
		CtrlLayout(*this);
	}
	void Fill();
	void ButUpdate_Push();
};
struct ScreenGrabTab : WithScreenGrabTab<StaticRect> {

	typedef ScreenGrabTab CLASSNAME;

	ScreenGrabTab()
	{
		CtrlLayout(*this);
	}
	void Fill();
	void ButGrab_Push();
	void ButSnap_Push();
	void SwGrabMode_Action();
};

class SysInfoDemo : public TopWindow {
public:
	typedef SysInfoDemo CLASSNAME;
	
	SysInfoDemo();
	
	MenuBar 		menu;
	StatusBar  		info;
	
	TabCtrl			filesTab;
	SpecialFolders 	specialFolders;	
	SystemInfo		systemInfo;
	ProcessList		processList;
	WindowsList_	windowsList;
	ScreenGrabTab	screenGrab;
	
	// Menus
	void MainMenu(Bar& bar);
	void FileMenu(Bar& bar);
	void About();
	
	void Exit();
};	


