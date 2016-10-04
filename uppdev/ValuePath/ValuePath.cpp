#include <Core/Core.h>

using namespace Upp;

class Path {
	bool   index;
	Value  key;
	Value  value;
	Path  *parent;
	Value *target;

	Path()                           { target = NULL; }

public:
	Path operator()(const Value& key);
	Path operator[](int i);
	void Set(const Value& key, const Value& v, bool ndx);
	void operator=(const Value& v)   { value = v; parent->Set(key, v, index); }

	Path(Value& v)                   { target = &v; value = v; parent = NULL; index = false; }
};

Path Path::operator()(const Value& k)
{
	Path p;
	ValueMap m;
	if(IsValueMap(value))
		m = value;
	p.parent = this;
	p.key = k;
//	DLOG("operator()(" << key << ": " << m[key]);
	p.value = m.GetAndClear(k); // Optimize!
	p.index = false;
	return p;
}

Path Path::operator[](int i)
{
	ASSERT(i >= 0);
	Path p;
	ValueArray va;
	if(IsValueArray(value))
		va = value;
	p.parent = this;
	p.key = i;
	p.index = true;
	if(i < va.GetCount())
		p.value = va.GetAndClear(i);
	p.index = true;
	return p;
}

void Path::Set(const Value& k, const Value& v, bool ndx)
{
//	DLOG("Set(" << k << ": " << v << "), current key: " << key << ", value: " << value);
	if(ndx) {
		ValueArray va;
		if(IsValueArray(value))
			va = value;
		va.Set((int)k, v);
		value = va;
	}
	else {
		ValueMap m;
		if(IsValueMap(value))
			m = value;
		m.Set(k, v);
		value = m;
	}
	if(parent)
		parent->Set(key, value, index);
	if(target)
		*target = value;
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Value x;
	
	Path(x)("hello")("world") = 555;
	DUMP(x);
	LOG("-------------------");
	Path(x)("hello")("you") = "this";
	DUMP(x);
	LOG("-------------------");
	Path(x)("hello")("x") = "y";
	DUMP(x);
	LOG("-------------------");
	for(int i = 0; i < 20; i++)
		Path(x)("hello")("array")[i] = i;
	DUMP(x);
//	Path p = Path(x)("hello")("array");
//	for(int i = 0; i < 20; i++)
//		p[i] = "*" + AsString(i);
}
