#include "DispatcherTest.h"

MyPane::MyPane()
{
	CtrlLayout(*this);
	
	ei.MinMax(0, 100);
	pi.SetTotal(100);
	for(int i = 0; i < 100; i++)
		dl.Add(i);
}

void MyPane::Dispatch(const Value& o)
{
	ei.SetData(o);
	//pi.SetData(o);
	pi.Set(o);
	dl.SetData(o);
	st.SetText(String().Cat() << o);
}

void MyPane::Dispatch(const int& o)
{
	ei.SetData(o);
	//pi.SetData(o);
	pi.Set(o);
	dl.SetData(o);
	st.SetText(String().Cat() << o);
}

//

void DispatcherTest::sliderCB()
{
	Value o = sl.GetData();
	disp.DoDispatch(o);
}

void DispatcherTest::slideriCB()
{
	int o = sli.GetData();
	dispi.DoDispatch(o);
}

DispatcherTest::DispatcherTest()
{
	CtrlLayout(*this, "Window title");

	splith1.Horz()
		<< pane1 << pane2;
	splith2.Horz()
		<< pane3 << pane4;
	splitv.Vert()
		<< splith1 << splith2;
	Add(splitv.HSizePos().VSizePos(50, 0));
	Sizeable().Zoomable();

	disp.Register(pane1);
	disp.Register(pane2);
	disp.Register(pane3);
	disp.Register(pane4);

	dispi.Register(pane1);
	dispi.Register(pane2);
	dispi.Register(pane3);
	dispi.Register(pane4);

	sl.MinMax(0,100);
	sl <<= THISBACK(sliderCB);
	sli.MinMax(0,100);
	sli <<= THISBACK(slideriCB);

	//for beauty only, send once to see stuff already
	sl.SetData(75);
	sliderCB();

	sli.SetData(75);
}

GUI_APP_MAIN
{
	DispatcherTest().Run();
}

