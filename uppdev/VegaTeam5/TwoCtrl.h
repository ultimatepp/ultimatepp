#ifndef _ForlanoVega_TwoCtrl_h_
#define _ForlanoVega_TwoCtrl_h_

#include <CtrlLib/CtrlLib.h>

class Ctrl2 : public ParentCtrl {
	EditField edit;
	Button button;
public:
	typedef Ctrl2 CLASSNAME;
	Ctrl2();
	~Ctrl2() {;}
};
#endif
