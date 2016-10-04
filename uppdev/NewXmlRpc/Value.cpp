#include "XmlRpc.h"

void ValueCheck(bool b)
{
	if(!b)
		throw ValueTypeMismatch();
}

void ValueGet(int& n, const Value& v)
{
	ValueCheck(IsNumber(v));
	n = v;
}

void ValueGet(String& s, const Value& v)
{
	ValueCheck(IsString(s));
	s = v;
}

void ValueGet(double& x, const Value& v)
{
	ValueCheck(IsNumber(v));
	x = v;
}

void ValueGet(bool& x, const Value& v)
{
	ValueCheck(IsNumber(x));
	x = v;
}

void ValueGet(Date& x, const Value& v)
{
	ValueCheck(IsDateTime(v));
	x = v;
}

void ValueGet(Time& x, const Value& v)
{
	ValueCheck(IsDateTime(v));
	x = v;
}

void ValueGet(Value& t, const Value& v)
{
	t = v;
}
	
void ValueGet(ValueArray& va, const Value& v)
{
	ValueCheck(IsValueArray(v));
	va = v;
}

void ValueGet(ValueMap& vm, const Value& v)
{
	ValueCheck(IsValueMap(v));
	vm = v;
}

void ValuePut(Value& v, int n)
{
	v = n;
}

void ValuePut(Value& v, const String& s)
{
	v = s;
}

void ValuePut(Value& v, const char *s)
{
	ValuePut(v, String(s));
}

void ValuePut(Value& v, double x)
{
	v = x;
}

void ValuePut(Value& v, bool x)
{
	v = x;
}

void ValuePut(Value& v, const Date& x)
{
	v = x;
}

void ValuePut(Value& v, const Time& x)
{
	v = x;
}

void ValuePut(Value& v, const Value& t)
{
	v = t;
}

void ValuePut(Value& v, const ValueArray& va)
{
	v = va;
}

void ValuePut(Value& v, const ValueMap& vm)
{
	v = vm;
}
