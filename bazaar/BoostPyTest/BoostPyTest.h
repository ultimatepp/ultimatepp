#ifndef _BoostPyTest_BoostPyTest_h_
#define _BoostPyTest_BoostPyTest_h_

#include <PyConsoleCtrl/PyConsoleCtrl.h>
using namespace boost::python;

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

//A Simple class

struct World
{
	void set(std::string msg) { this->msg = msg; }
	std::string greet() const { return msg; }
	std::string msg;
};

BOOST_PYTHON_MODULE(hello)
{
	scope().attr("__doc__") = "Hello module's docstring";
	class_<World>("World", "A simple world")
		.def("greet", &World::greet)
		.def("set", &World::set)
	;
}

//A wrapped instance

struct SliderCtrlPy
{
	SliderCtrlPy(SliderCtrl& o) : o(o) {}

	void Set(const int& d) { o.SetData(d); }
	int Get() const { return o.GetData(); }
	
	SliderCtrl& o;
};

BOOST_PYTHON_MODULE(UppCtrl)
{
	scope().attr("__doc__") = "ctrl module's docstring";
	class_<SliderCtrlPy, boost::noncopyable>("SliderCtrlPy", "A SliderCtrl wrapper", no_init)
		.def("get", &SliderCtrlPy::Get)
		.def("set", &SliderCtrlPy::Set)
	;
}

//a convertable instance
namespace UppStringModule {

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

String hello() { return String( "my new custom string" ); } //tests to-python
std::size_t size (const String& s) { return s.GetLength(); } //tests from-python

void init()
{
	to_python_converter<String, String_to_python_str>();
	String_from_python_str();
	def("hello", hello);
	def("size", size);
}

} //ns

BOOST_PYTHON_MODULE(UppString)
{
	UppStringModule::init();
}

#define LAYOUTFILE <BoostPyTest/BoostPyTest.lay>
#include <CtrlCore/lay.h>

class BoostPyTest : public WithBoostPyTestLayout<TopWindow> {
public:
	typedef BoostPyTest CLASSNAME;
	BoostPyTest();
	~BoostPyTest();

	void ExitHandler();

	World w;	
	SliderCtrlPy slpy;
};

#endif
