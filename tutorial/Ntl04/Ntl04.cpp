#include <Core/Core.h>

using namespace Upp;

struct Distribution : Moveable<Distribution> {
	String      text;
	Vector<int> data;
	
	rval_default(Distribution);
	Distribution() {}
};

Distribution CreateDist(int n)
{
	Distribution d;
	d.text << "Test (create) " << n;
	for(int i = 0; i < 10000; i++)
		d.data.At(rand() % n, 0)++;
	return pick(d);
}

void Dump(const Vector<Distribution>& dist)
{
	for(int i = 0; i < dist.GetCount(); i++) {
		DUMP(dist[i].text);
		DUMPC(dist[i].data);
	}
}

CONSOLE_APP_MAIN
{
	Vector<Distribution> dist;
	for(int n = 5; n <= 10; n++) {
		Distribution& d = dist.Add();
		d.text << "Test " << n;
		for(int i = 0; i < 10000; i++)
			d.data.At(rand() % n, 0)++;
	}
	Dump(dist);

	dist.Clear();
	for(int n = 5; n <= 10; n++)
		dist.AddPick(CreateDist(n));
	Dump(dist);

	dist.Clear();
	for(int n = 5; n <= 10; n++)
		dist.Add() = CreateDist(n);
	Dump(dist);
}
