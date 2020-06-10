extern StaticMutex ValueCacheMutex;

LRUCache<Value>& TheValueCache();

typedef LRUCache<Value>::Maker ValueMaker;

Value MakeValue(ValueMaker& m);

bool IsValueCacheActive();

void AdjustValueCache();
void ShrinkValueCache();

void SetupValueCache(int limit_low, int limit_high, double ratio);

template <class P>
int ValueCacheRemove(P what)
{
	Mutex::Lock __(ValueCacheMutex);
	return TheValueCache().Remove(what);
}

template <class P>
int ValueCacheRemoveOne(P what)
{
	Mutex::Lock __(ValueCacheMutex);
	return TheValueCache().Remove(what);
}

template <class P>
void ValueCacheAdjustSize(P getsize)
{
	Mutex::Lock __(ValueCacheMutex);
	TheValueCache().AdjustSize(getsize);
}
