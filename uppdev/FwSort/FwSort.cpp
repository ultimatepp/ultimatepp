#include <Core/Core.h>

using namespace Upp;

template <class I, class Less>
void FwSort(I begin, I end, const Less& less)
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

void Dump(const Vector<int>& data)
{
	String s;
	for(int q = 0; q < data.GetCount(); q++)
		s << data[q] << ' ';
	LOG(s);
}

CONSOLE_APP_MAIN
{
#ifdef _DEBUG
	for(int i = 0; i < 10; i++) {
		Vector<int> data;
		int n = rand() % 30;
		for(int q = 0; q < n; q++)
			data.Add(rand() % 100);
		Dump(data);
		FwSort(data.Begin(), data.End(), StdLess<int>());
		Dump(data);
		LOG("----");
	}
#else
	Vector< Vector<int> > set;
	for(int i = 0; i < 1000; i++) {
		Vector<int>& data = set.Add();
		int n = rand() % 20;
		n = 8;
		for(int q = 0; q < n; q++)
			data.Add(rand() % 100);
	}
	dword time = GetTickCount();
	while(time + 3000 > GetTickCount()) {
		Vector< Vector<int> > s;
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
				FwSort(s[i].Begin(), s[i].End(), StdLess<int>());
		}
	}
#endif
}
