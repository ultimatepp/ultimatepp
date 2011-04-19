#include "world.h"
using namespace boost::python;

NAMESPACE_UPP

void export_world()
{
ONCELOCK
{
	scope().attr("__doc__") = "Hello module's docstring";
	class_<World>("World", "A simple world")
		.def("greet", &World::greet)
		.def("set", &World::set)
	;
}
}

END_UPP_NAMESPACE
