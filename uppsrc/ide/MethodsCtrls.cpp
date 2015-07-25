#include "ide.h"

void TextOption::SetData(const Value& data)
{
	String s = data;
	Set(!(IsNull(s) || s == "0"));
}

Value TextOption::GetData() const
{
	return Get() ? "1" : "0";
}

void  TextSwitch::SetData(const Value& data)
{
	String s = data;
	Switch::SetData(atoi((String)data));
}

Value TextSwitch::GetData() const
{
	return AsString(Switch::GetData());
}
