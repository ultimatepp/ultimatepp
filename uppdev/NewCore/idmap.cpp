#include "Core.h"

#undef String
#undef StringBuffer
#undef WString
#undef WStringBuffer

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

//#define TEST_HASHMAP

#ifdef TEST_HASHMAP

#ifdef COMPILER_GCC
#include <tr1/unordered_map>
#else
#include <hash_map>
#endif

#endif

using namespace std;
using namespace Upp;

#define NO_OUTPUT // for benchmark purposes, output is omitted

void BenchNTL(const char *file) {
	FileIn in(file);
	if (!in) {
		std::cout << "Cannot open input file.\n";
		return;
	}

	VectorMap<String, Vector<int> > map;
	int line = 1;

	for(;;) {
		int c = in.Get();
		if(c < 0) break;
		if(isalpha(c) || c == '_') {
			String id;
			id.Cat(c);
			c = in.Get();
			while(c >= 0 && (isalnum(c) || c == '_')) {
				id.Cat(c);
				c = in.Get();
			}
			map.GetAdd(id).Add(line);
		}
		else
		if(isdigit(c))
			do c = in.Get();
			while(c >= 0 && (isalnum(c) || c == '.'));
		if(c == '\n')
			++line;
	}

	Vector<int> order = GetSortOrder(map.GetKeys());
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

void BenchNewNTL(const char *file) {
	FileIn in(file);
	if (!in) {
		std::cout << "Cannot open input file.\n";
		return;
	}

	VectorMap<NewString, Vector<int> > map;
	int line = 1;

	for(;;) {
		int c = in.Get();
		if(c < 0) break;
		if(isalpha(c) || c == '_') {
			NewString id;
			id.Cat(c);
			c = in.Get();
			while(c >= 0 && (isalnum(c) || c == '_')) {
				id.Cat(c);
				c = in.Get();
			}
			map.GetAdd(id).Add(line);
		}
		else
		if(isdigit(c))
			do c = in.Get();
			while(c >= 0 && (isalnum(c) || c == '.'));
		if(c == '\n')
			++line;
	}

	Vector<int> order = GetSortOrder(map.GetKeys());
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

template <class Container>
void BenchSTL(Container& imap, const char *file) {
	std::ifstream in(file);
	if (!in) {
		std::cout << "Cannot open input file.\n";
		return;
	}

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

#ifdef TEST_HASHMAP

#ifdef COMPILER_GCC
typedef std::tr1::unordered_map< string, vector<int> > HashMap;
#else
typedef stdext::hash_map< string, vector<int> > HashMap;
#endif

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

#endif

#ifdef NO_OUTPUT
#define N 10000
#else
#define N 1
#endif

void IdMapBench()
{
	String fn;
	int argc = CommandLine().GetCount();
	const Vector<String>& argv = CommandLine();
	if(argc < 1)
		fn = GetDataFile("main.cpp");
	else
		fn = argv[0];

	BenchNTL(fn); // first run to cache the file

#ifdef TEST_HASHMAP
	{
		BenchHashMap(fn);
		TimeStop tm;
		for(int n = 0; n < N; n++)
			BenchHashMap(fn);
		cout << "STL hash_map time: " << tm.Elapsed() << " ms \n";
	}
#endif

	{
		BenchMap(fn);
		TimeStop tm;
		for(int n = 0; n < N; n++)
			BenchMap(fn);
		cout << "STL map time: " << tm.Elapsed() << " ms \n";
	}

	{
		BenchNTL(fn);
		TimeStop tm;
		for(int n = 0; n < N; n++)
			BenchNTL(fn);
		cout << "Old U++ Core time: " << tm.Elapsed() << " ms\n";
	}

	{
		BenchNewNTL(fn);
		TimeStop tm;
		for(int n = 0; n < N; n++)
			BenchNewNTL(fn);
		cout << "New U++ Core time: " << tm.Elapsed() << " ms\n";
	}
}
