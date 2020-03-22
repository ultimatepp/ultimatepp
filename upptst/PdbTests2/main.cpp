#include <Core/Core.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <forward_list>
#include <deque>
#include <unordered_set>
#include <unordered_map>

using namespace Upp;

CONSOLE_APP_MAIN
{
//	std::set<int> s;
	std::unordered_map<int, std::string> l0;
	std::unordered_map<int, std::string> l;

	for(int i = 0; i < 5; i++)
		l[i] = AsString(i).ToStd();
	
	int n = l.size();
	for(auto i : l) {
		;
	}
}
