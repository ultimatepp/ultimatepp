#include "ChromiumBrowser.h"
#include "ClientApp.h"

#include "include/cef_app.h"
#include "include/cef_version.h"

#if defined(GUI_GTK) && defined(GDK_WINDOWING_X11)
#include <gdk/gdkx.h>
#endif

using namespace Upp;


#ifdef COMPILER_GCC
const char * const __attribute__((weak)) Upp::ChromiumBrowser::JSFunctions[]={
	/* DO NOT forget about nullptr at the end of the table */
	nullptr
};
#endif


#ifdef PLATFORM_POSIX
static Vector<char*> GetArgs()
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
	
#ifdef PLATFORM_POSIX

	Vector<char *> args = GetArgs();
	CefMainArgs main_args(args.GetCount(), &args[0]);

#elif defined(PLATFORM_WIN32)

	//Uninitialize COM, Chromium will take care of it
	CoUninitialize();
	CefMainArgs main_args(Ctrl::hInstance);

#else

	#error "Unsupported platform"

#endif
	
	int exit_code = CefExecuteProcess(main_args, app.get(), nullptr);
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


ChromiumBrowser::ChromiumBrowser(): handler(nullptr), start_page("about:blank")
{
	CefRefPtr<ClientApp> app(new ClientApp);
	
#ifdef PLATFORM_POSIX

	Vector<char *> args = GetArgs();
	CefMainArgs main_args(args.GetCount(), &args[0]);

#elif defined(PLATFORM_WIN32)

	CefMainArgs main_args(Ctrl::hInstance);

#else

	#error "Unsupported platform"

#endif
	
	int exit_code = CefExecuteProcess(main_args, app.get(), nullptr);
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
	CefInitialize(main_args, cef_settings, app.get(), nullptr);
}


void ChromiumBrowser::MessageLoop()
{
	CefDoMessageLoopWork();
	SetTimeCallback(30, THISBACK(MessageLoop));
}


void ChromiumBrowser::State(int reason)
{
	switch(reason){

		case FOCUS:
		case ACTIVATE:
		case DEACTIVATE:
		case SHOW:
		case ENABLE:
			break;
			
		case OPEN:
			AfterInit();
			break;
			
		case CLOSE:
			BeforeTerminate();
			break;
			
		case LAYOUTPOS:
		case POSITION:
			Layout();
			break;
	}
}


void ChromiumBrowser::AfterInit()
{
	WhenGotFocus << [this](){ SetFocus(); };
	
	handler = new ClientHandler(WhenUrlChange, WhenStatus, WhenMessage, WhenTakeFocus,
								WhenGotFocus, WhenKeyboard, WhenConsoleMessage, WhenCertificateError);

	CefBrowserSettings br_settings;
    br_settings.file_access_from_file_urls = STATE_DISABLED;
    
	Rect r = GetRect();
	CefWindowInfo info;

#ifdef GUI_X11

    info.SetAsChild(GetParentWindow(), CefRect(r.left, r.top, r.Width(), r.Height()));

#elif defined(GUI_GTK) && defined(GDK_WINDOWING_X11)

	info.SetAsChild(gdk_x11_drawable_get_xid(gtk_widget_get_window(GTK_WIDGET(GetTopCtrl()->gtk()))),
					CefRect(r.left, r.top, r.Width(), r.Height()));

#elif defined(GUI_WIN32)

	RECT rect;
	rect.left = r.left;
	rect.top = r.top;
	rect.right = r.Width();
	rect.bottom = r.Height();
	info.SetAsChild(GetTopCtrl()->GetHWND(), rect);

#else

	#error "Unsupported platform"

#endif

	CefBrowserHost::CreateBrowser(info, handler.get(), ~start_page, br_settings, nullptr);
	SetTimeCallback(50, THISBACK(MessageLoop));
}


void ChromiumBrowser::BeforeTerminate()
{
	if (handler && handler->GetBrowser()) {
		handler->GetBrowser()->StopLoad();
		Sleep(20);
		handler->GetBrowser()->GetHost()->CloseBrowser(false);
	}

	CefShutdown();
	DeleteFolderDeep(tmp_dir);
}


void ChromiumBrowser::Layout()
{
	if (handler && handler->GetBrowser()){

		Rect r = GetRect();
		
#ifdef GUI_X11

		XWindowChanges change = {0};
		change.x = r.left;
		change.y = r.top;
		change.width = r.Width();
		change.height = r.Height();
		XConfigureWindow(Xdisplay, handler->GetBrowser()->GetHost()->GetWindowHandle(), CWHeight | CWWidth | CWY, &change);

#elif defined(GUI_GTK) && defined(GDK_WINDOWING_X11)

		XWindowChanges change = {0};
		change.x = r.left;
		change.y = r.top;
		change.width = r.Width();
		change.height = r.Height();
		XConfigureWindow(GDK_DISPLAY_XDISPLAY(gdk_display_get_default()), handler->GetBrowser()->GetHost()->GetWindowHandle(), CWHeight | CWWidth | CWY, &change);

#elif defined(GUI_WIN32)

		CefWindowHandle hwnd = handler->GetBrowser()->GetHost()->GetWindowHandle();
		HDWP hdwp = BeginDeferWindowPos(1);
		hdwp = DeferWindowPos(hdwp, hwnd, nullptr, r.left, r.top, r.Width(), r.Height(), SWP_NOZORDER);
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


bool ChromiumBrowser::SetFocus()
{
	if (handler && handler->GetBrowser())
		handler->GetBrowser()->GetHost()->SendFocusEvent(true);
	return true;
}


void ChromiumBrowser::GotFocus()
{
	if (handler && handler->GetBrowser())
		handler->GetBrowser()->GetHost()->SendFocusEvent(true);
}


void ChromiumBrowser::LostFocus()
{
	if (handler && handler->GetBrowser())
		handler->GetBrowser()->GetHost()->SendFocusEvent(false);
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
