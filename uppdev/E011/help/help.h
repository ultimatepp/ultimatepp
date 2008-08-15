#ifndef _E011_help_h_
#define _E011_help_h_

#include <src/support/support.h>
#include <Core/Core.h>
#include <CtrlLib/CtrlLib.h>

#define IMAGEFILE <E011/help/help.iml>
#include <Draw/iml_header.h>

#define IMAGEFILE <E011/help/help.iml>
#include <Draw/iml_source.h>

#include <Topic/Topic.h>

class HelpWin : public TopWindow {
public:
	virtual bool Key(dword key, int);

private:
	RichTextView   view;
	TreeCtrl       tree;
	Splitter       tree_view;
	ToolBar        toolbar;
	Zoom           zoom;
	bool					 tree_enabled;

	struct Pos : Moveable<Pos> {
		String link;
		int    scy;
	};

	Vector<Pos>    back;
	Vector<Pos>    forward;
	Index<String>  tree_ndx;
	String         topic;
	String         label;

	bool 	GoTo0(const String& link, bool find = true);
	void 	Back();
	void 	Forward();
	void 	FontSize();
	void 	Tools(Bar& bar);
	void 	TreeCursor();
	void 	SetBar();
	void 	SetZoom();
	void 	Print();
	Pos  	GetPos();
	void 	SyncDocTree();

public:
	void 	GoTo(const String& link);

	void 	ClearTree();
	int  	AddTree(int parent, const Image& img, const String& topic, const String& title);
	void 	FinishTree();
	void	EnableTree(bool en=false)								{ tree_enabled = en; }
	bool 	IsTreeEnabled()													{ return tree_enabled; }
	void 	OpenHelp(const String& topic);
	void 	ShowTopics(const String& index);

	String GetCurrent() const                     { return topic; }
	String GetCurrentLabel() const                { return label; }

	typedef HelpWin CLASSNAME;

	HelpWin();
};

String GetTopicTitle(const String &path);
Topic AcquireTopic(const String& topic);

#endif
