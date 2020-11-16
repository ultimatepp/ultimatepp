#include <Core/Core.h>
#include <TestData/TestData.h>
#include <set>
#include <unordered_set>
#include <vector>

using namespace Upp;

namespace std {
  template<> struct hash<String>
  {
    std::size_t operator()(const String& k) const
    {
        return GetHashValue(k);
    }
  };
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Vector<String> w = AliceWords();
	
	std::vector<std::string> q;
	
	for(auto s : w)
		q.push_back(s.ToStd());
	
	for(int i = 0; i < 100; i++) {
		Index<String> ndx;
		SortedIndex<String> ndx2;
		std::set<std::string> st;
		std::set<String> sst;
		std::unordered_set<std::string> hst;
		std::unordered_set<String> hsst;
		{
			RTIMING("Index");
			for(const String& s : w)
				ndx.FindAdd(s);
		}
		{
			RTIMING("SortedIndex");
			for(const String& s : w)
				ndx2.FindAdd(s);
		}
		{
			RTIMING("set");
			for(const std::string& s : q)
				st.insert(s);
		}
		{
			RTIMING("set String");
			for(const String& s : w)
				sst.insert(s);
		}
		{
			RTIMING("unordered_set");
			for(const std::string& s : q)
				hst.insert(s);
		}
		{
			RTIMING("unordered_set String");
			for(const String& s : w)
				hsst.insert(s);
		}
		ONCELOCK {
			RDUMP(ndx.GetCount());
			RDUMP(ndx2.GetCount());
			RDUMP(st.size());
			RDUMP(sst.size());
			RDUMP(hst.size());
		}
	}
	return;
	
	w = AliceLines();
	
	q.clear();
	for(auto s : w)
		q.push_back(s.ToStd());
	
	for(int i = 0; i < 100; i++) {
		Index<String> ndx;
		SortedIndex<String> ndx2;
		std::set<std::string> st;
		std::set<String> sst;
		std::unordered_set<std::string> hst;
		std::unordered_set<String> hsst;
		{
			RTIMING("Index");
			for(const String& s : w)
				ndx.FindAdd(s);
		}
		{
			RTIMING("SortedIndex");
			for(const String& s : w)
				ndx2.FindAdd(s);
		}
		{
			RTIMING("set");
			for(const std::string& s : q)
				st.insert(s);
		}
		{
			RTIMING("set String");
			for(const String& s : w)
				sst.insert(s);
		}
		{
			RTIMING("unordered_set");
			for(const std::string& s : q)
				hst.insert(s);
		}
		{
			RTIMING("unordered_set String");
			for(const String& s : w)
				hsst.insert(s);
		}
		ONCELOCK {
			RDUMP(ndx.GetCount());
			RDUMP(ndx2.GetCount());
			RDUMP(st.size());
			RDUMP(sst.size());
			RDUMP(hst.size());
		}
	}
}
