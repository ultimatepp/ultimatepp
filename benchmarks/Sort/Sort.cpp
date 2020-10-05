#include <Core/Core.h>
#include <TestData/TestData.h>

#include <algorithm>
#include <vector>

using namespace Upp;

CONSOLE_APP_MAIN
{
//	StdLogSetup(LOG_COUT|LOG_FILE);
	for(int i = 0; i < 1000; i++) {
		Vector<String> w = AliceWords();	
		{
			std::vector<std::string> x;
			for(auto s : w)
				x.push_back(s.ToStd());
			RTIMING("std::sort std::vector<std::string>");
			std::sort(x.begin(), x.end());
		}
		{
			RTIMING("Sort Vector<String>");
			Sort(w);
		}
		ONCELOCK {
			RDUMPC(w);
		}
	}
}
