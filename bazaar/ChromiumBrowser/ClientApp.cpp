#include "ClientApp.h"


ClientApp::ClientApp()
{
	//Add our own JS functions to the list
	functions.Add("JSExample1", Upp::callback(this, &ClientApp::JSExample1));
}


void ClientApp::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
{
	//Extra command line switches
	command_line->AppendSwitch("enable-media-stream");
	command_line->AppendSwitch("no-sandbox");
}


void ClientApp::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
	//Register our JS functions
	CefRefPtr<CefV8Value> object = context->GetGlobal();
	for(int f = 0; f < functions.GetCount(); f++){
		CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction(~functions.GetKey(f), this);
		object->SetValue(~functions.GetKey(f), func, V8_PROPERTY_ATTRIBUTE_NONE);
	}			
}


void ClientApp::OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node)
{
	bool is_editable = (node.get() && node->IsEditable());
	browser->SendProcessMessage(PID_BROWSER, CefProcessMessage::Create(is_editable ? "show_keyboard" : "hide_keyboard"));
}


bool ClientApp::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
{
	int f = functions.Find(name.ToString());
	if (f >= 0){
		functions[f].Execute(arguments, retval, exception);
		return true;
	}else{
		RLOG(Upp::String("ERROR: Unknown native function: ") + name.ToString());
	}

	return false;
}


void ClientApp::JSExample1(const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exc)
{
	retval = CefV8Value::CreateString("Function that returns some string");
}
