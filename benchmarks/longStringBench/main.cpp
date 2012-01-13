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

#define TEST_HASHMAP

#ifdef TEST_HASHMAP

#ifdef COMPILER_GCC
#include <tr1/unordered_map>
#else
#include <hash_map>
#endif

#endif

using namespace std;
using namespace Upp;

#ifdef _DEBUG
#define N 1
#else
#define N 10000
#define NO_OUTPUT // for benchmark purposes, output is omitted
#endif

void BenchNTL(const char *file) {
	FileIn in(file);
	if (!in) {
		std::cout << "Cannot open input file.\n";
		return;
	}

	VectorMap<String, int> map;
	int line = 1;

	for(;;) {
		int c = in.Get();
		if(c < 0) break;
		String id;
		while(c >= 0 && c != '.') {
			if((byte)c >= ' ')
				id.Cat(c);
			c = in.Get();
		}
		id.Cat('.');
		map.GetAdd(id, 0)++;
	}

	Vector<int> order = GetSortOrder(map);
#ifndef NO_OUTPUT
	for(int i = 0; i < order.GetCount(); i++) {
		RLOG(map[order[i]] << ": " << map.GetKey(order[i]));
	}
#endif
}

CONSOLE_APP_MAIN
{
	String fn;
	int argc = CommandLine().GetCount();
	const Vector<String>& argv = CommandLine();
	if(argc < 1)
		fn = GetDataFile("main.cpp");
	else
		fn = argv[0];

	{
		BenchNTL(fn);
		TimeStop tm;
		for(int n = 0; n < N; n++)
			BenchNTL(fn);
		cout << "NTL time: " << tm.Elapsed() << " ms\n";
	}
}
