#include <Core/Core.h>
#include <TestData/TestData.h>
#include <set>
#include <unordered_set>
#include <vector>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Vector<String> w = AliceWords();
	
	std::vector<std::string> q;
	
	for(auto s : w)
		q.push_back(s.ToStd());
	
	for(int i = 0; i < 100; i++) {
		Index<String> ndx;
		std::set<std::string> st;
		std::set<std::string> hst;
		{
			RTIMING("Index");
			for(const String& s : w)
				ndx.FindAdd(s);
		}
		{
			RTIMING("set");
			for(const std::string& s : q)
				st.insert(s);
		}
		{
			RTIMING("unordered_set");
			for(const std::string& s : q)
				hst.insert(s);
		}
		ONCELOCK {
			RDUMP(ndx.GetCount());
			RDUMP(st.size());
			RDUMP(hst.size());
		}
	}
}
