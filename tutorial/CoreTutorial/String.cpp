#include "Tutorial.h"

void StringTutorial()
{

	/// .String

	/// String is a value type useful for storing text or binary data.
	
	String a = "Hello";
	DUMP(a);

	/// You can concatenate it with another String or literal:

	a = a + " world";
	DUMP(a);
 
	/// Or single character or specified number of characters from another `String` or literal:

	a.Cat('!');
	DUMP(a);
	
	///

	a.Cat("ABCDEFGHIJKLM", 3);
	DUMP(a);

	/// `Clear` method empties the String:

	a.Clear();
	DUMP(a);

	/// You can use `operator<<` to append to existing `String`. Non-string values are
	/// converted to appropriate `String` representation (using standard function `AsString`,
	/// whose default template definition calls `ToString` method for value):
 
	for(int i = 0; i < 10; i++)
		a << i << ", ";

	DUMP(a);

	/// Sometimes is is useful to use `operator<<` to produce a temporary `String` value (e.g. as
	/// real argument to function call):

	String b = String() << "Number is " << 123 << ".";

	DUMP(b);
 

	/// String provides many various methods for obtaining character count, inserting
	/// characters into `String` or removing them:

	a = "0123456789";

	DUMP(a.GetCount());
	
	///
	
	DUMP(a.GetLength()); // GetLength is a synonym of GetCount

	///

	a.Insert(6, "<inserted>");
	DUMP(a);
	
	///

	a.Remove(2, 2);
	DUMP(a);

	/// as well as searching and comparing methods:

	DUMP(a.Find('e'));
	DUMP(a.ReverseFind('e'));
	
	///
	
	DUMP(a.Find("ins"));
	
	///

	DUMP(a.StartsWith("ABC"));
	DUMP(a.StartsWith("01"));
	DUMP(a.EndsWith("89"));

	/// You can get slice of String using Mid method; with single parameter it provides slice
	/// to the end of String:

	DUMP(a.Mid(3, 3));
	DUMP(a.Mid(3));

	/// You can also trim the length of String using Trim (this is faster than using any other
	/// method):

	a.Trim(4);
	DUMP(a);

	/// You can obtain integer values of individual characters using operator[]:

	DUMP(a[0]);

	/// or the value of first character using operator* (note that if `GetCount() == 0`, this
	/// will return zero terminator):

    DUMP(*a);
    
    ///
    
    a.Clear();
    
    DUMP(*a);

	/// `String` has implicit cast to zero terminated `const char *ptr` (only valid as long as
	/// `String` does not mutate:
	
	a = "1234";
	const char *s = a;
	while(*s)
		LOG(*s++);

	/// `String` also has standard `begin` `end` methods, which e.g. allows for C++11 `for`:
	
	for(char ch : a)
		LOG(ch);
	
	/// It is absolutely OK and common to use String for storing binary data, including zeroes:
	
	a.Cat(0);
	
	DUMPHEX(a);
	
	///
}
