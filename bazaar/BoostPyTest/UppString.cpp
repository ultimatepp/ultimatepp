#include "UppString.h"

String Stringhello() { return String( "my new custom string" ); }
std::size_t Stringsize(const String& s) { return s.GetLength(); }

void export_UppString()
{
	to_python_converter<String, String_to_python_str>();
	String_from_python_str();
	def("hello", Stringhello);
	def("size", Stringsize);
}

