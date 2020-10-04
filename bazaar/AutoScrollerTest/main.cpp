#include "AutoScrollerTest.h"

using namespace Upp;

#define IMAGEFILE <AutoScrollerTest/AutoScrollerTest.iml>
#include <Draw/iml_source.h>

void AutoScrollerTest::Toggle()
{
	scroller.EnableScroll(!scroller.IsScrollEnabled());
}

AutoScrollerTest::AutoScrollerTest()
{
	CtrlLayout(*this, "AutoScroller Test");
	Sizeable().Zoomable();
	Size sz = GetSize();

	scroller.EnableScroll();
	Add(scroller.SizePos());

	CtrlLayout(scroller_view);       // will determine desired size of pane
	scroller.AddPane(scroller_view); // size remains as specified, if view changes size, scroller.Layout() needs to be called.
	                                 // if scroller changes its size, it recalculates the scroll bars automatically

	// scroller.WhenScrolled = [=] {}; //after scrolling events

	scroller_view.toggle_button << [=] { Toggle(); };
	
	scroller_view.doc0 <<= "doc0";
	scroller_view.doc1 <<= "doc1";
	scroller_view.doc2 <<= "doc2";
	scroller_view.doc3 <<= "doc3";
}

GUI_APP_MAIN
{
	AutoScrollerTest().Run();
}

