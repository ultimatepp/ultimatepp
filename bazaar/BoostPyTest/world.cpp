#include "world.h"
using namespace boost::python;
//#include <iostream>
//using namespace std;

NAMESPACE_UPP

void export_world()
{
ONCELOCK
{
	class_<World, boost::noncopyable>("World", "A simple world")
		//.def(init<const World&>())
		.def("__str__", &World::get) //somehow works only for print not for general return as string echo
		.def("greet", &World::greet)
		.def("get", &World::get)
		.def("set", &World::set)
	;

	class_<Universe, bases<World>, boost::noncopyable >("Universe", "A simple Universe")
		//.def(init<const World&>())
		.def("__str__", &Universe::getg) //somehow works only for print not for general return as string echo
		.def("getg", &Universe::getg)
		.def("setg", &Universe::setg)
	;
}
}

END_UPP_NAMESPACE
