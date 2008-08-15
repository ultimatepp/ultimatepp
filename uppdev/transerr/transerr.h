#ifndef _transerr_transerr_h
#define _transerr_transerr_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <transerr/transerr.lay>
#include <CtrlCore/lay.h>

class TabPageCtrl : public StaticRect {
public:
	TabPageCtrl() { Color(Null); /*Blend(SColorFace(), SColorPaper()));*/ }
};

class TransTest : public WithTransTestLayout<TopWindow> {
public:
	typedef TransTest CLASSNAME;
	TransTest();

	WithTab1Layout<TabPageCtrl> tab1;
	WithTab2Layout<TabPageCtrl> tab2;
};

#endif
