#include <Core/Core.h>
#include <plugin/tess2/tess2.h>

using namespace Upp;

Vector<Vector<Pointf>> TestData();

int N = 100;

CONSOLE_APP_MAIN
{
	auto data = TestData();
	
	InVector<double> y;
	for(auto& h : data)
		for(Pointf p : h)
			y.InsertUpperBound(p.y);
	RDUMPC(y);
	
	for(int i = 0; i < N; i++) {
		Vector<Pointf> vertex;
		Vector<int> index;
		{
			RTIMING("Tesselate");
			Tesselate(data, vertex, index, true);
		}
	}
}
