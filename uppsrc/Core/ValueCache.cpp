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

int ValueCacheMaxSize = 4000000;

int ValueCacheMaxSizeLimitLow = 1024*1024;
int ValueCacheMaxSizeLimitHigh = 0;
double ValueCacheRatio = 0.125;

void AdjustValueCache()
{
	Mutex::Lock __(ValueCacheMutex);
	uint64 total, available;
	GetSystemMemoryStatus(total, available);
	if(ValueCacheMaxSizeLimitHigh == 0)
		ValueCacheMaxSizeLimitHigh = INT_MAX;
	ValueCacheMaxSize = clamp((int)min((int64)(ValueCacheRatio * available), (int64)2000*1024*1024),
	                          ValueCacheMaxSizeLimitLow, ValueCacheMaxSizeLimitHigh);
	LLOG("New MakeValue max size " << ValueCacheMaxSize << " high limit " << ValueCacheMaxSizeLimitHigh);
	ShrinkValueCache();
}

void ShrinkValueCache()
{
	Mutex::Lock __(ValueCacheMutex);
	if(!ValueCacheMaxSizeLimitHigh)
		AdjustValueCache();
	TheValueCache().Shrink(ValueCacheMaxSize, 20000);
	LLOG("MakeValue cache size after shrink: " << TheValueCache().GetSize());
}

void SetupValueCache(int limit_low, int limit_high, double ratio)
{
	Mutex::Lock __(ValueCacheMutex);

	ValueCacheMaxSizeLimitLow = 1000000;
	ValueCacheMaxSizeLimitHigh = 256000000;
	ValueCacheRatio = 0.125;
}

Value MakeValue(ValueMaker& m)
{
	Mutex::Lock __(ValueCacheMutex);
	LLOG("MakeValue cache size before make: " << TheValueCache().GetSize());
	Value v = TheValueCache().Get(m);
	LLOG("MakeValue cache size after make: " << TheValueCache().GetSize());
	ShrinkValueCache();
	LLOG("-------------");
	return v;
}

};