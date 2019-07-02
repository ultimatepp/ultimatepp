#include <Core/Core.h>
#include <set>

using namespace Upp;

CONSOLE_APP_MAIN
{
#ifdef _DEBUG
	const int v_num = 10000;
#else
	const int v_num = 1000000;
#endif

	const int isize = 10;
	const int N = 1;
	
	Vector<int> data;
	for(int i = 0; i < isize * v_num; i++)
		data.Add(Random());

	for(int ii = 0; ii < N; ii++) {
		{
			Vector<Index<int> > v;
			v.SetCount(v_num);
			{
				RTIMING("inner FindAdd");
				int *s = data;
				for (int i = 0; i < isize; ++i)
					for (int j = 0; j < v_num; ++j)
						v[j].FindAdd(*s++);
			}
			{
				RTIMING("inner UnlinkKey");
				int *s = data;
				for (int i = 0; i < isize; ++i)
					for (int j = 0; j < v_num; ++j)
						v[j].UnlinkKey(*s++);
			}
		//	RTIMING("inner Sweep");
		//	const int jsize = v_num;
		//	for (int j = 0; j < jsize; ++j)
		//		v[j].Sweep();
		}
		{
			Vector<Index<int> > v;
			v.SetCount(v_num);
			{
				RTIMING("outer FindAdd");
				int *s = data;
				for (int j = 0; j < v_num; ++j)
					for (int i = 0; i < isize; ++i)
						v[j].FindAdd(*s++);
			}
			{
				RTIMING("outer UnlinkKey");
				int *s = data;
				for (int j = 0; j < v_num; ++j)
					for (int i = 0; i < isize; ++i)
						v[j].UnlinkKey(*s++);
			}
		//	RTIMING("outer Sweep");
		//	const int jsize = v_num;
		//	for (int j = 0; j < jsize; ++j)
		//		v[j].Sweep();
		}
	
		{
			std::set<int> *v = new std::set<int>[v_num];
			{
				RTIMING("outer insert");
				int *s = data;
				for (int j = 0; j < v_num; ++j)
					for (int i = 0; i < isize; ++i)
						v[j].insert(*s++);
			}
		
			{
				RTIMING("outer erase");
				int *s = data;
				for (int j = 0; j < v_num; ++j)
					for (int i = 0; i < isize; ++i)
						v[j].erase(*s++);
			}
			delete[] v;
		}
	
		{
			std::set<int> *v = new std::set<int>[v_num];
			{
				RTIMING("inner insert");
				int *s = data;
				for (int i = 0; i < isize; ++i)
					for (int j = 0; j < v_num; ++j)
						v[j].insert(*s++);
			}
		
			{
				RTIMING("inner erase");
				int *s = data;
				for (int i = 0; i < isize; ++i)
					for (int j = 0; j < v_num; ++j)
						v[j].erase(*s++);
			}
			delete[] v;
		}
	}
}
