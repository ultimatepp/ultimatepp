#include "TestData.h"

Vector<String> AliceWords()
{
	return Split(alice_text, [](int c) { return c > 32 ? 0 : c; });
}

Vector<String> AliceLines()
{
	Vector<String> s = Split(Filter(alice_text, [](int c) { return c == '\r' ? 0 : c; }), '\n');
	return s;
}
