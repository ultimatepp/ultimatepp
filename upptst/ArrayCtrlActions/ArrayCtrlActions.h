#ifndef _ArrayCtrlActions_ArrayCtrlActions_h
#define _ArrayCtrlActions_ArrayCtrlActions_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <ArrayCtrlActions/ArrayCtrlActions.lay>
#include <CtrlCore/lay.h>

class ArrayCtrlActions : public WithArrayCtrlActionsLayout<TopWindow> {
public:
	typedef ArrayCtrlActions CLASSNAME;
	ArrayCtrlActions();
};

#endif
