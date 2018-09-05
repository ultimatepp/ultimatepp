#include "ChromiumBrowser.h"
#include "ClientApp.h"


ClientApp::ClientApp()
{
}


void ClientApp::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
{
	//Extra command line switches
	command_line->AppendSwitch("enable-media-stream");
	command_line->AppendSwitch("no-sandbox");
	command_line->AppendSwitch("disable-pinch");

#ifdef PLATFORM_LINUX
	const char flash_name[] = "libpepflashplayer.so";
#elif defined(PLATFORM_WIN32)
	const char flash_name[] = "pepflashplayer.dll";
#endif

	Upp::String fp = Upp::GetExeDirFile(flash_name);
	if (Upp::FileExists(fp)){
		command_line->AppendSwitchWithValue("ppapi-flash-path", ~fp);
		Upp::String fv = ParseJSON(LoadFile(Upp::GetExeDirFile("manifest.json")))["version"];
		command_line->AppendSwitchWithValue("ppapi-flash-version", fv.IsEmpty() ? "30.0.0.154" : ~fv);
	}
}


void ClientApp::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
	//Register our JS functions
	CefRefPtr<CefV8Value> object = context->GetGlobal();
	for(int f = 0; Upp::ChromiumBrowser::JSFunctions[f] != nullptr; f++){
		CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction(Upp::ChromiumBrowser::JSFunctions[f], this);
		object->SetValue(Upp::ChromiumBrowser::JSFunctions[f], func, V8_PROPERTY_ATTRIBUTE_NONE);
	}
}


void ClientApp::OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node)
{
	bool is_editable = (node.get() && node->IsEditable());
	browser->SendProcessMessage(PID_BROWSER, CefProcessMessage::Create(is_editable ? "show_keyboard" : "hide_keyboard"));
}


bool ClientApp::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
{
	CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(name);
	CefRefPtr<CefListValue> par = message->GetArgumentList();
	V8ValueListToCefListValue(arguments, par);
	CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
	if (browser) browser->SendProcessMessage(PID_BROWSER, message);
	return true;
}


void ClientApp::V8ValueListToCefListValue(const CefV8ValueList& src, CefRefPtr<CefListValue> & dst) 
{
	for (const auto & s: src){
		if (!s->IsValid())						{ dst->SetString(dst->GetSize(),"Invalid V8Value");		continue;}
		if (s->IsUndefined() || s->IsNull())	{ dst->SetNull(dst->GetSize());							continue;}
		if (s->IsBool())						{ dst->SetBool(dst->GetSize(),	s->GetBoolValue());		continue;}
		if (s->IsInt())							{ dst->SetInt(dst->GetSize(),	s->GetIntValue());		continue;}
		if (s->IsDouble())						{ dst->SetDouble(dst->GetSize(),s->GetDoubleValue());	continue;}
		if (s->IsString())						{ dst->SetString(dst->GetSize(),s->GetStringValue());	continue;}
		
		dst->SetString(dst->GetSize(), "Unimplemented V8Value conversion");
    }
}
