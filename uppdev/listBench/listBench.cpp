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
#include <list>

using namespace std;

const int REP = 100000;
const int N   = 1000;
const int AND = 31;
const int LIM = 15;

inline bool llim(int x)
{
	return x < LIM;
}

CONSOLE_APP_MAIN
{
	Vector<int> seq;
	for(int i = 0; i < N; i++)
		seq.Add(rand() & AND);
	for(int n = 0; n < REP; n++) {
		{
			Vector<int> s(seq, 1);
			RTIMING("Vector trivial");
			Vector<int> t;
			for(int i = 0; i < s.GetCount(); i++)
				if(s[i] >= LIM)
					t.Add(s[i]);
		}
		{
			Vector<int> s(seq, 1);
			RTIMING("Vector reserve");
			Vector<int> t;
			t.Reserve(s.GetCount());
			for(int i = 0; i < s.GetCount(); i++)
				if(s[i] >= LIM)
					t.Add(s[i]);
		}
		{
			Vector<int> s(seq, 1);
			RTIMING("Vector single");
			int q = 0;
			for(int i = 0; i < s.GetCount(); i++)
				if(s[i] >= LIM)
					s[q++] = s[i];
			s.SetCount(q);
		}
		{
			list<int> l(seq.Begin(), seq.End());
			RTIMING("list remove_if");
			l.remove_if(llim);
		}
	}
}
