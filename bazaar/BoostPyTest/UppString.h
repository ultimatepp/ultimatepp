#ifndef _BoostPyTest_UppString_h_
#define _BoostPyTest_UppString_h_

#include <Py/Py.h>
using namespace boost::python;

#include <Core/Core.h>
using namespace Upp;

//a convertable instance

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
		if(!PyString_Check(po)) return 0;
		return po;
	}

	static void construct(PyObject* po, converter::rvalue_from_python_stage1_data* data)
	{
		const char* c = PyString_AsString(po);
		if(c == 0) throw_error_already_set();
		void* d = ((converter::rvalue_from_python_storage<String>*)data)->storage.bytes;
		new(d) String(c);
		data->convertible = d;
	}
};

String Stringhello(); //tests to-python
std::size_t Stringsize(const String& s); //tests from-python

//fw
void export_UppString();

#endif
