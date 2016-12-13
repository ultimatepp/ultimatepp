#include "Tutorial.h"

void CapturingContainers()
{
	/// .Capturing U++ containers into lambdas
	
	/// Capturing objects with pick/clone semantics can be achieved using %capture with an
	/// initializer%:
	
	Vector<int> x{ 1, 2 };
	Array<String> y{ "one", "two" };
	Event<> ev = [x = pick(x), y = clone(y)] { DUMP(x); DUMP(y); };
	
	DUMP(x); // x is picked, so empty
	DUMP(y); // y was cloned, so it retains original value
	
	LOG("About to invoke event");

	ev();
	
	///
}
