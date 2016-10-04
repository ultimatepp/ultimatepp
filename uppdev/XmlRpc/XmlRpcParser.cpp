#include "XmlRpc.h"

/**
* constructs a xmlrpcparser.constructs a xmlrpcparser
*/
XmlRpcParser::XmlRpcParser() {}

const char *primitives[]= {"string","int","i4","boolean","double","dateTime.iso8601","base64"};
/**
* parse primitve. this method parses the values, that are stored inside the Xml-document
* handled by the XmlParser \p p.
* @param p the parser holding the xml document
* @return the Value contained in the xml document
*/
Value XmlRpcParser::ParsePrimitive(XmlParser& p) {
	Value v,vv;
	if(p.IsText()) {
		return Value(p.ReadText());
	}
	for(int i=0;i<7;i++) {
		if(p.Tag(primitives[i])) {
			switch(i) {
			case 0: //string
				v=Value(p.ReadText());
				break;
			case 1: //int
			case 2: //i4
				v = Value(atoi(p.ReadText()));
				break;
			case 3: //boolean
				v = Value((bool)atoi(p.ReadText()));
				break;
			case 4: //double
				v = Value(atof(p.ReadText()));
				break;
			case 5: //dateTime.iso8601
				p.ReadTextE();
				v=Value(Date(1970,1,1));
				break;
			case 6: //base64
				LoadFromString(vv, p.ReadText());
				v = Value(vv);
				break;
			default:
				throw Exc("unexpected Error");
				break;
			}
			p.PassEnd();
			return Value(v);
		}
	}
	throw Exc("unknown primitive");
}
/**
* parse the special value "array". this method parses the complexer value "array" and returns it
* @param p XmlParser holding the xml document
* @return the parsed array as Value (not ValueArray!)
*/
Value XmlRpcParser::ParseArray(XmlParser& p) {
	ValueArray va;
	p.PassTag("data");
	while(!p.End()) {
		Value v = Parse(p);
		va.Add(v);
	}
	return Value(va);
}
/**
* parse the special value "struc". this method parses the complexer value "struct" and returns it
* @param p XmlParser holding the xml document
* @return the parsed struct as Value (not ValueMap!)
*/
Value XmlRpcParser::ParseStruct(XmlParser& p) {
	ValueMap vm;
	Value key, data;
	while(p.Tag("member")) {
		for(int i=0;i<2;i++){
			if(p.Tag("name")) {
				key = Value(p.ReadText());
				p.PassEnd(); //name
			} else
				data = Parse(p);
		}
		p.PassEnd(); //member
		vm.Add(key,data);
	}
	return Value(vm);
}
/**
* parse every possible value. this method parses every possible value (defined by the xmlrpc standard)
* and returns it. This is done by calling the special functions ParsePrimitive(), ParseArray()
* and ParseStruct().
* @param p XmlParser holding the xml document
* @return the parsed Value
*/
Value XmlRpcParser::Parse(XmlParser& p) {
	Value v;
	p.PassTag("value");
	if(p.Tag("struct")) {
		v=ParseStruct(p);
		p.PassEnd(); //struct
	}
	else if(p.Tag("array")) {
		v=ParseArray(p);
		p.PassEnd(); //array
	}
	else
		v=ParsePrimitive(p);
	p.PassEnd(); //value
	return Value(v);
}
/**
* parse a server response. it parses a xml response sent by a server and returns the value or
* error it contains.
* @param s the received data
* @return the value described by data
* @see ParseMethodCall()
*/
Value XmlRpcParser::ParseMethodResponse(String s){
	XmlParser p(s);
	Value v;
	String error;
	try{
		while(!p.Tag("methodResponse"))
			p.Skip();


		if(p.Tag("fault")) {
			ValueMap vm(Parse(p));
			error << vm["faultString"] <<" ("<<vm["faultCode"]<<")";
			p.PassEnd(); //fault
			p.PassEnd(); //methodResponse
			return ErrorValue(error);
		}
		p.PassTag("params");
		p.PassTag("param");
		v=Parse(p);
		p.PassEnd();	//param
		p.PassEnd();	//params
		p.PassEnd();	//methodResponse
	} catch(Exc e) {
		return ErrorValue(NFormat("EXCEPTION: %s",e));
	}
	return Value(v);
}
/**
* parse a method call. it parses a xml message sent by a client and returns the method name and
* the parameter it contains.
* @param s the received data
* @return the value, which includes an ValueArray with first element beeing the method name
* @see ParseMethodResponse()
*/
Value XmlRpcParser::ParseMethodCall(String s){
	XmlParser p(s);
	ValueArray va;

	try {
		while(!p.Tag("methodCall"))
		      p.Skip();
		p.PassTag("methodName");
		va.Add(Value(p.ReadText()));
		p.PassEnd(); //methodName
		p.PassTag("params");
		while(p.Tag("param")) {
			va.Add(Parse(p));
			p.PassEnd(); //param
		}
		p.PassEnd(); //params
		p.PassEnd(); //methodCall
		LOG("request parsed");
	} catch(Exc e) {
		return ErrorValue(NFormat("EXCEPTION: %s",e));
	}
	return Value(va);
}
