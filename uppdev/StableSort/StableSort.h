#ifndef _StableSort_StableSort_h_
#define _StableSort_StableSort_h_

template <class I, class Less, class T> // Optimize!
void Merge__(I i1, I e1, I e2, const Less& less, T *temp)
{
	T *t = temp;
	I i = i1;
	I i2 = e1;
	for(;;) {
		if(i1 == e1) {
			while(i2 != e2)
				::new(t++) T(*i2++);
			break;
		}
		if(i2 == e2) {
			while(i1 != e1)
				::new(t++) T(*i1++);
			break;
		}
		if(less(*i2, *i1))
			::new(t++) T(*i2++);
		else
			::new(t++) T(*i1++);
	}
	while(i < e2)
		*i++ = *temp++;
}

template <class T, class I, class Less>
void NewStableSort(I begin, I end, const Less& less)
{
	int n = end - begin;
	for(int q = 0; q < n - 1; q += 2)
		if(less(begin[q + 1], begin[q]))
			IterSwap(begin + q + 1, begin + q);
	T *temp = (T*) new byte[sizeof(T) * n];
	int step = 2;
	while(step < n) {
		int q = 0;
		int step2 = 2 * step;
		while(q + step2 < n) {
			Merge__(begin + q, begin + q + step, begin + q + step2, less, temp);
			q += step2;
		}
		if(q + step < n)
			Merge__(begin + q, begin + q + step, end, less, temp);
		step += step;
	}
	delete[] (byte *)temp;
}

template <class C, class Less>
void NewStableSort(C& c, const Less& less)
{
	NewStableSort<typename C::ValueType>(c.Begin(), c.End(), less);
}

template <class C>
void NewStableSort(C& c)
{
	NewStableSort<typename C::ValueType>(c.Begin(), c.End(), StdLess<typename C::ValueType>());
}

#endif
