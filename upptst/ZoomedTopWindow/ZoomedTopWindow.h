#ifndef _ZoomedTopWindow_ZoomedTopWindow_h
#define _ZoomedTopWindow_ZoomedTopWindow_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <ZoomedTopWindow/ZoomedTopWindow.lay>
#include <CtrlCore/lay.h>

class ZoomedTopWindow : public WithZoomedTopWindowLayout<TopWindow> {
public:
	typedef ZoomedTopWindow CLASSNAME;
	ZoomedTopWindow();
};

#endif
