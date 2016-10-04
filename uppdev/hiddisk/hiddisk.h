#ifndef _hiddisk_hiddisk_h
#define _hiddisk_hiddisk_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <hiddisk/hiddisk.lay>
#include <CtrlCore/lay.h>



class hiddisk : public WithhiddiskLayout<TopWindow> {
public:
	typedef hiddisk CLASSNAME;
	hiddisk();
private:
	void start();
	void stop();
	void setup();
	void uninst();
};

#endif

