#include "ChromiumBrowserExample.h"

using namespace Upp;

#define IMAGECLASS IMG
#define IMAGEFILE <ChromiumBrowserExample/ChromiumBrowserExample.iml>
#include <Draw/iml.h>

#include "files.brc"


/* Table of Javascript functions that are forwarded to native code */
const char * const Upp::ChromiumBrowser::JSFunctions[]={
	"JSExample1",
	"JSExample2",
	/* DO NOT forget about nullptr at the end of the table */
	nullptr
};


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
	
	MessagesLog.AddColumn("Time");
	MessagesLog.AddColumn("URL");
	MessagesLog.AddColumn("Line");
	MessagesLog.AddColumn("Message");
	MessagesLog.ColumnWidths("1 1 1 4");
	MessagesLog.OddRowColor();
	MessagesLog.EvenRowColor();
	
	Browser.WhenUrlChange			= LAMBDA(String url) { Url.SetData(url); Url.CancelSelection(); };
	Browser.WhenStatus				= THISBACK(OnStatus);
	Browser.WhenTakeFocus			= LAMBDA() { Url.SetFocus(); };
	Browser.WhenKeyboard			= STDBACK(::ShowKeyboard);
	Browser.WhenConsoleMessage		= THISBACK(OnConsoleMessage);
	Browser.WhenMessage				= THISBACK(OnMessage);
	Browser.WhenCertificateError	= THISBACK(OnCertificateError);
	
	Back.WhenAction					= callback(&Browser, &ChromiumBrowser::GoBack);
	Forward.WhenAction				= callback(&Browser, &ChromiumBrowser::GoForward);
	Refresh.WhenAction				= callback(&Browser, &ChromiumBrowser::RefreshPage);
	Url.WhenEnter = Go.WhenAction	= LAMBDA() { Browser.Browse(~Url); };
	Stop.WhenAction					= callback(&Browser, &ChromiumBrowser::Stop);
	JSTests.WhenAction				= LAMBDA() { Browser.ShowHTML(String(test_page, test_page_length)); };

	//Delayed maximization - workaround of layout problem
	SetTimeCallback(200, THISBACK1(Maximize, false));
}


void ChromiumBrowserExample::OnStatus(bool loading, bool back, bool forward)
{
	Back.Enable(back);
	Forward.Enable(forward);
	Url.SetEditable(!loading);
	Go.Enable(!loading);
	Refresh.Enable(!loading);
	Stop.Enable(loading);
}


void ChromiumBrowserExample::OnConsoleMessage(String url, int line, String msg)
{
	MessagesLog.Add(GetSysTime(), url, line, msg);
	MessagesLog.ScrollEnd();
	if (MessagesLog.GetCount() > 100) MessagesLog.Remove(0);
}


void ChromiumBrowserExample::OnMessage(String name, const Vector<Value>& par)
{
	String tmp = "Native function executed by JS:&[* " + name + "(";
	for (int i = 0; i < par.GetCount(); i++){
		if (i > 0) tmp += ',';
		tmp += DeQtfLf(par[i].ToString());
	}
	tmp += ") ]&&After you press OK javascript function will be executed by native code";
	PromptOK(tmp);
	
	Browser.ExecuteJavaScript(Format("CallbackExample(%d);", (int)Random()));
}


bool ChromiumBrowserExample::OnCertificateError(String url)
{
	//return true to load page with invalid certificate
	return PromptOKCancel(Format(t_("Connection to '%s' is untrusted&Load page anyway?"), DeQtfLf(url)));
}


GUI_APP_MAIN
{
	StdLogSetup(LOG_FILE | LOG_CERR | LOG_TIMESTAMP | LOG_APPEND);
	SetLanguage( SetLNGCharset( GetSystemLNG(), CHARSET_UTF8 ) );
	
	if (ChromiumBrowser::IsChildProcess()){
		ChromiumBrowser::ChildProcess();
	}else{
		ChromiumBrowserExample().Run();
	}
}


