#ifndef _ChromiumBrowserExample_ChromiumBrowserExample_h
#define _ChromiumBrowserExample_ChromiumBrowserExample_h

#include <CtrlLib/CtrlLib.h>
#include <ChromiumBrowser/ChromiumBrowser.h>
#include "Utils.h"

namespace Upp{

#define LAYOUTFILE <ChromiumBrowserExample/ChromiumBrowserExample.lay>
#include <CtrlCore/lay.h>

class ChromiumBrowserExample : public WithChromiumBrowserExampleLayout<TopWindow> {
	
private:

	void Close()													{ Upp::ShowKeyboard(false); TopWindow::Close(); }	
	void OnUrlChange(String url) 									{ Url.SetData(url); Url.CancelSelection(); }
	void OnTakeFocus()												{ Url.SetFocus(); }
	void OnConsoleMessage(String url, int line, String msg) 		{ RLOG(Format("Console message: url=%s, line=%d, msg=%s", url, line, msg)); }
	void OnBrowse() 												{ Browser.Browse(~Url); }
	void OnMessage(String name, WithDeepCopy<Vector<Value> > par);
	void OnJSTests();

public:
	typedef ChromiumBrowserExample CLASSNAME;
	ChromiumBrowserExample();
};

}

#endif
