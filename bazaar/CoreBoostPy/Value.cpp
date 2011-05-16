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
			case VALUEMAP_V: return incref(object(ValueMap(v)).ptr());
			case COLOR_V: return incref(object(Color(v)).ptr());
			case POINT_V: return incref(object(Point(v)).ptr());
			case SIZE_V: return incref(object(Size(v)).ptr());
			case RECT_V: return incref(object(Rect(v)).ptr());
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
		if(PyBool_Check(po)) return po;
		if(PyInt_Check(po)) return po;
		if(PyLong_Check(po)) return po;
		if(PyFloat_Check(po)) return po;
		if(PyString_Check(po)) return po;
		if(PyList_Check(po)) return po;
		if(PyDict_Check(po)) return po;
		if(extract<Color>(po).check()) return po;
		if(extract<Point>(po).check()) return po;
		if(extract<Size>(po).check()) return po;
		if(extract<Rect>(po).check()) return po;
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
		new(d) Value(int(c));
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
		String c = extract<String>(po); //use upper converter
		new(d) Value(c);
		}
		else
		if(PyList_Check(po))
		{
		ValueArray c = extract<ValueArray>(po); //use upper converter
		new(d) Value(c);
		}
		else
		if(PyDict_Check(po))
		{
		ValueMap c = extract<ValueMap>(po); //use upper converter
		new(d) Value(c);
		}
		else
		if(extract<Color>(po).check())
		{
		Color c = extract<Color>(po); //use upper converter
		new(d) Value(c.operator Value());
		}
		else
		if(extract<Point>(po).check())
		{
		Point c = extract<Point>(po); //use upper converter
		new(d) Value(c.operator Value());
		}
		else
		if(extract<Size>(po).check())
		{
		Size c = extract<Size>(po); //use upper converter
		new(d) Value(c.operator Value());
		}
		else
		if(extract<Rect>(po).check())
		{
		Rect c = extract<Rect>(po); //use upper converter
		new(d) Value(c.operator Value());
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

//ValueMap

struct ValueMap_to_python
{
	static PyObject* convert(const ValueMap& v)
	{
		dict dt;
		const Index<Value>& k = v.GetKeys();
		const Vector<Value>& vv = v.GetValues().Get();
		ASSERT(k.GetCount() == vv.GetCount());
		for(int i = 0; i < k.GetCount(); i++)
			dt[object(k[i])] = object(vv[i]);
		return incref(dt.ptr()); //need to create a list
	}
};

struct ValueMap_from_python
{
	ValueMap_from_python()
	{
		converter::registry::push_back(&convertible, &construct, type_id<ValueMap>());
	}

	static void* convertible(PyObject* po)
	{
		if(PyDict_Check(po)) return po;
		return 0;
	}

	static void construct(PyObject* po, converter::rvalue_from_python_stage1_data* data)
	{
		void* d = ((converter::rvalue_from_python_storage<ValueMap>*)data)->storage.bytes;
		
		Index<Value> k;
		Vector<Value> vv;
		dict dt(handle<>(borrowed(po)));
		list lk = dt.keys();
		list lv = dt.values();
		for(int i = 0; i < len(dt); i++)
		{
			k.Add(extract<Value>(lk[i]));
			vv.Add(extract<Value>(lv[i]));
		}
		new(d) ValueMap(k, vv);

		data->convertible = d;
	}
};

void export_ValueMap()
{
ONCELOCK
{
	to_python_converter<ValueMap, ValueMap_to_python>();
	ValueMap_from_python();
}
}

END_UPP_NAMESPACE