#define  NDEBUG
#define  _SECURE_SCL 0

#include <Core/Core.h>

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include <algorithm>
#include <map>
#include <deque>
#include <string>
#include <unordered_map>

using namespace std;
using namespace Upp;

#ifndef _DEBUG
#define NO_OUTPUT // for benchmark purposes, output is omitted
#endif

#ifdef flagMEMORYTEST

#include <new>

int64 alloc_len;

void *TestAlloc(size_t size)
{
	alloc_len += size;
	int64 *ptr = (int64 *)malloc(size + sizeof(int64));
	*ptr = size;
	return ptr + 1;
}

void TestFree(void *ptr)
{
	if(ptr) {
		int64 *p = (int64 *)ptr - 1;
		alloc_len -= *p;
		free(p);
	}
}

void *operator new(size_t size) throw(std::bad_alloc) { void *ptr = TestAlloc(size); return ptr; }
void operator  delete(void *ptr) throw()              { TestFree(ptr); }

void *operator new[](size_t size) throw(std::bad_alloc) { void *ptr = TestAlloc(size); return ptr; }
void operator  delete[](void *ptr) throw()              { TestFree(ptr); }

void *operator new(size_t size, const std::nothrow_t&) throw() { void *ptr = TestAlloc(size); return ptr; }
void operator  delete(void *ptr, const std::nothrow_t&) throw() { TestFree(ptr); }

void *operator new[](size_t size, const std::nothrow_t&) throw() { void *ptr = TestAlloc(size); return ptr; }
void operator  delete[](void *ptr, const std::nothrow_t&) throw() { TestFree(ptr); }

void InitMemoryTest() { RLOG("------"); alloc_len = 0; }
void PrintMemoryTest(const char *s) { RLOG(s << " " << alloc_len); }

#else

inline void InitMemoryTest() {}
inline void PrintMemoryTest(const char *) {}

#endif

void BenchNTL(const char *file) {
	FileIn in(file);
	if (!in) {
		RLOG("Cannot open input file.");
		return;
	}

	InitMemoryTest();

	VectorMap<String, Vector<int> > map;
	int line = 1;

	for(;;) {
		int c = in.Get();
		if(c < 0) break;
		if(IsAlpha(c) || c == '_') {
			String id;
			id.Cat(c);
			c = in.Get();
			while(c >= 0 && (IsAlNum(c) || c == '_')) {
				id.Cat(c);
				c = in.Get();
			}
			map.GetAdd(id).Add(line);
		}
		else
		if(IsDigit(c))
			do c = in.Get();
			while(c >= 0 && (IsAlNum(c) || c == '.'));
		if(c == '\n')
			++line;
	}

	Vector<int> order = GetSortOrder(map.GetKeys());
	
	PrintMemoryTest("VectorMap");

#ifdef flagMEMORYTEST
	RLOG(order.GetCount());
#endif

#ifndef NO_OUTPUT
	for(int i = 0; i < order.GetCount(); i++) {
		std::cout << ~map.GetKey(order[i]) << ": ";
		const Vector<int>& l = map[order[i]];
		for(int i = 0; i < l.GetCount(); i++) {
			if(i) std::cout << ", ";
			std::cout << l[i];
		}
		std::cout << '\n';
	}
#endif
}

void BenchSortedVectorMap(const char *file) {
	FileIn in(file);
	if (!in) {
		RLOG("Cannot open input file.");
		return;
	}

	InitMemoryTest();

	SortedVectorMap<String, Vector<int> > map;
	int line = 1;

	for(;;) {
		int c = in.Get();
		if(c < 0) break;
		if(IsAlpha(c) || c == '_') {
			String id;
			id.Cat(c);
			c = in.Get();
			while(c >= 0 && (IsAlNum(c) || c == '_')) {
				id.Cat(c);
				c = in.Get();
			}
			map.GetAdd(id).Add(line);
		}
		else
		if(IsDigit(c))
			do c = in.Get();
			while(c >= 0 && (IsAlNum(c) || c == '.'));
		if(c == '\n')
			++line;
	}

	Vector<int> order = GetSortOrder(map.GetKeys());
	PrintMemoryTest("SortedVectorMap");
	
#ifndef NO_OUTPUT
	for(int i = 0; i < order.GetCount(); i++) {
		std::cout << ~map.GetKey(order[i]) << ": ";
		const Vector<int>& l = map[order[i]];
		for(int i = 0; i < l.GetCount(); i++) {
			if(i) std::cout << ", ";
			std::cout << l[i];
		}
		std::cout << '\n';
	}
#endif
}

bool SimpleValueOrder(const Value& a, const Value& b)
{
	return AsString(a) < AsString(b);
}

void BenchValueMap(const char *file) {
	FileIn in(file);
	if (!in) {
		RLOG("Cannot open input file.");
		return;
	}

	InitMemoryTest();

	ValueMap map;
	int line = 1;

	for(;;) {
		int c = in.Get();
		if(c < 0) break;
		if(IsAlpha(c) || c == '_') {
			String id;
			id.Cat(c);
			c = in.Get();
			while(c >= 0 && (IsAlNum(c) || c == '_')) {
				id.Cat(c);
				c = in.Get();
			}
			map(id) << line;
		}
		else
		if(IsDigit(c))
			do c = in.Get();
			while(c >= 0 && (IsAlNum(c) || c == '.'));
		if(c == '\n')
			++line;
	}

	Vector<int> order = GetSortOrder(map.GetKeys(), SimpleValueOrder);
	PrintMemoryTest("ValueMap");

#ifndef NO_OUTPUT
	for(int i = 0; i < order.GetCount(); i++) {
		std::cout << ~(String)(map.GetKeys()[order[i]]) << ": ";
		ValueArray l = map.GetValues()[order[i]];
		for(int i = 0; i < l.GetCount(); i++) {
			if(i) std::cout << ", ";
			std::cout << (int)l[i];
		}
		std::cout << '\n';
	}
#endif
}

template <class Container>
void BenchSTL(Container& imap, const char *file) {
	std::ifstream in(file);
	if (!in) {
		RLOG("Cannot open input file.");
		return;
	}

	InitMemoryTest();

	int line = 1;

	for(;;) {
		int c = in.get();
		if(c == EOF) break;
		if(isalpha(c) || c == '_') {
			string id;
			id += c;
			c = in.get();
			while(c != EOF && (isalnum(c) || c == '_')) {
				id += c;
				c = in.get();
			}
			imap[id].push_back(line);
		}
		else
		if(isdigit(c))
			do c = in.get();
			while(c != EOF && (isalnum(c) || c == '.'));
		if(c == '\n')
			++line;
	}
}

void BenchMap(const char *file)
{
	map< string, vector<int> > imap;
	BenchSTL(imap, file);
	PrintMemoryTest("std::map");
#ifndef NO_OUTPUT
	map< std::string, vector<int> >::const_iterator e = imap.end();
	for(map< std::string, vector<int> >::const_iterator i = imap.begin(); i != e; i++) {
		std::cout << i->first << ": ";
		vector<int>::const_iterator e = i->second.end();
		vector<int>::const_iterator b = i->second.begin();
		for(vector<int>::const_iterator j = b; j != e; j++) {
			if(j != b) std::cout << ", ";
			std::cout << *j;
		}
		std::cout << '\n';
	}
#endif
}

typedef std::unordered_map< string, vector<int> > HashMap;

inline bool h_less(const HashMap::value_type *a, const HashMap::value_type *b)
{
	return a->first < b->first;
}

void BenchHashMap(const char *file)
{
	HashMap imap;
	BenchSTL(imap, file);
	vector< const HashMap::value_type * > order;
	for(HashMap::const_iterator i = imap.begin(); i != imap.end(); i++)
		order.push_back(&*i);
	sort(order.begin(), order.end(), h_less);
	PrintMemoryTest("std::unordered_map");

#ifndef NO_OUTPUT
	vector< const HashMap::value_type * >::const_iterator e = order.end();
	for(vector< const HashMap::value_type * >::const_iterator i = order.begin(); i != e; i++) {
		std::cout << (*i)->first << ": ";
		vector<int>::const_iterator e = (*i)->second.end();
		vector<int>::const_iterator b = (*i)->second.begin();
		for(vector<int>::const_iterator j = b; j != e; j++) {
			if(j != b) std::cout << ", ";
			std::cout << *j;
		}
		std::cout << '\n';
	}
#endif
}

#if defined(_DEBUG) || defined(flagMEMORYTEST)
#define N 1
#else
#define N 1000
#endif

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	String fn;
	int argc = CommandLine().GetCount();
	const Vector<String>& argv = CommandLine();
	if(argc < 1)
		fn = GetDataFile("alice30.txt");
	else
		fn = argv[0];

	LoadFile(fn); // cache the file
	BenchNTL(fn); // alloc memory

#ifdef UPP_HEAP
	MemoryProfile f;
//	RLOG(f);
//	allocated_bytes0 = GetAllocatedBytes(f);

#endif


#if 1
	{
		TimeStop tm;
		for(int n = 0; n < N; n++)
			BenchHashMap(fn);
		RLOG("STL hash_map time: " << tm.Elapsed() << " ms");
	}

	{
		TimeStop tm;
		for(int n = 0; n < N; n++)
			BenchMap(fn);
		RLOG("STL map time: " << tm.Elapsed() << " ms");
	}

	{
		TimeStop tm;
		for(int n = 0; n < N; n++)
			BenchValueMap(fn);
		RLOG("ValueMap time: " << tm.Elapsed() << " ms");
	}
	{
		TimeStop tm;
		for(int n = 0; n < N; n++)
			BenchSortedVectorMap(fn);
		RLOG("SortedVectorMap time: " << tm.Elapsed() << " ms");
	}
#endif
	{
		TimeStop tm;
		for(int n = 0; n < N; n++)
			BenchNTL(fn);
		RLOG("VectorMap time: " << tm.Elapsed() << " ms");
	}
}
