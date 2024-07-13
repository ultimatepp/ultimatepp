#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	String x = "0123456789012345";
	ASSERT(x.FindFirstOf("654") == 4);
	ASSERT(x.FindFirstOf("46") == 4);
	ASSERT(x.FindFirstOf("32", 10) == 12);
	ASSERT(x.FindFirstOf("23", 10) == 12);
	ASSERT(x.FindFirstOf(x) == 0);
	ASSERT(x.Mid(3, 4) == String("3456"));
	ASSERT(x.Mid(3) == String("3456789012345"));
	ASSERT(x.Left(2) == String("01"));
	ASSERT(x.Right(2) == String("45"));
	ASSERT(x.Find('3') == 3);
	ASSERT(x.ReverseFind('3') == 13);
	ASSERT(x.GetLength() == 16);
	ASSERT(x.Begin() + x.GetLength() == x.End());
	ASSERT(x[0] == '0');
	x.Insert(1, 'a');
	ASSERT(x == String("0a123456789012345"));
	x.Insert(2, ~String("bcd"), 2);
	ASSERT(x == String("0abc123456789012345"));
	x.Insert(1, ~String("_"));
	ASSERT(x == String("0_abc123456789012345"));
	x.Insert(1, x);
	ASSERT(x == String("00_abc123456789012345_abc123456789012345"));
	x.Remove(1, 24);
	ASSERT(x == String("0123456789012345"));
	String b = "013";
	ASSERT(x < b);
	ASSERT(x <= b);
	ASSERT(x <= x);
	ASSERT(x == x);
	ASSERT(b > x);
	ASSERT(b >= x);
	ASSERT(x >= x);
	ASSERT(x != b);
	ASSERT(x.Compare(b) < 0);
	ASSERT(b.Compare(x) > 0);
	const char *s = "013";
	ASSERT(x < s);
	ASSERT(x <= s);
	ASSERT(x <= x);
	ASSERT(x == x);
	ASSERT(s > x);
	ASSERT(s >= x);
	ASSERT(x >= x);
	ASSERT(x != s);
	ASSERT(s != x);
	ASSERT(s == b);
	ASSERT(b == s);
	ASSERT(x.Compare(s) < 0);
	ASSERT(b.Compare(s) == 0);
	ASSERT(!b.IsEqual(x));
	ASSERT(b.IsEqual(b));
	ASSERT(b.IsEqual(s));
	ASSERT(!x.IsEqual(b));
	b.Cat("ab");
	ASSERT(b == String("013ab"));
	b.Cat("ABCD", 2);
	ASSERT(b == String("013abAB"));
	b.Cat('x', 3);
	ASSERT(b == String("013abABxxx"));
	x.Cat(b);
	ASSERT(x == String("0123456789012345013abABxxx"));
	b.Cat(x, 4);
	ASSERT(b == String("013abABxxx0123"));
	b.Cat(~x, ~x + 3);
	ASSERT(b == String("013abABxxx0123012"));
	b.Cat((byte *)s, 2);
	ASSERT(b == String("013abABxxx012301201"));
	ASSERT((String().Cat() << 123) == String("123"));
	ASSERT(b.Find(2, "ABc", 1) == 5);
	ASSERT(b.Find(2, "ABc", 6) < 0);
	ASSERT(b.Find("AB", 0) == 5);
	ASSERT(b.Find("AB", 6) == -1);
	ASSERT(b.Find(b) == 0);
	ASSERT(b.Find(b, 1) < 0);
	ASSERT(b.StartsWith("013"));
	ASSERT(b.StartsWith(String("013")));
	ASSERT(!b.StartsWith("012"));
	ASSERT(!b.StartsWith(String("012")));
	ASSERT(b.StartsWith("012", 2));
	ASSERT(b.EndsWith("201"));
	ASSERT(b.EndsWith(String("201")));
	ASSERT(!b.EndsWith("202"));
	ASSERT(!b.EndsWith(String("202")));
	String rp = "abCCdd";
	String h;
	(h = rp).Replace("CC", "0");
	ASSERT(h == "ab0dd");
	(h = rp).Replace("CC", String("0"));
	ASSERT(h == "ab0dd");
	(h = rp).Replace(String("CC"), "0");
	ASSERT(h == "ab0dd");
	(h = rp).Replace(String("CC"), String("0"));
	ASSERT(h == "ab0dd");
	x = "ab";
	b = "12";
	ASSERT(x + b == String("ab12"));
	ASSERT(x + "xx" == String("abxx"));
	ASSERT("xx" + x == String("xxab"));
	ASSERT(x + 'z' == String("abz"));
	ASSERT('z' + x == String("zab"));
//	ASSERT(x + (int)'z' == String("abz")); Removed because of ambiguity
//	ASSERT((int)'z' + x == String("zab")); Removed because of ambiguity
	String xx = x;
	String bb = b;
	Swap(b, x);
	ASSERT(xx == b);
	ASSERT(bb == x);
	StringBuffer sb;
	sb.Cat("a");
	sb.Cat("1234", 3);
	String sx(sb);
	ASSERT(sx == "a123");
	sb = bb;
	sb[0] = 'a';
	bb = sb;
	ASSERT(bb == "a2");
	
	String lc, lc2;
	for(int i = 0; i < 100; i++) {
		lc.Cat(i % 10 + '0');
		ASSERT(lc == ~lc);
		ASSERT(lc != ~lc2);
		ASSERT(lc != ~(lc + 'x'));
		lc2 = lc;
	}
	
	for(int c = '1'; c <= '9'; c++) {
		String h(c, 64);
		
		for(int i = 0; i < h.GetCount(); i++) {
			String q = h;
			q.Trim(i);
			String w = h.Mid(0, i);
			ASSERT(GetHashValue(q) == GetHashValue(w));
			ASSERT(q == w);
		}
	}
	
	LOG("============== OK");
}
