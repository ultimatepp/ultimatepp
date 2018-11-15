#include "LionBenchmark.h"

void SomeRocks(Painter& sw)
{
	static Painting rocks;

	ONCELOCK {
		rocks.Serialize(FileIn(GetDataFile("SomeRocks.painting")));
	}
	
	sw.EvenOdd();
	sw.Paint(rocks);
}