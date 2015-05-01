#include "ChromiumBrowser.h"
#include "ClientApp.h"

#include "include/cef_app.h"
#include "include/cef_version.h"

using namespace Upp;


#ifdef PLATFORM_LINUX
Vector<char*> GetArgs()
{
	const Vector<String>& cmdline = CommandLine();
	static String app;
	Vector<char*> argv;
	
	app = GetExeTitle();
	argv.Clear();
	argv.Add((char *)~app);
	for (unsigned i = 0; i < cmdline.GetCount(); i++){
		argv.Add((char *)~cmdline[i]);
	}
	return argv;
}
#endif


void ChromiumBrowser::ChildProcess()
{
	CefRefPtr<ClientApp> app(new ClientApp);
	
#ifdef PLATFORM_LINUX

	Vector<char *> args = GetArgs();
	RDUMP(args);
	CefMainArgs main_args(args.GetCount(), &args[0]);

#elif defined(PLATFORM_WIN32)

	CefMainArgs main_args(Ctrl::hInstance);

#else

	#error "Unsupported platform"

#endif
	
	int exit_code = CefExecuteProcess(main_args, app.get(), NULL);
  	if (exit_code >= 0){
    	RLOG("CefExecuteProcess error");
    	exit(exit_code);
  	}
}


bool ChromiumBrowser::IsChildProcess()
{
	const Vector<String>& cmdline = CommandLine();
	return (cmdline.GetCount() > 0 && cmdline[0].StartsWith("--type="));
}


ChromiumBrowser::ChromiumBrowser(): handler(NULL), start_page("about:blank")
{
	CefRefPtr<ClientApp> app(new ClientApp);
	
#ifdef PLATFORM_LINUX

	Vector<char *> args = GetArgs();
	RDUMP(args);
	CefMainArgs main_args(args.GetCount(), &args[0]);

#elif defined(PLATFORM_WIN32)

	CefMainArgs main_args(Ctrl::hInstance);

#else

	#error "Unsupported platform"

#endif
	
	int exit_code = CefExecuteProcess(main_args, app.get(), NULL);
  	if (exit_code >= 0){
    	RLOG("CefExecuteProcess error");
    	exit(exit_code);
  	}
  	
	CefSettings cef_settings;
	//cef_settings.multi_threaded_message_loop = true;
#ifdef _DEBUG
	cef_settings.log_severity = LOGSEVERITY_VERBOSE;
#else
	cef_settings.log_severity = LOGSEVERITY_DISABLE;
#endif
	tmp_dir = GetTempFileName();
	CefString(&cef_settings.cache_path) = ~tmp_dir;
  	CefInitialize(main_args, cef_settings, app.get(), NULL);
}


void ChromiumBrowser::MessageLoop()
{
	CefDoMessageLoopWork();
	SetTimeCallback(30, THISBACK(MessageLoop));
}


#ifdef PLATFORM_WIN32
LRESULT ChromiumBrowser::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		
		case WM_SIZE:
			Layout();
			break;

		case WM_ERASEBKGND:
			if (handler) {
				// Dont erase the background if the browser window has been loaded
				// (this avoids flashing)
				return 0;
			}
			break;

		case WM_CREATE:
			AfterInit(false);
			break;
			
		case WM_QUIT:
			//TODO: explain why we do not get QUIT message
			BeforeTerminate();
			break;
	}
	
	return DHCtrl::WindowProc(message, wParam, lParam);
}
#endif



void ChromiumBrowser::AfterInit(bool Error)
{
	handler = new ClientHandler(WhenUrlChange, WhenMessage, WhenTakeFocus, WhenKeyboard, WhenConsoleMessage);

	CefBrowserSettings br_settings;
    br_settings.file_access_from_file_urls = STATE_DISABLED;
    
	Size sz = GetSize();
 	CefWindowInfo info;

#ifdef PLATFORM_LINUX

    info.SetAsChild(hwnd, CefRect(0, 0, sz.cx, sz.cy));

#elif defined(PLATFORM_WIN32)

	RECT rect;
	rect.left = rect.top = 0;
	rect.right = sz.cx;
	rect.bottom = sz.cy;
	HWND hwnd = GetHWND();
	info.SetAsChild(hwnd, rect); 

#else

	#error "Unsupported platform"

#endif

	CefBrowserHost::CreateBrowser(info, handler.get(), ~start_page, br_settings, NULL);
	SetTimeCallback(50, THISBACK(MessageLoop));

#ifdef PLATFORM_WIN32

	EnableWindow(hwnd, true);

#endif

}


void ChromiumBrowser::BeforeTerminate()
{
	if (handler && handler->GetBrowser()) {
		handler->GetBrowser()->StopLoad();
		Sleep(20);
		handler->GetBrowser()->GetHost()->CloseBrowser(false);
	}

#ifdef PLATFORM_WIN32

	EnableWindow(GetHWND(), false);

#endif

	CefShutdown();
	DeleteFolderDeep(tmp_dir);
}


void ChromiumBrowser::Layout()
{
	if (handler && handler->GetBrowser()){
#ifdef PLATFORM_LINUX

		Size sz = GetSize();
		XWindowChanges change = {0};
		change.width = sz.cx;
		change.height = sz.cy;
		change.x = 0;
		change.y = 0;
		XConfigureWindow(Xdisplay, handler->GetBrowser()->GetHost()->GetWindowHandle(), CWHeight | CWWidth | CWY, &change);

#elif defined(PLATFORM_WIN32)

		CefWindowHandle hwnd = handler->GetBrowser()->GetHost()->GetWindowHandle();
		HDWP hdwp = BeginDeferWindowPos(1);
		Size sz = GetSize();
		hdwp = DeferWindowPos(hdwp, hwnd, NULL, 0, 0, sz.cx, sz.cy, SWP_NOZORDER);
		EndDeferWindowPos(hdwp);

#else

	#error "Unsupported platform"

#endif
	}
}


ChromiumBrowser::~ChromiumBrowser()
{
	BeforeTerminate();
}


void ChromiumBrowser::SetFocus0(bool focus)
{
	if (focus) DHCtrl::SetFocus();
	else DHCtrl::LostFocus();
	
	if (handler && handler->GetBrowser()) 
		handler->GetBrowser()->GetHost()->SendFocusEvent(focus);
}


void ChromiumBrowser::Browse(const String & url)
{
	if (handler && handler->GetBrowser()){
		handler->GetBrowser()->StopLoad();
		handler->GetBrowser()->GetMainFrame()->LoadURL(~url);
	}
}


void ChromiumBrowser::ShowHTML(const String& html)
{
	if (handler && handler->GetBrowser()) 
		handler->GetBrowser()->GetMainFrame()->LoadString(~html, "about:blank");
}


String ChromiumBrowser::GetLocation()
{
	if (handler && handler->GetBrowser()) 
		return String(handler->GetBrowser()->GetMainFrame()->GetURL());
	
	return String::GetVoid();
}


void ChromiumBrowser::GoBack()
{
	if (handler && handler->GetBrowser())
		handler->GetBrowser()->GoBack();
}


void ChromiumBrowser::RefreshPage()
{
	if (handler && handler->GetBrowser())
		handler->GetBrowser()->Reload();
}


void ChromiumBrowser::GoForward()
{
	if (handler && handler->GetBrowser())
		handler->GetBrowser()->GoForward();
}


void ChromiumBrowser::Stop()
{
	if (handler && handler->GetBrowser())
		handler->GetBrowser()->StopLoad();
}


String ChromiumBrowser::GetVersion()
{
	return Format("CEF %d.%d Chromium %d.%d.%d.%d", cef_version_info(0), cef_version_info(1),
	              cef_version_info(2), cef_version_info(3), cef_version_info(4), cef_version_info(5));
}


void ChromiumBrowser::ExecuteJavaScript(const char * js)
{
	if (handler && handler->GetBrowser()){
		CefRefPtr<CefFrame> frame = handler->GetBrowser()->GetMainFrame();
		frame->ExecuteJavaScript(js, frame->GetURL(), 0);
	}
}
