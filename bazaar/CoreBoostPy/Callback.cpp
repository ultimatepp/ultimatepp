#include "Callback.h"
using namespace boost::python;

NAMESPACE_UPP

struct CallbackActionPy : public CallbackAction {
	object o;
	void Execute() { call<void>(o.ptr()); }
	CallbackActionPy(PyObject *callable) : o(handle<>(borrowed(callable))) {}
};

bool CallbackIsValid(const Callback& c) { return c; }

struct Callback_to_python //FIXME translation of Callback internals to python
{
	static PyObject* convert(const Callback& s) { return incref(object(s).ptr()); }
};

struct Callback_from_python
{
	Callback_from_python()
	{
		converter::registry::push_back(&convertible, &construct, type_id<Callback>());
	}

	static void* convertible(PyObject* po)
	{
		if(po == Py_None) return po;
		if(PyFunction_Check(po)) return po;
		if(PyCallable_Check(po) && PyMethod_Self(po)) return po;
		if(extract<Callback&>(po).check()) return po;
		return 0;
	}

	static void construct(PyObject* po, converter::rvalue_from_python_stage1_data* data)
	{
		void* d = ((converter::rvalue_from_python_storage<Callback>*)data)->storage.bytes;

		if(po == Py_None)
		{
			new(d) Callback();
		}
		else
		if(PyFunction_Check(po))
		{
		new(d) Callback(new CallbackActionPy(po));
		}
		else
		if(PyCallable_Check(po) && PyMethod_Self(po))
		{
		new(d) Callback(new CallbackActionPy(po));
		}
#if 1 //actually isnt used anywhere so far
		else
		if(extract<Callback&>(po).check())
		{
		Callback& c = extract<Callback&>(po);
		new(d) Callback(c);
		}
#endif
		else
		{
			throw_error_already_set(); //FIXME dont know type, which to throw?
		}
		
		data->convertible = d;
	}
};

void export_Callback()
{
ONCELOCK
{
	//to_python_converter<Callback, Callback_to_python>();
	Callback_from_python();
	
	class_<Callback>("Callback", "Upp Callback")
		.def(init<const Callback&>())
		.def("clear", &Callback::Clear)
		.def("valid", &CallbackIsValid)
		.def("__call__", &Callback::Execute)
	;
}
}

END_UPP_NAMESPACE
