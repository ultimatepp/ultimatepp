#include "CMeterTest.h"

CMeterTest::CMeterTest()
{
	CtrlLayout(*this, "Window title");

	metersl.MinMax(-100,100).SetData(0);
	metersl <<= THISBACK(metersl_CB);
	REV <<= THISBACK(meterrev_CB);
	VERT <<= THISBACK(metervert_CB);
	STEP <<= THISBACK(meterstep_CB);
	STEPLIN <<= THISBACK(metersteplin_CB);
	PERC <<= THISBACK(meterperc_CB);

	mode.Add(CMeter::IMGCACHE, "IMGCACHE");
	mode.Add(CMeter::STATIC, "STATIC");
	mode.Add(CMeter::GRADIENT, "GRADIENT");
	mode.Add(CMeter::DYNGRADIENT, "DYNGRADIENT");
	mode <<= THISBACK(metermodeCB);

	double min = metersl.GetMin();
	double max = metersl.GetMax();
	
	sledit.MinMax(min, max);
	sledit <<= THISBACK(editCB);
	
	//setup the marks
	Vector<double> vmarks;
	vmarks.Add(max);
	vmarks.Add(max-(max-min)*0.1);
	vmarks.Add(max-(max-min)*0.2);
	vmarks.Add(max-(max-min)*0.3);
	vmarks.Add(max-(max-min)*0.4);
	vmarks.Add(max-(max-min)*0.7);

	meter <<= THISBACK1(meterCB, &meter);
	meter2 <<= THISBACK1(meterCB, &meter2);

	meter.ShowValue(false);
	//meter2.ShowValue(false);

	meter.Min(min).Max(max).Steps(vmarks, true).Set(min);  //use reverse order, meter needs starting vector with min
	meter2.Min(min).Max(max).Steps(vmarks, true).Set(min);  //use reverse order, meter needs starting vector with min

	Vector<CMeter::CVMark> cv;
	cv.Add().v = min;
	//cv.Add().v = -75.0;
	cv.Add().v = -50.0;
	//cv.Add().v = -25.0;
	cv.Add().v = 0.0;
	//cv.Add().v = 25.0;
	cv.Add().v = 50.0;
	//cv.Add().v = 75.0;
	cv.Add().v = max;
	meter.cvmarks <<= cv;

	cv.Clear();

	for(int i = 0; i < vmarks.GetCount(); i++)
		cv.Add().v = (double)(i);

	meter2.cvmarks <<= cv;

	meter2.SetFont(meter2.GetFont().Height(8));

	Add(meter.LeftPos(10,50).VSizePos(10,70));
	Add(meter2.HSizePos(70, 10).BottomPos(10, 50));
	
	metersl_CB();
}

void CMeterTest::meterCB(const CMeter * m)
{
	Value v = m->GetData();
	metersl.SetData(v);
	if(m == &meter) meter2.SetData(v);
	else meter.SetData(v);
	sledit.SetData(v);
}

void CMeterTest::metersl_CB()
{
	Value v = metersl.GetData();
	sledit.SetData(v);
	meter.SetData(v);
	meter2.SetData(v);
}

void CMeterTest::editCB()
{
	Value v = sledit.GetData();
	sledit.SetData(v);
	meter.SetData(v);
	meter2.SetData(v);
}

void CMeterTest::meterrev_CB()
{
	meter.Reverse(REV.GetData()).Refresh();
	meter2.Reverse(REV.GetData()).Refresh();
}

void CMeterTest::metervert_CB()
{
	meter.Vertical(VERT.GetData()).Refresh();
	meter2.Vertical(VERT.GetData()).Refresh();
}

void CMeterTest::meterstep_CB()
{
	meter.Stepping(STEP.GetData()).Refresh();
	meter2.Stepping(STEP.GetData()).Refresh();
}

void CMeterTest::metersteplin_CB()
{
	meter.StepLinear(STEPLIN.GetData()).Refresh();
	meter2.StepLinear(STEPLIN.GetData()).Refresh();
}

void CMeterTest::meterperc_CB()
{
	meter.ShowPercent(PERC.GetData()).Refresh();
	meter2.ShowPercent(PERC.GetData()).Refresh();
}

void CMeterTest::metermodeCB()
{
	int m = mode.GetData();
	meter.SetMode(m).Refresh();
	meter2.SetMode(m).Refresh();
}

GUI_APP_MAIN
{
	CMeterTest().Sizeable().Zoomable().Run();
}

