#include "DispatcherTest.h"

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

	splitv1.Vert();
	for(int i = 0; i < 10; i++)
	{
		MyEditInt* pctrl = new MyEditInt();
		ctrls.Add(pctrl);
		pctrl->MinMax(0, 100);
		disp.Register(*pctrl);
		dispi.Register(*pctrl);

		//disp.Unregister(*pctrl);
		//dispi.Unregister(*pctrl);

		splitv1 << *pctrl;
	}

	splitv2.Vert();
	for(int i = 0; i < 10; i++)
	{
		MyProgressIndicator* pctrl = new MyProgressIndicator();
		ctrls.Add(pctrl);
		pctrl->SetTotal(100);
		disp.Register(*pctrl);
		dispi.Register(*pctrl);
		
		//disp.Unregister(*pctrl);
		//dispi.Unregister(*pctrl);
		
		splitv2 << *pctrl;
	}
	
	splith.Horz()
		<< splitv1 << splitv2;

	Add(splith.HSizePos().VSizePos(50, 0));
	Sizeable().Zoomable();

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

