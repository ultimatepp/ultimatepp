#include "Core/Core.h"

#define TEST_HASHMAP

#ifdef TEST_HASHMAP

#ifdef COMPILER_GCC
#include <tr1/unordered_map>
#else
#include <hash_map>
#endif

#include <map>
#include <string>
#include <vector>

#endif

using namespace std;
using namespace Upp;

template <class C>
void BenchNTL(const char *file, Stream& out) {
	FileIn in(file);
	if (!in) {
		out << "Cannot open input file.\n";
		return;
	}

	C map;
	
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
			map.GetAdd(id, 0)++;
		}
		else
		if(IsDigit(c))
			do c = in.Get();
			while(c >= 0 && (IsAlNum(c) || c == '.'));
	}

	for(int i = 0; i < map.GetCount(); i++)
		out << ~map.GetKey(i) << ": " << map[i] << '\n';
}

void BenchNTL2(const char *file, Stream& out) {
	FileIn in(file);
	if (!in) {
		out << "Cannot open input file.\n";
		return;
	}

	VectorMap<String, int> map;
	
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
			map.GetAdd(id, 0)++;
		}
		else
		if(IsDigit(c))
			do c = in.Get();
			while(c >= 0 && (IsAlNum(c) || c == '.'));
	}

	Vector<int> order = GetSortOrder(map.GetKeys());
	for(int i = 0; i < order.GetCount(); i++)
		out << ~map.GetKey(order[i]) << ": " << map[order[i]] << '\n';
}

void BenchSTL(const char *file, Stream& out) {
	FileIn in(file);
	if (!in) {
		out << "Cannot open input file.\n";
		return;
	}

	map<string, int> imap;

	for(;;) {
		int c = in.Get();
		if(c == EOF) break;
		if(isalpha(c) || c == '_') {
			string id;
			id += c;
			c = in.Get();
			while(c != EOF && (isalnum(c) || c == '_')) {
				id += c;
				c = in.Get();
			}
			imap[id]++;
		}
		else
		if(isdigit(c))
			do c = in.Get();
			while(c != EOF && (isalnum(c) || c == '.'));
	}
	map< std::string, int >::const_iterator e = imap.end();
	for(map< std::string, int >::const_iterator i = imap.begin(); i != e; i++)
		out << i->first.c_str() << ": " << i->second << '\n';
}

void BenchSTL2(const char *file, Stream& out) {
	FileIn in(file);
	if (!in) {
		out << "Cannot open input file.\n";
		return;
	}

	int line = 1;
	map<String, int> imap;

	for(;;) {
		int c = in.Get();
		if(c == EOF) break;
		if(isalpha(c) || c == '_') {
			string id;
			id += c;
			c = in.Get();
			while(c != EOF && (isalnum(c) || c == '_')) {
				id += c;
				c = in.Get();
			}
			imap[id]++;
		}
		else
		if(isdigit(c))
			do c = in.Get();
			while(c != EOF && (isalnum(c) || c == '.'));
		if(c == '\n')
			++line;
	}
	map< String, int >::const_iterator e = imap.end();
	for(map< String, int >::const_iterator i = imap.begin(); i != e; i++)
		out << i->first << ": " << i->second << '\n';
}

#ifdef _DEBUG
#define N 0
#else
#define N 10
#endif

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	String fn;
	int argc = CommandLine().GetCount();
	const Vector<String>& argv = CommandLine();
	if(argc < 1)
		fn = GetDataFile("test.txt");
	else
		fn = argv[0];

	BenchSTL(fn, NilStream()); // first run to cache the file

	{
		FileOut out(GetHomeDirFile("stl.txt"));
		BenchSTL(fn, out);
		TimeStop tm;
		for(int n = 0; n < N; n++)
			BenchSTL(fn, NilStream());
		RLOG("std::map<std::string, int> time: " << tm.Elapsed() << " ms");
	}

	{
		FileOut out(GetHomeDirFile("stl.txt"));
		BenchSTL2(fn, out);
		TimeStop tm;
		for(int n = 0; n < N; n++)
			BenchSTL2(fn, NilStream());
		RLOG("std::map<String, int> time: " << tm.Elapsed() << " ms");
	}

	{
		FileOut out(GetHomeDirFile("ntl.txt"));
		BenchNTL< SortedVectorMap<String, int> >(fn, out);
		TimeStop tm;
		for(int n = 0; n < N; n++)
			BenchNTL< SortedVectorMap<String, int> >(fn, NilStream());
		RLOG("SortedVectorMap<String, int> time: " << tm.Elapsed() << " ms");
	}

	{
		FileOut out(GetHomeDirFile("ntla.txt"));
		BenchNTL< SortedArrayMap<String, int> >(fn, out);
		TimeStop tm;
		for(int n = 0; n < N; n++)
			BenchNTL< SortedArrayMap<String, int> >(fn, NilStream());
		RLOG("SortedArrayMap<String, int> time: " << tm.Elapsed() << " ms");
	}

	{
		FileOut out(GetHomeDirFile("ntl2.txt"));
		BenchNTL2(fn, out);
		TimeStop tm;
		for(int n = 0; n < N; n++)
			BenchNTL2(fn, NilStream());
		RLOG("VectorMap<String, int> time: " << tm.Elapsed() << " ms");
	}
}
