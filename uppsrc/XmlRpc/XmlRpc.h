#ifndef _XmlRpc_XmlRpc_h
#define _XmlRpc_XmlRpc_h

#include <Web/Web.h>

using namespace Upp;

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
void ValueGet(Array<T>& x, Value v) {
	ValueCheck(IsValueArray(v));
	ValueArray va = v;
	x.SetCount(va.GetCount());
	for(int i = 0; i < va.GetCount(); i++)
		ValueGet(x[i], va[i]);	
}

template <class T>
void ValueGet(Vector<T>& x, Value v) {
	ValueCheck(IsValueArray(v));
	ValueArray va = v;
	x.SetCount(va.GetCount());
	for(int i = 0; i < va.GetCount(); i++)
		ValueGet(x[i], va[i]);	
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
void ValueGet(T& x, const Value& v) {
	ValueMapper m;
	m.map = v;
	m.get = true;
	x.Map(m);
}

template <class T>
void ValuePut(Value& v, const T& x) {
	ValueMapper m;
	m.get = false;
	const_cast<T&>(x).Map(m);
	v = m.map;
}

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
	
	void                         Reset() { in.Clear(); out.Clear(); ii = 0; }

	XmlRpcData() { ii = 0; }
};

String FormatXmlRpcValue(const Value& value);
String FormatXmlRpcParam(const Value& param);
String FormatXmlRpcParams(const ValueArray& params);

String FormatXmlRpcError(int code, const char *text);

void   Register(const char *name, void (*method)(XmlRpcData&), const char *group = NULL);
bool   XmlRpcServer(int port = 80, const char *group = NULL);

#define XMLRPC_METHOD(x) \
void xmlrpc##x(XmlRpcData& rpc); \
INITBLOCK { Register(#x, xmlrpc##x); } \
void xmlrpc##x(XmlRpcData& rpc)

#define XMLRPC_GMETHOD(x, group) \
void xmlrpc##x(XmlRpcData& rpc); \
INITBLOCK { Register(#x, xmlrpc##x, group); } \
void xmlrpc##x(XmlRpcData& rpc)

class XmlRpcCall {
	bool       shorted;
	HttpClient server;
	XmlRpcData data;
	String     method;
	String     error;
	int        timeout;

public:
	XmlRpcCall& Method(const char *name)              { method = name; data.Reset(); error.Clear(); return *this; }

	template <class T>
	XmlRpcCall& operator<<(const T& x)                { data << x; return *this; }

	Value       Execute();

	template <class T>
	bool operator>>(T& x) { if(IsError(Execute())) return false; try { data >> x; } catch(ValueTypeMismatch) { return false; } return true; }

	XmlRpcCall& operator()(const char *method)        { Method(method); return *this; }

#define E__Templ(I)  class COMBINE(T, I)
#define E__Decl(I)   const COMBINE(T, I)& COMBINE(p, I)
#define E__Param(I)  *this << COMBINE(p, I)
#define E__Body(I) \
	template <__List##I(E__Templ)> \
	XmlRpcCall& operator()(const char *method, __List##I(E__Decl)) { \
		Method(method); \
		__List##I(E__Param); \
		return *this; \
	}

	__Expand20(E__Body)

#undef E__Templ
#undef E__Decl
#undef E__Param
#undef E__Body

	String GetError() const                             { return error; }
	XmlRpcCall& TimeOut(int msec)                       { timeout = msec; }

	XmlRpcCall(const char *url);
};

#endif
