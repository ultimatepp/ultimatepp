#ifndef _DarkTheme_DarkTheme_h
#define _DarkTheme_DarkTheme_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <DarkTheme/DarkTheme.lay>
#include <CtrlCore/lay.h>

class DarkTheme : public WithDarkThemeLayout<TopWindow> {
public:
	typedef DarkTheme CLASSNAME;
	DarkTheme();
};

#endif
