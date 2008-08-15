#ifndef _ArrayCtrlEdit_ArrayCtrlEdit_h
#define _ArrayCtrlEdit_ArrayCtrlEdit_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <ArrayCtrlEdit/ArrayCtrlEdit.lay>
#include <CtrlCore/lay.h>



class ArrayCtrlEdit : public WithArrayCtrlEditLayout<TopWindow> {
public:
	typedef ArrayCtrlEdit CLASSNAME;
	ArrayCtrlEdit();
};

#endif
