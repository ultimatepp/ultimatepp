#ifndef _SkylarkGuiExample_SkylarkGuiExample_h
#define _SkylarkGuiExample_SkylarkGuiExample_h

#include <CtrlLib/CtrlLib.h>
#include "WebInterface.h"

using namespace Upp;

#define LAYOUTFILE <SkylarkGuiExample/SkylarkGuiExample.lay>
#include <CtrlCore/lay.h>

class SkylarkGuiExample : public WithSkylarkGuiExampleLayout<TopWindow> {
	
	static Callback1<int> WhenSignal;
	WebInterface web;
	
	static void SignalHandler(int signal);

public:
	typedef SkylarkGuiExample CLASSNAME;
	void Run();
	SkylarkGuiExample();
};

#endif
