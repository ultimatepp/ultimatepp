#include "world.h"
using namespace boost::python;
//#include <iostream>
//using namespace std;


class WorldWrap : public World, public wrapper<World> {
public:
	virtual std::string vir(int a) const {
		//if pure virtual
#if PUREVIRTEST
		return this->get_override("vir")(a);
#else
		//from c++ calls to WorldWrap instaces as World instance (i.e. InvokeWorld)
		//calls python derived function if present, or base function.
		if(override f = this->get_override("vir"))
			return f(a);
		return World::vir(a);
#endif
	}

#if PUREVIRTEST
#else	
	//from python calls this, if not derived in python
	//if vir derived in python, gets called directly
	std::string default_vir(int a) const {
		return World::vir(a);
	}
#endif

};

std::string InvokeWorld(const World& w, int a) //for testing the instances from python env
{
	return w.vir(a);	
}

class UniverseWrap : public Universe, public wrapper<Universe> {
public:
	virtual std::string vir(int a) const {
		//if pure virtual
#if PUREVIRTEST
		return this->get_override("vir")(a);
#else
		//from c++ calls to WorldWrap instaces as World instance (i.e. InvokeWorld)
		//calls python derived function if present, or base function.
		if(override f = this->get_override("vir"))
			return f(a);
		return Universe::vir(a);
#endif
	}

#if PUREVIRTEST
#else	
	//from python calls this, if not derived in python
	//if vir derived in python, gets called directly
	std::string default_vir(int a) const {
		return Universe::vir(a);
	}
#endif

};

NAMESPACE_UPP

void export_world()
{
ONCELOCK
{
	class_<WorldWrap, boost::noncopyable>("World", "A simple world"
#if 0 //PUREVIRTEST, not needed though abstract, otherwise it cant instantiate in python
		, no_init
#endif	
		)
		//.def(init<const World&>())
		.def("__str__", &World::get) //somehow works only for print not for general return as string echo
		.def("greet", &World::greet)
		.def("get", &World::get)
		.def("set", &World::set)
#if PUREVIRTEST
		.def("vir", pure_virtual(&World::vir))
#else
		.def("vir",  &World::vir, &WorldWrap::default_vir)
#endif
	;

	def("invworld", &InvokeWorld);

	class_<UniverseWrap, bases<World>, boost::noncopyable >("Universe", "A simple Universe")
		//.def(init<const Universe&>())
		.def("__str__", &Universe::getg) //somehow works only for print not for general return as string echo
		.def("getg", &Universe::getg)
		.def("setg", &Universe::setg)
#if 1
//somehow does not override the World export of vir
#if PUREVIRTEST
		.def("vir", pure_virtual(&Universe::vir))
#else
		.def("vir",  &Universe::vir, &UniverseWrap::default_vir)
#endif
#endif
	;
}
}

END_UPP_NAMESPACE
