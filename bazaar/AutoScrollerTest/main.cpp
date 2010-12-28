#include "AutoScrollerTest.h"

#define IMAGEFILE <AutoScrollerTest/AutoScrollerTest.iml>
#include <Draw/iml_source.h>

void AutoScrollerTest::Toggle()
{
	sc.EnableScroll(!sc.IsEnabledScroll());
}

AutoScrollerTest::AutoScrollerTest()
{
	CtrlLayout(*this, "Window title");
	Sizeable().Zoomable();

	CtrlLayout(sc.pane);

	//determine the total size of the scrolling pane
	//or updaten it, i.e. after CtrlLayout
	sc.SetPaneSize(sc.pane.GetSize());

	//add content to the scrolling pane, if not CtrlLayouted
	//sc.pane.Add(view);

	//add the AutoScroller itself to application
	Add(sc.SizePos());

	//do other setup stuff
	sc.pane.tg <<= THISBACK(Toggle);
	
	sc.pane.doc0 <<= "doc0";
	sc.pane.doc1 <<= "doc1";
	sc.pane.doc2 <<= "doc2";
	sc.pane.doc3 <<= "doc3";
}

GUI_APP_MAIN
{
	AutoScrollerTest().Run();
}

