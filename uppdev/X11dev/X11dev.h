#ifndef _X11dev_X11dev_h
#define _X11dev_X11dev_h

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <X11dev/X11dev.lay>
#include <CtrlCore/lay.h>



class X11dev : public WithX11devLayout<TopWindow> {
public:
	virtual void Paint(Draw& w);

	typedef X11dev CLASSNAME;
	X11dev();
};

#endif
