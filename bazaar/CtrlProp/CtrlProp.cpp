#include "CtrlProp.h"

void AddToValueArray(Value& v, const Vector<String>& k)
{
	if(!v.Is<ValueArray>()) 
	{
		v = ValueArray(Vector<Value>());
	}
	ValueArray va = v;
	Vector<Value>& vv = const_cast<Vector<Value>&>(va.Get());
	for(int i = 0; i < k.GetCount(); i++)
		vv << k[i];
}

bool GetSetPropRecurseDone(Ctrl& c, const String& p, Value& v, int f) { return false; }

VectorMap<String, ParamHandler>& pmap()
{
	static VectorMap<String, ParamHandler> _;
	return _;
}

bool Property(Ctrl& c, const String& p, Value& v, int f)
{
	int i = pmap().Find(String(typeid(c).name()));
	if(i<0) return false;
	return pmap()[i](c,p,v,f);
}

