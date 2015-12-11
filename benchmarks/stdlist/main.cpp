#include <CtrlLib/CtrlLib.h>

#include <list>
#include <vector>

using namespace Upp;

CONSOLE_APP_MAIN
{
	const int N = 20000;
	
	std::list<int> l;
	std::vector<int> v;
	Vector<int> u;

	for(int j = 0; j < 1000; j++) {
		{
			l.clear();
			RTIMING("list::push_back");
			for(int i = 0; i < N; i++)
				l.push_back(i);
		}
		{
			v.clear();
			RTIMING("vector::push_back");
			for(int i = 0; i < N; i++)
				v.push_back(i);
		}
		{
			u.clear();
			RTIMING("Vector::Add");
			for(int i = 0; i < N; i++)
				u.Add(i);
		}
	}
	
	for(int j = 0; j < 1000; j++) {
		{
			int sum = 0;
			{
				RTIMING("list iterate");
				for(const auto& i : l)
					sum += i;
			}
			RDUMP(sum);
		}
		{
			int sum = 0;
			{
				RTIMING("vector iterate");
				for(const auto& i : v)
					sum += i;
			}
			RDUMP(sum);
		}
		{
			int sum = 0;
			{
				RTIMING("Vector iterate");
				for(const auto& i : u)
					sum += i;
			}
			RDUMP(sum);
		}
	}
	
}
