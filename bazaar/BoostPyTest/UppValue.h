#ifndef _BoostPyTest_UppValue_h_
#define _BoostPyTest_UppValue_h_

#include <Py/Py.h>
using namespace boost::python;

#include <Core/Core.h>
using namespace Upp;

//a convertable instance

struct Value_to_python
{
	static PyObject* convert(const Value& v) { return incref(object(int(v)).ptr()); }
};

struct Value_from_python
{
	Value_from_python()
	{
		converter::registry::push_back(&convertible, &construct, type_id<Value>());
	}

	static void* convertible(PyObject* po)
	{
		if(!PyInt_Check(po)) return 0;
		return po;
	}

	static void construct(PyObject* po, converter::rvalue_from_python_stage1_data* data)
	{
		long c = PyInt_AS_LONG(po);
		if(c == 0) throw_error_already_set();
		void* d = ((converter::rvalue_from_python_storage<Value>*)data)->storage.bytes;
		new(d) Value(int(c));
		data->convertible = d;
	}
};

Value Valuehelloval(); //tests to-python
int Valuedoubleit(const Value& v); //tests from-python

//fw
void export_UppValue();

#endif
