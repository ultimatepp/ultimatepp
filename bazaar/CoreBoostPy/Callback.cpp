#include "Callback.h"
using namespace boost::python;

NAMESPACE_UPP

//Callback

struct CallbackActionPy : public CallbackAction {
	object o;
	virtual void Execute() { call<void>(o.ptr()); }
	CallbackActionPy(PyObject *callable) : o(handle<>(borrowed(callable))) {}
};

struct Callback_to_python //FIXME translation of Callback internals to python
{
	static PyObject* convert(const Callback& s) { return incref(object(s).ptr()); }
};

struct Callback_from_python
{
	Callback_from_python() { converter::registry::push_back(&convertible, &construct, type_id<Callback>()); }

	static void* convertible(PyObject* po)
	{
		if(po == Py_None) return po;
		if(extract<Callback&>(po).check()) return po;
		if(PyFunction_Check(po)) return po;
		if(PyCallable_Check(po) && PyMethod_Self(po)) return po;
		return 0;
	}

	static void construct(PyObject* po, converter::rvalue_from_python_stage1_data* data)
	{
		void* d = ((converter::rvalue_from_python_storage<Callback>*)data)->storage.bytes;

		if(po == Py_None) { new(d) Callback(); }
#if 1 //actually isnt used anywhere so far
		else if(extract<Callback&>(po).check()) { 
			Callback& c = extract<Callback&>(po); new(d) Callback(c); }
#endif
		else if(PyFunction_Check(po)) { new(d) Callback(new CallbackActionPy(po)); }
		else if(PyCallable_Check(po) && PyMethod_Self(po)) { new(d) Callback(new CallbackActionPy(po)); }
		else { throw_error_already_set(); } //FIXME dont know type, which to throw? 
	
		data->convertible = d;
	}
};

//Gate

struct GateActionPy : public GateAction {
	object o;
	virtual bool Execute() { return call<bool>(o.ptr()); }
	GateActionPy(PyObject *callable) : o(handle<>(borrowed(callable))) {}
};

struct Gate_from_python
{
	Gate_from_python() { converter::registry::push_back(&convertible, &construct, type_id<Gate>()); }

	static void* convertible(PyObject* po)
	{
		if(po == Py_None) return po;
		if(PyFunction_Check(po)) return po;
		if(PyCallable_Check(po) && PyMethod_Self(po)) return po;
		if(extract<Gate&>(po).check()) return po;
		return 0;
	}

	static void construct(PyObject* po, converter::rvalue_from_python_stage1_data* data)
	{
		void* d = ((converter::rvalue_from_python_storage<Gate>*)data)->storage.bytes;

		if(po == Py_None) { new(d) Gate(); }
		else if(PyFunction_Check(po)) { new(d) Gate(new GateActionPy(po)); }
		else if(PyCallable_Check(po) && PyMethod_Self(po)) { new(d) Gate(new GateActionPy(po)); }
#if 1 //actually isnt used anywhere so far
		else if(extract<Gate&>(po).check()) { Gate& c = extract<Gate&>(po); new(d) Gate(c); }
#endif
		else { throw_error_already_set(); } //FIXME dont know type, which to throw? 
	
		data->convertible = d;
	}
};

///

struct Callback1Action_int_Py : public Callback1Action<int> {
	object o;
	virtual void Execute(int p1) { call<void>(o.ptr(), "i", p1); }
	Callback1Action_int_Py(PyObject *callable) : o(handle<>(borrowed(callable))) {}
};

struct Callback1_int_from_python
{
	Callback1_int_from_python() { converter::registry::push_back(&convertible, &construct, type_id<Callback1<int> >()); }

	static void* convertible(PyObject* po)
	{
		if(po == Py_None) return po;
		if(PyFunction_Check(po)) return po;
		if(PyCallable_Check(po) && PyMethod_Self(po)) return po;
		if(extract<Callback1<int>&>(po).check()) return po;
		return 0;
	}

	static void construct(PyObject* po, converter::rvalue_from_python_stage1_data* data)
	{
		void* d = ((converter::rvalue_from_python_storage<Callback1<int> >*)data)->storage.bytes;

		if(po == Py_None) { new(d) Callback1<int>(); }
		else if(PyFunction_Check(po)) { new(d) Callback1<int>(new Callback1Action_int_Py(po)); }
		else if(PyCallable_Check(po) && PyMethod_Self(po)) { new(d) Callback1<int>(new Callback1Action_int_Py(po)); }
#if 1 //actually isnt used anywhere so far
		else if(extract<Callback1<int>&>(po).check()) { Callback1<int>& c = extract<Callback1<int>&>(po); new(d) Callback1<int>(c); }
#endif
		else { throw_error_already_set(); } //FIXME dont know type, which to throw?
		
		data->convertible = d;
	}
};

//Gate1<int>

struct Gate1Action_int_Py : public Gate1Action<int> {
	object o;
	virtual bool Execute(int p1) { return call<bool>(o.ptr(), "i", p1); }
	Gate1Action_int_Py(PyObject *callable) : o(handle<>(borrowed(callable))) {}
};

struct Gate1_int_from_python
{
	Gate1_int_from_python() { converter::registry::push_back(&convertible, &construct, type_id<Gate1<int> >()); }

	static void* convertible(PyObject* po)
	{
		if(po == Py_None) return po;
		if(PyFunction_Check(po)) return po;
		if(PyCallable_Check(po) && PyMethod_Self(po)) return po;
		if(extract<Gate1<int>&>(po).check()) return po;
		return 0;
	}

	static void construct(PyObject* po, converter::rvalue_from_python_stage1_data* data)
	{
		void* d = ((converter::rvalue_from_python_storage<Gate1<int> >*)data)->storage.bytes;

		if(po == Py_None) { new(d) Gate1<int>(); }
		else if(PyFunction_Check(po)) { new(d) Gate1<int>(new Gate1Action_int_Py(po)); }
		else if(PyCallable_Check(po) && PyMethod_Self(po)) { new(d) Gate1<int>(new Gate1Action_int_Py(po)); }
#if 1 //actually isnt used anywhere so far
		else if(extract<Gate1<int>&>(po).check()) { Gate1<int>& c = extract<Gate1<int>&>(po); new(d) Gate1<int>(c); }
#endif
		else { throw_error_already_set(); } //FIXME dont know type, which to throw?
		
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
		.def("valid", &Callback::operator bool)
		.def("__call__", &Callback::Execute)
	;

	class_<Gate>("Gate", "Upp Gate")
		.def(init<const Gate&>())
		.def("clear", &Gate::Clear)
		.def("valid", &Gate::operator bool)
		.def("__call__", &Gate::Execute)
	;

	class_<Callback1<int> >("Callback1<int>", "Upp Callback1<int>")
		.def(init<const Callback1<int>&>())
		.def("clear", &Callback1<int>::Clear)
		.def("valid", &Callback1<int>::operator bool)
		.def("__call__", &Callback1<int>::Execute)
	;

	class_<Gate1<int> >("Gate1<int>", "Upp Gate1<int>")
		.def(init<const Gate1<int>&>())
		.def("clear", &Gate1<int>::Clear)
		.def("valid", &Gate1<int>::operator bool)
		.def("__call__", &Gate1<int>::Execute)
	;

}
}

END_UPP_NAMESPACE
