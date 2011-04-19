#include "Value.h"
using namespace boost::python;

NAMESPACE_UPP

struct Value_to_python
{
	static PyObject* convert(const Value& v)
	{
		switch(v.GetType())
		{
			case INT_V: return incref(object(int(v)).ptr());
			case BOOL_V: return incref(object(bool(v)).ptr());
			case INT64_V: return incref(object(int64(v)).ptr());
			case DOUBLE_V: return incref(object(double(v)).ptr());
			case STRING_V: return incref(object(String(v)).ptr());
			case VALUEARRAY_V: return incref(object(ValueArray(v)).ptr());
			case VOID_V:
			default: return incref(object().ptr()); //none
		}
	}
};

struct Value_from_python
{
	Value_from_python()
	{
		converter::registry::push_back(&convertible, &construct, type_id<Value>());
	}

	static void* convertible(PyObject* po)
	{
		if(po == Py_None) return po;
		if(PyInt_Check(po)) return po;
		if(PyBool_Check(po)) return po;
		if(PyLong_Check(po)) return po;
		if(PyFloat_Check(po)) return po;
		if(PyString_Check(po)) return po;
		if(PyList_Check(po)) return po;
		return 0;
	}

	static void construct(PyObject* po, converter::rvalue_from_python_stage1_data* data)
	{
		void* d = ((converter::rvalue_from_python_storage<Value>*)data)->storage.bytes;
		
		if(po == Py_None)
		{
			new(d) Value();
		}
		else
		if(PyBool_Check(po)) //before Int, cause bool is derived from PyTypeInt
		{
		bool c = false;
		if(po != Py_False) {
		if(po != Py_True) throw_error_already_set();
		c = true;
		}
		new(d) Value(c);
		}
		else
		if(PyInt_Check(po))
		{
		long c = PyInt_AsLong(po);
		new(d) Value(c);
		}
		else
		//notmally here bool
		if(PyLong_Check(po))
		{
		long long c = PyLong_AsLongLong(po);
		new(d) Value(c);
		}
		else
		if(PyFloat_Check(po))
		{
		double c = PyFloat_AsDouble(po);
		new(d) Value(c);
		}
		else
		if(PyString_Check(po))
		{
		String c = extract<String>(object(handle<>(borrowed(po)))); //use upper converter, really borrowed?
		new(d) Value(c);
		}
		else
		if(PyList_Check(po))
		{
		ValueArray c = extract<ValueArray>(object(handle<>(borrowed(po)))); //use upper converter, really borrowed?
		new(d) Value(c);
		}
		else
		{
			throw_error_already_set(); //FIXME dont know type, which to throw?
		}

		data->convertible = d;
	}
};

void export_Value()
{
ONCELOCK
{
	to_python_converter<Value, Value_to_python>();
	Value_from_python();
}
}

//ValueArray

struct ValueArray_to_python
{
	static PyObject* convert(const ValueArray& v)
	{
		list l;
		const Vector<Value>& vv = v.Get();
		for(int i = 0; i < vv.GetCount(); i++)
			l.append(object(vv[i]));
		return incref(l.ptr()); //need to create a list
	}
};

struct ValueArray_from_python
{
	ValueArray_from_python()
	{
		converter::registry::push_back(&convertible, &construct, type_id<ValueArray>());
	}

	static void* convertible(PyObject* po)
	{
		if(PyList_Check(po)) return po;
		return 0;
	}

	static void construct(PyObject* po, converter::rvalue_from_python_stage1_data* data)
	{
		void* d = ((converter::rvalue_from_python_storage<ValueArray>*)data)->storage.bytes;
		
		Vector<Value> vv;
		list l(handle<>(borrowed(po)));
		for(int i = 0; i < len(l); i++)
			vv.Add(extract<Value>(l[i]));
		new(d) ValueArray(vv);

		data->convertible = d;
	}
};

void export_ValueArray()
{
ONCELOCK
{
	to_python_converter<ValueArray, ValueArray_to_python>();
	ValueArray_from_python();
}
}

END_UPP_NAMESPACE