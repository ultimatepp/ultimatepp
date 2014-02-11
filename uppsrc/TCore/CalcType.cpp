#include "TCoreCalc.h"
#pragma hdrstop

NAMESPACE_UPP

String CalcTypeDescribeInt()             { return t_("integer"); }

String CalcType<double>::Describe()      { return t_("real number"); }
String CalcType<int64>::Describe()       { return t_("64-bit integer"); }
String CalcType<String>::Describe()      { return t_("string"); }
String CalcType<Date>::Describe()        { return t_("date"); }
String CalcType<Time>::Describe()        { return t_("date/time"); }
String CalcType<bool>::Describe()        { return t_("logical value"); }
String CalcType<Value>::Describe()       { return t_("any data type"); }
String CalcType<ValueArray>::Describe()  { return t_("array"); }
String CalcType<Nuller>::Describe()      { return t_("null value"); }

RegisterStdCalcTypeName(double)
RegisterStdCalcTypeName(String)
RegisterStdCalcTypeName(WString)
RegisterStdCalcTypeName(Date)
RegisterStdCalcTypeName(Time)
RegisterStdCalcTypeName(bool)
RegisterStdCalcTypeName(Value)
RegisterStdCalcTypeName(ValueArray)

const char *CalcNanError()
{
	return t_("real number operation error");
}

const char *CalcInfError()
{
	return t_("real number overflow");
}

VectorMap<String, String (*)()>& CalcTypeNameConvert::Get()
{
	static VectorMap<String, String (*)()> list;
	return list;
}

String CalcTypeNameConvert::Format(const char *raw_name)
{
	static const char rvr[] = "RawValueRep<";
	static const char cvr[] = "RichValueRep<";
	const char *s = raw_name;
	if(!memcmp(s, "struct ", 7))
		s += 7;
	else if(!memcmp(s, "class ", 6))
		s += 6;
	if(!memcmp(s, rvr, sizeof(rvr) - 1))
		s += sizeof(rvr) - 2;
	else if(!memcmp(s, cvr, sizeof(cvr) - 1))
		s += sizeof(cvr) - 2;
	String temp;
	if(*s == '<')
	{
		const char *p = ++s;
		p += strlen(p);
		if(p > s && p[-1] == '>')
			p--;
		temp = String(s, p);
		raw_name = temp;
	}
	int i = Get().Find(raw_name);
	if(i >= 0)
		return Get()[i]();
	return GetTypeName(raw_name);
}

Value CalcType<double>::ToValue(double t)
{
	if(IsNaN(t))
		throw Exc(CalcNanError());
	if(IsInf(t))
		throw Exc(CalcInfError());
	return t;
}

bool CalcType<bool>::ValueTo(Value v)
{
	if(IsNull(v))
		return false;
	return !IsNumber(v) || (double)v != 0;
}

END_UPP_NAMESPACE
