#ifndef _FileTabsExample_FileTabsExample_h
#define _FileTabsExample_FileTabsExample_h

#include <CtrlLib/CtrlLib.h>
#include <TabBar/TabBar.h>
using namespace Upp;

#define LAYOUTFILE <FileTabsExample/FileTabsExample.lay>
#include <CtrlCore/lay.h>

class FileTabsExample : public WithFileTabsExampleLayout<TopWindow> {
private:
	FileTabs bar[4];
	String barbackup;	
public:
	typedef FileTabsExample CLASSNAME;
	FileTabsExample();
	
	// TabBar events
	void OnCursor();
	bool OnCloseConfirm(Value v);
	bool OnCloseAllConfirm();
	// Buttons
	void OnAdd();
	void OnInsert();
	// Options
	void OnGrouping();
	void OnGroupSort();
	void OnSeparators();
	void OnStacking();	
	void OnIcons();
	void OnPrompt();
	void OnAllAlign();
	void OnInactive();
	// Ctrl overloads
	virtual bool Key(dword key, int count);
	virtual void DragAndDrop(Point p, PasteClip& d);
};

#endif

