#include <Core/Core.h>

void Main()
{
	for(int i = 0; i < 10000; i++)
		RDUMP(Uuid::Create());
	TimeStop();
	for(int i = 0; i < 1000000; i++) {
		Uuid::Create();
	}
	RDUMP(TimeStop());
}
