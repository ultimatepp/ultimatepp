extern StaticMutex ValueCacheMutex;

LRUCache<Value>& TheValueCache();

typedef LRUCache<Value>::Maker ValueMaker;

Value MakeValueSz(ValueMaker& m, int& sz);
Value MakeValue(ValueMaker& m);

bool IsValueCacheActive();

void AdjustValueCache();
void ShrinkValueCache();

void SetupValueCache(int maxsize, int maxcount);

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
