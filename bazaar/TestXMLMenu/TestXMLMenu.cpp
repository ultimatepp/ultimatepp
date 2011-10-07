#include "TestXMLMenu.h"

using namespace Upp;

#define IMAGECLASS TestImg
#define IMAGEFILE <TestXMLMenu/TestXMLMenu.iml>
#include <Draw/iml_source.h>

TestXMLMenu::TestXMLMenu()
{
}

static void dummyCb(void)
{
}

// testing callback -- to be removed once XML is in place
static void toolBarCb(Bar &bar)
{
	bar.Add(t_("New")			, TestImg::New()			, callback(dummyCb));
	bar.Add(t_("NewCalc")		, TestImg::NewCalc()		, callback(dummyCb));
	bar.Add(t_("Open")			, TestImg::Open()			, callback(dummyCb));
	bar.Add(t_("Save")			, TestImg::Save()			, callback(dummyCb));
	bar.Add(t_("SaveAs")		, TestImg::SaveAs()			, callback(dummyCb));
	bar.Add(t_("JobInfo")		, TestImg::JobInfo()		, callback(dummyCb));
	bar.Add(t_("PrintPreview")	, TestImg::PrintPreview()	, callback(dummyCb));
	bar.Add(t_("Print")			, TestImg::Print()			, callback(dummyCb));
	bar.Add(t_("Next")			, TestImg::Next()			, callback(dummyCb));
	bar.Add(t_("Previous")		, TestImg::Previous()		, callback(dummyCb));
/*
	bar.Add(t_("Settings")		, TestImg::Settings()		, callback(dummyCb));
	bar.Add(t_("Help")			, TestImg::Help()			, callback(dummyCb));
	bar.Add(t_("Quit")			, TestImg::Quit()			, callback(dummyCb));
	bar.Add(t_("Exit")			, TestImg::Exit()			, callback(dummyCb));
	bar.Add(t_("Flag")			, TestImg::Flag()			, callback(dummyCb));
	bar.Add(t_("Remove")		, TestImg::Remove()			, callback(dummyCb));
	bar.Add(t_("Delete")		, TestImg::Delete()			, callback(dummyCb));
	bar.Add(t_("ListAdd")		, TestImg::ListAdd()		, callback(dummyCb));
	bar.Add(t_("ListRemove")	, TestImg::ListRemove()		, callback(dummyCb));
	bar.Add(t_("RtfImport")		, TestImg::RtfImport()		, callback(dummyCb));
*/
}

GUI_APP_MAIN
{

/*
	DragWindow w;
	w.Sizeable().Zoomable();
	w.Run();
*/
	TestXMLMenu testXMLMenu;
	
	testXMLMenu.GetMenuBar().Set(STDBACK(toolBarCb));
	
	XMLToolBar *tb;
	
	tb = new XMLToolBar(&testXMLMenu);
	tb->Set(STDBACK(toolBarCb));
	testXMLMenu.AddTop(tb, 0, 50);
	
	tb = new XMLToolBar(&testXMLMenu);
	tb->Set(STDBACK(toolBarCb));
	testXMLMenu.AddTop(tb, 0, 550);
	
	tb = new XMLToolBar(&testXMLMenu);
	tb->Set(STDBACK(toolBarCb));
	testXMLMenu.AddTop(tb, 1, 300);
	
	tb = new XMLToolBar(&testXMLMenu);
	tb->Set(STDBACK(toolBarCb));
	testXMLMenu.AddLeft(tb, 0, 50);
	
	tb = new XMLToolBar(&testXMLMenu);
	tb->Set(STDBACK(toolBarCb));
	testXMLMenu.AddRight(tb, 0, 100);
	
	tb = new XMLToolBar(&testXMLMenu);
	tb->Set(STDBACK(toolBarCb));
	testXMLMenu.AddBottom(tb, 0, 200);
	
	testXMLMenu.Sizeable().Zoomable();
	testXMLMenu.Run();
}

