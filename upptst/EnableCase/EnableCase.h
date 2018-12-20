#ifndef _EnableCase_EnableCase_h
#define _EnableCase_EnableCase_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <EnableCase/EnableCase.lay>
#include <CtrlCore/lay.h>

class EnableCase : public WithEnableCaseLayout<TopWindow> {
public:
	typedef EnableCase CLASSNAME;
	EnableCase();
};

#endif
