#include <Core/Core.h>

using namespace Upp;

Vector<int> ih;
Vector<String> sh;
Vector<String> sh2;
Vector<String> sh3;

#define N 20000
#define R 100000
#define LDUMP(x)

void Reset()
{
	ih.Clear();
	sh.Clear();
	sh2.Clear();
	sh3.Clear();
	for(int i = 0; i < N; i++) {
		int x = Random(R);
		ih.Add(x);
		String h = Format("%06d", x);
		sh.Add(h);
		sh2.Add(h + "A");
		sh3.Add(h + "B");
	}
}

template <class Range>
void Check(const Range& r)
{
	for(int i = 1; i < r.GetCount(); i++)
		ASSERT(r[i] >= r[i - 1]);
}

template <class Range>
void Check2(const Range& r)
{
	for(int i = 1; i < r.GetCount(); i++)
		ASSERT(r[i] <= r[i - 1]);
}

template <class Range>
void Check(const Vector<int>& o, const Range& r)
{
	for(int i = 1; i < r.GetCount(); i++)
		ASSERT(r[o[i]] >= r[o[i - 1]]);
}


CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);

	SeedRandom(0);

	Reset();
	IndexSort(ih, sh);
	LDUMP(ih);
	LDUMP(sh);
	Check(ih);
	Check(sh);
	LOG("==============");
	Reset();
	IndexSort2(ih, sh, sh2);
	LDUMP(ih);
	LDUMP(sh);
	LDUMP(sh2);
	Check(ih);
	Check(sh);
	Check(sh2);
	LOG("==============");
	Reset();
	IndexSort3(ih, sh, sh2, sh3);
	LDUMP(ih);
	LDUMP(sh);
	LDUMP(sh2);
	Check(ih);
	Check(sh);
	Check(sh2);
	Check(sh3);
	LOG("==============");
	Reset();
	StableIndexSort(ih, sh);
	LDUMP(ih);
	LDUMP(sh);
	Check(ih);
	Check(sh);
	LOG("==============");
	Reset();
	StableIndexSort2(ih, sh, sh2);
	LDUMP(ih);
	LDUMP(sh);
	LDUMP(sh2);
	Check(ih);
	Check(sh);
	Check(sh2);
	LOG("==============");
	Reset();
	StableIndexSort3(ih, sh, sh2, sh3);
	LDUMP(ih);
	LDUMP(sh);
	LDUMP(sh2);
	Check(ih);
	Check(sh);
	Check(sh2);
	Check(sh3);
	LOG("==============");
	Reset();
	StableSort(ih, std::greater<int>());
	LDUMP(ih);
	Check2(ih);
	Reset();
	StableSort(ih);
	LDUMP(ih);
	Check(ih);
	LOG("===============");
	{
		Reset();
		Vector<int> o = GetSortOrder(ih);
		LDUMP(ih);
		LDUMP(o);
		Check(o, ih);
		LDUMP(ViewRange(ih, pick(o)));
	}
	LOG("===============");
	{
		Reset();
		Vector<int> o = GetStableSortOrder(ih);
		LDUMP(ih);
		LDUMP(o);
		Check(o, ih);
		LDUMP(ViewRange(ih, pick(o)));
	}
	LOG("==========================");
	LOG("CoSorts");
	Reset();
	CoIndexSort(ih, sh);
	LDUMP(ih);
	LDUMP(sh);
	Check(ih);
	Check(sh);
	LOG("==============");
	Reset();
	CoIndexSort2(ih, sh, sh2);
	LDUMP(ih);
	LDUMP(sh);
	LDUMP(sh2);
	Check(ih);
	Check(sh);
	Check(sh2);
	LOG("==============");
	Reset();
	CoIndexSort3(ih, sh, sh2, sh3);
	LDUMP(ih);
	LDUMP(sh);
	LDUMP(sh2);
	Check(ih);
	Check(sh);
	Check(sh2);
	Check(sh3);
	LOG("==============");
	Reset();
	CoStableIndexSort(ih, sh);
	LDUMP(ih);
	LDUMP(sh);
	Check(ih);
	Check(sh);
	LOG("==============");
	Reset();
	CoStableIndexSort2(ih, sh, sh2);
	LDUMP(ih);
	LDUMP(sh);
	LDUMP(sh2);
	Check(ih);
	Check(sh);
	Check(sh2);
	LOG("==============");
	Reset();
	CoStableIndexSort3(ih, sh, sh2, sh3);
	LDUMP(ih);
	LDUMP(sh);
	LDUMP(sh2);
	Check(ih);
	Check(sh);
	Check(sh2);
	Check(sh3);
	LOG("==============");
	Reset();
	CoStableSort(ih, std::greater<int>());
	LDUMP(ih);
	Check2(ih);
	Reset();
	CoStableSort(ih);
	LDUMP(ih);
	Check(ih);
	LOG("===============");
	{
		Reset();
		Vector<int> o = CoGetSortOrder(ih);
		LDUMP(ih);
		LDUMP(o);
		Check(o, ih);
		LDUMP(ViewRange(ih, pick(o)));
	}
	LOG("===============");
	{
		Reset();
		Vector<int> o = CoGetStableSortOrder(ih);
		LDUMP(ih);
		LDUMP(o);
		Check(o, ih);
		LDUMP(ViewRange(ih, pick(o)));
	}
	LOG("========================= OK");
}
