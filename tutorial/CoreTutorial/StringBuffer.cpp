#include "Tutorial.h"

void CApiFunction(char *c)
{
	strcpy(c, "Hello");
}

void StringBufferTutorial()
{

	/// .StringBuffer
	
	/// If you need a direct write access to `String`'s C-string character buffer, you can use
	/// complementary `StringBuffer` class. One of reasons to do so is when you have to deal
	/// with some C-API functions that expects to write directly to `char *` and you would like
	/// that result converted to the `String`:

#if 0
	void CApiFunction(char *c)
	{
		strcpy(c, "Hello");
	}
#endif
	
	StringBuffer b;
	b.SetLength(200);
	CApiFunction(b);
	b.Strlen();
	String x = b;
	
	DUMP(x);

	/// In this case, `SetLength` creates a C array of 200 characters. You can then call C-API
	/// function. Later you set the real length using `Strlen` - this function performs strlen
	/// of buffer and sets the length accordingly. Later you simply assign the `StringBuffer`
	/// to `String`. Note that for performance reasons, this operation clears the
	/// `StringBuffer` content (operation is fast and does not depend on the number of
	/// characters).
	
	/// Another usage scenario of StringBuffer is altering existing String:

	b = x;
	b[1] = 'a';
	x = b;
	
	DUMP(x);

	/// Similar to assigning StringBuffer to String, assigning String to StringBuffer clears
	/// the source String.

	/// StringBuffer also provides appending operations:

	b = x;
	b.Cat('!');
	x = b;
	
	DUMP(x);

	/// Note that sometimes when creating some String from a lot of single characters, using
	/// StringBuffer for the operation is slightly faster then using String directly.
}
