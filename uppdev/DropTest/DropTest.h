#ifndef _DropTest_DropTest_h
#define _DropTest_DropTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <DropTest/DropTest.lay>
#include <CtrlCore/lay.h>

class DropTest : public WithDropTestLayout<TopWindow> {
public:
	typedef DropTest CLASSNAME;

//	MyFrameTop<DropList> drop;
	FrameTop<DropList> drop;

	DropTest();
};

#endif
