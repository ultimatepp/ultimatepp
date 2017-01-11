#ifndef _Days_Days_h
#define _Days_Days_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <Days/Days.lay>
#include <CtrlCore/lay.h>

class Days : public WithDaysLayout<TopWindow> {
public:
	typedef Days CLASSNAME;
	Days();
};

#endif
