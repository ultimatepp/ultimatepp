#ifndef _DoubleClick_DoubleClick_h
#define _DoubleClick_DoubleClick_h

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <DoubleClick/DoubleClick.lay>
#include <CtrlCore/lay.h>



class DoubleClick : public WithDoubleClickLayout<TopWindow> {
public:
	typedef DoubleClick CLASSNAME;
	DoubleClick();
	void LeftDouble();
	void ShowSelected();

};

#endif
