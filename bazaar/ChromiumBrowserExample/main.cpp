#include "ChromiumBrowserExample.h"
#include "Utils.h"

using namespace Upp;

#define IMAGECLASS IMG
#define IMAGEFILE <ChromiumBrowserExample/ChromiumBrowserExample.iml>
#include <Draw/iml.h>


ChromiumBrowserExample::ChromiumBrowserExample()
{
	CtrlLayout(*this, "Embedded Chromium Example " + Browser.GetVersion());
	Sizeable().MaximizeBox();
	Icon(IMG::icon);
	
	Back.SetImage(IMG::back);
	Forward.SetImage(IMG::forward);
	Refresh.SetImage(IMG::refresh);
	Go.SetImage(IMG::go);
	Stop.SetImage(IMG::stop);
	
	Browser.WhenUrlChange		= THISBACK(OnUrlChange);
	Browser.WhenTakeFocus		= THISBACK(OnTakeFocus);
	Browser.WhenKeyboard		= STDBACK(::ShowKeyboard);
	Browser.WhenConsoleMessage	= THISBACK(OnConsoleMessage);
	
	Back.WhenAction				= callback(&Browser, &ChromiumBrowser::GoBack);
	Forward.WhenAction			= callback(&Browser, &ChromiumBrowser::GoForward);
	Refresh.WhenAction			= callback(&Browser, &ChromiumBrowser::RefreshPage);
	Url.WhenEnter				= THISBACK(OnBrowse);
	Go.WhenAction				= THISBACK(OnBrowse);
	Stop.WhenAction				= callback(&Browser, &ChromiumBrowser::Stop);

	Maximize();
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


