#ifndef _TabBarTest_TabBarTest_h
#define _TabBarTest_TabBarTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <TabBar/TabBar.h>

#define LAYOUTFILE <TabBarCtrlTest/TabBarCtrlTest.lay>
#include <CtrlCore/lay.h>

class TabBarTest : public WithTabBarTestLayout<TopWindow>
{
	struct ReverseValueOrder : public StdValueOrder
	{
		virtual bool operator()(const Value& a, const Value& b) const 
			{ return StdValueCompare(b, a, language) < 0; }	
	};
	
	private:
		TabBarCtrl tabs;
		StdValueOrder forward;
		ReverseValueOrder backward;
		
		WithLayout1<ParentCtrl> layout1;
		WithLayout2<ParentCtrl> layout2;
		WithLayout3<ParentCtrl> layout3;
		
		void OnTabsSortOnce();
		void OnTabSort();
		void OnGrouping();
		void OnGroupSort();
		void OnStacking();
		void OnStackSort();
		void OnTabCursor();
		void OnAdd();
		void OnSBThickness();
		void OnGroupSeps();
		
		void OnChooseCrosses();
		void OnEmptySeps();
		
		ValueOrder &GetSortOrder();
	public:
		typedef TabBarTest CLASSNAME;

		TabBarTest();
};

#endif

