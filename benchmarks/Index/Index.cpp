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
			RTIMING("FindAdd v_num outer");
			for (int j = 0; j < v_num; ++j)
				for (int i = 0; i < isize; ++i)
					v[j].FindAdd(i);
		}
		{
			RTIMING("UnlinkKey v_num outer");
			for (int j = 0; j < v_num; ++j)
				for (int i = 0; i < isize; ++i)
					v[j].UnlinkKey(i);
		}
		RTIMING("Sweep v_num outer");
		const int jsize = v_num;
		for (int j = 0; j < jsize; ++j)
			v[j].Sweep();
	}
	{
		Vector<Index<int> > v;
		v.SetCount(v_num);
		{
			RTIMING("FindAdd v_num inner");
			for (int i = 0; i < isize; ++i)
				for (int j = 0; j < v_num; ++j)
					v[j].FindAdd(i);
		}
		{
			RTIMING("UnlinkKey v_num inner");
			for (int i = 0; i < isize; ++i)
				for (int j = 0; j < v_num; ++j)
					v[j].UnlinkKey(i);
		}
		RTIMING("Sweep v_num inner");
		const int jsize = v_num;
		for (int j = 0; j < jsize; ++j)
			v[j].Sweep();
	}

	{
		std::set<int> *v = new std::set<int>[v_num];
		{
			RTIMING("insert v_num outer");
			for (int j = 0; j < v_num; ++j)
				for (int i = 0; i < isize; ++i)
					v[j].insert(i);
		}
	
		{
			RTIMING("erase v_num outer");
			for (int j = 0; j < v_num; ++j)
				for (int i = 0; i < isize; ++i)
					v[j].erase(i);
		}
		delete[] v;
	}

	{
		std::set<int> *v = new std::set<int>[v_num];
		{
			RTIMING("insert v_num inner");
			for (int i = 0; i < isize; ++i)
				for (int j = 0; j < v_num; ++j)
					v[j].insert(i);
		}
	
		{
			RTIMING("erase v_num inner");
			for (int i = 0; i < isize; ++i)
				for (int j = 0; j < v_num; ++j)
					v[j].erase(i);
		}
		delete[] v;
	}
}
