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

	float min = (float)metersl.GetMin();
	float max = (float)metersl.GetMax();
	
	//setup the marks
	Vector<float> vmarks;
	vmarks.Add(max);
	vmarks.Add(max-(max-min)*0.1f);
	vmarks.Add(max-(max-min)*0.2f);
	vmarks.Add(max-(max-min)*0.3f);
	vmarks.Add(max-(max-min)*0.4f);
	vmarks.Add(max-(max-min)*0.7f);

	meter <<= THISBACK(meterCB);
	meter.Min(min).Max(max).Steps(vmarks, true).Set(min);  //use reverse order, meter needs starting vector with min
	Add(meter.LeftPos(10,50).TopPos(10,200));
}

void CMeterTest::meterCB()
{
	double f = meter.GetData();
	metersl.SetData(f);
}

void CMeterTest::metersl_CB()
{
	sledit.SetData(metersl.GetData());
	meter.Set((float)(double)metersl.GetData());
}

void CMeterTest::meterrev_CB()
{
	meter.Reverse(REV.GetData()).Refresh();
}

void CMeterTest::metervert_CB()
{
	meter.Vertical(VERT.GetData()).Refresh();
}

void CMeterTest::meterstep_CB()
{
	meter.Stepping(STEP.GetData()).Refresh();
}
void CMeterTest::metersteplin_CB()
{
	meter.StepLinear(STEPLIN.GetData()).Refresh();
}
void CMeterTest::meterperc_CB()
{
	meter.ShowPercent(PERC.GetData()).Refresh();
}

void CMeterTest::metermodeCB()
{
	int m = mode.GetData();
	meter.SetMode(m).Refresh();
}

GUI_APP_MAIN
{
	CMeterTest().Run();
}

