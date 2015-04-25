#ifndef _KioskChromium_CefApp_h_
#define _KioskChromium_CefApp_h_

#include <CtrlLib/CtrlLib.h>
#undef LOG
#undef DLOG
#undef CHECK
#undef LOG_INFO
#undef LOG_WARNING

#include "include/cef_app.h"
#include "include/cef_dom.h"
#undef LOG
#undef DLOG
#undef CHECK
#undef LOG_INFO
#undef LOG_WARNING


class ClientApp:	public CefApp,
             		public CefBrowserProcessHandler,
             		public CefRenderProcessHandler,
             		public CefV8Handler
{
private:

	CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler()	{ return this; }

	Upp::VectorMap<Upp::String, Upp::Callback3<const CefV8ValueList&, CefRefPtr<CefV8Value>&, CefString&> > functions;
	void JSExample1(const CefV8ValueList&, CefRefPtr<CefV8Value>&, CefString&);

public:
	typedef ClientApp CLASSNAME;
	ClientApp();
		
	virtual void OnBeforeCommandLineProcessing(const CefString& process_type,
      											CefRefPtr<CefCommandLine> command_line); 
      			
	virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
									CefRefPtr<CefFrame> frame,
									CefRefPtr<CefV8Context> context);

	virtual void OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, 
										CefRefPtr<CefFrame> frame, 
										CefRefPtr<CefDOMNode> node) OVERRIDE; 
										
	virtual bool Execute(const CefString& name,
                       CefRefPtr<CefV8Value> object,
                       const CefV8ValueList& arguments,
                       CefRefPtr<CefV8Value>& retval,
                       CefString& exception) OVERRIDE;


	IMPLEMENT_REFCOUNTING(ClientApp);
};


#endif
