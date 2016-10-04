#ifndef _Flex_Flex_h_
#define _Flex_Flex_h_

#include <Core/Core.h>

using namespace Upp;

template <class T>
class Flex {
//	enum { SHIFT = 11, NBLK = 1 << SHIFT, MASK = NBLK - 1,  };

	int SHIFT, NBLK, MASK;

	int          *offset;
	T            *data;
	int           alloc;
	int           count;
	int           blk_items;
	int           blk_count;

	void          Move(int tgt, int src, int count) {
		if(count > 0)
			memmove(&data[tgt], &data[src], count * sizeof(T));
	}

	template <class Less>
	int LBound(const T& x, int l, int h, const Less& less) const;

	template <class Less>
	int UBound(const T& x, int l, int h, const Less& less) const;

	template <class Less>
	int FindLowerBound0(const T& x, const Less& less, bool *eq) const;
	
	void Destroy(int from, int count);
	void Free();

	void Expand(int n);
	T   *RawInsert(int i);
	void Init();
	template <class Less>
	int FindLowerBound0(const T& x, const Less& less) const;

public:
	T& operator[](int i) {
		return data[((i + offset[i >> SHIFT]) & MASK) + (i & ~MASK)];
	}

	const T& operator[](int i) const {
		return data[((i + offset[i >> SHIFT]) & MASK) + (i & ~MASK)];
	}

	int  GetCount() const { return count; }

	void Insert(int i, const T& x);

	void Clear();

	template <class Less>
	int FindLowerBound(const T& x, const Less& less) const {
		return blk_count == 0 ? LBound(x, 0, GetCount(), less) : FindLowerBound0(x, less);
	}
	int FindLowerBound(const T& x) const { return FindLowerBound(x, StdLess<T>()); }

	template <class Less>
	int FindUpperBound(const T& x, const Less& less = StdLess<T>()) const;
	int FindUpperBound(const T& x) const { return FindUpperBound(x, StdLess<T>()); }
	
	String GetInfo() const { return String().Cat() << "NBLK:" << NBLK << ", blocks:" << blk_count; }

	Flex(int sh = 11) {
		Init();
	}
	
	~Flex() {
		Free();
	}
};

#include "Flex.hpp"
#include "Order.h"

#endif
