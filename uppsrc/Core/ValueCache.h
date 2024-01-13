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

template <class M>
Value MakeValue_(const String& key, const M& m, int& sz)
{
	struct Maker : ValueMaker {
		const String& key;
		const M& m;

		String Key() const override {
			return key;
		}

		int Make(Value& object) const override {
			return m(object);
		}
		
		Maker(const String& key, const M& m) : key(key), m(m) {}
	};
	
	Maker maker(key, m);
	return MakeValueSz(maker, sz);
}

template <class K, class M>
String MakeKey_(const K& k, const M& m)
{
	StringBuffer key;
	RawCat(key, StaticTypeNo<K>());
	RawCat(key, StaticTypeNo<M>());
	key.Cat(k());
	return String(key);
}

template <class K, class M>
Value MakeValue(const K& k, const M& m)
{
	int sz;
	return MakeValue_(MakeKey_(k, m), m, sz);
}

template <class K, class M>
Value MakeValueTL(const K& k, const M& m)
{
	String key = MakeKey_(k, m);

	struct Maker : ValueMaker {
		const String& key;
		const M& m;

		String Key() const override {
			return key;
		}

		int Make(Value& object) const override {
			int sz;
			object = MakeValue_(key, m, sz);
			return sz;
		}
		
		Maker(const String& key, const M& m) : key(key), m(m) {}
	};
	Maker maker(key, m);
	if(IsMainThread()) {
		static LRUCache<Value> cache; // this is basically to avoid problem with leaks detection
		Value v = cache.Get(maker);
		cache.Shrink(128 * 1024, 1000);
		return v;
	}
	else {
		thread_local LRUCache<Value> cache;
		Value v = cache.Get(maker);
		cache.Shrink(128 * 1024, 1000);
		return v;
	}
}
