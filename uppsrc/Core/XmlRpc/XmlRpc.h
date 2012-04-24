#ifndef _XmlRpc_XmlRpc_h
#define _XmlRpc_XmlRpc_h

#include <Web/Web.h>

NAMESPACE_UPP

enum {
	XMLRPC_CLIENT_HTTP_ERROR = -1000000,
	XMLRPC_CLIENT_XML_ERROR,
	XMLRPC_CLIENT_RETTYPE_ERROR,
	XMLRPC_SERVER_PARAM_ERROR,
	XMLRPC_SERVER_XML_ERROR,
	XMLRPC_SERVER_PROCESSING_ERROR,
	XMLRPC_UNKNOWN_METHOD_ERROR,
};

struct ValueTypeMismatch {};

void ValueCheck(bool b);

void ValueGet(int& n, const Value& v);
void ValueGet(bool& b, const Value& v);
void ValueGet(String& s, const Value& v);
void ValueGet(double& x, const Value& v);
void ValueGet(Date& x, const Value& v);
void ValueGet(Time& x, const Value& v);
void ValueGet(Value& t, const Value& v);
void ValueGet(ValueArray& va, const Value& v);
void ValueGet(ValueMap& vm, const Value& v);

template <class T>
void ValueGet(Array<T>& x, const Value& v)
{
	ValueCheck(IsValueArray(v));
	ValueArray va = v;
	x.SetCount(va.GetCount());
	for(int i = 0; i < va.GetCount(); i++)
		ValueGet(x[i], va[i]);	
}

template <class T>
void ValueGet(Vector<T>& x, const Value& v)
{
	ValueCheck(IsValueArray(v));
	ValueArray va = v;
	x.SetCount(va.GetCount());
	for(int i = 0; i < va.GetCount(); i++)
		ValueGet(x[i], va[i]);	
}

template <class T>
void ValueGet(ArrayMap<String, T>& x, const Value& v)
{
	ValueCheck(IsValueMap(v));
	ValueMap vm = v;
	const Index<Value>& k = vm.GetKeys();
	ValueArray va = vm.GetValues();
	x.Clear();
	for(int i = 0; i < k.GetCount(); i++)
		x.Add(k[i], va[i]);
}

template <class T>
void ValueGet(VectorMap<String, T>& x, const Value& v)
{
	ValueCheck(IsValueMap(v));
	ValueMap vm = v;
	const Index<Value>& k = vm.GetKeys();
	ValueArray va = vm.GetValues();
	x.Clear();
	for(int i = 0; i < k.GetCount(); i++)
		x.Add(k[i], va[i]);
}

template <class T>
void ValueGet(ArrayMap<int, T>& x, const Value& v)
{
	ValueCheck(IsValueMap(v));
	ValueMap vm = v;
	const Index<Value>& k = vm.GetKeys();
	ValueArray va = vm.GetValues();
	x.Clear();
	for(int i = 0; i < k.GetCount(); i++)
		x.Add(atoi(AsString(k[i])), va[i]);
}

template <class T>
void ValueGet(VectorMap<int, T>& x, const Value& v)
{
	ValueCheck(IsValueMap(v));
	ValueMap vm = v;
	const Index<Value>& k = vm.GetKeys();
	ValueArray va = vm.GetValues();
	x.Clear();
	for(int i = 0; i < k.GetCount(); i++)
		x.Add(atoi(AsString(k[i])), va[i]);
}

void ValuePut(Value& v, int n);
void ValuePut(Value& v, const String& s);
void ValuePut(Value& v, const char *s);
void ValuePut(Value& v, double x);
void ValuePut(Value& v, bool x);
void ValuePut(Value& v, const Date& x);
void ValuePut(Value& v, const Time& x);
void ValuePut(Value& v, const Value& t);
void ValuePut(Value& v, const ValueArray& va);
void ValuePut(Value& v, const ValueMap& vm);

template <class T>
Value AsXmlRpcValue(const T& x)
{
	Value vs;
	ValuePut(vs, x);
	return vs;
}

template <class T>
void ValuePut(Value& v, const Array<T>& x)
{
	ValueArray va;
	for(int i = 0; i < x.GetCount(); i++)
		va.Add(AsXmlRpcValue(x[i]));
	v = va;
}

template <class T>
void ValuePut(Value& v, const Vector<T>& x)
{
	ValueArray va;
	for(int i = 0; i < x.GetCount(); i++)
		va.Add(AsXmlRpcValue(x[i]));
	v = va;
}

template <class T>
void ValuePut(Value& v, const ArrayMap<String, T>& x)
{
	ValueMap vm;
	for(int i = 0; i < x.GetCount(); i++)
		if(!x.IsUnlinked(i))
			vm.Add(x.GetKey(i), AsXmlRpcValue(x[i]));
	v = vm;
}

template <class T>
void ValuePut(Value& v, const VectorMap<String, T>& x)
{
	ValueMap vm;
	for(int i = 0; i < x.GetCount(); i++)
		if(!x.IsUnlinked(i))
			vm.Add(x.GetKey(i), AsXmlRpcValue(x[i]));
	v = vm;
}

template <class T>
void ValuePut(Value& v, const ArrayMap<int, T>& x)
{
	ValueMap vm;
	for(int i = 0; i < x.GetCount(); i++)
		if(!x.IsUnlinked(i))
			vm.Add(AsString(x.GetKey(i)), AsXmlRpcValue(x[i]));
	v = vm;
}

template <class T>
void ValuePut(Value& v, const VectorMap<int, T>& x)
{
	ValueMap vm;
	for(int i = 0; i < x.GetCount(); i++)
		if(!x.IsUnlinked(i))
			vm.Add(AsString(x.GetKey(i)), AsXmlRpcValue(x[i]));
	v = vm;
}

struct ValueMapper {
	ValueMap map;
	bool     get;

	template <class T>
	ValueMapper& operator()(const char *name, T& x) {
		if(get)
			ValueGet(x, map[name]);
		else {
			Value v;
			ValuePut(v, x);
			map.Add(name, v);
		}
		return *this;
	}
};

template <class T>
void ValueGetStruct_(T& x, const Value& v) {
	ValueCheck(IsNull(v) || IsValueMap(v));
	ValueMapper m;
	m.map = v;
	m.get = true;
	x.Map(m);
}

template <class T>
void ValuePutStruct_(Value& v, const T& x) {
	ValueMapper m;
	m.get = false;
	const_cast<T&>(x).Map(m);
	v = m.map;
}

template <class T>
struct XmlRpcStruct {
	friend void ValueGet(T& x, const Value& v) { ValueGetStruct_(x, v); }
	friend void ValuePut(Value& v, const T& x) { ValuePutStruct_(v, x); }
};

#define XMLRPC_STRUCT(T) \
inline void ValueGet(T& x, const Value& v) { ValueGetStruct_(x, v); } \
inline void ValuePut(Value& v, const T& x) { ValuePutStruct_(v, x); }

Value      ParseXmlRpcValue(XmlParser& p);
Value      ParseXmlRpcParam(XmlParser& p);
ValueArray ParseXmlRpcParams(XmlParser& p);

struct XmlRpcData {
	String     peeraddr;
	ValueArray in;
	int        ii;
	ValueArray out;
	
	Value Get()                          { if(ii >= in.GetCount()) throw ValueTypeMismatch(); return in[ii++]; }
	
	template <class T>
	XmlRpcData& operator>>(T& x)         { ValueGet(x, Get()); return *this; }
	
	template <class T>
	XmlRpcData& operator<<(const T& x)   { Value v; ValuePut(v, x); out.Add(v); return *this; }
	template <class T>
	void        Set(int i, const T& x)   { Value v; ValuePut(v, x); out.Set(i, v); }
	
	void        Reset()                  { in.Clear(); out.Clear(); ii = 0; }

	XmlRpcData() { ii = 0; }
};

String FormatXmlRpcValue(const Value& value);
String FormatXmlRpcParam(const Value& param);
String FormatXmlRpcParams(const ValueArray& params);

String FormatXmlRpcError(int code, const char *text);

void   Register(const char *name, void (*method)(XmlRpcData&), const char *group = NULL);

#define XMLRPC_METHOD(x) \
void xmlrpc##x(XmlRpcData& rpc); \
INITBLOCK { Register(#x, xmlrpc##x); } \
void xmlrpc##x(XmlRpcData& rpc)

#define XMLRPC_GMETHOD(x, group) \
void xmlrpc##x(XmlRpcData& rpc); \
INITBLOCK { Register(#x, xmlrpc##x, group); } \
void xmlrpc##x(XmlRpcData& rpc)

struct XmlRpcError {
	int    code;
	String text;
};

void   SetXmlRpcMethodFilter(String (*filter)(const String& methodname));
String XmlRpcExecute(const String& request, const char *group, const char *peeraddr);
bool   XmlRpcPerform(TcpSocket& http, const char *group);
bool   XmlRpcServerLoop(int port, const char *group);

void ThrowXmlRpcError(int code, const char *s);
void ThrowXmlRpcError(const char *s);

class XmlRpcRequest : HttpRequest {
	bool       shorted;
	XmlRpcData data;
	String     method;
	String     error;
	String     faultString;
	int        faultCode;
	bool       shouldExecute;

public:
	XmlRpcRequest& Method(const char *name);

	template <class T>
	XmlRpcRequest& operator<<(const T& x)                { data << x; return *this; }
	template <class T>
	void        Set(int i, const T& x)                { data.Set(i, x); }

	Value       Execute();
	Value       Retry();

	template <class T>
	bool operator>>(T& x)                             { if(Execute().IsError()) return false;
	                                                    try { data >> x; } catch(ValueTypeMismatch) { return false; } return true; }
	XmlRpcRequest& operator()(const char *method)        { Method(method); return *this; }

#define E__Templ(I)  class COMBINE(T, I)
#define E__Decl(I)   const COMBINE(T, I)& COMBINE(p, I)
#define E__Param(I)  *this << COMBINE(p, I)
#define E__Body(I) \
	template <__List##I(E__Templ)> \
	XmlRpcRequest& operator()(const char *method, __List##I(E__Decl)) { \
		Method(method); \
		__List##I(E__Param); \
		return *this; \
	}

	__Expand20(E__Body)

#undef E__Templ
#undef E__Decl
#undef E__Param
#undef E__Body

	String GetFaultString() const                               { return faultString; }
	int    GetFaultCode() const                                 { return faultCode; }
	String GetError() const                                     { return error; }
	String GetMethod() const                                    { return method; }
	void   ClearError();
	
	XmlRpcRequest& Url(const char *url);
	
	XmlRpcRequest(const char *url);
	XmlRpcRequest();
};

void LogXmlRpcRequests(bool b = true);

void SetXmlRpcServerTrace(Stream& s, int level = 1);

END_UPP_NAMESPACE

#endif
