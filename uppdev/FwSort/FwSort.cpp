#include <Core/Core.h>

using namespace Upp;

template <class I, class Less>
void FwSort2(I begin, I end, const Less& less)
{
	while(begin != end && begin + 1 != end) {
		I min = begin;
		I max = begin;
		for(I i = begin + 1; i != end; i++) {
			if(less(*i, *min))
				min = i;
			if(!less(*i, *max))
				max = i;
		}
		IterSwap(begin, min);
		IterSwap(--end, max == begin ? min : max);
		begin++;
	}
}

template <class I, class Less>
void FwSortSimple(I begin, I end, const Less& less)
{
	while(begin != end) {
		I min = begin;
		for(I i = begin; ++i != end;)
			if(less(*i, *min))
				min = i;
		IterSwap(begin++, min);
	}
}

template <class I, class Less>
inline int SelectMin2(I ptr, int i, const Less& less)
{
	return less(ptr[i + 1], ptr[i]);
}

template <class I, class Less>
inline int SelectMin3(I ptr, int i, const Less& less)
{
	int l = SelectMin2(ptr, less);
	I h = ptr + 2;
	return less(*l, *h) ? l : h;
}

template <class I, class Less>
inline I SelectMin4(I ptr, const Less& less)
{
	I l = SelectMin2(ptr, less);
	I h = SelectMin2(ptr + 2, less);
	return less(*l, *h) ? l : h;
}

template <class I, class Less>
inline I SelectMin5(I ptr, const Less& less)
{
	I l = SelectMin4(ptr, less);
	I h = ptr + 4;
	return less(*l, *h) ? l : h;
}

template <class I, class Less>
inline I SelectMin6(I ptr, const Less& less)
{
	I l = SelectMin4(ptr, less);
	I h = SelectMin2(ptr + 4, less);
	return less(*l, *h) ? l : h;
}

template <class I, class Less>
inline I SelectMin7(I ptr, const Less& less)
{
	I l = SelectMin4(ptr, less);
	I h = SelectMin3(ptr + 4, less);
	return less(*l, *h) ? l : h;
}

template <class I, class Less>
inline I SelectMin8(I ptr, const Less& less)
{
	I l = SelectMin4(ptr, less);
	I h = SelectMin4(ptr + 4, less);
	return less(*l, *h) ? l : h;
}

template <class I, class Less>
inline I SelectMin9(I ptr, const Less& less)
{
	I l = SelectMin8(ptr, less);
	I h = ptr + 8;
	return less(*l, *h) ? l : h;
}

template <class I, class Less>
inline I SelectMin10(I ptr, const Less& less)
{
	I l = SelectMin8(ptr, less);
	I h = SelectMin2(ptr + 8, less);
	return less(*l, *h) ? l : h;
}

template <class I, class Less>
inline I SelectMin11(I ptr, const Less& less)
{
	I l = SelectMin8(ptr, less);
	I h = SelectMin3(ptr + 8, less);
	return less(*l, *h) ? l : h;
}

template <class I, class Less>
inline I SelectMin12(I ptr, const Less& less)
{
	I l = SelectMin8(ptr, less);
	I h = SelectMin4(ptr + 8, less);
	return less(*l, *h) ? l : h;
}

template <class I, class Less>
inline I SelectMin13(I ptr, const Less& less)
{
	I l = SelectMin8(ptr, less);
	I h = SelectMin5(ptr + 8, less);
	return less(*l, *h) ? l : h;
}

template <class I, class Less>
inline I SelectMin14(I ptr, const Less& less)
{
	I l = SelectMin8(ptr, less);
	I h = SelectMin6(ptr + 8, less);
	return less(*l, *h) ? l : h;
}

template <class I, class Less>
inline I SelectMin15(I ptr, const Less& less)
{
	I l = SelectMin8(ptr, less);
	I h = SelectMin7(ptr + 8, less);
	return less(*l, *h) ? l : h;
}

template <class I, class Less>
inline I SelectMin16(I ptr, const Less& less)
{
	I l = SelectMin8(ptr, less);
	I h = SelectMin8(ptr + 8, less);
	return less(*l, *h) ? l : h;
}

template <class I, class Less>
void FwSort(I begin, int len, const Less& less)
{
	switch(len) {
	case 16: IterSwap(begin, SelectMin16(begin, less)); begin++;
	case 15: IterSwap(begin, SelectMin15(begin, less)); begin++;
	case 14: IterSwap(begin, SelectMin14(begin, less)); begin++;
	case 13: IterSwap(begin, SelectMin13(begin, less)); begin++;
	case 12: IterSwap(begin, SelectMin12(begin, less)); begin++;
	case 11: IterSwap(begin, SelectMin11(begin, less)); begin++;
	case 10: IterSwap(begin, SelectMin10(begin, less)); begin++;
	case  9: IterSwap(begin, SelectMin9(begin, less)); begin++;
	case  8: IterSwap(begin, SelectMin8(begin, less)); begin++;
	case  7: IterSwap(begin, SelectMin7(begin, less)); begin++;
	case  6: IterSwap(begin, SelectMin6(begin, less)); begin++;
	case  5: IterSwap(begin, SelectMin5(begin, less)); begin++;
	case  4: IterSwap(begin, SelectMin4(begin, less)); begin++;
	case  3: IterSwap(begin, SelectMin3(begin, less)); begin++;
	case  2: IterSwap(begin, SelectMin2(begin, less));
	}
}

template <class T>
void Dump(const Vector<T>& data)
{
	String s;
	for(int q = 0; q < data.GetCount(); q++)
		s << data[q] << ' ';
	LOG(s);
}

CONSOLE_APP_MAIN
{
#ifdef _DEBUG
	Vector<int> x;
	x << 40 << 20 << 4 << 10 << 11 << 17 << 2 << 12;
	FwSort(x.Begin(), x.GetCount(), StdLess<int>());
	Dump(x);
	Vector<String> y;
	y << "40" << "20" << "4" << "10" << "11" << "17" << "2" << "12";
	FwSort(y.Begin(), y.GetCount(), StdLess<String>());
	Dump(y);
	for(int i = 0; i < 10; i++) {
		Vector<int> data;
		int n = rand() % 16;
		for(int q = 0; q < n; q++)
			data.Add(rand() % 100);
		Dump(data);
		FwSort(data.Begin(), data.GetCount(), StdLess<int>());
		Dump(data);
		LOG("----");
	}
#else
	Vector< Vector<int> > set;
	for(int i = 0; i < 1000; i++) {
		Vector<int>& data = set.Add();
		int n = rand() % 15;
		n = 8;
		for(int q = 0; q < n; q++)
			data.Add(rand() % 100);
	}
	double x = 100.0 * set.GetCount();
	RDUMP(x);
	dword time = GetTickCount();
	while(time + 3000 > GetTickCount()) {
		Vector< Vector<int> > s;
		s <<= set;
		{
			RTIMING("Simple FW sort");
			for(int i = 0; i < s.GetCount(); i++)
				ForwardSort(s[i].Begin(), s[i].End(), StdLess<int>());
		}
		s <<= set;
		{
			RTIMING("Normal FW sort");
			for(int i = 0; i < s.GetCount(); i++)
				ForwardSort(s[i].Begin(), s[i].End(), StdLess<int>());
		}
		s <<= set;
		{
			RTIMING("New FW sort");
			for(int i = 0; i < s.GetCount(); i++)
				FwSort(s[i].Begin(), s[i].GetCount(), StdLess<int>());
		}
	}
#endif
}
