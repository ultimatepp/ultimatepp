#include "TabBarCtrlTest.h"

TabBarTest::TabBarTest()
{
	CtrlLayout(*this, "TabBarCtrl Test");
	CtrlLayout(layout1);
	CtrlLayout(layout2);
	CtrlLayout(layout3);
	Sizeable().Zoomable();
	
	tabs <<= THISBACK(OnTabCursor);
	layout2.add <<= THISBACK(OnAdd);
	layout3.grouping <<= THISBACK(OnGrouping);
	layout3.groupsort <<= THISBACK(OnGroupSort);
	layout3.stacking <<= THISBACK(OnStacking);
	layout3.stacksort <<= THISBACK(OnStackSort);
	layout3.sortonce <<= THISBACK(OnTabsSortOnce);
	layout3.sortorder <<= THISBACK(OnTabsSortOnce);
	layout3.keepsorted <<= THISBACK(OnTabSort);
	layout3.sb_thickness <<= THISBACK(OnSBThickness);
	layout3.groupseps <<= THISBACK(OnGroupSeps);
	layout3.variantcrosses <<= THISBACK(OnChooseCrosses);
	layout3.emptyseps <<= THISBACK(OnEmptySeps);

	layout2.newvalue <<= "New Item";
	layout2.newkey <<= "A Key";
	layout2.newgroup <<= "Group 4";
	layout3.stacksort <<= true;
	layout3.grouping <<= true;
	for(int i = 1; i < 11; i++)
		layout3.sb_thickness.Add(i, i);	
	layout3.sb_thickness <<= 2;

	layout3.sortorder.Add("Forwards");
	layout3.sortorder.Add("Backwards");
	layout3.sortorder.SetIndex(0);

	for(int i = 6; i < 9; i++)
		tabs.AddKey(i, "Item " + FormatInt(i), Null, "Group 2");
	for(int i = 9; i < 11; i++)
		tabs.AddKey(i, "Item " + FormatInt(i), Null, "Group 3");

	Add(tabs.SizePos());
	tabs.AddCtrl(layout1, 1, "Layout 1", Null, "Group 1");
	tabs.AddCtrl(layout2, 2, "Layout 2", Null, "Group 1");
	tabs.AddCtrl(layout3, 3, "Layout 3", Null, "Group 2");
	
	tabs.AllowReorder(false);
}

void TabBarTest::OnGroupSeps()
{
	tabs.GroupSeparators(layout3.groupseps);
}

void TabBarTest::OnChooseCrosses()
{
	if (layout3.variantcrosses)
		TabBar::StyleDefault().Write().Variant1Crosses();
	else
		TabBar::StyleDefault().Write().DefaultCrosses();		
	tabs.Refresh();
}

void TabBarTest::OnEmptySeps()
{
	if (layout3.emptyseps)
		TabBar::StyleDefault().Write().NoGroupSeparators();
	else
		TabBar::StyleDefault().Write().DefaultGroupSeparators();
	tabs.Refresh();
}

void TabBarTest::OnSBThickness()
{
	tabs.SetScrollThickness(~layout3.sb_thickness);
}

void TabBarTest::OnTabsSortOnce()
{
	tabs.SortTabValuesOnce(GetSortOrder());
}

void TabBarTest::OnTabSort()
{
	if (layout3.keepsorted)
		tabs.SortTabValues(GetSortOrder());
	else
		tabs.SortTabs(false);
}

void TabBarTest::OnGrouping()
{
	tabs.Grouping(layout3.grouping);
}

void TabBarTest::OnGroupSort()
{
	tabs.SortGroups(layout3.groupsort);
}

void TabBarTest::OnStacking()
{
	tabs.Stacking(layout3.stacking);
}

void TabBarTest::OnStackSort()
{
	tabs.SortStacks(layout3.stacksort);
}

void TabBarTest::OnTabCursor()
{
	Value v = tabs.GetData();
	if (!IsNull(v)) {
		layout1.selected_key = (String)StdConvert().Format(tabs.GetData());
		layout1.selected_value = (String)StdConvert().Format(tabs.GetValue(tabs.GetCursor()));
	}
	else {
		layout1.selected_key = "";
		layout1.selected_value = "";
	}
	layout1.selected_index.SetLabel(AsString(tabs.GetCursor()));
}

void TabBarTest::OnAdd()
{
	if (layout2.newvalue.GetLength() == 0)
		PromptOK("You must supply a value");
	tabs.AddKey(Nvl(~layout2.newkey, ~layout2.newvalue), ~layout2.newvalue, Null, ~layout2.newgroup);
}

ValueOrder & TabBarTest::GetSortOrder()
{
	switch (layout3.sortorder.GetIndex()) {
	case 1:
		return backward;
	}
	return forward;
}

GUI_APP_MAIN
{
	TabBarTest tabBarTest;
	
	tabBarTest.Run();
}

