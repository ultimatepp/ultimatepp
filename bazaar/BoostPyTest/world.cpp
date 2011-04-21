#include "world.h"
using namespace boost::python;
//#include <iostream>
//using namespace std;

NAMESPACE_UPP

void export_world()
{
ONCELOCK
{
	class_<World>("World", "A simple world")
		.def("__str__", &World::greet) //somehow works only for print not for general return as string echo
		.def("greet", &World::greet)
		.def("set", &World::set)
	;
}
}

END_UPP_NAMESPACE
