#include <Core/Core.h>

using namespace Upp;

class MyString {
	int len;
	union {
		char  *ptr;
		char   data[16];
	};

public:
	MyString(const char *s) {
		len = strlen(s);
		if(len < 16)
			memcpy(data, s, len + 1);
		else {
			ptr = new char[len + 1];
			memcpy(ptr, s, len + 1);
		}
	}

	~MyString() {
		if(len >= 16)
			delete []ptr;
	}
};

template <class T>
class MyArray {
	T *begin;
	T *end;
	
public:
	MyArray(int n) { begin = new T[n]; end = begin + n; }
	~MyArray()     { delete begin; }
	
	T& operator[](int i) { return begin[i]; }
};

template <class T>
class MyIndirectArray {
	int          n;
	MyArray<T *> a;
public:
	T& operator[](int i) { return *a[i]; }

	MyIndirectArray(int n) : a(n), n(n) { for(int i = 0; i < n; i++) { a[i] = new T; } }
	~MyIndirectArray() { for(int i = 0; i < n; i++) { delete a[i]; } }
};

template <class K, class V>
class MyMap {
	MyArray<K> keys;
	MyArray<V> values;
	
public:
	MyMap(int n) : keys(n), values(n) {}
	
	void Set(int i, K key, V value) { keys[i] = key; values[i] = value; }
};

CONSOLE_APP_MAIN
{
	MyString short_string("Hello world!");
	MyString long_string("This is a very long string!");
	
	MyArray<String> a(3);
	a[0] = "zero";
	a[1] = "one";
	a[2] = "two";
	
	MyIndirectArray<int> b(4);
	b[0] = 1000;
	b[1] = 1001;
	b[2] = 1002;
	b[3] = 1003;
	
	MyMap<String, int> m(3);
	m.Set(0, "zero", 0);
	m.Set(1, "one", 1);
	m.Set(2, "two", 2);

	LOG("BREAKPOINT");
}
