#include <Core/Core.h>

using namespace Upp;

int N = 10;

CONSOLE_APP_MAIN
{
	Vector<float> iv;
	for(int i = 0; i < 11000; i++)
		iv.Add((float)Randomf());
	
	InVector<float> y, yb[100];
	Vector<float> vy;
	for(int i = 0; i < N; i++) {
		{
			y.Clear();
			RTIMING("Insert");
			for(float v : iv)
				y.InsertUpperBound(v);
		}
		{
			vy.Clear();
			RTIMING("Insert2");
			for(float v : iv)
				vy.Insert(FindUpperBound(vy, v), v);
		}
	}
	RDUMP(y);
	RDUMP(vy);
}
