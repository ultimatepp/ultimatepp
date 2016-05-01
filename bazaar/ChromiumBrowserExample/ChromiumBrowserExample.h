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
	void OnMessage(String name, const Vector<Value>& par);
	void OnStatus(bool loading, bool back, bool forward);
	void OnConsoleMessage(String url, int line, String msg);
	bool OnCertificateError(String url);

public:
	typedef ChromiumBrowserExample CLASSNAME;
	ChromiumBrowserExample();
};

}

#endif
