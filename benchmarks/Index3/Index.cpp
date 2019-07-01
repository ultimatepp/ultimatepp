#include <Core/Core.h>
#include <set>

using namespace Upp;

CONSOLE_APP_MAIN
{
#ifdef _DEBUG
	const int v_num = 50000;
#else
	const int v_num = 5000;
#endif

	const int isize = 1000;
	
	Vector<String> data;
	for(int i = 0; i < isize; i++)
		data.Add(AsString(Random()));

	{
		Vector<Index<String> > v;
		v.SetCount(v_num);
		{
			RTIMING("FindAdd v_num outer");
			for (int j = 0; j < v_num; ++j)
				for (int i = 0; i < isize; ++i)
					v[j].FindAdd(data[i]);
		}
		{
			RTIMING("UnlinkKey v_num outer");
			for (int j = 0; j < v_num; ++j)
				for (int i = 0; i < isize; ++i)
					v[j].UnlinkKey(data[i]);
		}
		RTIMING("Sweep v_num outer");
		const int jsize = v_num;
		for (int j = 0; j < jsize; ++j)
			v[j].Sweep();
	}
	return;
	{
		Vector<Index<String> > v;
		v.SetCount(v_num);
		{
			RTIMING("FindAdd v_num inner");
			for (int i = 0; i < isize; ++i)
				for (int j = 0; j < v_num; ++j) {
					v[j].FindAdd(data[i]);
				}
		}
		{
			RTIMING("UnlinkKey v_num inner");
			for (int i = 0; i < isize; ++i)
				for (int j = 0; j < v_num; ++j)
					v[j].UnlinkKey(data[i]);
		}
		RTIMING("Sweep v_num inner");
		const int jsize = v_num;
		for (int j = 0; j < jsize; ++j)
			v[j].Sweep();
	}

	{
		std::set<String> *v = new std::set<String>[v_num];
		{
			RTIMING("insert v_num outer");
			for (int j = 0; j < v_num; ++j)
				for (int i = 0; i < isize; ++i)
					v[j].insert(data[i]);
		}
	
		{
			RTIMING("erase v_num outer");
			for (int j = 0; j < v_num; ++j)
				for (int i = 0; i < isize; ++i)
					v[j].erase(data[i]);
		}
		delete[] v;
	}

	{
		std::set<String> *v = new std::set<String>[v_num];
		{
			RTIMING("insert v_num inner");
			for (int i = 0; i < isize; ++i)
				for (int j = 0; j < v_num; ++j)
					v[j].insert(data[i]);
		}
	
		{
			RTIMING("erase v_num inner");
			for (int i = 0; i < isize; ++i)
				for (int j = 0; j < v_num; ++j)
					v[j].erase(data[i]);
		}
		delete[] v;
	}
}
