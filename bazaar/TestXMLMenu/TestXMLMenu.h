#ifndef _TestXMLMenu_TestXMLMenu_h
#define _TestXMLMenu_TestXMLMenu_h

#include <XMLMenu/XMLMenu.h>

using namespace Upp;

#define IMAGECLASS TestImg
#define IMAGEFILE <TestXMLMenu/TestXMLMenu.iml>
#include <Draw/iml_header.h>

#define LAYOUTFILE <TestXMLMenu/TestXMLMenu.lay>
#include <CtrlCore/lay.h>

struct Pane : public StaticRect
{
	virtual void RightDown(Point p, dword d) { Context(p, d); }
		
	Callback2<Point, dword> Context;
};

class TestXMLMenu : public WithTestXMLMenuLayout<TopWindow>
{
		// context menu handler
		virtual void RightDown(Point p, dword);

		void commandCb(XMLCommands &cmds);
		
		void quitCb(void) { Break(0); Close(); }
		
		DropList testDrop;
		
	public:
		typedef TestXMLMenu CLASSNAME;

		WithXMLMenu<Pane> menuCtrl;
		LineEdit bottomCtrl;

		TestXMLMenu();
		
};

#endif

