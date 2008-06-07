#include <Core/Core.h>
#include <list>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Array< std::list<int> > al;
	for(int i = 0; i < 4; i++) {
		std::list<int>& l = al.Add();
		for(int q = 0; q < i; q++)
			l.push_back(q);
	}

	for(int i = 0; i < al.GetCount(); i++) {
		const std::list<int>& l = al[i];
		for(std::list<int>::const_iterator it = l.begin(); it != l.end(); it++)
			LOG(*it);
		LOG("------");
	}
}
