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
	
	/// `At` method returns element at specified position ensuring that such a position exists.
	/// If there is not enough elements in `Vector`, required number of elements is added. If
	/// second parameter of `At` is present, newly added elements are initialized to this value.

	v.Clear();
	for(int i = 0; i < 10000; i++)
		v.At(Random(10), 0)++;
	
	DUMP(v);
	
	/// Referencing invalid index is undefined operation. Sometimes however it is useful to
	/// return the element value if index is valid and some default value if it is not. This
	/// can be achieved with two parameter Get method:
		
	DUMP(v.Get(4, 0));
	DUMP(v.Get(-10, 0));
	DUMP(v.Get(13, -1));

	/// You can apply algorithms on containers, e.g. Sort

	Sort(v);
	
	DUMP(v);
	
	///
}