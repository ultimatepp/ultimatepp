#include "Core.h"

#define LLOG(x) // LOG(x)

namespace Upp {

StaticMutex ValueCacheMutex;

std::atomic<bool> sValueCacheFinished;

struct ValueMakeCacheClass : LRUCache<Value> {
	~ValueMakeCacheClass() { sValueCacheFinished = true; }
};

LRUCache<Value>& TheValueCache()
{
	static ValueMakeCacheClass m;
	return m;
}

bool IsValueCacheActive()
{
	return !sValueCacheFinished;
}

bool ValueCacheFixed = false;
int  ValueCacheMaxSize = 0;
int  ValueCacheMaxCount = 20000;

void AdjustValueCache()
{
	Mutex::Lock __(ValueCacheMutex);
	if(ValueCacheFixed)
		return;
	uint64 total, available;
	GetSystemMemoryStatus(total, available);
	ValueCacheMaxSize = int(available >> 10);
	if(!ValueCacheMaxSize)
		ValueCacheMaxSize = available ? 128*1024*1024 : 16*1024;
	ValueCacheMaxCount = max(ValueCacheMaxSize / 200, 20000);
	LLOG("New MakeValue max size " << ValueCacheMaxSize << " count " << ValueCacheMaxCount);
	ShrinkValueCache();
}

void ShrinkValueCache()
{
	Mutex::Lock __(ValueCacheMutex);
	if(!ValueCacheMaxSize) {
		static bool lock;
		if(!lock) { // prevent (unlikely) recursion
			lock = true;
			AdjustValueCache();
			lock = false;
		}
	}
	LLOG("MakeValue cache size before shrink: " << TheValueCache().GetSize());
	TheValueCache().Shrink(ValueCacheMaxSize, ValueCacheMaxCount);
	LLOG("MakeValue cache size after shrink: " << TheValueCache().GetSize());
}

void SetupValueCache(int maxsize, int maxcount)
{
	Mutex::Lock __(ValueCacheMutex);

	if(maxsize <= 0) {
		ValueCacheFixed = false;
		AdjustValueCache();
	}
	else {
		ValueCacheMaxSize = maxsize;
		ValueCacheMaxCount = maxcount;
		ValueCacheFixed = true;
	}
}


Value MakeValueSz(ValueMaker& m, int& sz)
{
	Mutex::Lock __(ValueCacheMutex);
	LLOG("MakeValue cache size before make: " << TheValueCache().GetSize());
	Value v = TheValueCache().Get(m, [] { ValueCacheMutex.Leave(); }, [] { ValueCacheMutex.Enter(); }, sz);
	LLOG("MakeValue cache size after make: " << TheValueCache().GetSize());
	ShrinkValueCache();
	LLOG("-------------");
	return v;
}

Value MakeValue(ValueMaker& m)
{
	int sz;
	return MakeValueSz(m, sz);
}

};