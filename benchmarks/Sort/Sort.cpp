#include <Core/Core.h>
#include <TestData/TestData.h>

#include <algorithm>
#include <vector>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	RDUMP(AliceWords().GetCount());
	for(int i = 0; i < 300; i++) {
		Vector<String> w = AliceWords();
		std::vector<std::string> x;
		{
			for(auto s : w)
				x.push_back(s.ToStd());
			RTIMING("std::sort std::vector<std::string>");
			std::sort(x.begin(), x.end());
		}
		{
			std::vector<std::string> x;
			for(auto s : w)
				x.push_back(s.ToStd());
			RTIMING("std::stable_sort std::vector<std::string>");
			std::stable_sort(x.begin(), x.end());
		}
		Vector<String> w2 = clone(w);
		{
			RTIMING("Sort Vector<String>");
			Sort(w2);
		}
		
		for(int i = 0; i < w2.GetCount(); i++)
			if(w2[i] != x[i])
				Panic("Failed!");
/*		{
			RTIMING("Sort Vector<String>");
			Sort(w);
		}
*/	#if 0
		ONCELOCK {
			RDUMPC(w);
		}
	#endif
	}

	for(int i = 0; i < 300; i++) {
		Vector<int> w;
		for(int i = 0; i < 40000; i++)
			w.Add(Random());
		{
			std::vector<int> x(w.begin(), w.end());
			RTIMING("std::sort std::vector<int>");
			std::sort(x.begin(), x.end());
		}
		{
			std::vector<int> x(w.begin(), w.end());
			RTIMING("std::sort std::vector<int>");
			std::sort(x.begin(), x.end());
		}
		{
			RTIMING("Sort Vector<int>");
			Sort(w);
		}
	#if 0
		ONCELOCK {
			RDUMPC(w);
		}
	#endif
	}
}
