#ifndef _BoostPyTest_world_h_
#define _BoostPyTest_world_h_

#include <Py/Py.h>

struct World
{
	void set(std::string msg) { this->msg = msg; }
	std::string greet() const { return msg; }
	std::string msg;
};

NAMESPACE_UPP

void export_world();

END_UPP_NAMESPACE

#endif
