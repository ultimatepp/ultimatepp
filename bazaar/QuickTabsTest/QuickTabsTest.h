#ifndef _App_h
#define _App_h

#define QUICKTABS_TEST
#include <CtrlLib/CtrlLib.h>
#include <QuickTabs/QuickTabs.h>

using namespace Upp;

#define LAYOUTFILE <QuickTabsTest/QuickTabsTest.lay>
#include <CtrlCore/lay.h>

Image IdeFileImage(const String& t, bool a);

class App : public WithQuickTabsLayout<TopWindow> 
{
	private:
		QuickTabs l_tabs;
		QuickTabs t_tabs;
		QuickTabs r_tabs;
		QuickTabs b_tabs;
	public:
		typedef App CLASSNAME;
		App();
		void Drag();
		void GoLastVis();
		
};

#endif

