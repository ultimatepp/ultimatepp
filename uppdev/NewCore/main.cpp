#include "Core.h"

#undef String
#undef StringBuffer
#undef WString
#undef WStringBuffer

#define LDUMP(x)  //  DUMP(x)

void TestAllocator()
{
	static void *ptr[8192];
	for(int n = 1; n < INT_MAX; n++) {
		if(n % 10000000 == 0)
			Cout() << n << "\r\n";
		int q = n & 8191;
		size_t sz = rand() & 8191;
		MFree(ptr[q]);
		byte *s = (byte *)MAlloc(sz);
		*(qword *)s = INT64_MAX;
		*(qword *)(s + sz - 8) = INT64_MAX;
		ptr[q] = s;
	}
	for(int i = 0; i < 8192; i++)
		MFree(ptr[i]);
	Cout() << "Allocator test Passed\r\n";
}

template <class Str, class StrBuf, class tchar, class bchar>
void ATest()
{
	Str x = "0123456789012345";
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
	Str _s("013");
	const tchar *s = ~_s;
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
	b.Cat((bchar *)s, 2);
	ASSERT(b == Str("013abABxxx012301201"));
//	ASSERT((Str().Cat() << 123) == Str("123"));
	ASSERT(b.Find(2, ~Str("ABc"), 1) == 5);
	ASSERT(b.Find(2, ~Str("ABc"), 6) < 0);
	ASSERT(b.Find(~Str("AB"), 0) == 5);
	ASSERT(b.Find(~Str("AB"), 6) == -1);
	ASSERT(b.Find(b) == 0);
	ASSERT(b.Find(b, 1) < 0);
	ASSERT(b.StartsWith(~Str("013")));
	ASSERT(b.StartsWith(Str("013")));
	ASSERT(!b.StartsWith(~Str("012")));
	ASSERT(!b.StartsWith(Str("012")));
	ASSERT(b.StartsWith(~Str("012"), 2));
	ASSERT(b.StartsWith(Str("012"), 2));
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
	ASSERT(x + (tchar)'z' == Str("abz"));
	ASSERT((tchar)'z' + x == Str("zab"));
	ASSERT(x + (int)'z' == Str("abz"));
	ASSERT((int)'z' + x == Str("zab"));
	Str xx = x;
	Str bb = b;
	Swap(b, x);
	ASSERT(xx == b);
	ASSERT(bb == x);
	StrBuf sb;
	sb.Cat(~Str("a"));
	sb.Cat(~Str("1234"), 3);
	Str sx(sb);
	ASSERT(sx == Str("a123"));
	sb = bb;
	sb[0] = 'a';
	bb = sb;
	ASSERT(bb == Str("a2"));
	Str bak = bb;
	sb = bb;
	sb[0] = 'b';
	bb = sb;
	ASSERT(bb == Str("b2"));
	ASSERT(bak == Str("a2"));
	ASSERT(!xx.IsVoid());
	xx = Str::GetVoid();
	ASSERT(xx.IsVoid());
	xx.Cat('a');
	ASSERT(!xx.IsVoid());
}


template <class NewString, class String, class tchar, class NewStringBuffer, class StringBuffer>
void UnitTest()
{
	String x("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ012345678901234567890"
"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ012345678901234567890"
"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ012345678901234567890"
"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ012345678901234567890"
	);
	const tchar *ins = ~x;
	String ss, css;
	NewString ns, cns;
	int size = 40 * 1024 * 1024;
	int size2 = 600;
	int maxlen = 0;
	for(int n = 0; n < 40000000; n++) {
		NewString zerotest;
		if(n % 1000000 == 0) {
			Cout() << n << ", maxlen: " << maxlen << "\r\n";
			maxlen = 0;
		}
		if(ss.GetCount() > maxlen)
			maxlen = ss.GetCount();
		if(ss.GetCount() > size) {
			ss.Clear();
			ns.Clear();
			if(rand() % 2000 == 0) {
				ss = String::GetVoid();
				ns = NewString::GetVoid();
			}
			Swap(size, size2);
		}
		int ch = rand() % 96 + 32;
		int pos = rand() % (ss.GetCount() + 1);
		int cpos = Upp::max(0, pos - 1);
		int icount = rand() % 160;
		int pos2 = rand() % (ss.GetCount() + 1);
		int rcount = rand() % (ss.GetCount() - pos + 1);
		int op = rand() % 30;
		LDUMP(n);
		LDUMP(op);
		switch(op) {
		default:
			ss.Cat(ch);
			ns.Cat(ch);
			break;
		case 1:
			ss.Insert(pos, ins, icount);
			ns.Insert(pos, ins, icount);
			break;
		case 2:
			ss.Remove(pos, rcount);
			ns.Remove(pos, rcount);
			break;
		case 8:
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
			ss.Insert(pos2, ~ss + pos, rcount);
			ns.Insert(pos2, ~ns + pos, rcount);
			break;
		case 3:
			ss.Cat(ins, icount);
			ns.Cat(ins, icount);
			break;
		case 4: {
				String sh = ss;
				NewString nh = ns;
				ss.Insert(pos, sh.Begin(), sh.GetCount());
				ns.Insert(pos, nh.Begin(), nh.GetCount());
			}
			break;
		case 5:
			ss = String(ss.Begin() + pos, rcount);
			ns = NewString(ns.Begin() + pos, rcount);
			break;
		case 6:
			css = ss;
			cns = ns;
			break;
		case 7:
			ss = css;
			ns = cns;
			break;
		case 9:
			if(ss.GetLength()) {
				ss.Set(cpos, ch);
				ns.Set(cpos, ch);
			}
			break;
		case 10:
			ss.Trim(pos);
			ns.Trim(pos);
			break;
		case 11:
			if(ss.GetLength()) {
				StringBuffer sb = ss;
				sb[cpos] = 'X';
				ss = sb;
				NewStringBuffer nb = ns;
				nb[cpos] = 'X';
				ns = nb;
			}
			break;
		case 12:
			if(ss.GetLength()) {
				String h = ss;
				StringBuffer sb = ss;
				sb.Cat(h.Begin() + pos, rcount);
				ss = sb;
				NewStringBuffer nb = ns;
				nb.Cat(nb.Begin() + pos, rcount);
				ns = nb;
			}
			break;
		case 13:
			NewStringBuffer nb;
			StringBuffer sb;
			nb.SetLength(1000);
			sb.SetLength(1000);
			const char *s = "Hello!";
			tchar *a = nb;
			tchar *b = sb;
			while(*s) {
				*a++ = *s;
				*b++ = *s;
				s++;
			}
			*a = *b = 0;
			nb.Strlen();
			sb.Strlen();
			ns = nb;
			ss = sb;

		}
		MCheck();
		NewString s(~ns, ns.GetCount());
		MCheck();
		LDUMP(ss.Begin());
		LDUMP(ns.Begin());
		LDUMP(ss.GetLength());
		LDUMP(ns.GetCount());
		LDUMP(css.GetLength());
		LDUMP(cns.GetCount());
		LDUMP(String(ns.Begin(), ns.GetCount()));
		LDUMP(String(cns.Begin(), cns.GetCount()));
		ASSERT(ns[ns.GetCount()] == 0);
		ASSERT(cns[cns.GetCount()] == 0);
		ASSERT(ss[ns.GetCount()] == 0);
		ASSERT(String(ns.Begin(), ns.GetCount()) == ss);
		ASSERT(String(cns.Begin(), cns.GetCount()) == css);
		ASSERT(ns.GetHashValue() == NewString(~ns, ns.GetCount()).GetHashValue());
		ASSERT(ns.IsVoid() == ss.IsVoid());
	}
	Cout() << "Passed";
	RLOG("Test passed");
}

int timenew;

void TimeNew(TimeStop& tm)
{
	timenew = tm.Elapsed();
}

void TimeOld(TimeStop& tm, const char *text)
{
	RLOG(Format("%-20s New:%5d`ms Old:%5d`ms Ratio:%3.1f",
	            text,
	            timenew, tm.Elapsed(),
	            (double)tm.Elapsed()/ (double)timenew));
}

template <class NewString, class String, class tchar>
void BenchmarkCat(int nn, int rep = 1000000)
{
	TimeStop tm1;
	for(int i = 0; i < rep; i++) {
		NewString s;
		for(int c = 'a'; c <= 'a' + nn; c++)
			s.Cat(c);
	}
	TimeNew(tm1);
	TimeStop tm2;
	for(int i = 0; i < rep; i++) {
		String s;
		for(int c = 'a'; c <= 'a' + nn; c++)
			s.Cat(c);
	}
	TimeOld(tm2, Upp::String("Cat ") + AsString(nn));
}

int dummy;

template <class NewString, class String, class tchar>
void BenchmarkCopy(int nn, int rep = 1000000)
{
	String s_x('x', nn);
	NewString x = ~s_x;
	int n = 0;
	unsigned h = 0;
	TimeStop tm1;
	NewString a[100];
	for(int i = 0; i < rep; i++) {
		for(int i = 0; i < 100; i++)
			a[i] = x;
	}
	TimeNew(tm1);
	n = 0;
	TimeStop tm2;
	String sa[100];
	for(int i = 0; i < rep; i++) {
		for(int i = 0; i < 100; i++)
			sa[i] = s_x;
	}
	TimeOld(tm2, Upp::String("Copy ") + AsString(nn));
}

template <class NewString, class NewStringBuffer, class String, class tchar>
void Benchmark()
{
	String s_x = "abcde";
	NewString x = ~s_x;
	NewString s;
	for(int c = 32; c < 128; c++)
		s.Cat(c);
	{
		TimeStop tm1;
		for(int i = 0; i < 1000000; i++) {
			for(int c = 0; c <= 17; c++) {
				NewString ss(s.Begin(), c);
			}
		}
		TimeNew(tm1);
		TimeStop tm2;
		for(int i = 0; i < 1000000; i++) {
			for(int c = 0; c <= 17; c++) {
				String ss(s.Begin(), c);
			}
		}
		TimeOld(tm2, "Slices");
	}

	BenchmarkCat<NewString, String, tchar>(10);
	BenchmarkCat<NewString, String, tchar>(20);
	BenchmarkCat<NewString, String, tchar>(30);
	BenchmarkCat<NewString, String, tchar>(64);
	BenchmarkCat<NewString, String, tchar>(200);
	BenchmarkCat<NewString, String, tchar>(20000, 10000);
	BenchmarkCat<NewString, String, tchar>(200000, 1000);

	{
		int n = 0;
		unsigned h = 0;
		TimeStop tm1;
		for(int i = 0; i < 10000; i++) {
			NewStringBuffer s;
			for(int c = 0; c < 20000; c++)
				s.Cat(c);
			NewString x = s;
		}

		TimeNew(tm1);

		n = 0;
		TimeStop tm2;
		for(int i = 0; i < 10000; i++) {
			StringBuffer s;
			for(int c = 0; c < 20000; c++)
				s.Cat(c);
			String x = s;
		}
		TimeOld(tm2, "Buffer Cat");
	}
	return;
	BenchmarkCopy<NewString, String, tchar>(10);
	BenchmarkCopy<NewString, String, tchar>(20);
	BenchmarkCopy<NewString, String, tchar>(30);
	BenchmarkCopy<NewString, String, tchar>(64);

	{
		int n = 0;
		unsigned h = 0;
		TimeStop tm1;
		for(int i = 0; i < 1000000; i++) {
			NewString s;
			for(int c = 'a'; c <= 'a' + 17; c++) {
				s.Cat(c);
				dummy += s.GetHashValue();
				if(s.IsEqual(x)) dummy++;
			}
		}

		TimeNew(tm1);

		n = 0;
		TimeStop tm2;
		for(int i = 0; i < 1000000; i++) {
			String s;
			for(int c = 'a'; c <= 'a' + 17; c++) {
				s.Cat(c);
				dummy += GetHashValue(s);
				if(s == s_x) dummy++;
			}
		}
		TimeOld(tm2, "Key mix");
	}
}

//typedef int NewString;

CONSOLE_APP_MAIN
{
	IdMapBench();
//	return;
	ATest<NewWString, NewWStringBuffer, wchar, wchar>();
	ATest<NewString, NewStringBuffer, char, byte>();
	RLOG("WString");
	UnitTest<NewWString, WString, wchar, NewWStringBuffer, WStringBuffer>();
	RLOG("String");
	UnitTest<NewString, String, char, NewStringBuffer, StringBuffer>();
	RLOG("Benchmarks");
	RLOG("String");
	Benchmark<NewString, NewStringBuffer, String, char>();
	RLOG("WString");
	Benchmark<NewWString, NewWStringBuffer, WString, wchar>();
	RLOG("finished");
	return;

//	TestAllocator();

/*	WString ws;
	for(int q = 0; q < 230; q++) {
		ws.Cat(q);
		MemoryCheck();
	}
	ws.Cat(123);
	return;*/
//	UnitTest();

	String x = String::GetVoid();
	String y = x;
	DUMP(x.IsVoid());
	DUMP(y.IsVoid());
	x.Cat('1');
	DUMP(x.IsVoid());
	DUMP(y.IsVoid());
	DUMP(x);
	DUMP(y);
	return;


	NewString s;
	for(int c = 32; c < 128; c++)
		s.Cat(c);
	DUMP(s.Begin());
//	__BREAK__;
	NewString s1(s, 10);
	DUMP(s1.Begin());
	NewString s2(s, 20);
	DUMP(s2.Begin());
	NewString s3(s, 40);
	DUMP(s3.Begin());

//	if(0)


}
