#include <Draw/Draw.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	RDUMP(sizeof(bool));
	RDUMP(sizeof(int));
	RDUMP(sizeof(unsigned));
	RDUMP(sizeof(long int));
	RDUMP(sizeof(long));
	RDUMP(sizeof(unsigned long));
	RDUMP(sizeof(short int));
	RDUMP(sizeof(byte));
	RDUMP(sizeof(word));
	RDUMP(sizeof(dword));
	RDUMP(sizeof(qword));
	RDUMP(sizeof(int64));
	RDUMP(sizeof(uint64));
	RDUMP(sizeof(float));
	RDUMP(sizeof(double));
	RDUMP(sizeof(long double));
	RLOG("========================");
	RDUMP(sizeof(Date));
	RDUMP(sizeof(Time));
	RDUMP(sizeof(Size));
	RDUMP(sizeof(Point));
	RDUMP(sizeof(Rect));
	RDUMP(sizeof(Color));
	RDUMP(sizeof(Complex));
	RDUMP(sizeof(Uuid));
	RDUMP(sizeof(Mutex));
	RDUMP(sizeof(RWMutex));
	RLOG("========================");
	RDUMP(sizeof(String));
	RDUMP(sizeof(WString));
	RDUMP(sizeof(Value));
	RDUMP(sizeof(ValueArray));
	RDUMP(sizeof(ValueMap));
	RLOG("========================");
	RDUMP(sizeof(Vector<int>));
	RDUMP(sizeof(Array<int>));
	RDUMP(sizeof(Index<int>));
	RDUMP(sizeof(ArrayIndex<int>));
	RDUMP(sizeof(ArrayMap<int, int>));
	RDUMP(sizeof(VectorMap<int, int>));
	RDUMP(sizeof(BiArray<int>));
	RDUMP(sizeof(BiVector<int>));
	RDUMP(sizeof(InVector<int>));
	RDUMP(sizeof(InArray<int>));
	RDUMP(sizeof(SortedIndex<int>));
	RDUMP(sizeof(SortedVectorMap<int, int>));
	RDUMP(sizeof(SortedArrayMap<int, int>));
	RLOG("========================");
	RDUMP(sizeof(Image));
	RDUMP(sizeof(Font));
	RDUMP(sizeof(AttrText));
	RDUMP(sizeof(Painting));
	RDUMP(sizeof(Drawing));
}
