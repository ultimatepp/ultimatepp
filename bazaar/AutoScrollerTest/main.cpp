#include "AutoScrollerTest.h"

#define IMAGEFILE <AutoScrollerTest/AutoScrollerTest.iml>
#include <Draw/iml_source.h>

void AutoScrollerTest::Toggle()
{
	sc.EnableScroll(!sc.IsEnabledScroll());
}

AutoScrollerTest::AutoScrollerTest()
{
	CtrlLayout(*this, "AutoScroller Test");
	Sizeable().Zoomable();
	Size sz = GetSize();

	sc.EnableScroll();
	Add(sc.SizePos());

	CtrlLayout(view); //will determine desired size of pane
	sc.AddPane(view); //size remains as specified, if view changes size, sc.Layout() needs to be called.
						//if sc changes its size, it recalculates the scroll bars automatically

	//sc.WhenScrolled = THISBACK(); //after scrolling events

	view.tg <<= THISBACK(Toggle);
	
	view.doc0 <<= "doc0";
	view.doc1 <<= "doc1";
	view.doc2 <<= "doc2";
	view.doc3 <<= "doc3";
}

GUI_APP_MAIN
{
	AutoScrollerTest().Run();
}

