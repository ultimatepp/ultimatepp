#include <CtrlLib/CtrlLib.h>

#include <list>
#include <vector>

using namespace Upp;

int sum = 0;

CONSOLE_APP_MAIN
{
	const int N = 20000;
	
	
	RDUMP(sizeof(std::string));
	RDUMP(sizeof(std::wstring));

	for(int j = 0; j < 10000; j++) {
		std::list<int> l;
		std::vector<int> v;
		Vector<int> u;
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

		{
			{
				RTIMING("list iterate");
				for(const auto& i : l)
					sum += i;
			}
		}
		{
			{
				RTIMING("vector iterate");
				for(const auto& i : v)
					sum += i;
			}
		}
		{
			{
				RTIMING("Vector iterate");
				for(const auto& i : u)
					sum += i;
			}
		}
	}
}
