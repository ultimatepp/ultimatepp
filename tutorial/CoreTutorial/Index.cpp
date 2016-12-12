#include "Tutorial.h"

void IndexTutorial() {
	/// .`Index`
	
	/// `Index` is the the foundation of all U++ associative operations and is one of defining
	/// features of U++.

	/// `Index` is a container very similar to the plain `Vector` (it is random access array of
	/// elements with fast addition at the end) with one additional feature - it is able to fast
	/// retrieve position of element with required value using `Find` method:

	Index<String> ndx;
	ndx.Add("alfa");
	ndx.Add("beta");
	ndx.Add("gamma");
	ndx.Add("delta");
	ndx.Add("kappa");

	DUMP(ndx);
	DUMP(ndx.Find("beta"));

	/// If element is not present in `Index`, `Find` returns a negative value:
	
	DUMP(ndx.Find("something"));

	/// Any element can be replaced using `Set` method:

	ndx.Set(1, "alfa");
	
	DUMP(ndx);

	/// If there are more elements with the same value, they can be iterated using `FindNext`
	/// method:

	int fi = ndx.Find("alfa");
	while(fi >= 0) {
		DUMP(fi);
		fi = ndx.FindNext(fi);
	}

	/// `FindAdd` method retrieves position of element like `Find`, but if element is not
	/// present in `Index`, it is added:
	
	DUMP(ndx.FindAdd("one"));
	DUMP(ndx.FindAdd("two"));
	DUMP(ndx.FindAdd("three"));
	DUMP(ndx.FindAdd("two"));
	DUMP(ndx.FindAdd("three"));
	DUMP(ndx.FindAdd("one"));

	/// Removing elements from random access sequence tends to be expensive, that is why rather
	/// than remove, `Index` supports `Unlink` and `UnlinkKey` operations, which retain the
	/// element in `Index` but make it invisible for `Find` operation:
	
	ndx.Unlink(2);
	ndx.UnlinkKey("kappa");

	DUMP(ndx.Find(ndx[2]));
	DUMP(ndx.Find("kappa"));

	/// You can test whether element at given position is unlinked using `IsUnlinked` method

	DUMP(ndx.IsUnlinked(1));
	DUMP(ndx.IsUnlinked(2));

	/// Unlinked positions can be reused by `Put` method:
	
	ndx.Put("foo");
	
	DUMP(ndx);
	DUMP(ndx.Find("foo"));

	/// You can also remove all unlinked elements from `Index` using `Sweep` method:

	ndx.Sweep();

	DUMP(ndx);
	
	/// Operations directly removing or inserting elements of Index are expensive, but
	/// available too:

	ndx.Remove(1);
	
	DUMP(ndx);

	///

	ndx.RemoveKey("two");

	DUMP(ndx);
	
	///
	
	ndx.Insert(0, "insert");
	
	DUMP(ndx);
	
	/// PickKeys operation allows you to obtain Vector of elements of Index in low
	/// constant time operation (while destroying source Index)
	
	Vector<String> d = ndx.PickKeys();
	
	DUMP(d);
	
	/// Pick-assigning `Vector` to `Index` is supported as well:
	
	d[0] = "test";
	
	ndx = pick(d);
	
	DUMP(ndx);
	
	///
}
