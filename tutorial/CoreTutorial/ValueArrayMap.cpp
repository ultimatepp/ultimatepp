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
	
	/// When key is not present in the map, `operator[]` returns void Value (which is also
	/// Null):
	
	DUMP(m["key"]);
	DUMP(m["key"].IsVoid());
	DUMP(IsNull(m["key"]));
	
	/// Just like `VectorMap`, `ValueMap` is ordered, so the order of adding pairs to it
	/// matters:
	
	ValueMap m2;

	m2.Add("two", 2);
	m2("one", 1)("three", 3);

	DUMP(m2);
	DUMP(m == m2); // different order of adding means they are not equal
	
	/// 'Unordered' equality test can be done using `IsSame`:
	
	DUMP(m.IsSame(m2));
	
	/// Iterating ValueMap can be achieved with `GetCount`, `GetKey` and `GetValue`:
	
	for(int i = 0; i < m.GetCount(); i++)
		LOG(m.GetKey(i) << " = " << m.GetValue(i));
	
	/// It is possible to get `ValueArray` of values:
	
	LOG(m.GetValues());
	
	/// `GetKeys` gets constant reference to `Index<Value>` of keys:
	
	LOG(m.GetKeys());
	
	/// It is possible to change the value with `Set`:
	
	m.Set("two", 4);
	
	DUMP(m);
	
	/// Or to change the value of key with `SetKey`:
	
	m.SetKey(1, "four");
	
	DUMP(m);
	
	/// It is possible get a reference of value at given key, (with
	/// ^topic://Core/srcdoc/ValueReference$en-us:special rules^) with `GetAdd` or `operator()`:
	
	Value& h = m("five");
	
	h = 5;
	
	DUMP(m);
	
	/// When ValueMap is stored into Value, `operator[](String)` provides access to value at
	/// key. Note that this narrows keys to text values:
	
	v = m;
	DUMP(v);
	DUMP(v["five"]);
	
	/// `Value::GetAdd` and `Value::operator()` provide a reference to value at key, with
	/// ^topic://Core/srcdoc/ValueReference$en-us:special rules^:
	
	v.GetAdd("newkey") = "foo";
	v("five") = "FIVE";
	
	DUMP(v);
	
	/// `ValueMap` and `ValueArray` are convertible with each other. When assigning `ValueMap`
	/// to `ValueArray`, values are simply used:
	
	ValueArray v2 = m;

	DUMP(v2);
	
	/// When assigning `ValueArray` to `ValueMap`, keys are set as indices of elements:
	
	ValueMap m3 = v2;

	DUMP(m3);
	
	/// With basic `Value` types `int`, `String`, `ValueArray` and `ValueMap`, `Value` can
	/// represent JSON:
	
	Value j = ParseJSON("{ \"array\" : [ 1, 2, 3 ] }");
	
	DUMP(j);
	
	///
	
	j("value") = m;
	
	DUMP(AsJSON(j));
	
	///
	
	j("array").At(1) = ValueMap()("key", 1);
	
	DUMP(AsJSON(j));

	///
}
