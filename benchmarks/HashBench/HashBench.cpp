#include <Core/Core.h>

using namespace Upp;

#define LDUMP(x) x

template <class T>
String AsHash(const T& x)
{
	Sha1Stream s;
	const_cast<T&>(x).Serialize(s);
	return s.FinishString();
}

CONSOLE_APP_MAIN
{
	Pointf x(1, 2);
	DDUMP(AsHash(x));
	DDUMP(AsHash(Pointf(1, 2)));
	DDUMP(AsHash(Pointf(2, 2)));
}
