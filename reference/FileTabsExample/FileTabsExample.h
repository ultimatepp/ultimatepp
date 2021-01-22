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
	void OnValueSort();
	void OnKeySort();
	void OnSeparators();
	void OnStacking();
	void OnStackSort();	
	void OnIcons();
	void OnPrompt();
	void OnAllAlign();
	void OnInactive();
	// Ctrl overloads
	virtual bool Key(dword key, int count);
	virtual void DragAndDrop(Point p, PasteClip& d);
	
	struct MyValueOrder : public ValueOrder
	{
		virtual bool operator()(const Value& a, const Value& b) const { return (Random()%2); }
	};

	StdValueOrder vo;
	MyValueOrder myvo;
};

#endif

