#include "Tutorial.h"

void WStringTutorial()
{
	/// .WString

	/// String works with 8 bit characters. For 16-bit character encoding use `WString`. Both
	/// classes are closely related and share most of interface methods. U++ also provides
	/// conversions between `String` and `WString` and you can also use 8 bit string literals with
	/// `WString`. Conversion is ruled by current default character set. Default value of default
	/// character set is `CHARSET_UTF8`. This conversion is also used in `WString::ToString`,
	/// e.g. when putting `WString` to log:

	WString x = "characters 280-300: "; // you can assign 8-bit character literal to WString
	for(int i = 280; i < 300; i++)
		x.Cat(i);
	
	DUMP(x);
	
	/// `ToString` converts `WString` to `String`:

	String y = x.ToString();
	DUMP(y);

	/// `ToWString` converts `String` to `WString`:

	y.Cat(" (appended)"); // you can use 8-bit character literals in most WString operations
	x = y.ToWString();
	
	DUMP(x);
	
	///
}
