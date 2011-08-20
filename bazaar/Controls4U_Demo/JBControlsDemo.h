#ifndef _JBControlsDemo_JBControlsDemo_h
#define _JBControlsDemo_JBControlsDemo_h

#include <CtrlLib/CtrlLib.h>
#include <Controls4U/Controls4U.h>

using namespace Upp;

#define LAYOUTFILE <Controls4U_Demo/JBControlsDemo.lay>
#include <CtrlCore/lay.h>

class JBControlsDemo : public WithJBControlsDemoLayout<StaticRect> {
public:
	typedef JBControlsDemo CLASSNAME;
	JBControlsDemo();
	void NoVotes();
	void Slide();
	void Slide2();
	void VSlide();
};

#endif

