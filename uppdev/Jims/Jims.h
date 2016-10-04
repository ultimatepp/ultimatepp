#ifndef _Jims_Jims_h
#define _Jims_Jims_h

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <Jims/Jims.lay>
#include <CtrlCore/lay.h>



class Jims : public WithJimsLayout<TopWindow> {
public:
	typedef Jims CLASSNAME;
	Jims();
};

#endif
