#include "Core.h"

namespace Upp {

bool sFinished;

struct ValueMakeCacheClass : LRUCache<Value> {
	~ValueMakeCacheClass() { sFinished = true; }
};

LRUCache<Value>& TheValueCache()
{
	static ValueMakeCacheClass m;
	return m;
}

int sMaxSize = 1000000;

void ShrinkValueCache()
{
	TheValueCache().Shrink(sMaxSize, 2000);
}

void ShrinkValueCache(int maxsize)
{
	sMaxSize = maxsize;
	ShrinkValueCache();
}

Value MakeValue(ValueMaker& m)
{
	Value v = TheValueCache().Get(m);
	ShrinkValueCache();
	return v;
}

};