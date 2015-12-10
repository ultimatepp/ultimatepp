#include <Core/Core.h>

#include <vector>

using namespace Upp;

CONSOLE_APP_MAIN
{
	int vector_capacity = 0;
	int vector_reallocs = 0;
	int vector_total = 0;
	int Vector_alloc = 0;
	int Vector_reallocs = 0;
	int Vector_total = 0;
	for(int i = 0; i < 20000; i++) {
		std::vector<int> a;
		Vector<int> b;
		
		for(int j = 0; j < i; j++) {
			a.push_back(j);
			b.Add(j);
		}
		
		if(vector_capacity != a.capacity()) {
			vector_reallocs += a.size();
			vector_capacity = a.capacity();
		}

		if(Vector_alloc != b.GetAlloc()) {
			Vector_reallocs += b.GetCount();
			Vector_alloc = b.GetAlloc();
		}
		
		RLOG(i << ", vector: " << vector_capacity << " / " << vector_reallocs
		       << ", Vector: " << Vector_alloc << " / " << Vector_reallocs);
		
		vector_total += vector_capacity;
		Vector_total += Vector_alloc;
	}
	RDUMP(vector_total);
	RDUMP(Vector_total);
}
