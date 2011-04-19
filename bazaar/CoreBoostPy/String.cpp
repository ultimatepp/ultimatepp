#include "String.h"
using namespace boost::python;

NAMESPACE_UPP

struct String_to_python_str
{
	static PyObject* convert(const String& s) { return incref(object(s.Begin()).ptr()); }
};

struct String_from_python_str
{
	String_from_python_str()
	{
		converter::registry::push_back(&convertible, &construct, type_id<String>());
	}

	static void* convertible(PyObject* po)
	{
		if(PyString_Check(po)) return po;
		return 0;
	}

	static void construct(PyObject* po, converter::rvalue_from_python_stage1_data* data)
	{
		void* d = ((converter::rvalue_from_python_storage<String>*)data)->storage.bytes;

		const char* c = PyString_AsString(po);
		if(c == 0) throw_error_already_set();
		new(d) String(c);

		data->convertible = d;
	}
};

void export_String()
{
ONCELOCK
{
	to_python_converter<String, String_to_python_str>();
	String_from_python_str();
}
}

END_UPP_NAMESPACE
