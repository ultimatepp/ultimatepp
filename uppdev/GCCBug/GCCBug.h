#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>

template <class T> inline const T& my_max(const T& a, const T& b) { return a > b ? a : b; }
template <class T> inline const T& my_min(const T& a, const T& b) { return a < b ? a : b; }

template <class T>
inline T minmax(T x, T _min, T _max) { return my_min(my_max(x, _min), _max); }

void *MemoryAllocSz(size_t& sz);
void  MemoryFree(void *);

template <class T>
class Vector {
	T       *vector;
	int      items;
	int      alloc;

	static void RawFree(T *ptr)            { if(ptr) MemoryFree(ptr); }
	static T   *RawAlloc(int& n);

	void RawInsert(int q, int count);
	

public:
	void  InsertN(int q, int count);
	const T& First() { return vector[0]; }
	int   GetCount() const { return items; }
	
	Vector() { vector = NULL; items = alloc = 0; }
};

template <class T>
T * Vector<T>::RawAlloc(int& n)
{
	size_t sz0 = n * sizeof(T);
	size_t sz = sz0;
	void *q = MemoryAllocSz(sz);
	n += (int)((sz - sz0) / sizeof(T));
	return (T *)q;
}

template <class T>
void Vector<T>::RawInsert(int q, int count)
{
	if(!count) return;
	if(items + count > alloc) {
		alloc = alloc + my_max(alloc, count);
		T *newvector = RawAlloc(alloc);
		if(vector) {
			memcpy(newvector, vector, q * sizeof(T));
			memcpy(newvector + q + count, vector + q, (items - q) * sizeof(T));
			RawFree(vector);
		}
		vector = newvector;
	}
	else {
		memmove(vector + q + count, vector + q, (items - q) * sizeof(T));
	}
	items += count;
}

template <class T>
void Vector<T>::InsertN(int q, int count)
{
	RawInsert(q, count);
}

void *MemoryAllocSz(size_t& sz);
void  MemoryFree(void *);

struct Item {
	char h[32];
};

struct Bar {
	Vector<Item> li;
	
	void DoTest(int i, int count);
};
