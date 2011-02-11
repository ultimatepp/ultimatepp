#include "Property.h"

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
