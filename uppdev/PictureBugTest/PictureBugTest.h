#ifndef _PictureBugTest_PictureBugTest_h
#define _PictureBugTest_PictureBugTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <PictureBugTest/PictureBugTest.lay>
#include <CtrlCore/lay.h>



class PictureBugTest : public WithPictureBugTestLayout<TopWindow> {
public:
	typedef PictureBugTest CLASSNAME;
	PictureBugTest();
	void Open();
};

#endif

