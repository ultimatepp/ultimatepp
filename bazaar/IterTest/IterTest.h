#ifndef _IterTest_IterTest_h
#define _IterTest_IterTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <IterTest/IterTest.lay>
#include <CtrlCore/lay.h>

#include <Iter/Iter.h>

class Foo : public Link<Foo>
{
public:
	int d;
};


class IterTest : public WithIterTestLayout<TopWindow> {
public:
	typedef IterTest CLASSNAME;
	IterTest();
};

#endif

