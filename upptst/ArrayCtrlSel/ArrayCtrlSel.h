#ifndef _ArrayCtrlSel_ArrayCtrlSel_h
#define _ArrayCtrlSel_ArrayCtrlSel_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <ArrayCtrlSel/ArrayCtrlSel.lay>
#include <CtrlCore/lay.h>

class ArrayCtrlSel : public WithArrayCtrlSelLayout<TopWindow> {
public:
	int serial = 0;

	typedef ArrayCtrlSel CLASSNAME;
	ArrayCtrlSel();
};

#endif
