#include <Core/Core.h>

#define LDUMP(x) // DUMP(x)

using namespace Upp;

#define STEPS 40000000

namespace Upp {
typedef wchar *HT;
template <>
String AsString(const HT& s)
{
	return WString(s).ToString();
}
}

template <class Str, class StrBuf>
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
	b.Cat((byte *)s, 2);
	ASSERT(b == Str("013abABxxx012301201"));
	ASSERT((Str().Cat() << 123) == Str("123"));
	ASSERT(b.Find(2, "ABc", 1) == 5);
	ASSERT(b.Find(2, "ABc", 6) < 0);
	ASSERT(b.Find("AB", 0) == 5);
	ASSERT(b.Find("AB", 6) == -1);
	ASSERT(b.Find(b) == 0);
	ASSERT(b.Find(b, 1) < 0);
	ASSERT(b.StartsWith("013"));
	ASSERT(b.StartsWith(Str("013")));
	ASSERT(!b.StartsWith("012"));
	ASSERT(!b.StartsWith(Str("012")));
	ASSERT(b.StartsWith("012", 2));
	ASSERT(b.EndsWith("201"));
	ASSERT(b.EndsWith(Str("201")));
	ASSERT(!b.EndsWith("202"));
	ASSERT(!b.EndsWith(Str("202")));
	x = "ab";
	b = "12";
	ASSERT(x + b == Str("ab12"));
	ASSERT(x + "xx" == Str("abxx"));
	ASSERT("xx" + x == Str("xxab"));
	ASSERT(x + 'z' == Str("abz"));
	ASSERT('z' + x == Str("zab"));
	ASSERT(x + (int)'z' == Str("abz"));
	ASSERT((int)'z' + x == Str("zab"));
	Str xx = x;
	Str bb = b;
	Swap(b, x);
	ASSERT(xx == b);
	ASSERT(bb == x);
	StrBuf sb;
	sb.Cat("a");
	sb.Cat("1234", 3);
	Str sx(sb);
	ASSERT(sx == "a123");
	sb = bb;
	sb[0] = 'a';
	bb = sb;
	ASSERT(bb == "a2");
}

template <class tchar>
class TStr {
	Vector<tchar> v;
	bool          isvoid;

public:
	const tchar *Begin() const                      { return v.Begin(); }
	const tchar *operator~() const                  { return Begin(); }
	operator const char *() const                   { return v.Begin(); }

	tchar *Begin()                                  { return v.Begin(); }
	tchar *operator~()                              { return Begin(); }
	operator tchar *()                              { return v.Begin(); }


	TStr& operator=(const TStr& s)                  { v <<= s.v; isvoid = s.isvoid; return *this; }

	void Init()                                     { v.Add(0); isvoid = false; }

	TStr()                                          { Init(); }
	TStr(const tchar *s)                            { TStr h = *this; Init(); while(*s) Cat(*s++); }
	TStr(const tchar *s, int count)                 { Init(); while(count--) Cat(*s++); }
	TStr(const TStr& s)                             { Init(); v <<= s.v; isvoid = s.isvoid; }

	void Cat(int c)                                 { v.Top() = c; v.Add(0); isvoid = false; }

	void Insert(int pos, const tchar *s, int count) {
		Vector<tchar> vv(v, 1);
		vv.InsertN(pos, count);
		while(count--)
			vv[pos++] = *s++;
		v = vv;
		isvoid = false;
	}

	void Cat(const tchar *s, int count) {
		TStr h(*this);
		while(count--)
			h.Cat(*s++);
		*this = h;
	}

	void Remove(int pos, int count)                 { v.Remove(pos, count); v[GetCount()] = 0; isvoid = false; }
	void Trim(int pos)                              { v.SetCount(pos + 1); v[pos] = 0; isvoid = false; }
	void Set(int pos, int c)                        { v[pos] = c; isvoid = false; }
	void Clear()                                    { v.Clear(); Init(); }

	void Strlen() {
		for(int i = 0; i < GetCount(); i++)
			if(v[i] == 0) {
				Trim(i);
				break;
			}
		isvoid = false;
	}

	void SetLength(int n)                           { v.SetCount(n + 1); v[n] = 0; isvoid = false; }
	int  GetLength() const                          { return v.GetCount() - 1; }
	int  GetCount() const                           { return v.GetCount() - 1; }

	tchar& operator[](int i)                        { return v[i]; }

	static TStr GetVoid()                           { TStr x; x.isvoid = true; return x; }
	bool IsVoid() const                             { return isvoid; }

	String ToString() const {
		String x;
		for(int i = 0; i < v.GetCount(); i++)
			x.Cat(v[i]);
		return x;
	}
};

template <class NewString, class NewStringBuffer, class tchar>
void UnitTest()
{
	typedef TStr<tchar> String;
	typedef TStr<tchar> StringBuffer;
	String x;
	for(int q = 4; q--;)
		for(int c = 32; c < 128; c++)
			x.Cat(c);
	const tchar *ins = ~x;
	String ss, css;
	NewString ns, cns;
	int size = 40 * 1024 * 1024;
	int size2 = 600;
	int maxlen = 0;
	for(int n = 0; n < STEPS; n++) {
		NewString zerotest;
		if(n % 1000000 == 0) {
			Cout() << n << ", maxlen: " << maxlen << "\r\n";
			maxlen = 0;
		}
	//	if(n == 86)
	//		Cout() << "Now!";
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
				NewStringBuffer nb(ns);
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
				NewStringBuffer nb(ns);
				nb.Cat(nb.Begin() + pos, rcount);
				ns = nb;
			}
			break;
		case 14:
			ss = ss;
			ns = ns;
			break;
		case 15:
			ss = ~ss + pos;
			ns = ~ns + pos;
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
			break;
		}
		MemoryCheck();
		NewString s(~ns, ns.GetCount());
		MemoryCheck();
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
		ASSERT(memcmp(ns.Begin(), ss.Begin(), sizeof(tchar) * ns.GetCount() + 1) == 0);
		ASSERT(memcmp(cns.Begin(), css.Begin(), sizeof(tchar) * cns.GetCount() + 1) == 0);
		ASSERT(GetHashValue(ns) == GetHashValue(NewString(~ns, ns.GetCount())));
		ASSERT(ns.IsVoid() == ss.IsVoid());
	}
	Cout() << "Passed";
	RLOG("Test passed");
}

template <class NewString, class StringBuffer, class tchar>
void TestBufferCat()
{
	typedef TStr<tchar> String;
	for(int n = 0; n < 2000; n++) {
		StringBuffer *b = new StringBuffer;
		String ss;
		for(int x = 0; x < n; x++) {
			int c = rand();
			b->Cat(c);
			ss.Cat(c);
			b->Begin();
		}
		NewString ns = *b;
//		LOGHEXDUMP(ns.Begin(), ns.GetCount());
//		LOGHEXDUMP(ss.Begin(), ss.GetCount());
		ASSERT(ss.GetLength() == ns.GetLength());
		ASSERT(memcmp(ns.Begin(), ss.Begin(), sizeof(tchar) * ns.GetCount() + 1) == 0);
		delete b;
	}
}

CONSOLE_APP_MAIN
{
	ATest<String, StringBuffer>();
	TestBufferCat<String, StringBuffer, char>();
	TestBufferCat<WString, WStringBuffer, wchar>();
//	ATest<WString, WStringBuffer>();
	UnitTest<String, StringBuffer, char>();
	UnitTest<WString, WStringBuffer, wchar>();
}
