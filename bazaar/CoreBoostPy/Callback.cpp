#include "Callback.h"
using namespace boost::python;

NAMESPACE_UPP

void* call_convertible(PyObject* po)
{
	if(po == Py_None) return po;
	if(PyFunction_Check(po)) return po;
	if(PyCallable_Check(po) && PyMethod_Self(po)) return po;
	return 0;
}

//Callback
struct CallbackActionPy : public CallbackAction {
	object o;
	virtual void Execute()
	{
		tuple args;
		invoke<void>(o.ptr(), args.ptr());
	}
	CallbackActionPy(PyObject *callable) : o(handle<>(borrowed(callable))) {}
};

struct Callback_from_python
{
	Callback_from_python() { converter::registry::push_back(&call_convertible, &construct, type_id<Callback>()); }

	static void construct(PyObject* po, converter::rvalue_from_python_stage1_data* data)
	{
		void* d = ((converter::rvalue_from_python_storage<Callback>*)data)->storage.bytes;

		if(po == Py_None) { new(d) Callback(); }
		else if(PyFunction_Check(po)) { new(d) Callback(new CallbackActionPy(po)); }
		else if(PyCallable_Check(po) && PyMethod_Self(po)) { new(d) Callback(new CallbackActionPy(po)); }
		else { throw_error_already_set(); }
	
		data->convertible = d;
	}
};

//Gate
struct GateActionPy : public GateAction {
	object o;
	virtual bool Execute()
	{
		tuple args;
		return invoke<bool>(o.ptr(), args.ptr());
	}
	GateActionPy(PyObject *callable) : o(handle<>(borrowed(callable))) {}
};

struct Gate_from_python
{
	Gate_from_python() { converter::registry::push_back(&call_convertible, &construct, type_id<Gate>()); }

	static void construct(PyObject* po, converter::rvalue_from_python_stage1_data* data)
	{
		void* d = ((converter::rvalue_from_python_storage<Gate>*)data)->storage.bytes;

		if(po == Py_None) { new(d) Gate(); }
		else if(PyFunction_Check(po)) { new(d) Gate(new GateActionPy(po)); }
		else if(PyCallable_Check(po) && PyMethod_Self(po)) { new(d) Gate(new GateActionPy(po)); }
		else { throw_error_already_set(); }
	
		data->convertible = d;
	}
};

void export_Callback()
{
ONCELOCK
{
	Callback_from_python();
	class_<Callback>("Callback", "Upp Callback")
		.def(init<const Callback&>())
		.def("clear", &Callback::Clear)
		.def("valid", &Callback::operator bool)
		.def("__call__", &Callback::Execute)
	;
}
}

void export_Gate()
{
ONCELOCK
{
	Callback_from_python();
	class_<Gate>("Gate", "Upp Gate")
		.def(init<const Gate&>())
		.def("clear", &Gate::Clear)
		.def("valid", &Gate::operator bool)
		.def("__call__", &Gate::Execute)
	;
}
}

END_UPP_NAMESPACE
