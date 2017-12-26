#include <Core/Core.h>

#include <queue>

using namespace Upp;

#ifdef _DEBUG
#define M 1
#define N 10
#else
#define M 10000
#define N 100
#endif

CONSOLE_APP_MAIN
{
	Vector<int> data;
	for(int j = 0; j < M; j++) {
		for(int i = 0; i < N; i++)
			data.Add(Random());
		{
			RTIMING("std::priority_queue");
			std::priority_queue<int> pq;
			for(int i = 0; i < N; i++)
				pq.push(data[i]);
			while(pq.size()) {
				DUMP(pq.top());
				pq.pop();
			}
		}
		LOG("----------------------");
		LOG("----------------------");
		{
			RTIMING("SortedIndex");
			SortedIndex<int> sv;
			for(int i = 0; i < N; i++)
				sv.FindAdd(data[i]);
			while(sv.GetCount()) {
				DUMP(sv.Top());
				sv.Drop();
			}
		}
	}
}

/*
PriorityQueue<int, String> pq;
pq.GetCount();
pq.Trim();
pq.Top();
pq.TopKey();
pq.Drop();
pq.Pop();
*/
