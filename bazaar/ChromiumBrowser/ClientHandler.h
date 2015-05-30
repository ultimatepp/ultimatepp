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
					public CefJSDialogHandler
{

public:
	ClientHandler(	Upp::Callback1<Upp::String> & wuc, 
					Upp::Callback2<Upp::String, Upp::WithDeepCopy<Upp::Vector<Upp::Value> > > & wm,
					Upp::Callback & tf,
					Upp::Callback1<bool> & wk,
					Upp::Callback3<Upp::String, int, Upp::String> & wcm): 
					browser(NULL), WhenUrlChange(wuc), WhenMessage(wm), WhenTakeFocus(tf),
					WhenKeyboard(wk), WhenConsoleMessage(wcm) { }

	~ClientHandler() { }
	
	Upp::Callback1<Upp::String> & WhenUrlChange;
	Upp::Callback2<Upp::String, Upp::WithDeepCopy<Upp::Vector<Upp::Value> > > & WhenMessage;
	Upp::Callback & WhenTakeFocus;
	Upp::Callback1<bool> & WhenKeyboard;
	Upp::Callback3<Upp::String, int, Upp::String> & WhenConsoleMessage;

    virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE			{ return this; }
   	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE			{ return this; }
	virtual CefRefPtr<CefFocusHandler> GetFocusHandler() OVERRIDE				{ return this; }
//	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE					{ return this; }
	virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() OVERRIDE	{ return this; }
	virtual CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() OVERRIDE			{ return this; }

#if (CHROME_VERSION_BUILD >= 2357)
	virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
                             CefRefPtr<CefFrame> frame,
                             const CefString& target_url,
                             const CefString& target_frame_name,
                             WindowOpenDisposition target_disposition,
                             bool user_gesture,
                             const CefPopupFeatures& popupFeatures,
                             CefWindowInfo& windowInfo,
                             CefRefPtr<CefClient>& client,
                             CefBrowserSettings& settings,
                             bool* no_javascript_access) OVERRIDE; 
#else
	virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
                             CefRefPtr<CefFrame> frame,
                             const CefString& target_url,
                             const CefString& target_frame_name,
                             const CefPopupFeatures& popupFeatures,
                             CefWindowInfo& windowInfo,
                             CefRefPtr<CefClient>& client,
                             CefBrowserSettings& settings,
                             bool* no_javascript_access) OVERRIDE;
#endif

    virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
							CefRefPtr<CefFrame> frame,
                            ErrorCode errorCode,
                            const CefString& errorText,
                            const CefString& failedUrl) OVERRIDE;

    virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
  
	virtual void OnAddressChange(CefRefPtr<CefBrowser> browser,	
							CefRefPtr<CefFrame> frame, 
							const CefString& url) OVERRIDE;
	
	virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser, 
							const CefString& message, 
							const CefString& source, 
							int line) OVERRIDE;


	virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                        CefProcessId source_process,
                                        CefRefPtr<CefProcessMessage> message) OVERRIDE;

	virtual void OnTakeFocus( CefRefPtr<CefBrowser> browser, bool next ) OVERRIDE;

	virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,	
                CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model) OVERRIDE;
                
    virtual bool OnJSDialog(CefRefPtr<CefBrowser> browser,
                          const CefString& origin_url,
                          const CefString& accept_lang,
                          JSDialogType dialog_type,
                          const CefString& message_text,
                          const CefString& default_prompt_text,
                          CefRefPtr<CefJSDialogCallback> callback,
                          bool& suppress_message) OVERRIDE;

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
