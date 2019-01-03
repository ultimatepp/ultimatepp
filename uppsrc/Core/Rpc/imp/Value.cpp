#include <Core/Core.h>
#include <Core/Rpc/Rpc.h>

namespace Upp {

String FormatIso8601(Time t)
{
	return Format("%04.4d%02.2d%02.2d`T%02.2d`:%02.2d`:%02.2d",
	              t.year, t.month, t.day, t.hour, t.minute, t.second);
}

Time ScanIso8601(const String& p)
{
	String s = TrimBoth(p);
	// 19980717T14:08:55
	// 01234567890123456
	if(s.GetCount() != 17 || s[8] != 'T' || s[11] != ':' || s[14] != ':')
		return Null;
	Time tm;
	tm.year = atoi(s.Mid(0, 4));
	tm.month = atoi(s.Mid(4, 2));
	tm.day = atoi(s.Mid(6, 2));
	tm.hour = atoi(s.Mid(9, 2));
	tm.minute = atoi(s.Mid(12, 2));
	tm.second = atoi(s.Mid(15, 2));
	if(!tm.IsValid())
		return Null;
	return tm;
}

void ValueCheck(bool b)
{
	if(!b)
		throw ValueTypeMismatch();
}

void ValueGet(int& n, const Value& v)
{
	ValueCheck(IsNull(v) || IsNumber(v));
	n = v;
}

void ValueGet(int64& n, const Value& v)
{
	ValueCheck(IsNull(v) || IsNumber(v));
	n = v;
}

void ValueGet(String& s, const Value& v)
{
	ValueCheck(IsNull(v) || IsString(v));
	s = v;
}

void ValueGet(double& x, const Value& v)
{
	ValueCheck(IsNull(v) || IsNumber(v));
	x = v;
}

void ValueGet(bool& x, const Value& v)
{
	ValueCheck(IsNull(v) || IsNumber(v));
	x = v;
}

Time IsoTime(const Value& v)
{
	Time tm = ScanIso8601((String)v);
	if(IsNull(tm))
		throw ValueTypeMismatch();
	return tm;
}

void ValueGet(Date& x, const Value& v)
{
	if(IsString(v)) {
		x = IsoTime(v);
		return;
	}
	ValueCheck(IsNull(v) || IsDateTime(v));
	x = v;
}

void ValueGet(Time& x, const Value& v)
{
	if(IsString(v)) {
		x = IsoTime(v);
		return;
	}
	ValueCheck(IsNull(v) || IsDateTime(v));
	x = v;
}

void ValueGet(Value& t, const Value& v)
{
	t = v;
}
	
void ValueGet(ValueArray& va, const Value& v)
{
	ValueCheck(IsNull(v) || IsValueArray(v));
	va = v;
}

void ValueGet(ValueMap& vm, const Value& v)
{
	ValueCheck(IsNull(v) || IsValueMap(v));
	vm = v;
}

void ValuePut(Value& v, int n)
{
	v = n;
}

void ValuePut(Value& v, int64 n)
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

void ValuePut(Value& v, const Json& json)
{
	RawJsonText x;
	x.json = json;
	v = RawToValue(x);
}

}