#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Index<String> m;
	String s = "hello";
	DUMP(s);
	m.Add(pick(s));
	DUMP(s);
	DUMP(m);
	ASSERT(s.GetCount() == 0);
	
	s = "alfa";
	int w = m.FindAdd(pick(s));
	DUMP(m);
	DUMP(w);
	DUMP(s);
	ASSERT(w == 1);
	ASSERT(s.GetCount() == 0);
	
	m.Unlink(0);
	s = "aaa";
	w = m.Put(pick(s));
	DUMP(m);
	DUMP(s);
	DUMP(w);
	ASSERT(w == 0);
	ASSERT(s.GetCount() == 0);
	ASSERT(m[0] == "aaa");
	
	m.Unlink(0);
	s = "alfa";
	ASSERT(m.FindPut(pick(s)) == 1);
	
	s = "beta";
	ASSERT(m.FindPut(pick(s)) == 0);
	ASSERT(s.GetCount() == 0);
	
	s = "gamma";
	m.Set(0, pick(s));
	ASSERT(s.GetCount() == 0);
	ASSERT(m[0] == "gamma");
	
	s = "delta";
	m << pick(s);
	DUMP(m);
	ASSERT(s.GetCount() == 0);
	
	LOG("============= OK");
}
