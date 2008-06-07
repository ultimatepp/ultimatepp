#include <Core/Core.h>

using namespace Upp;

struct BinFoo {
	int x;

	String ToString() const   { return FormatIntBase(x, 2); }

	BinFoo(int x) : x(x) {}
};

struct RomanFoo {
	int x;

	RomanFoo(int x) : x(x) {}
};

namespace Upp {
template <> String AsString(const RomanFoo& a) { return FormatIntRoman(a.x); }
};

CONSOLE_APP_MAIN
{
	FileOut fout(ConfigFile("test.txt"));
	String  sout;

	fout << 1.23 << ' ' << GetSysDate() << ' ' << GetSysTime();
	sout << 1.23 << ' ' << GetSysDate() << ' ' << GetSysTime();

	DUMP(sout);

	sout.Clear();
	fout << '\n';

	sout << BinFoo(30) << ' ' << RomanFoo(30);
	fout << BinFoo(30) << ' ' << RomanFoo(30);

	DUMP(sout);
}
