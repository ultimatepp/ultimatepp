#include "world.h"

void export_world()
{
	scope().attr("__doc__") = "Hello module's docstring";
	class_<World>("World", "A simple world")
		.def("greet", &World::greet)
		.def("set", &World::set)
	;
}
