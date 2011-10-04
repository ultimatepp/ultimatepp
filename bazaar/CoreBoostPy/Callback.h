#ifndef _CoreBoostPy_Callback_h_
#define _CoreBoostPy_Callback_h_

#include "Common.h"

namespace boost { namespace python {

template <
    class R
    >
typename detail::returnable<R>::type
invoke(PyObject* callable
    , PyObject* args
    , boost::type<R>* = 0
    )
{
    PyObject* const result = 
        PyObject_CallObject(
            callable
            , args
            );
    
    // This conversion *must not* be done in the same expression as
    // the call, because, in the special case where the result is a
    // reference a Python object which was created by converting a C++
    // argument for passing to PyEval_CallFunction, its reference
    // count will be 2 until the end of the full expression containing
    // the conversion, and that interferes with dangling
    // pointer/reference detection.
    converter::return_from_python<R> converter;
    return converter(result);
}

}} //ns

NAMESPACE_UPP

void* call_convertible(PyObject* po);

//1
#define def_export_Callback1(P1) \
 \
struct Callback1Action_##P1##_Py : public Callback1Action<P1> { \
	object o; \
	virtual void Execute(P1 p1) \
	{ \
		tuple args = make_tuple(p1); \
		invoke<void>(o.ptr(), args.ptr()); \
	} \
	Callback1Action_##P1##_Py(PyObject *callable) : o(handle<>(borrowed(callable))) {} \
}; \
 \
struct Callback1_##P1##_from_python \
{ \
	Callback1_##P1##_from_python() { converter::registry::push_back(&call_convertible, &construct, type_id<Callback1<P1> >()); } \
 \
	static void construct(PyObject* po, converter::rvalue_from_python_stage1_data* data) \
	{ \
		void* d = ((converter::rvalue_from_python_storage<Callback1<P1> >*)data)->storage.bytes; \
 \
		if(po == Py_None) { new(d) Callback1<P1>(); } \
		else if(PyFunction_Check(po)) { new(d) Callback1<P1>(new Callback1Action_##P1##_Py(po)); } \
		else if(PyCallable_Check(po) && PyMethod_Self(po)) { new(d) Callback1<P1>(new Callback1Action_##P1##_Py(po)); } \
		else { throw_error_already_set(); } \
 \
		data->convertible = d; \
	} \
}; \
 \
void export_Callback1_##P1() \
{ \
ONCELOCK \
{ \
	Callback1_##P1##_from_python(); \
	class_<Callback1<P1> >("Callback1_"#P1, "Upp Callback1<"#P1">") \
		.def(init<const Callback1<P1>&>()) \
		.def("clear", &Callback1<P1>::Clear) \
		.def("valid", &Callback1<P1>::operator bool) \
		.def("__call__", &Callback1<P1>::Execute) \
	; \
} \
} \


#define decl_export_Callback1(P1) \
void export_Callback1_##P1() \


#define export_Callback1(P1) \
export_Callback1_##P1() \


#define def_export_Gate1(P1) \
 \
struct Gate1Action_##P1##_Py : public Gate1Action<P1> { \
	object o; \
	virtual bool Execute(P1 p1) \
	{ \
		tuple args = make_tuple(p1); \
		return invoke<bool>(o.ptr(), args.ptr()); \
	} \
	Gate1Action_##P1##_Py(PyObject *callable) : o(handle<>(borrowed(callable))) {} \
}; \
 \
struct Gate1_##P1##_from_python \
{ \
	Gate1_##P1##_from_python() { converter::registry::push_back(&call_convertible, &construct, type_id<Gate1<P1> >()); } \
 \
	static void construct(PyObject* po, converter::rvalue_from_python_stage1_data* data) \
	{ \
		void* d = ((converter::rvalue_from_python_storage<Gate1<P1> >*)data)->storage.bytes; \
 \
		if(po == Py_None) { new(d) Gate1<P1>(); } \
		else if(PyFunction_Check(po)) { new(d) Gate1<P1>(new Gate1Action_##P1##_Py(po)); } \
		else if(PyCallable_Check(po) && PyMethod_Self(po)) { new(d) Gate1<P1>(new Gate1Action_##P1##_Py(po)); } \
		else { throw_error_already_set(); } \
 \
		data->convertible = d; \
	} \
}; \
 \
void export_Gate1_##P1() \
{ \
ONCELOCK \
{ \
	Gate1_##P1##_from_python(); \
	class_<Gate1<P1> >("Gate1_"#P1, "Upp Gate1<"#P1">") \
		.def(init<const Gate1<P1>&>()) \
		.def("clear", &Gate1<P1>::Clear) \
		.def("valid", &Gate1<P1>::operator bool) \
		.def("__call__", &Gate1<P1>::Execute) \
	; \
} \
} \


#define decl_export_Gate1(P1) \
void export_Gate1_##P1() \


#define export_Gate1(P1) \
export_Gate1_##P1() \


//2
#define def_export_Callback2(P1, P2) \
 \
struct Callback2Action_##P1##_##P2##_Py : public Callback2Action<P1, P2> { \
	object o; \
	virtual void Execute(P1 p1, P2 p2) \
	{ \
		tuple args = make_tuple(p1, p2); \
		invoke<void>(o.ptr(), args.ptr()); \
	} \
	Callback2Action_##P1##_##P2##_Py(PyObject *callable) : o(handle<>(borrowed(callable))) {} \
}; \
 \
struct Callback2_##P1##_##P2##_from_python \
{ \
	Callback2_##P1##_##P2##_from_python() { converter::registry::push_back(&call_convertible, &construct, type_id<Callback2<P1, P2> >()); } \
 \
	static void construct(PyObject* po, converter::rvalue_from_python_stage1_data* data) \
	{ \
		void* d = ((converter::rvalue_from_python_storage<Callback2<P1, P2> >*)data)->storage.bytes; \
 \
		if(po == Py_None) { new(d) Callback2<P1, P2>(); } \
		else if(PyFunction_Check(po)) { new(d) Callback2<P1, P2>(new Callback2Action_##P1##_##P2##_Py(po)); } \
		else if(PyCallable_Check(po) && PyMethod_Self(po)) { new(d) Callback2<P1, P2>(new Callback2Action_##P1##_##P2##_Py(po)); } \
		else { throw_error_already_set(); } \
 \
		data->convertible = d; \
	} \
}; \
 \
void export_Callback2_##P1##_##P2() \
{ \
ONCELOCK \
{ \
	Callback2_##P1##_##P2##_from_python(); \
	class_<Callback2<P1, P2> >("Callback2_"#P1"_"#P2, "Upp Callback2<"#P1","#P2">") \
		.def(init<const Callback2<P1, P2>&>()) \
		.def("clear", &Callback2<P1, P2>::Clear) \
		.def("valid", &Callback2<P1, P2>::operator bool) \
		.def("__call__", &Callback2<P1, P2>::Execute) \
	; \
} \
} \


#define decl_export_Callback2(P1, P2) \
void export_Callback2_##P1##_##P2() \


#define export_Callback2(P1, P2) \
export_Callback2_##P1##_##P2() \


#define def_export_Gate2(P1, P2) \
 \
struct Gate2Action_##P1##_##P2##_Py : public Gate2Action<P1, P2> { \
	object o; \
	virtual bool Execute(P1 p1, P2 p2) \
	{ \
		tuple args = make_tuple(p1, p2); \
		return invoke<bool>(o.ptr(), args.ptr()); \
	} \
	Gate2Action_##P1##_##P2##_Py(PyObject *callable) : o(handle<>(borrowed(callable))) {} \
}; \
 \
struct Gate2_##P1##_##P2##_from_python \
{ \
	Gate2_##P1##_##P2##_from_python() { converter::registry::push_back(&call_convertible, &construct, type_id<Gate2<P1, P2> >()); } \
 \
	static void construct(PyObject* po, converter::rvalue_from_python_stage1_data* data) \
	{ \
		void* d = ((converter::rvalue_from_python_storage<Gate2<P1, P2> >*)data)->storage.bytes; \
 \
		if(po == Py_None) { new(d) Gate2<P1, P2>(); } \
		else if(PyFunction_Check(po)) { new(d) Gate2<P1, P2>(new Gate2Action_##P1##_##P2##_Py(po)); } \
		else if(PyCallable_Check(po) && PyMethod_Self(po)) { new(d) Gate2<P1, P2>(new Gate2Action_##P1##_##P2##_Py(po)); } \
		else { throw_error_already_set(); } \
 \
		data->convertible = d; \
	} \
}; \
 \
void export_Gate2_##P1##_##P2() \
{ \
ONCELOCK \
{ \
	Gate2_##P1##_##P2##_from_python(); \
	class_<Gate2<P1, P2> >("Gate2_"#P1"_"#P2, "Upp Gate2<"#P1","#P2">") \
		.def(init<const Gate2<P1, P2>&>()) \
		.def("clear", &Gate2<P1, P2>::Clear) \
		.def("valid", &Gate2<P1, P2>::operator bool) \
		.def("__call__", &Gate2<P1, P2>::Execute) \
	; \
} \
} \


#define decl_export_Gate2(P1, P2) \
void export_Gate2_##P1##_##P2() \


#define export_Gate2(P1, P2) \
export_Gate2_##P1##_##P2() \


//3
#define def_export_Callback3(P1, P2, P3) \
 \
struct Callback3Action_##P1##_##P2##_##P3##_Py : public Callback3Action<P1, P2, P3> { \
	object o; \
	virtual void Execute(P1 p1, P2 p2, P3 p3) \
	{ \
		tuple args = make_tuple(p1, p2, p3); \
		invoke<void>(o.ptr(), args.ptr()); \
	} \
	Callback3Action_##P1##_##P2##_##P3##_Py(PyObject *callable) : o(handle<>(borrowed(callable))) {} \
}; \
 \
struct Callback3_##P1##_##P2##_##P3##_from_python \
{ \
	Callback3_##P1##_##P2##_##P3##_from_python() { converter::registry::push_back(&call_convertible, &construct, type_id<Callback3<P1, P2, P3> >()); } \
 \
	static void construct(PyObject* po, converter::rvalue_from_python_stage1_data* data) \
	{ \
		void* d = ((converter::rvalue_from_python_storage<Callback3<P1, P2, P3> >*)data)->storage.bytes; \
 \
		if(po == Py_None) { new(d) Callback3<P1, P2, P3>(); } \
		else if(PyFunction_Check(po)) { new(d) Callback3<P1, P2, P3>(new Callback3Action_##P1##_##P2##_##P3##_Py(po)); } \
		else if(PyCallable_Check(po) && PyMethod_Self(po)) { new(d) Callback3<P1, P2, P3>(new Callback3Action_##P1##_##P2##_##P3##_Py(po)); } \
		else { throw_error_already_set(); } \
 \
		data->convertible = d; \
	} \
}; \
 \
void export_Callback3_##P1##_##P2##_##P3() \
{ \
ONCELOCK \
{ \
	Callback3_##P1##_##P2##_##P3##_from_python(); \
	class_<Callback3<P1, P2, P3> >("Callback3_"#P1"_"#P2"_"#P3, "Upp Callback3<"#P1","#P2","#P3">") \
		.def(init<const Callback3<P1, P2, P3>&>()) \
		.def("clear", &Callback3<P1, P2, P3>::Clear) \
		.def("valid", &Callback3<P1, P2, P3>::operator bool) \
		.def("__call__", &Callback3<P1, P2, P3>::Execute) \
	; \
} \
} \


#define decl_export_Callback3(P1, P2, P3) \
void export_Callback3_##P1##_##P2##_##P3() \


#define export_Callback3(P1, P2, P3) \
export_Callback3_##P1##_##P2##_##P3() \


#define def_export_Gate3(P1, P2, P3) \
 \
struct Gate3Action_##P1##_##P2##_##P3##_Py : public Gate3Action<P1, P2, P3> { \
	object o; \
	virtual bool Execute(P1 p1, P2 p2, P3 p3) \
	{ \
		tuple args = make_tuple(p1, p2, p3); \
		return invoke<bool>(o.ptr(), args.ptr()); \
	} \
	Gate3Action_##P1##_##P2##_##P3##_Py(PyObject *callable) : o(handle<>(borrowed(callable))) {} \
}; \
 \
struct Gate3_##P1##_##P2##_##P3##_from_python \
{ \
	Gate3_##P1##_##P2##_##P3##_from_python() { converter::registry::push_back(&call_convertible, &construct, type_id<Gate3<P1, P2, P3> >()); } \
 \
	static void construct(PyObject* po, converter::rvalue_from_python_stage1_data* data) \
	{ \
		void* d = ((converter::rvalue_from_python_storage<Gate3<P1, P2, P3> >*)data)->storage.bytes; \
 \
		if(po == Py_None) { new(d) Gate3<P1, P2, P3>(); } \
		else if(PyFunction_Check(po)) { new(d) Gate3<P1, P2, P3>(new Gate3Action_##P1##_##P2##_##P3##_Py(po)); } \
		else if(PyCallable_Check(po) && PyMethod_Self(po)) { new(d) Gate3<P1, P2, P3>(new Gate3Action_##P1##_##P2##_##P3##_Py(po)); } \
		else { throw_error_already_set(); } \
 \
		data->convertible = d; \
	} \
}; \
 \
void export_Gate3_##P1##_##P2##_##P3() \
{ \
ONCELOCK \
{ \
	Gate3_##P1##_##P2##_##P3##_from_python(); \
	class_<Gate3<P1, P2, P3> >("Gate3_"#P1"_"#P2"_"#P3, "Upp Gate3<"#P1","#P2","#P3">") \
		.def(init<const Gate3<P1, P2, P3>&>()) \
		.def("clear", &Gate3<P1, P2, P3>::Clear) \
		.def("valid", &Gate3<P1, P2, P3>::operator bool) \
		.def("__call__", &Gate3<P1, P2, P3>::Execute) \
	; \
} \
} \


#define decl_export_Gate3(P1, P2, P3) \
void export_Gate3_##P1##_##P2##_##P3() \


#define export_Gate3(P1, P2, P3) \
export_Gate3_##P1##_##P2##_##P3() \


//4
#define def_export_Callback4(P1, P2, P3, P4) \
 \
struct Callback4Action_##P1##_##P2##_##P3##_##P4##_Py : public Callback4Action<P1, P2, P3, P4> { \
	object o; \
	virtual void Execute(P1 p1, P2 p2, P3 p3, P4 p4) \
	{ \
		tuple args = make_tuple(p1, p2, p3, p4); \
		invoke<void>(o.ptr(), args.ptr()); \
	} \
	Callback4Action_##P1##_##P2##_##P3##_##P4##_Py(PyObject *callable) : o(handle<>(borrowed(callable))) {} \
}; \
 \
struct Callback4_##P1##_##P2##_##P3##_##P4##_from_python \
{ \
	Callback4_##P1##_##P2##_##P3##_##P4##_from_python() { converter::registry::push_back(&call_convertible, &construct, type_id<Callback4<P1, P2, P3, P4> >()); } \
 \
	static void construct(PyObject* po, converter::rvalue_from_python_stage1_data* data) \
	{ \
		void* d = ((converter::rvalue_from_python_storage<Callback4<P1, P2, P3, P4> >*)data)->storage.bytes; \
 \
		if(po == Py_None) { new(d) Callback4<P1, P2, P3, P4>(); } \
		else if(PyFunction_Check(po)) { new(d) Callback4<P1, P2, P3, P4>(new Callback4Action_##P1##_##P2##_##P3##_##P4##_Py(po)); } \
		else if(PyCallable_Check(po) && PyMethod_Self(po)) { new(d) Callback4<P1, P2, P3, P4>(new Callback4Action_##P1##_##P2##_##P3##_##P4##_Py(po)); } \
		else { throw_error_already_set(); } \
 \
		data->convertible = d; \
	} \
}; \
 \
void export_Callback4_##P1##_##P2##_##P3##_##P4() \
{ \
ONCELOCK \
{ \
	Callback4_##P1##_##P2##_##P3##_##P4##_from_python(); \
	class_<Callback4<P1, P2, P3, P4> >("Callback4_"#P1"_"#P2"_"#P3"_"#P4, "Upp Callback4<"#P1","#P2","#P3","#P4">") \
		.def(init<const Callback4<P1, P2, P3, P4>&>()) \
		.def("clear", &Callback4<P1, P2, P3, P4>::Clear) \
		.def("valid", &Callback4<P1, P2, P3, P4>::operator bool) \
		.def("__call__", &Callback4<P1, P2, P3, P4>::Execute) \
	; \
} \
} \


#define decl_export_Callback4(P1, P2, P3, P4) \
void export_Callback4_##P1##_##P2##_##P3##_##P4() \


#define export_Callback4(P1, P2, P3, P4) \
export_Callback4_##P1##_##P2##_##P3##_##P4() \


#define def_export_Gate4(P1, P2, P3, P4) \
 \
struct Gate4Action_##P1##_##P2##_##P3##_##P4##_Py : public Gate4Action<P1, P2, P3, P4> { \
	object o; \
	virtual bool Execute(P1 p1, P2 p2, P3 p3, P4 p4) \
	{ \
		tuple args = make_tuple(p1, p2, p3, p4); \
		return invoke<bool>(o.ptr(), args.ptr()); \
	} \
	Gate4Action_##P1##_##P2##_##P3##_##P4##_Py(PyObject *callable) : o(handle<>(borrowed(callable))) {} \
}; \
 \
struct Gate4_##P1##_##P2##_##P3##_##P4##_from_python \
{ \
	Gate4_##P1##_##P2##_##P3##_##P4##_from_python() { converter::registry::push_back(&call_convertible, &construct, type_id<Gate4<P1, P2, P3, P4> >()); } \
 \
	static void construct(PyObject* po, converter::rvalue_from_python_stage1_data* data) \
	{ \
		void* d = ((converter::rvalue_from_python_storage<Gate4<P1, P2, P3, P4> >*)data)->storage.bytes; \
 \
		if(po == Py_None) { new(d) Gate4<P1, P2, P3, P4>(); } \
		else if(PyFunction_Check(po)) { new(d) Gate4<P1, P2, P3, P4>(new Gate4Action_##P1##_##P2##_##P3##_##P4##_Py(po)); } \
		else if(PyCallable_Check(po) && PyMethod_Self(po)) { new(d) Gate4<P1, P2, P3, P4>(new Gate4Action_##P1##_##P2##_##P3##_##P4##_Py(po)); } \
		else { throw_error_already_set(); } \
 \
		data->convertible = d; \
	} \
}; \
 \
void export_Gate4_##P1##_##P2##_##P3##_##P4() \
{ \
ONCELOCK \
{ \
	Gate4_##P1##_##P2##_##P3##_##P4##_from_python(); \
	class_<Gate4<P1, P2, P3, P4> >("Gate4_"#P1"_"#P2"_"#P3"_"#P4, "Upp Gate4<"#P1","#P2","#P3","#P4">") \
		.def(init<const Gate4<P1, P2, P3, P4>&>()) \
		.def("clear", &Gate4<P1, P2, P3, P4>::Clear) \
		.def("valid", &Gate4<P1, P2, P3, P4>::operator bool) \
		.def("__call__", &Gate4<P1, P2, P3, P4>::Execute) \
	; \
} \
} \


#define decl_export_Gate4(P1, P2, P3, P4) \
void export_Gate4_##P1##_##P2##_##P3##_##P4() \


#define export_Gate4(P1, P2, P3, P4) \
export_Gate4_##P1##_##P2##_##P3##_##P4() \


//

void export_Callback();
void export_Gate();

END_UPP_NAMESPACE

#endif
