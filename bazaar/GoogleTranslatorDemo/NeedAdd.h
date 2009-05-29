#ifndef _GoogleTranslatorDemo_NeedAdd_h_
#define _GoogleTranslatorDemo_NeedAdd_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class TopWindowSpecific : public TopWindow{
public:
	typedef TopWindowSpecific CLASSNAME;
#ifdef PLATFORM_WIN32	
	// Overriding default WindowProc
	virtual LRESULT  WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
#endif
	//... Need to add this event for other plathorms		
	Callback WhenMinimize;
};

#endif
