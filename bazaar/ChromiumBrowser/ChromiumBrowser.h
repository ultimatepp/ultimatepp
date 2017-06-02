#ifndef _KioskChromium_ChromeBrowser_h_
#define _KioskChromium_ChromeBrowser_h_

#include <CtrlLib/CtrlLib.h>
#include "ClientHandler.h"

namespace Upp{

class ChromiumBrowser : public DHCtrl {
private:
	CefRefPtr<ClientHandler> handler;

	String		start_page;
	String		tmp_dir;
	Callback	WhenGotFocus;
	
	virtual void State(int reason);
	virtual bool SetFocus();
	virtual void GotFocus();
	virtual void LostFocus();

	void AfterInit();
	void BeforeTerminate();
	void Layout();
	void MessageLoop();

public:
	typedef ChromiumBrowser CLASSNAME;
	ChromiumBrowser();
	~ChromiumBrowser();

	static const char * const JSFunctions[];
	Callback1<String>						WhenUrlChange;
	Callback3<bool, bool, bool>				WhenStatus;
	Callback2<String, const Vector<Value>&>	WhenMessage;
	Callback								WhenTakeFocus;
	Callback1<bool>							WhenKeyboard;
	Callback3<String, int, String>			WhenConsoleMessage;
	Gate1<String>							WhenCertificateError;
	
	static void ChildProcess();
	static bool IsChildProcess();
	
	ChromiumBrowser & StartPage(const char * url)	{start_page = url; return *this;}
	
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
