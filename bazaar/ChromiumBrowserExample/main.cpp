#include "ChromiumBrowserExample.h"
#include "Utils.h"

#define IMAGECLASS IMG
#define IMAGEFILE <ChromiumBrowserExample/ChromiumBrowserExample.iml>
#include <Draw/iml.h>


ChromiumBrowserExample::ChromiumBrowserExample()
{
	CtrlLayout(*this, "Embedded Chromium Example");
	Sizeable().MaximizeBox();
	Icon(IMG::icon);
	
	Back.SetImage(IMG::back);
	Forward.SetImage(IMG::forward);
	Refresh.SetImage(IMG::refresh);
	Go.SetImage(IMG::go);
	Stop.SetImage(IMG::stop);
	
	Browser.WhenUrlChange		= LAMBDA(String url) { Url.SetData(url); Url.CancelSelection(); };
	Browser.WhenTakeFocus		= LAMBDA() { Url.SetFocus(); };
	Browser.WhenKeyboard		= STDBACK(::ShowKeyboard);
	Browser.WhenMessage			= LAMBDA(String msg) { RLOG(msg);};
	Browser.WhenConsoleMessage	= LAMBDA(String url, int line, String msg) {RLOG(Format("Console message: url=%s, line=%d, msg=%s", url, line, msg));};
	
	Back.WhenAction				= callback(&Browser, &ChromiumBrowser::GoBack);
	Forward.WhenAction			= callback(&Browser, &ChromiumBrowser::GoForward);
	Refresh.WhenAction			= callback(&Browser, &ChromiumBrowser::RefreshPage);
	Url.WhenEnter				= LAMBDA() { Browser.Browse(~Url); };
	Go.WhenAction				= LAMBDA() { Browser.Browse(~Url); };
	Stop.WhenAction				= callback(&Browser, &ChromiumBrowser::Stop);
}


GUI_APP_MAIN
{
	StdLogSetup(LOG_FILE | LOG_CERR | LOG_TIMESTAMP | LOG_APPEND);
	SetLanguage( SetLNGCharset( GetSystemLNG(), CHARSET_UTF8 ) );
	
	if (ChromiumBrowser::IsChildProcess()){
		ChromiumBrowser::ChildProcess();
	}else{
		ChromiumBrowserExample().Run();
		::ShowKeyboard(false);
	}
}
