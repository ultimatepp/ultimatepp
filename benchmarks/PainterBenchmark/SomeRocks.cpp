#include "LionBenchmark.h"

void SomeRocks(Painter& sw)
{
	static Painting rocks;

	ONCELOCK {
		FileIn in(GetDataFile("SomeRocks.painting"));
		rocks.Serialize(in);
	}
	
	sw.EvenOdd();
	sw.Paint(rocks);
}