#include "Tutorial.h"

void NullTutorial()
{
	/// .`Null`
	
	/// U++ defines a special `Null` constant to represent an empty value. This constant is
	/// convertible to many value types including primitive types `double`, `int` and `int64`
	/// (defined as lowest number the type can represent). If type supports ordering (<, >),
	/// all values of the type are greater than Null value. To test whether a value is empty,
	/// use `IsNull` function.

	int x = Null;
	int y = 120;
	Date d = Null;
	Date e = GetSysDate();

	DUMP(x);
	DUMP(y);
	DUMP(d);
	DUMP(e > d);

	/// `Null` is the only instance of `Nuller` type. Assigning `Null` to
	/// primitive types is achieved by cast operators of `Nuller`, other types can do it using
	/// constructor from `Nuller`.
	
	/// As a special case, if `Value` contains `Null`, it is convertible to any value type that can contain `Null`:

	Value v = x; // x is int
	e = v; // e is Date, but v is Null, so Null is assigned to e

	DUMP(IsNull(e));

	/// Function `Nvl` is U++ analog of well known SQL function coalesce (ifnull, Nvl), which
	/// returns the first non-null argument (or `Null` if all are `Null`).

	int a = Null;
	int b = 123;
	int c = 1;

	DUMP(Nvl(a, b, c));
	
	///
}
