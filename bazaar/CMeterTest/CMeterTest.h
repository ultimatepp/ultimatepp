#ifndef _CMeterTest_CMeterTest_h
#define _CMeterTest_CMeterTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <CMeterTest/CMeterTest.lay>
#include <CtrlCore/lay.h>

#include <CMeter/CMeter.h>

class CMeterTest : public WithCMeterTestLayout<TopWindow> {
public:
	typedef CMeterTest CLASSNAME;
	CMeterTest();

	void meterrev_CB();
	void metervert_CB();
	void meterstep_CB();
	void meterperc_CB();
	void metersteplin_CB();
	void metermodeCB();

	void meterCB(const CMeter * m);
	void metersl_CB();
	void editCB();

	CMeter meter, meter2;
};

#endif

