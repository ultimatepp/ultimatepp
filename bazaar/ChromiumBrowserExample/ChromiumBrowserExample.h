#ifndef _ChromiumBrowserExample_ChromiumBrowserExample_h
#define _ChromiumBrowserExample_ChromiumBrowserExample_h

#include <CtrlLib/CtrlLib.h>
#include <ChromiumBrowser/ChromiumBrowser.h>

using namespace Upp;

#define LAYOUTFILE <ChromiumBrowserExample/ChromiumBrowserExample.lay>
#include <CtrlCore/lay.h>

class ChromiumBrowserExample : public WithChromiumBrowserExampleLayout<TopWindow> {
public:
	typedef ChromiumBrowserExample CLASSNAME;
	ChromiumBrowserExample();
};

#endif
