#include <Core/Core.h>


struct TestMaker : LRUCache<String>::Maker {
	int x;

	virtual String Key() const  { return AsString(x); }
	virtual int    Make(String& h) const {
		LOG("MAKE " << x);
		for(int i = 0; i < x; i++)
			h << AsString(i);
		return h.GetLength();
	}
};

CONSOLE_APP_MAIN
{
	LRUCache<String> cache;
	cache.SetMaxSize(INT_MAX);
	for(int i = 0; i < 100000; i++) {
		TestMaker m;
		m.x = rand();
		DUMP(m.x);
		DUMP(cache.Get(m));
		LOG("-------");
	}
	cache.SetMaxSize(0);
	cache.Dump();
}

/*
#ifdef _DEBUG
	void Dump() {
		LOGBEGIN();
		DUMP(size);
		DUMP(head);
		int i = head;
		if(i >= 0) {
			do {
				if(!key.IsUnlinked(i))
					LOG(i << ": " << *data[i].data << " (" << data[i].size << ")" << ", prev: " << data[i].prev);
				i = data[i].next;
			}
			while(i != head);
		}
		LOGEND();
	}
#endif

*/