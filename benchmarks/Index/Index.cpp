#include <Core/Core.h>
#include <set>

using namespace Upp;

CONSOLE_APP_MAIN
{
#ifdef _DEBUG
	const int v_num = 10000;
#else
	const int v_num = 10;
#endif

	const int isize = 1000000;

	{
		Vector<Index<int> > v;
		v.SetCount(v_num);
		{
			RTIMING("inner FindAdd");
			for (int i = 0; i < isize; ++i)
				for (int j = 0; j < v_num; ++j)
					v[j].FindAdd(i);
		}
		{
			RTIMING("inner UnlinkKey");
			for (int i = 0; i < isize; ++i)
				for (int j = 0; j < v_num; ++j)
					v[j].UnlinkKey(i);
		}
	//	RTIMING("inner Sweep");
	//	const int jsize = v_num;
	//	for (int j = 0; j < jsize; ++j)
	//		v[j].Sweep();
	}
	return;
	{
		Vector<Index<int> > v;
		v.SetCount(v_num);
		{
			RTIMING("outer FindAdd");
			for (int j = 0; j < v_num; ++j)
				for (int i = 0; i < isize; ++i)
					v[j].FindAdd(i);
		}
		{
			RTIMING("outer UnlinkKey");
			for (int j = 0; j < v_num; ++j)
				for (int i = 0; i < isize; ++i)
					v[j].UnlinkKey(i);
		}
	//	RTIMING("Sweep v_num outer");
	//	const int jsize = v_num;
	//	for (int j = 0; j < jsize; ++j)
	//		v[j].Sweep();
	}
	{
		std::set<int> *v = new std::set<int>[v_num];
		{
			RTIMING("outer insert");
			for (int j = 0; j < v_num; ++j)
				for (int i = 0; i < isize; ++i)
					v[j].insert(i);
		}
	
		{
			RTIMING("outer erase");
			for (int j = 0; j < v_num; ++j)
				for (int i = 0; i < isize; ++i)
					v[j].erase(i);
		}
		delete[] v;
	}

	{
		std::set<int> *v = new std::set<int>[v_num];
		{
			RTIMING("inner insert");
			for (int i = 0; i < isize; ++i)
				for (int j = 0; j < v_num; ++j)
					v[j].insert(i);
		}
	
		{
			RTIMING("inner erase");
			for (int i = 0; i < isize; ++i)
				for (int j = 0; j < v_num; ++j)
					v[j].erase(i);
		}
		delete[] v;
	}
}
