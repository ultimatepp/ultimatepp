#include "Tutorial.h"

void Vector2()
{
	/// .`Vector` operations

	/// You can `Insert` or `Remove` elements at random positions of Vector (O(n) complexity):

	Vector<int> v;
	v.Add(1);
	v.Add(2);
	
	v.Insert(1, 10);

	DUMP(v);
	
	///
	
	v.Insert(0, { 7, 6, 5 });
	
	DUMP(v);

	///

	v.Remove(0);
	
	DUMP(v);
	
	/// At method returns element at specified position ensuring that such a position exists.
	/// If there is not enough elements in `Vector`, required number of elements is added. If
	/// second parameter of `At` is present, newly added elements are initialized to this value.

	v.Clear();
	for(int i = 0; i < 10000; i++)
		v.At(Random(10), 0)++;
	
	DUMP(v);

	/// You can apply algorithms on containers, e.g. Sort

	Sort(v);
	
	DUMP(v);
	
	///
}