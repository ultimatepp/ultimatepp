#ifndef _CtrlLibTest_CtrlLibTest_h
#define _CtrlLibTest_CtrlLibTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <CtrlLibTest/CtrlLibTest.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS MyIm
#define IMAGEFILE <CtrlLibTest/CtrlLibTest.iml>
#include <Draw/iml_header.h>

class CtrlLibTest : public WithCtrlLibTestLayout<TopWindow> {
public:
	typedef CtrlLibTest CLASSNAME;
	CtrlLibTest();
};

#endif


