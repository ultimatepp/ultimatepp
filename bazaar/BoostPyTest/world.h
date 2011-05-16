#ifndef _BoostPyTest_world_h_
#define _BoostPyTest_world_h_

#include <Py/Py.h>

#define PUREVIRTEST 0

class World
{
public:
	void set(std::string msg) { this->msg = msg; }
	std::string get() const { return msg; }
	std::string msg;
	
	void greet()
	{
		PySys_WriteStdout("%.100s", msg.data());
	}
	
	virtual std::string vir(int a = 2) const 
#if PUREVIRTEST
	= 0;
#else
	{ return "virtual World"; }
#endif
	World() {}
protected:
	World(const World& w) {
		msg = w.msg;
	}
};

class Universe : public World
{
public:
	double g;	
	void setg(double _g) { g = _g; }
	double getg() const { return g; }

	virtual std::string vir(int a = 3) const { return "virtual Universe"; }

	Universe() { g = 9.81; }

#if 0
	Universe(const Universe& u) : World(u), g(u.g) {}
#endif
};

NAMESPACE_UPP

void export_world();

END_UPP_NAMESPACE

#endif
