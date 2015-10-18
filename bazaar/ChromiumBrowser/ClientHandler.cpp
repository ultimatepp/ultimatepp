#include "ClientHandler.h"
#include "include/cef_task.h"

void ClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> new_browser)
{
    if (!browser.get())   {
        // We need to keep the main child window, but not popup windows
        browser = new_browser;
    }
}


void ClientHandler::OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url)
{
	PostCallback(callback1(WhenUrlChange, url.ToString()));
}


bool ClientHandler::OnConsoleMessage(CefRefPtr<CefBrowser> browser, const CefString& message, const CefString& source, int line)
{
	ASSERT(CefCurrentlyOn(TID_UI));
	PostCallback(callback3(WhenConsoleMessage, source.ToString(), line, message.ToString()));
	return true;
}


bool ClientHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser,
                             CefRefPtr<CefFrame> frame,
                             const CefString& target_url,
                             const CefString& target_frame_name,
                             WindowOpenDisposition target_disposition,
                             bool user_gesture,
                             const CefPopupFeatures& popupFeatures,
                             CefWindowInfo& windowInfo,
                             CefRefPtr<CefClient>& client,
                             CefBrowserSettings& settings,
                             bool* no_javascript_access)
{
    RLOG(Upp::Format("Popup canceled, frame name: '%s', url '%s'", target_frame_name.ToString().c_str(), target_url.ToString().c_str()));
	// Cancel popups
    return true;
}


void ClientHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                ErrorCode errorCode,
                                const CefString& errorText,
								const CefString& failedUrl)
{
	frame->LoadString("<html><head></head><body><center><h1>Page not found</h1></center></body><html>", failedUrl);
}


bool ClientHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
	if (message->GetName().ToString() == "show_keyboard"){
		
		PostCallback(callback1(WhenKeyboard, true));

	}else if (message->GetName().ToString() == "hide_keyboard"){
		
		PostCallback(callback1(WhenKeyboard, false));
		
	}else{

		CefRefPtr<CefListValue> args = message->GetArgumentList();
		Upp::Vector<Upp::Value> * par = new Upp::Vector<Upp::Value>;
	
		for (unsigned i = 0; i < args->GetSize(); i++){
			CefValueType type = args->GetType(i);
			switch(type){
				
				case VTYPE_BOOL:
					par->Add(args->GetBool(i));
					break;
				
				case VTYPE_DOUBLE:
					par->Add(args->GetDouble(i));
					break;
					
				case VTYPE_INT:
					par->Add(args->GetInt(i));
					break;
					
				case VTYPE_STRING:
					par->Add(args->GetString(i).ToString().c_str());
					break;
				
				default:
					par->Add("OnProcessMessageReceived: unsupported parameter type");
					break;
			}
		}
		
		PostCallback(THISBACK2(WhenMessageWrapper, message->GetName().ToString(), par));
	}
	return true;
}


void ClientHandler::WhenMessageWrapper(Upp::String name, Upp::Vector<Upp::Value> * par)
{
	WhenMessage(name, *par);
	delete par;
}


void ClientHandler::OnTakeFocus(CefRefPtr<CefBrowser> browser, bool next )
{
	PostCallback(WhenTakeFocus);
}


void ClientHandler::OnGotFocus(CefRefPtr<CefBrowser> browser)
{
	PostCallback(WhenGotFocus);
}


void ClientHandler::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
										CefRefPtr<CefFrame> frame,
										CefRefPtr<CefContextMenuParams> params,
										CefRefPtr<CefMenuModel> model)
{
#ifndef _DEBUG
	//Empty context menu
	model->Clear();
#endif
}


bool ClientHandler::OnJSDialog(CefRefPtr<CefBrowser> browser, const CefString& origin_url,
								const CefString& accept_lang, JSDialogType dialog_type,
								const CefString& message_text, const CefString& default_prompt_text,
								CefRefPtr<CefJSDialogCallback> callback, bool& suppress_message)
{
	RLOG(message_text.ToString().c_str());
	return false;
}
