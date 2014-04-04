#include <Core/Core.h>

using namespace Upp;

#define LDUMP(x) DUMP(x)

template <class T>
void Check(T haystack, T needle, int pos)
{
	if(haystack.Find(needle) != pos) {
		DUMP(haystack);
		DUMP(needle);
		DUMP(haystack.Find(needle));
		NEVER();
	}
	if(haystack.Find(needle + '!') >= 0) {
		DUMP(haystack);
		DUMP(needle + '!');
		DUMP(haystack.Find(needle));
		NEVER();
	}
	if(haystack.ReverseFind(needle) != pos) {
		DUMP(haystack);
		DUMP(needle);
		DUMP(haystack.ReverseFind(needle));
		NEVER();
	}
}

template <class T>
void Check()
{
	int time0 = msecs();
	while(msecs(time0) < 60000) {
		int l = Random(40);
		int r = Random(40);
		LOG(l << ":" << r);
		for(int nl = 0; nl < 30; nl++)
			for(int nr = 0; nr < 30; nr++)
				for(int nc = 1; nc < 30; nc++) {
					T needle = T('a', nl) + T('x', nc) + T('a', nr);
					Check(T('a', l) + needle + T('a', r), needle, l);
					Check(T('b', l) + needle + T('c', r), needle, l);
				}
	}
	
	ASSERT(T("test").Find("x") < 0);
}

template <class T>
void CheckReplace()
{
	T x("Just a test");
	x.Replace(" a ", " another ");
	DUMP(x);
	ASSERT(x == T("Just another test"));
	
	x = T('_', 5) + x + T('_', 5) + x + T('_', 5);
	DUMP(x);
	x.Replace("test", "run");
	DUMP(x);
	T y("Just another run");
	ASSERT(x == T('_', 5) + y + T('_', 5) + y + T('_', 5));
}


CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	CheckReplace<String>();
	CheckReplace<WString>();					

	Check<String>();
	Check<WString>();					
	
	LOG("Everything OK");
}
