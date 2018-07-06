#include <Core/Core.h>

using namespace Upp;

void DumpFile(One<Stream>& s)
{
	LOG("=========");
	int ii = 0;
	while(!s->IsEof())
		LOG(++ii << ": " << s->GetLine());
}

CONSOLE_APP_MAIN
{
	String fn = GetDataFile("One.cpp");
	{
		One<Stream> s;
		s.Create<FileIn>(fn);
		DumpFile(s);
	}
	{
		One<Stream> s = MakeOne<FileIn>(fn);
		DumpFile(s);
	}
}
