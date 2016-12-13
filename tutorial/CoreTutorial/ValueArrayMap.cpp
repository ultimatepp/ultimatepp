#include "Tutorial.h"

void ValueArrayMap()
{
	/// .`ValueArray` and `ValueMap`
	
	/// `ValueArray` is a type that represents an array of `Value`s:
	
	ValueArray va{1, 2, 3};
	
	DUMP(va);
	
	/// ValueArray can be assigned to Value (and back):
	
	Value v = va;
	
	DUMP(v);
	DUMP(v.Is<ValueArray>()); // must be exactly ValueArray
	DUMP(IsValueArray(v)); // is ValueArray or ValueMap (which is convertible to ValueArray)
	
	ValueArray va2 = v;
	
	DUMP(va2);

	/// Elements can be appended using `Add` method or `operator<<`, element at
	/// index can be changed with `Set`:
	
	va.Add(10);
	va << 20 << 21;
	va.Set(0, 999);
	
	DUMP(va);

	/// Elements can be removed:
	
	va.Remove(0, 2);
	
	DUMP(va);

	/// and inserted:
	
	va.Insert(1, v);
	
	DUMP(va);
	
	/// It is possible to get a reference to element at index, however note that some
	/// ^topic://Core/srcdoc/ValueReference$en-us:special rules^ apply here:
	
	va.At(0) = 222;
	
	DUMP(va);
	
	/// If `Value` contains `ValueArray`, `Value::GetCount` method returns the number of
	/// elements in the array (if there is no `ValueArray` in `Value`, it returns zero). You
	/// can use `Value::operator[](int)` to get constant reference to `ValueArray` elements:
	
	for(int i = 0; i < v.GetCount(); i++)
		LOG(v[i]);
	
	/// It is even possible to directly add element to `Value` if it contains `ValueArray`:
	
	v.Add(4);
	
	DUMP(v);
	
	/// Or even get a reference to element at some index (with
	/// ^topic://Core/srcdoc/ValueReference$en-us:special rules^):
	
	v.At(0) = 111;
	
	DUMP(v);
	
	/// `ValueMap` can store key - value pairs and retrieve value for key quickly. Note that
	/// keys are not limited to `String`, but can be any `Value` with `operator==` and hash
	/// code defined.
	
	/// `Add` method or `operator()` add data to `ValueMap`:
	
	ValueMap m;

	m.Add("one", 1);
	m("two", 2)("three", 3);

	DUMP(m);
	
	/// `operator[]` retrieves the value at the key:
	
	DUMP(m["two"]);
	
	/// Just like `VectorMap`, `ValueMap` is ordered, so the order of adding pairs to it
	/// matters:
	
	ValueMap m2;

	m2.Add("two", 2);
	m2("one", 1)("three", 3);

	DUMP(m2);
	DUMP(m == m2); // different order of adding means they are not equal
	
	/// 'Unordered' equality test can be done using `IsSame`:
	
	DUMP(m.IsSame(m2));
	
	/// 
	
	Value j = ParseJSON("{ \"array\" : [ 1, 2, 3 ] }");
	
	DUMP(j);
	
	///
	
	j("value") = m;
	
	DUMP(AsJSON(j));
	
	///
}
