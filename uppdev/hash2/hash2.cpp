#include <Core/Core.h>

using namespace Upp;

int q = 0;

#define N 100 * 1000 * 1000

void Benchmark()
{
	{
		RTIMING("XOR-folding");
		for(int h = 0; h < N; h++) {
			q += 255 & ((h >> 24) ^ (h >> 16) ^ (h >> 8) ^ h);
		}
	}
	{
		RTIMING("%");
		for(int i = 0; i < N; i++) {
			q += i % 257;
		}
	}
	{
		RTIMING("&");
		for(int i = 0; i < N; i++) {
			q += i & 255;
		}
	}
}

unsigned HashFold(unsigned h)
{
	return (h >> 23) - (h >> 9) - (h >> 15) - h;
}

template <class T>
void TestCollisions(const char *desc, const Index<T>& data)
{
	VectorMap<unsigned, int> hash1, hash2, hash3;
	int m1 = 0, m2 = 0, m3 = 0;
	int a = Pow2Bound(data.GetCount()) - 1;
	int mod = PrimeBound(data.GetCount());
	for(int i = 0; i < data.GetCount(); i++) {
		unsigned h = GetHashValue(data[i]);
		m1 = max(m1, ++hash1.GetAdd(h % mod, 0));
		m2 = max(m2, ++hash2.GetAdd(a & HashFold(h), 0));
	}
	LOG(desc << ", unique " << data.GetCount());
	LOG("mod unique: " << hash1.GetCount() << ", worst: " << m1 << " (% " << mod << ")");
	LOG("fold unique: " << hash2.GetCount() << ", worst: " << m2 << " (& " << a << ")");
}

void TestCollisions(String desc, const char *file)
{
	FileIn in(file);
	Index<String> data;
	while(!in.IsEof())
		data.FindAdd(in.GetLine());
	TestCollisions(desc + " " + file, data);
}

CONSOLE_APP_MAIN {
	Benchmark();
	int q = 0;
	for(int i = 0; i < 100000; i++)
		if((HashFold(i) & 31) == 0) {
			LOG(i << " - " << i - q);
			q = i;
		}
	TestCollisions("", "e:/bookmarks.html");
	TestCollisions("", "e:/test.txt");
	Index<String> x;
	for(int i = 0; i < 1000 * 100; i++)
		x.FindAdd(FormatIntBase(i, 2));
	TestCollisions("Bin100", x);
	x.Clear();
	for(int i = 0; i < 1000 * 10; i++)
		x.FindAdd(FormatIntBase(i, 2));
	TestCollisions("Bin10", x);
	x.Clear();
	for(int i = 0; i < 1000 * 100; i++)
		x.FindAdd(FormatIntBase(i, 16));
	TestCollisions("Hex100", x);
	x.Clear();
	for(int i = 0; i < 1000 * 10; i++)
		x.FindAdd(FormatIntBase(i, 16));
	TestCollisions("Hex10", x);

	for(int n = 10; n <= 1000 * 1000; n = 10 * n) {
		x.Clear();
		for(int i = 0; i < n; i++)
			x.FindAdd(FormatIntBase(i, 10));
		TestCollisions("Dec" + AsString(n), x);
	}
	x.Clear();
	FileIn in("d:/uppsrc/CtrlLib/ArrayCtrl.cpp");
	for(;;) {
		int c = in.Get();
		if(c < 0) break;
		if(isalpha(c) || c == '_') {
			String id;
			id.Cat(c);
			c = in.Get();
			while(c >= 0 && (isalnum(c) || c == '_')) {
				id.Cat(c);
				c = in.Get();
			}
			x.FindAdd(id);
		}
		else
		if(isdigit(c))
			do c = in.Get();
			while(c >= 0 && (isalnum(c) || c == '.'));
	}
	TestCollisions("cpp ids", x);
	
	Index<int> y;
	for(int i = 0; i < 100000; i++)
		y.FindAdd(i);
	TestCollisions("i100000", y);
	y.Clear();
	for(int i = 0; i < 30000; i++)
		y.FindAdd(rand());
	TestCollisions("i rand", y);
}
