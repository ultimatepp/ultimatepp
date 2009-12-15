#ifndef _NewXmlRpc_XmlRpc_h_
#define _NewXmlRpc_XmlRpc_h_

#include <Core/Core.h>

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
	ValueArray in;
	int        ii;
	ValueArray out;
	
	Value Get()                  { return in[ii++]; }
	
	template <class T>
	XmlRpcData& operator>>(T& x) { ValueGet(x, Get()); return *this; }
	
	template <class T>
	XmlRpcData& operator<<(const T& x) { Value v; ValuePut(v, x); DDUMP(v); out.Add(v); return *this; }

	XmlRpcData() { ii = 0; }
};

String FormatXmlRpcValue(const Value& value);
String FormatXmlRpcParam(const Value& param);
String FormatXmlRpcParams(const ValueArray& params);

String FormatXmlRpcError(int code, const char *text);

void   Register(const char *name, Callback1<XmlRpcData&> cb);
String XmlRpcExecute(const String& request);

Value XmlRpcCall(const char *name, XmlRpcData& param);

#endif
