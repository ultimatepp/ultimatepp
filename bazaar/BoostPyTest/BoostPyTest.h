#ifndef _BoostPyTest_BoostPyTest_h_
#define _BoostPyTest_BoostPyTest_h_

#include <PyConsoleCtrl/PyConsoleCtrl.h>

using namespace boost::python;

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

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

struct SliderCtrlPy
{
	SliderCtrlPy(SliderCtrl& o) : o(o) {}

	//void Set(const int& d) { o.SetData(d); }
	void Set(int d) { o.SetData(d); }
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

#define LAYOUTFILE <BoostPyTest/BoostPyTest.lay>
#include <CtrlCore/lay.h>

class BoostPyTest : public WithBoostPyTestLayout<TopWindow> {
public:
	typedef BoostPyTest CLASSNAME;
	BoostPyTest();
	~BoostPyTest();

	World w;	
	SliderCtrlPy slpy;
};

#endif
