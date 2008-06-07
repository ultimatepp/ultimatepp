#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	String a;
	a = "Hello";
	DUMP(a);
	a = a + " world";
	DUMP(a);
	a.Cat('!');
	DUMP(a);
	a.Cat("ABCDEFGHIJKLM", 3);
	DUMP(a);
	a.Clear();
	for(int i = 0; i < 10; i++)
		a << i << ", ";
	DUMP(a);
	DUMP(String().Cat() << "Number is " << 123 << ".");
	a = "0123456789";
	DUMP(a.GetLength());
	a.Insert(6, "<inserted>");
	DUMP(a);
	a.Remove(2, 2);
	DUMP(a);
	DUMP(a.Find('e'));
	DUMP(a.ReverseFind('e'));
	DUMP(a.StartsWith("ABC"));
	DUMP(a.EndsWith("KLM"));
	DUMP(a.Find("ted"));
	DUMP(a.Mid(3, 3));
	DUMP(a.Mid(3));
	a.Trim(4);
	DUMP(a);
	DUMP(a[0]);
	DUMP(*a);
	DUMP(a);
}
