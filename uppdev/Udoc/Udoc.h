#ifndef _Udoc_Udoc_h
#define _Udoc_Udoc_h

#include <RichEdit/RichEdit.h>

#define LAYOUTFILE <Udoc/Udoc.lay>
#include <CtrlCore/lay.h>

#define IMAGEFILE <Udoc/Udoc.iml>
#include <Draw/iml_header.h>

class Udoc : public TopWindow {
	MenuBar    menu;
	ToolBar    tool;
	DropList   group;
	FileList   package;
	FileList   topic;
	RichEdit   editor;
	DropList   lang;
	EditString title;

	Splitter   package_topic;
	StaticRect left, right;
	Splitter   left_right;

	Vector<String> packagedir;

	String         filepath;
	Vector<String> grouptopics;

	void MainMenu(Bar& bar);
	void MainTool(Bar& bar);
	void EditBar(Bar& bar);
	void Exit();

public:
	void Serialize(Stream& s);

	void ClearPackages();
	void AddPackage(const char *name, const char *dir);

	typedef Udoc CLASSNAME;
	Udoc();
};

// appdoc$main$cs-cz.topic
// TOPIC("appdoc", "main", "cs-cz")
// #include "appdoc$main$cs-cz.topic"

#endif
