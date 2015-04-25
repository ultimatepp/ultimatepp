#ifndef _ChromiumBrowserExample_ChromiumBrowserExample_h
#define _ChromiumBrowserExample_ChromiumBrowserExample_h

#include <CtrlLib/CtrlLib.h>
#include <ChromiumBrowser/ChromiumBrowser.h>

namespace Upp{

#define LAYOUTFILE <ChromiumBrowserExample/ChromiumBrowserExample.lay>
#include <CtrlCore/lay.h>

class ChromiumBrowserExample : public WithChromiumBrowserExampleLayout<TopWindow> {
	
private:
	
	void OnUrlChnage(String url) 							{ Url.SetData(url); Url.CancelSelection(); }
	void OnTakeFocus()										{ Url.SetFocus(); }
	void OnConsoleMessage(String url, int line, String msg) { RLOG(Format("Console message: url=%s, line=%d, msg=%s", url, line, msg)); }
	void OnBrowser() 										{ Browser.Browse(~Url); }

public:
	typedef ChromiumBrowserExample CLASSNAME;
	ChromiumBrowserExample();
};

}

#endif
