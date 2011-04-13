#ifndef _BoostPyTest_world_h_
#define _BoostPyTest_world_h_

#include <Py/Py.h>
using namespace boost::python;

//A Simple class

struct World
{
	void set(std::string msg) { this->msg = msg; }
	std::string greet() const { return msg; }
	std::string msg;
};

void export_world();

#endif
