#include <Core/Core.h>

using namespace Upp;

template <class Str, class StrBuf, class chart, class chart2>
void ATest()
{
	Str x = "0123456789012345";
	ASSERT(x.FindFirstOf("654") == 4);
	ASSERT(x.FindFirstOf("46") == 4);
	ASSERT(x.FindFirstOf("32", 10) == 12);
	ASSERT(x.FindFirstOf("23", 10) == 12);
	ASSERT(x.FindFirstOf(x) == 0);
	ASSERT(x.Mid(3, 4) == Str("3456"));
	ASSERT(x.Mid(3) == Str("3456789012345"));
	ASSERT(x.Left(2) == Str("01"));
	ASSERT(x.Right(2) == Str("45"));
	ASSERT(x.Find('3') == 3);
	ASSERT(x.ReverseFind('3') == 13);
	ASSERT(x.GetLength() == 16);
	ASSERT(x.Begin() + x.GetLength() == x.End());
	ASSERT(x[0] == '0');
	x.Insert(1, 'a');
	ASSERT(x == Str("0a123456789012345"));
	x.Insert(2, ~Str("bcd"), 2);
	ASSERT(x == Str("0abc123456789012345"));
	x.Insert(1, ~Str("_"));
	ASSERT(x == Str("0_abc123456789012345"));
	x.Insert(1, x);
	ASSERT(x == Str("00_abc123456789012345_abc123456789012345"));
	x.Remove(1, 24);
	ASSERT(x == Str("0123456789012345"));
	Str b = "013";
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
	Str hh = "013";
	const chart *s = hh;
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
	ASSERT(b == Str("013ab"));
	b.Cat("ABCD", 2);
	ASSERT(b == Str("013abAB"));
	b.Cat('x', 3);
	ASSERT(b == Str("013abABxxx"));
	x.Cat(b);
	ASSERT(x == Str("0123456789012345013abABxxx"));
	b.Cat(x, 4);
	ASSERT(b == Str("013abABxxx0123"));
	b.Cat(~x, ~x + 3);
	ASSERT(b == Str("013abABxxx0123012"));
	b.Cat((chart2 *)s, 2);
	ASSERT(b == Str("013abABxxx012301201"));
	ASSERT(b.Find(2, ~Str("ABc"), 1) == 5);
	ASSERT(b.Find(2, ~Str("ABc"), 6) < 0);
	ASSERT(b.Find(~Str("AB"), 0) == 5);
	ASSERT(b.Find("AB", 6) == -1);
	ASSERT(b.Find(b) == 0);
	ASSERT(b.Find(b, 1) < 0);
	ASSERT(b.StartsWith("013"));
	ASSERT(b.StartsWith(Str("013")));
	ASSERT(!b.StartsWith(~Str("012")));
	ASSERT(!b.StartsWith(Str("012")));
	ASSERT(b.StartsWith(~Str("012"), 2));
	ASSERT(b.EndsWith(~Str("201")));
	ASSERT(b.EndsWith(Str("201")));
	ASSERT(!b.EndsWith(~Str("202")));
	ASSERT(!b.EndsWith(Str("202")));
	x = "ab";
	b = "12";
	ASSERT(x + b == Str("ab12"));
	ASSERT(x + "xx" == Str("abxx"));
	ASSERT("xx" + x == Str("xxab"));
	ASSERT(x + 'z' == Str("abz"));
	ASSERT('z' + x == Str("zab"));
	Str xx = x;
	Str bb = b;
	Swap(b, x);
	ASSERT(xx == b);
	ASSERT(bb == x);
	StrBuf sb;
	sb.Cat(~Str("a"));
	sb.Cat(~Str("1234"), 3);
	Str sx(sb);
	ASSERT(sx == ~Str("a123"));
	sb = bb;
	sb[0] = 'a';
	bb = sb;
	ASSERT(bb == ~Str("a2"));
	
	x = Str("beginneedneneneedleneneeneedletheend.");

	ASSERT(x.Find(~Str("")) == 0);
	ASSERT(x.Find(~Str("xxx")) == -1);
	ASSERT(x.Find(~Str("begin")) == 0);
	ASSERT(x.Find(~Str("begi")) == 0);
	ASSERT(x.Find(~Str("beg")) == 0);
	ASSERT(x.Find(~Str("be")) == 0);
	ASSERT(x.Find(~Str("b")) == 0);
	ASSERT(x.Find(~Str("begin"), 1) == -1);
	ASSERT(x.Find(~Str("begi"), 1) == -1);
	ASSERT(x.Find(~Str("beg"), 1) == -1);
	ASSERT(x.Find(~Str("be"), 1) == -1);
	ASSERT(x.Find(~Str("b"), 1) == -1);
	ASSERT(x.Find(~Str("theend.")) == 30);
	ASSERT(x.Find(~Str("end.")) == 33);
	ASSERT(x.Find(~Str("nd.")) == 34);
	ASSERT(x.Find(~Str("d.")) == 35);
	ASSERT(x.Find(~Str(".")) == 36);
	ASSERT(x.Find(~Str("theend."), 34) == -1);
	ASSERT(x.Find(~Str("end."), 34) == -1);
	ASSERT(x.Find(~Str("nd."), 36) == -1);
	ASSERT(x.Find(~Str("d."), 36) == -1);
	ASSERT(x.Find(~Str("."), 37) == -1);
	ASSERT(x.Find(~Str("needle")) == 13);
	ASSERT(x.Find(~Str("needle"), 20) == 24);
	ASSERT(x.ReverseFind(~Str("")) == 37);
	ASSERT(x.ReverseFind(~Str("xxx")) == -1);
	ASSERT(x.ReverseFind(~Str("begin")) == 0);
	ASSERT(x.ReverseFind(~Str("begi")) == 0);
	ASSERT(x.ReverseFind(~Str("beg")) == 0);
	ASSERT(x.ReverseFind(~Str("be")) == 0);
	ASSERT(x.ReverseFind(~Str("b")) == 0);
	ASSERT(x.ReverseFind(~Str("begin"), 1) == -1);
	ASSERT(x.ReverseFind(~Str("begi"), 1) == -1);
	ASSERT(x.ReverseFind(~Str("beg"), 1) == -1);
	ASSERT(x.ReverseFind(~Str("be"), 1) == 0);
	ASSERT(x.ReverseFind(~Str("b"), 1) == 0);
	ASSERT(x.ReverseFind(~Str("theend.")) == 30);
	ASSERT(x.ReverseFind(~Str("end.")) == 33);
	ASSERT(x.ReverseFind(~Str("nd.")) == 34);
	ASSERT(x.ReverseFind(~Str("d.")) == 35);
	ASSERT(x.ReverseFind(~Str(".")) == 36);
	ASSERT(x.ReverseFind(~Str("theend."), 34) == -1);
	ASSERT(x.ReverseFind(~Str("end."), 34) == -1);
	ASSERT(x.ReverseFind(~Str("nd."), 36) == 34);
	ASSERT(x.ReverseFind(~Str("d."), 36) == 35);
	ASSERT(x.ReverseFind(~Str("."), 37) == -1);
	ASSERT(x.ReverseFind(~Str("needle")) == 24);
	ASSERT(x.ReverseFind(~Str("needle"), 20) == 13);
	
	x.Replace(~Str("needle"), ~Str("<x>"));
	ASSERT(x == ~Str("beginneednene<x>nenee<x>theend."));
	x.Replace("begin", "<b>");
	ASSERT(x == ~Str("<b>neednene<x>nenee<x>theend."));
	x.Replace("end.", "<e>");
	ASSERT(x == ~Str("<b>neednene<x>nenee<x>the<e>"));
	
	x.Replace("", "x");
	
	ASSERT(x.Find("", 10) == 10);
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	MemoryLimitKb(1024*1024);

	ATest<String, StringBuffer, char, byte>();
	ASSERT((String().Cat() << 123) == "123");

	ATest<WString, WStringBuffer, wchar, wchar>();
	
	LOG("======== Everything OK");
}
