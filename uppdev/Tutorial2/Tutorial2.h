#ifndef _Tutorial2_Tutorial2_h
#define _Tutorial2_Tutorial2_h

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <Tutorial2/Tutorial2.lay>
#include <CtrlCore/lay.h>



class Tutorial2 : public WithTutorial2Layout<TopWindow> {
public:
	typedef Tutorial2 CLASSNAME;
	Tutorial2();
};

#endif
