#ifndef _KioskChromium_ChromeBrowser_h_
#define _KioskChromium_ChromeBrowser_h_

#include <CtrlLib/CtrlLib.h>
#include "ClientHandler.h"

namespace Upp{

class ChromiumBrowser : public DHCtrl {
private:
	CefRefPtr<ClientHandler> handler;

	String start_page;
	String tmp_dir;
	virtual void AfterInit(bool Error);
	virtual void BeforeTerminate();
	virtual void Layout();
	virtual void SetFocus0(bool focus);
	virtual void GotFocus()						{SetFocus0(true);}
	virtual void LostFocus()					{SetFocus0(false);}

	void MessageLoop();	
#ifdef PLATFORM_WIN32
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
#endif 

	
public:
	typedef ChromiumBrowser CLASSNAME;
	ChromiumBrowser();
	~ChromiumBrowser();
	
	Callback1<String>								WhenUrlChange;
	Callback2<String, WithDeepCopy<Vector<Value> > >	WhenMessage;
	Callback										WhenTakeFocus;
	Callback1<bool>									WhenKeyboard;
	Callback3<Upp::String, int, Upp::String>		WhenConsoleMessage;
	
	static void ChildProcess();
	static bool IsChildProcess();
	
	ChromiumBrowser & StartPage(const char * url)			{start_page = url; return *this;}
	
	void Browse(const String & url);
	void ShowHTML(const String& html);
	String GetLocation();
	void GoBack();
	void GoForward();
	void Stop();
	void RefreshPage();
	String GetVersion();
	void ExecuteJavaScript(const char * js);
};

}

#endif
