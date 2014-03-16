#include <Core/Core.h>

using namespace Upp;

void Do(Any& x)
{
	if(x.Is<String>())
		LOG("String: " << x.Get<String>());
	if(x.Is<FileIn>()) {
		LOG("--- File: ");
		LOG(LoadStream(x.Get<FileIn>()));
		LOG("----------");
	}
	if(x.IsEmpty())
		LOG("empty");
}

CONSOLE_APP_MAIN
{
	Any x;
	x.Create<String>() = "Hello!";
	Do(x);
	x.Create<FileIn>().Open(GetDataFile("Ntl12.cpp"));
	Do(x);
	x.Clear();
	Do(x);
}
