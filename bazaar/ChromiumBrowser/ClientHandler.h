#ifndef _ClinetHandler_h
#define _ClinetHandler_h

#include <CtrlLib/CtrlLib.h>
#undef LOG
#undef DLOG
#undef CHECK
#undef LOG_INFO
#undef LOG_WARNING
#undef ITEM

#include "include/cef_client.h"
#include "include/cef_version.h"

#ifndef CHROME_VERSION_BUILD
#error "Unknown CEF version"
#endif

class ClientHandler : public CefClient, public CefLifeSpanHandler, public CefDisplayHandler,
					public CefLoadHandler, public CefFocusHandler, public CefContextMenuHandler,
					public CefJSDialogHandler, public CefRequestHandler, public CefDialogHandler
{

public:
	typedef ClientHandler CLASSNAME;

	ClientHandler(	Upp::Callback1<Upp::String> & wuc,
					Upp::Callback3<bool, bool, bool> & wsc,
					Upp::Callback2<Upp::String, const Upp::Vector<Upp::Value>&> & wm,
					Upp::Callback & tf,
					Upp::Callback & gf,
					Upp::Callback1<bool> & wk,
					Upp::Callback3<Upp::String, int, Upp::String> & wcm,
					Upp::Gate1<Upp::String> & wce):
					browser(nullptr), WhenUrlChange(wuc), WhenStateChange(wsc), WhenMessage(wm),
					WhenTakeFocus(tf), WhenGotFocus(gf), WhenKeyboard(wk), WhenConsoleMessage(wcm),
					WhenCertificateError(wce) { }

	~ClientHandler() { }
	
	const Upp::Callback1<Upp::String> & WhenUrlChange;
	const Upp::Callback3<bool, bool, bool> & WhenStateChange;
	const Upp::Callback2<Upp::String, const Upp::Vector<Upp::Value>&> & WhenMessage;
	const Upp::Callback & WhenTakeFocus;
	const Upp::Callback & WhenGotFocus;
	const Upp::Callback1<bool> & WhenKeyboard;
	const Upp::Callback3<Upp::String, int, Upp::String> & WhenConsoleMessage;
	const Upp::Gate1<Upp::String> & WhenCertificateError;
	
	void WhenMessageWrapper(Upp::String name, Upp::Vector<Upp::Value> * par);

	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE			{ return this; }
	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE			{ return this; }
	virtual CefRefPtr<CefFocusHandler> GetFocusHandler() OVERRIDE				{ return this; }
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE					{ return this; }
	virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() OVERRIDE	{ return this; }
	virtual CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() OVERRIDE			{ return this; }
	virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE			{ return this; }
	virtual CefRefPtr<CefDialogHandler> GetDialogHandler() OVERRIDE				{ return this; }

	virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
								CefRefPtr<CefFrame> frame,
								const CefString& target_url,
								const CefString& target_frame_name,
								CefLifeSpanHandler::WindowOpenDisposition target_disposition,
								bool user_gesture,
								const CefPopupFeatures& popupFeatures,
								CefWindowInfo& windowInfo,
								CefRefPtr<CefClient>& client,
								CefBrowserSettings& settings,
								bool* no_javascript_access) OVERRIDE;

    virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
							CefRefPtr<CefFrame> frame,
                            ErrorCode errorCode,
                            const CefString& errorText,
                            const CefString& failedUrl) OVERRIDE;

    virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
  
	virtual void OnAddressChange(CefRefPtr<CefBrowser> browser,
							CefRefPtr<CefFrame> frame,
							const CefString& url) OVERRIDE;
	
	virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
							bool isLoading,
							bool canGoBack,
							bool canGoForward) OVERRIDE;
 
	virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser,
							cef_log_severity_t level,
							const CefString& message,
							const CefString& source,
							int line) OVERRIDE;

	virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                        CefProcessId source_process,
                                        CefRefPtr<CefProcessMessage> message) OVERRIDE;

	virtual void OnTakeFocus( CefRefPtr<CefBrowser> browser, bool next ) OVERRIDE;
	virtual void OnGotFocus(CefRefPtr<CefBrowser> browser) OVERRIDE;

	virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model) OVERRIDE;
                
    virtual bool OnJSDialog(CefRefPtr<CefBrowser> browser,
                          const CefString& origin_url,
                          JSDialogType dialog_type,
                          const CefString& message_text,
                          const CefString& default_prompt_text,
                          CefRefPtr<CefJSDialogCallback> callback,
                          bool& suppress_message) OVERRIDE;

    virtual bool OnCertificateError(CefRefPtr<CefBrowser> browser,
									ErrorCode cert_error,
									const CefString& request_url,
									CefRefPtr<CefSSLInfo> ssl_info,
									CefRefPtr<CefRequestCallback> callback) OVERRIDE;

	virtual bool OnFileDialog(CefRefPtr<CefBrowser> browser,
								FileDialogMode mode,
								const CefString& title,
								const CefString& default_file_path,
								const std::vector<CefString>& accept_filters,
								int selected_accept_filter,
								CefRefPtr<CefFileDialogCallback> callback) OVERRIDE;


	CefRefPtr<CefBrowser> GetBrowser() { return browser; }

protected:
	// The child browser window
	CefRefPtr<CefBrowser> browser;
    
	// Note that any of the IMPLEMENT_WHATEVER
	// macros that come with CEF can (and do) set
	// access modifiers, so you'll want them after
	// everything else in your class or you may be
	// in for a surprise when the access of a member
	// isn't what you expect it to be!
	IMPLEMENT_REFCOUNTING(ClientHandler);
};



#endif
