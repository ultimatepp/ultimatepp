#include "UppValue.h"

Value Valuehelloval() { return 2244; }
int Valuedoubleit(const Value& v) { return int(v) * 2; }

void export_UppValue()
{
	to_python_converter<Value, Value_to_python>();
	Value_from_python();
	def("helloval", Valuehelloval);
	def("doubleit", Valuedoubleit);
}

void export_UppValueArray()
{
	to_python_converter<ValueArray, ValueArray_to_python>();
	ValueArray_from_python();
}
