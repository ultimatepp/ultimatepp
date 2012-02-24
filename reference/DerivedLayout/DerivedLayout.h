#ifndef _DerivedLayout_DerivedLayout_h
#define _DerivedLayout_DerivedLayout_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <DerivedLayout/DerivedLayout.lay>
#include <CtrlCore/lay.h>

struct Base : public WithBaseLayout<TopWindow> {
	Base();
};

struct Derived : public WithDerivedLayout<Base> {
	Derived();
};

#endif
