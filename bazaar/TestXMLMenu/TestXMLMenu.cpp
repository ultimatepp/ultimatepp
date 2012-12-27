#include "TestXMLMenu.h"

using namespace Upp;

#define IMAGECLASS TestImg
#define IMAGEFILE <TestXMLMenu/TestXMLMenu.iml>
#include <Draw/iml_source.h>

static void dummyCb(void)
{
}

// commands generator callback
void TestXMLMenu::commandCb(XMLCommands &cmds)
{
	cmds
		.Add("New"			, callback(dummyCb))
		.Add("NewCalc"		, callback(dummyCb))
		.Add("Open"			, callback(dummyCb))
		.Add("Save"			, callback(dummyCb))
		.Add("SaveAs"		, callback(dummyCb))
		.Add("JobInfo"		, callback(dummyCb))
		.Add("PrintPreview"	, callback(dummyCb))
		.Add("Print"		, callback(dummyCb))
		.Add("Next"			, callback(dummyCb))
		.Add("Previous"		, "PREVIOUS")	// custom command
		.Add("Settings"		, "SETTINGS")	// custom command
		.Add("Help"			, "HELP")		// custom command
		.Add("Quit"			, THISBACK(quitCb))
		.Add("Exit"			, callback(dummyCb))
		.Add("Flag"			, callback(dummyCb))
		.Add("Remove"		, callback(dummyCb))
		.Add("Delete"		, callback(dummyCb))
		.Add("ListAdd"		, callback(dummyCb))
		.Add("ListRemove"	, callback(dummyCb))
		.Add("RtfImport"	, callback(dummyCb))
		.Add("TestControl"	, testDrop, Size(150, 0))
	;
}

// menu structure callback
void menuCb(XMLToolBars &tb)
{
	tb
		.Add("Main", tb.MenuBar()
			.Add("File", tb.SubMenu()
				.Add("New"		, t_("New")		, TestImg::New())
				.Add("Open"		, t_("Open")	, TestImg::Open())
				.Add("Save"		, t_("Save")	, TestImg::Save())
				.Add("SaveAs"	, t_("SaveAs")	, TestImg::SaveAs())
				.Add("Quit"		, t_("Quit")	, TestImg::Quit())
			)
			.Add("Calc", tb.SubMenu()
				.Add("NewCalc"	, t_("NewCalc")	, TestImg::NewCalc())
			)
			.Add("Printer", tb.SubMenu()
				.Add("Print", t_("Print"), TestImg::Print())
				.Add("PrintPreview", t_("PrintPreview"), TestImg::PrintPreview())
			)
			.Add("Other", tb.SubMenu()
				.Add("Next"			, TestImg::Next())
				.Add("Previous"		, TestImg::Previous())
				.Add("Settings"		, TestImg::Settings())
				.Add("Help"			, TestImg::Help())
				.Add("More"	, tb.SubMenu()
					.Add("Flag"			, TestImg::Flag())
					.Add("Remove"		, TestImg::Remove())
					.Add("Delete"		, TestImg::Delete())
				)
				.Add("Exit"			, TestImg::Exit())
			)
		)
		.Add("Context", tb.MenuBar()
			.Add("ListAdd"		, TestImg::ListAdd())
			.Add("ListRemove"	, TestImg::ListRemove())
			.Add("RtfImport"	, TestImg::RtfImport())
		)
	;
}

// toolbars structure callback
void toolBarsCb(XMLToolBars &tb)
{
	tb
		.Add("File", tb.ToolBar(TOOLBAR_TOP, 0, 0)
			.Add("New"		, t_("New")		, TestImg::New())
			.Add("Open"		, t_("Open")	, TestImg::Open())
			.Add("Save"		, t_("Save")	, TestImg::Save())
			.Add("SaveAs"	, t_("SaveAs")	, TestImg::SaveAs())
			.Add("Quit"		, t_("Quit")	, TestImg::Quit())
		
		)
		.Add("Calc", tb.ToolBar(TOOLBAR_TOP, 10, 0)
			.Add("NewCalc"	, t_("NewCalc")	, TestImg::NewCalc())
			.Add("TestControl")
		)
		.Add("Printer", tb.ToolBar(TOOLBAR_LEFT, 0, 0)
			.Add("Print", t_("Print"), TestImg::Print())
			.Add("PrintPreview", t_("PrintPreview"), TestImg::PrintPreview())
		)
		.Add("Other", tb.ToolBar(TOOLBAR_BOTTOM, 0, 0)
			.Add("Next"			, TestImg::Next())
			.Add("Previous"		, TestImg::Previous())
			.Add("Settings"		, TestImg::Settings())
			.Add("Help"			, TestImg::Help())
			.Add("Separator"	, tb.SubMenu()
				.Add("Flag"			, TestImg::Flag())
				.Add("Remove"		, TestImg::Remove())
				.Add("Delete"		, TestImg::Delete())
				.Add("ListAdd"		, TestImg::ListAdd())
				.Add("ListRemove"	, TestImg::ListRemove())
				.Add("RtfImport"	, TestImg::RtfImport())
			)
			.Add("Exit"			, TestImg::Exit())
		)
		     
	;
}

TestXMLMenu::TestXMLMenu()
{
	CtrlLayout(*this);
	splitter.Vert(menuCtrl.SizePos(), bottomCtrl.SizePos());
	splitter.SetPos(8000);
	menuCtrl.Color(Black());
	
	testDrop
		.Add("Some lines")
		.Add("Inside DropList")
		.Add("To show")
		.Add("Embedded controls")
		.Add("In custom menus")
		.SetIndex(0)
	;

	// adds built-in commands
	menuCtrl.SetCommands(THISBACK(commandCb));
	
	// build default menu structure
	menuCtrl.SetMenuBars(STDBACK(menuCb));
	
	// build default toolbars structure
	menuCtrl.SetToolBars(STDBACK(toolBarsCb));
	
	menuCtrl.Context = THISBACK(RightDown);
	
	// don't allow dock right
//	menuCtrl.NoDockBottom();
	
}

// context menu handler
void TestXMLMenu::RightDown(Point p, dword)
{
	MenuBar *menu = menuCtrl.GetContextMenu("Context");
	if(menu)
		menu->Execute();
}

GUI_APP_MAIN
{

	// create window with dockable menus/toolbars
	TestXMLMenu testXMLMenu;

	String path = AppendFileName(GetHomeDirectory(), "TestXMLMenu.xml");
	if(FileExists(path))
		LoadFromXMLFile(testXMLMenu.menuCtrl, path);
	
	testXMLMenu.Sizeable().Zoomable();
	testXMLMenu.Run();
	
	StoreAsXMLFile(testXMLMenu.menuCtrl, "SavedMenu", path);
}

