#include "TestData.h"

Vector<String> AliceWords()
{
	Vector<String> w = Split(alice_text, [](int c) { return c > 32 ? 0 : c; });
	Vector<String> r;
	for(String s : w) {
		s = ToLower(s);
		int q = 0;
		while(!IsAlNum(s[q]) && q < s.GetCount())
			q++;
		s = s.Mid(q);
		while(s.GetCount() && !IsAlNum(*s.Last()))
			s.TrimLast();
		if(s.GetCount())
			r.Add(s);
	}
	return r;
}

Vector<String> AliceLines()
{
	Vector<String> s = Split(Filter(alice_text, [](int c) { return c == '\r' ? 0 : c; }), '\n');
	return s;
}
