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
