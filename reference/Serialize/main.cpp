#include <Core/Core.h>

using namespace Upp;

struct Foo {
	int         q;
	String      text;
	Vector<int> data;

	void Serialize(Stream& s)
	{
		s % q % text % data;
	}

	String ToString() const
	{
		String s;
		s << "q = " << q << ", text = \"" << text << "\", data:";
		for(int i = 0; i < data.GetCount(); i++)
			s << ' ' << data[i];
		return s;
	}

	Foo() { q = 0; }
};

CONSOLE_APP_MAIN
{
	Foo foo;
	LoadFromFile(foo, ConfigFile("data.bin"));
	Cout() << "Loaded Foo: " << foo << '\n';
	foo.text = FormatIntRoman(foo.q);
	foo.q++;
	foo.data.Add(foo.q);
	StoreToFile(foo, ConfigFile("data.bin"));
	Cout() << "Saved Foo: " << foo << '\n';
}
