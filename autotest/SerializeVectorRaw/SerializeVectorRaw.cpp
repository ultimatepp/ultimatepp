#include <Core/Core.h>

using namespace Upp;

template <class T>
void Test(const char *name)
{
	Vector<T> v;
	for(int i = 0; i < 100000; i++)
		v.Add((T)i);
	Sha1Stream sha1;
	v.Serialize(sha1);
	DLOG(name << " " << sha1.FinishStringS());
	
	
	for(int i = 0; i < 100000; i++)
		v.Add((T)Random());
	
	StringStream mm;
	v.Serialize(mm);
	
	
	StringStream ss(mm.GetResult());
	Vector<T> vv;
	vv.Serialize(ss);
	
	for(int i = 0; i < v.GetCount(); i++)
		ASSERT(v[i] == vv[i]);
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Test<byte>("byte");
	Test<word>("word");
	Test<int>("int");
	Test<int64>("int64");
	Test<float>("float");
	Test<double>("double");
	
	CheckLogEtalon();
}
