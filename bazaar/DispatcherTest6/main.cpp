#include "DispatcherTest.h"

void DispatcherTest::sliderCB()
{
	Value o = sl.GetData();
	disp.DoDispatch(o);
}

void DispatcherTest::slideriCB()
{
	int o = sli.GetData();
	disp.DoDispatch(o);
}

void DispatcherTest::Dispatch(const Value & o)
{
	int a = 0;
}

DispatcherTest::DispatcherTest()
{
	CtrlLayout(*this, "Window title");

	splitv1.Vert();
	int i = 0;
	for(; i < 10; i++)
	{
		MyEditInt* pctrl = new MyEditInt();
		ctrls.Add(pctrl);
		pctrl->MinMax(0, 100);

		disp.Register<Value>(callback(pctrl, &MyEditInt::DispatchV), i);
		disp.Register<int>(callback(pctrl, &MyEditInt::DispatchI), 100+i);
		
		//disp.Unregister<Value>(i);
		//disp.Unregister<int>(100+i);
		
		splitv1 << *pctrl;
	}

	splitv2.Vert();
	for(; i < 20; i++)
	{
		MyProgressIndicator* pctrl = new MyProgressIndicator();
		ctrls.Add(pctrl);
		pctrl->SetTotal(100);

		disp.Register<Value>(callback(pctrl, &MyProgressIndicator::DispatchV), i);
		disp.Register<int>(callback(pctrl, &MyProgressIndicator::DispatchI), 100+i);

		//disp.Unregister<Value>(i);
		//disp.Unregister<int>(100+i);

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

DispatcherTest::~DispatcherTest()
{
	for(int i = 0; i < 20; i++)
	{
		disp.Unregister<Value>(i);
		disp.Unregister<int>(100+i);
	}
}
GUI_APP_MAIN
{
	DispatcherTest().Run();
}

