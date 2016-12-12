#include "Tutorial.h"

void ArrayTutorial()
{
	/// .Array flavor

	/// If elements are not `Moveable` and therefore cannot be stored in `Vector` flavor, they
	/// can still be stored in `Array` flavor. Another reason for using Array is the need for
	/// referencing elements - Array flavor never invalidates references or pointers to them.
	/// Finally, if sizeof(T) is large (say more than 100-200 bytes), using Array might be better
	/// from performance perspective.

	/// Example of elements that cannot be stored in Vector flavor are standard library objects like
	/// `std::string` (because obviously, standard library knows nothing about U++ Moveable
	/// concept):

	Array<std::string> as;
	for(int i = 0; i < 4; i++)
		as.Add("Test");

	for(auto s : as)
		DUMP(s.c_str());
	
	///
}
