#include "Tutorial.h"

void PolyArray()
{
	/// .Polymorphic `Array`

	/// `Array` can even be used for storing polymorphic elements:

	struct Number {
		virtual double Get() const = 0;
		String ToString() const { return AsString(Get()); }
		virtual ~Number() {}
	};
	
	struct Integer : public Number {
		int n;
		virtual double Get() const { return n; }
	};
	
	struct Double : public Number {
		double n;
		virtual double Get() const { return n; }
	};
	
	/// To add such derived types to `Array`, you can best use in-place creation with `Create`
	/// method:
	
	Array<Number> num;
	num.Create<Double>().n = 15.5;
	num.Create<Integer>().n = 3;
	
	DUMP(num);
	
	/// Alternatively, you can use `Add(T *)` method and provide a pointer to the newly created
	/// instance on the heap (`Add` returns a reference to the instance):
	
	Double *nd = new Double;
	nd->n = 1.1;
	num.Add(nd);
	
	DUMP(num);
	
	/// Array takes ownership of heap object and deletes it as appropriate. We recommend to use
	/// this variant only if in-place creation with `Create` is not possible.

	/// It is OK do directly apply U++ algorithms on `Array` (the most stringent requirement of
	/// any of basic algorithms is that there is `IterSwap` provided for container iterators
	/// and that is specialized for `Array` iterators):
	
	Sort(num, [](const Number& a, const Number& b) { return a.Get() < b.Get(); });

	DUMP(num);
	
	///
}
