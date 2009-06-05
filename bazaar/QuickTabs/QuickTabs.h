#ifndef _QuickTabs_QuickTabs_h
#define _QuickTabs_QuickTabs_h

#include <TabBar/TabBar.h>
using namespace Upp;

class QuickTabs : public FileTabs
{
		void OnCloseAll() { WhenCloseRest(); }	
		virtual void ContextMenu(Bar& bar);
		void SettingsMenu(Bar &bar);
	public:
		typedef QuickTabs CLASSNAME;
		Callback WhenCloseRest;

		QuickTabs();

		void Add(const char *name, bool make_active = false);

		QuickTabs& FileIcons(bool b = true) { TabBar::Icons(b); return *this; }

		int    GetCount() const 			{ return tabs.GetCount(); }
		int    GetCursor() const 			{ return active; }
		String GetFile(int n) const 		{ return tabs[n].key; }
		bool   FindSetFile(const String& fn);
		void   SetAddFile(const String& fn);
		void   RenameFile(const String& fn, const String& nn);
		void   Set(const QuickTabs& t);
		
		void ToggleStacking() 			{ Stacking(!IsStacking()); }
		void ToggleGroupSort() 			{ GroupSort(!IsGroupSort()); }
		void ToggleGroupSeparators() 	{ GroupSeparators(!HasGroupSeparators()); }
		void ToggleInactive() 			{ InactiveDisabled(!IsShowInactive()); }
};

#endif
