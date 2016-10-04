#include "XmlRpc.h"

/**
* constructor. constructs a new Call without setting the method-name that is to be called.
*/
XmlRpcCall::XmlRpcCall() {};

/**
* constructs a call to _method. constructs a new Call and setting the method-name of the method
* that should be called to \p _method.
* @param _method the name of the method to be called
*/
XmlRpcCall::XmlRpcCall(String _method) {
	method = _method;
};

/**
* constructs a call to _method. constructs a new Call and setting the method-name of the method
* that should be called to \p _method.
* @param _method the name of the method to be called
*/
XmlRpcCall::XmlRpcCall(const char *_method) {
	method = String(_method);
};

/**
* clear the method-name and the parametes. clears the list of all parameter that are added to
* the call and its method name
*/
void XmlRpcCall::Clear() {
	method.Clear();
	va.Clear();
	NumberToValue.Clear();
}

/**
* set the method name or add a parameter. if no method-name is set yet and \p v is a string it
* is set as method name else it is added as parameter to the call.
* @param v either the method name or a paramter to be added
*/
XmlRpcCall& XmlRpcCall::operator()(const Value& v) {
	if(IsString(v) && method.IsEmpty()) {
		method = v;
		return *this;
	}
	va.Add(v);
	return *this;
};
/**
* create the xml to send. This function creates a string, that contains the xml data representing
* the method call, which was created by using XmlRpcCall() and/or operator()(const Value& v) .
* @return a string containing the xml-methodCall
*/
String XmlRpcCall::ToXml() const {
	XmlNode xml;

	xml("methodCall")("methodName").Add().CreateText(method);
	XmlNode& params=xml.At(xml.FindTag("methodCall"))("params");

	for(int i=0;i<va.GetCount();i++) {
		XmlNode& current = params.Add("param").Add("value");
		Value v(va[i]);
		ValueToXml(current,v);
	}
	return AsXML(xml,XML_HEADER);
};
