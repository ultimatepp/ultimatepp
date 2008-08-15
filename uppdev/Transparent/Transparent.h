#ifndef _Transparent_Transparent_h
#define _Transparent_Transparent_h

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <Transparent/Transparent.lay>
#include <CtrlCore/lay.h>

class App : public WithAppLayout<TopWindow> {
public:
	typedef App CLASSNAME;
	App();
};

#endif
