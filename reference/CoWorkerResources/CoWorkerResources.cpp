#include <Core/Core.h>

using namespace Upp;

struct Model { // some computation model that is not stateless, abstract definition
	virtual void   Start(int i) = 0;
	virtual int    Compute() = 0;
};

struct Model1 : Model { // concrete model (there would be more that this one)
	int n;

	virtual void   Start(int i) { n = i; }
	virtual int    Compute()    { return n++; }
};

CONSOLE_APP_MAIN
{
	Vector<int> data;
	data.SetCount(3000);
	
	// generate data using some model:

	CoWorkerResources<One<Model>> res;
	for(auto& r : res)
		r.Create<Model1>(); // all should be the same
	
	CoPartition(0, data.GetCount(), [&data, &res](int l, int h) {
		Model& m = *~res; // gets resource unique for worker
		m.Start(l);
		for(int i = l; i < h; i++)
			data[i] = m.Compute();
	});
	
	DUMP(data);
}
