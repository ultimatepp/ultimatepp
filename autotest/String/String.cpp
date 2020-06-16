#include <Core/Core.h>

#define LDUMP(x) // DUMP(x)

using namespace Upp;

#ifdef flagLONG
#define N 100
#else
#define N 5
#endif

#ifdef CPU_ARM
#define STEPS 40000 * N
#else
#define STEPS 400000 * N
#endif

namespace Upp {
typedef wchar *HT;
template <>
String AsString(const HT& s)
{
	return WString(s).ToString();
}
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
		v = pick(vv);
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
		ASSERT(ns == ~ns);
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
	StdLogSetup(LOG_FILE|LOG_COUT);

	TimeStop tm;

	TestBufferCat<String, StringBuffer, char>();
	TestBufferCat<WString, WStringBuffer, wchar>();
	LOG("--- BUFFER CAT finished");

//	ATest<WString, WStringBuffer>();
	UnitTest<String, StringBuffer, char>();
	UnitTest<WString, WStringBuffer, wchar>();
	
	LOG("============= OK " << tm << " s");
}
