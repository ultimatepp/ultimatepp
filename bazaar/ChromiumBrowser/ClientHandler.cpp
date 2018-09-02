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


void ClientHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward)
{
	PostCallback(callback3(WhenStateChange, isLoading, canGoBack, canGoForward));
}


bool ClientHandler::OnConsoleMessage(CefRefPtr<CefBrowser> browser, cef_log_severity_t level, const CefString& message, const CefString& source, int line)
{
	ASSERT(CefCurrentlyOn(TID_UI));
	PostCallback(callback3(WhenConsoleMessage, source.ToString(), line, message.ToString()));
	return true;
}


bool ClientHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser,
                             CefRefPtr<CefFrame> frame,
                             const CefString& target_url,
                             const CefString& target_frame_name,
                             CefLifeSpanHandler::WindowOpenDisposition target_disposition,
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
	if (errorCode == ERR_ABORTED) return;
	
	Upp::String err = Upp::String("<html><head></head><body><center><h1>") +
						Upp::t_("Page not found") +
						"<br/>" + failedUrl.ToString().c_str() +
						"</h1></center></body><html>";

	frame->LoadString(~err, failedUrl);
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
								JSDialogType dialog_type,
								const CefString& message_text, const CefString& default_prompt_text,
								CefRefPtr<CefJSDialogCallback> callback, bool& suppress_message)
{
	RLOG(message_text.ToString().c_str());
	return false;
}


bool ClientHandler::OnCertificateError(CefRefPtr<CefBrowser> browser, ErrorCode cert_error,
										const CefString& request_url, CefRefPtr<CefSSLInfo> ssl_info,
										CefRefPtr<CefRequestCallback> callback)
{
	ASSERT(CefCurrentlyOn(TID_UI));

	bool cont = WhenCertificateError(request_url.ToString());

	if (!cont){
		Upp::String valid_start;
		Upp::String valid_expiry;
		Upp::String subject_str = "&nbsp;";
		Upp::String issuer_str = "&nbsp;";
		
		if (ssl_info){
			CefRefPtr<CefX509Certificate> cert = ssl_info->GetX509Certificate();
			if (cert.get()){
				valid_start = Upp::Format(Upp::TimeFromUTC(cert->GetValidStart().GetTimeT()));
				valid_expiry = Upp::Format(Upp::TimeFromUTC(cert->GetValidExpiry().GetTimeT()));
				CefRefPtr<CefX509CertPrincipal> subject = cert->GetSubject();
				subject_str = subject.get() ? subject->GetDisplayName().ToString().c_str() : "&nbsp;";
				CefRefPtr<CefX509CertPrincipal> issuer = cert->GetIssuer();
				issuer_str = issuer.get() ? issuer->GetDisplayName().ToString().c_str() : "&nbsp;";
			}
		}
	
		// certificate information
		Upp::String ss;
		ss << "<h1>Page certificate is not trusted</h1><br/><br/>X.509 Certificate Information:"
	        "<table border=1><tr><th>Field</th><th>Value</th></tr>" <<
	        "<tr><td>Subject</td><td>" << subject_str << "</td></tr>"
	        "<tr><td>Issuer</td><td>" << issuer_str << "</td></tr>"
	        "<tr><td>Valid Start</td><td>" << valid_start << "</td></tr>"
	        "<tr><td>Valid Expiry</td><td>" << valid_expiry << "</td></tr>"
	        "</table>";
	
		browser->GetMainFrame()->LoadString(~ss, request_url);
	}
	
	callback->Continue(cont);
	return cont;
}


bool ClientHandler::OnFileDialog(CefRefPtr<CefBrowser> browser,
								FileDialogMode mode,
								const CefString& title,
								const CefString& default_file_path,
								const std::vector<CefString>& accept_filters,
								int selected_accept_filter,
								CefRefPtr<CefFileDialogCallback> callback)
{
	RLOG("File dialog blocked");
	return true;
}