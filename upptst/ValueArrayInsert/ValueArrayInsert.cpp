#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	ValueArray va;
	for(int i = 0; i < 10; i++)
		va.Add(i);
	LOG(va);
	
	ValueArray va2 = va;
	va2.Remove(3, 3);
	LOG(va2);
	
	va2.Insert(3, va2);
	LOG(va2);
	
	va2.Append(va);
	LOG(va2);
}

