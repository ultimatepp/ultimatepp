#include <Core/Core.h>

#include <algorithm>
#include <vector>

using namespace Upp;

struct Arry {
	typedef T **Cursor;
	typedef T  *SlotBuffer;

	Cursor GetCursor(int at) { return vector.GetCursor(); }
	const T& operator[Cursor c] const { return **c; }
	bool   IsDone(Cursor c) const;
	void   PickSlot(SlotBuffer& buffer, Cursor cursor) { buffer = *cursor; }
	void   WriteSlot(Cursor cursor, SlotBuffer& buffer) { *cursor = buffer; }
	void   RemovePickedSlots(Cursor cursor, int n);
	void   SlotMove(Cursor t, Cursor s) { *t = *s; }
	void   Swap(Cursor a, Cursor b) { Swap(*a, *b); }
};

void RemoveIf(Container& c, Pred p)
{
	Container::Cursor i;
	c.First(i);
	for(; !c.IsDone(i); i++)
		if(p(c[i])) break;
	Container::Cursor ti = i;
	for(; !c.IsDone(i); i++)
		if(!p(c[i]))
			c.MoveSlot(c[ti++], c[i]);
	c.RemovePickedSlots(ti, i - ti);
}

/*wep password 153ab2c29a
template <class C>
class C_Slice {
	typedef
	C&  cont;
	int pos, count;

	int Ndx(int i) const             { ASSERT(i >= 0 && i < count); return i - pos; }

	typedef typename C::ValueType VT;

public:
	typename VT& operator[](int i)             { return cont[Ndx(i)]; }
	const typename VT& operator[](int i) const { return cont[Ndx(i)]; }
	void Swap(int i1, int i2)         { cont.Swap(Ndx(i1), Ndx(i2)); }
	int  GetCount() const             { return count; }

	C_Slice(C& cont, int pos, int count) : cont(cont), pos(pos), count(count) {}
};

template <class C>
C_Slice<C> Slice(C& cont, int pos, int count)
{
	return C_Slice<C>(cont, pos, count);
}
*/

template <class C, class Less>
void C_ForwardSort0(C& cont, int begin, int end, const Less& less)
{
	if(begin == end)
		return;
	int limit = end - 1;
	while(!(begin == limit))
	{
		for(int best = limit, next = limit, ptr = limit;; best = ptr)
			if(!less(cont[best], cont[--ptr]))
			{
				if(ptr == begin)
				{
					begin = next;
					break;
				}
			}
			else
			{
				do
				{
					if(ptr == begin)
					{
						cont.Swap(begin, best);
						++begin;
						goto NEXT_ITEM;
					}
				}
				while(less(cont[best], cont[--ptr]));
				if(ptr == begin)
				{
					cont.Swap(++begin, best);
					++begin;
					break;
				}
				next = ptr;
				++next;
			}
	NEXT_ITEM:
		;
	}
}

template <class T, class Less>
void C_ForwardSort(T& c, const Less& less)
{
	C_ForwardSort0(c, 0, c.GetCount(), less);
}

template <class T>
void C_ForwardSort(T& c)
{
	typedef typename T::ValueType VT;
	C_ForwardSort0(c, 0, c.GetCount(), StdLess<VT>());
}

template <class C, class Less>
void C_Sort0(C& cont, int begin, int end, const Less& less)
{
	int count;
	while((count = end - begin) > __SORT_THRESHOLD) {
		int expected = count >> 1, deviation = expected - (expected >> 8);
		int b = begin, e = end, m = b + expected;
		for(int pass = 1;; pass++) {
			for(;; ++b) {
				while(less(cont[m], cont[--e]))
					;
				while(less(cont[b], cont[m]))
					++b;
				if(!(b < e))
					break;
				if(m == b) m = e;
				else if(m == e) m = b;
				cont.Swap(b, e);
			}
			if(pass >= __SORT_MEDIAN_PASSES)
				break;
			int pos = b - begin;
			if(pos <= expected - deviation)
				e = end;
			else if(pos >= expected + deviation) {
				e = b;
				b = begin;
			}
			else
				break;
			m = b + 1 + (int)((unsigned)rand() % (e - b - 2));
		}
		if(b - begin < end - e) {
			C_Sort0(cont, begin, b, less);
			begin = b;
		}
		else {
			C_Sort0(cont, b, end, less);
			end = b;
		}
	}
	if(count >= 2)
		C_ForwardSort0(cont, begin, end, less);
}

template <class T, class Less>
void C_Sort(T& c, const Less& less)
{
	C_Sort0(c, 0, c.GetCount(), less);
}

template <class T>
void C_Sort(T& c)
{
	typedef typename T::ValueType VT;
	C_Sort0(c, 0, c.GetCount(), StdLess<VT>());
}

struct HH : Moveable<HH> {
	int    x;
	String y;

	bool operator<(const HH& b) const { return y < b.y; }
};

CONSOLE_APP_MAIN
{
	for(int q = 0; q < 100; q++) {
		Vector<String> d1, d2, d3;
		std::vector<std::string> d4;
		for(int i = 0; i < 10000; i++) {
			String x = AsString(rand());
			d1.Add(x);
			d2.Add(x);
			d3.Add(x);
			d4.push_back(to_string(x));
		}
		{
			RTIMING("Old");
			Sort(d1);
		}
		{
			RTIMING("New");
			C_Sort(d2);
		}
		{
			RTIMING("Order");
			GetSortOrder(d3);
		}
		{
			RTIMING("Std");
			std::sort(d3.Begin(), d3.End());
		}
		{
			RTIMING("std");
			std::sort(d4.begin(), d4.end());
		}
	}
}
