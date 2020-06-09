typedef LRUCache<Value>::Maker ValueMaker;

LRUCache<Value>& TheValueCache();

Value MakeValue(ValueMaker& m);

void ShrinkValueCache();
void ShrinkValueCache(int maxsize);

template <class P>
int ValueCacheRemove(P what)
{
	return TheValueCache().Remove(what);
}

template <class P>
int ValueCacheRemoveOne(P what)
{
	return TheValueCache().Remove(what);
}
