#ifndef _BoostPyTest_world_h_
#define _BoostPyTest_world_h_

#include <Py/Py.h>

struct World
{
	void set(std::string msg) { this->msg = msg; }
	std::string get() const { return msg; }
	std::string msg;
	
	void greet()
	{
		PySys_WriteStdout("%.100s", msg.data());
	}
	
	World() {}
protected:
	World(const World& w) {
		msg = w.msg;
	}
};

struct Universe : public World
{
	double g;	
	void setg(double _g) { g = _g; }
	double getg() const { return g; }

	Universe() { g = 9.81; }

#if 0
	Universe(const Universe& u) : World(u), g(u.g) {}
#endif
};

NAMESPACE_UPP

void export_world();

END_UPP_NAMESPACE

#endif
