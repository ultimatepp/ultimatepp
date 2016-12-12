#include "Tutorial.h"

void AsStringTutorial()
{
	/// .`AsString`, `ToString` and `operator<<`

	/// U++ Core provides simple yet effective standard schema for converting values to default
	/// textual form. System is based on the combination of template functions (following code
	/// is part of U++ library):

#if 0
namespace Upp {
	template <class T>
	inline String AsString(const T& x)
	{
		return x.ToString();
	}
	
	template <class T>
	inline Stream& operator<<(Stream& s, const T& x)
	{
		s << AsString(x);
		return s;
	}
	
	template <class T>
	inline String& operator<<(String& s, const T& x)
	{
		s.Cat(AsString(x));
		return s;
	}
};
#endif

	/// Client types have to either define `String ToString` method or specialize `AsString`
	/// template in `Upp` namespace. Such types can be appended to Streams or Strings using
	/// `operator<<`. Of course, U++ value types and primitive types have required items
	/// predefined by U++:

	FileOut fout(ConfigFile("test.txt"));
	String  sout;
	
	fout << 1.23 << ' ' << GetSysDate() << ' ' << GetSysTime();
	sout << 1.23 << ' ' << GetSysDate() << ' ' << GetSysTime();
	
	fout.Close();

	DUMP(LoadFile(ConfigFile("test.txt")));
	DUMP(sout);

	/// Getting client types involved into this schema is not too difficult, all you need to do
	/// is to add `ToString` method:

	struct BinFoo {
		int x;
		
		String ToString() const   { return FormatIntBase(x, 2); }
	};
	
	BinFoo bf;
	bf.x = 30;
	
	sout.Clear();
	sout << bf;
	DUMP(sout);
	
	/// If you cannot add `ToString`, you can still specialize template in Upp namespace:
	
	struct RomanFoo {
		int x;
		
		RomanFoo(int x) : x(x) {}
	};

#if 0
	namespace Upp {
	template <> String Upp::AsString(const RomanFoo& a) { return FormatIntRoman(a.x); }
	};
#endif

	///
}
