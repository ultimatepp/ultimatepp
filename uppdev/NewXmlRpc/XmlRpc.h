#ifndef _NewXmlRpc_XmlRpc_h_
#define _NewXmlRpc_XmlRpc_h_

#include <Core/Core.h>

using namespace Upp;

struct ValueTypeMismatch {};

void ValueCheck(bool b);

void ValueGet(int& n, const Value& v);
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

void ValuePut(Value& v, int& n);
void ValuePut(Value& v, String& s);
void ValuePut(Value& v, double& x);
void ValuePut(Value& v, Date& x);
void ValuePut(Value& v, Time& x);
void ValuePut(Value& v, Value& t);
void ValuePut(Value& v, ValueArray& va);
void ValuePut(Value& v, ValueMap& vm);

struct ValueMapper {
	ValueMap map;
	bool     get;

	template <class T>
	ValueMapper& operator()(const char *name, T& x) {
		if(get)
			ValueGet(x, map[name]);
		else
			ValuePut(map[name], x);
		return *this;
	}
	                        
	ValueMapper(ValueMap m) : map(m) {}
};

template <class T>
void ValueGet(T& x, const Value& v) {
	ValueMapper m(v);
	m.get = true;
	x.Map(m);
}

template <class T>
void ValuePut(Value& v, T& x) {
	ValueMapper m(v);
	m.get = false;
	x.Map(m);
}

Value      ParseXmlRpcValue(XmlParser& p);
Value      ParseXmlRpcParam(XmlParser& p);
ValueArray ParseXmlRpcParams(XmlParser& p);

#endif
