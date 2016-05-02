#include <Core/Core.h>

using namespace Upp;

Vector<String> GetStringData()
{
	Vector<String> data;
	int n = Random(100) * Random(100) * Random(10);
	for(int i = 0; i < n; i++)
		data.Add(AsString(Random(n)));
	return data;
}

Vector<int> GetIntData()
{
	Vector<int> data;
	int n = Random(100) * Random(100) * Random(10);
	for(int i = 0; i < n; i++)
		data.Add(Random(n));
	return data;
}

template <class C>
void CheckSorted(C& data)
{
	for(int i = 0; i < data.GetCount() - 1; i++) {
		if(!(data[i] <= data[i + 1])) {
			RDUMPC(data);
			RLOG("ERROR! " << i);				
			Exit(1);
		}
	}
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);
	TimeStop tm;
	while(tm.Seconds() < 150) {
		Vector<int> data = GetIntData();
		LOG("int " << data.GetCount());
		Sort(data);
		CheckSorted(data);
		Vector<String> sdata = GetStringData();
		LOG("String " << sdata.GetCount());
		Sort(sdata);
		CheckSorted(sdata);
		{
			Vector<int> data = GetIntData();
			LOG("int " << data.GetCount());
			StableSort(data);
			CheckSorted(data);
			Vector<String> sdata = GetStringData();
			LOG("String " << sdata.GetCount());
			StableSort(sdata);
			CheckSorted(sdata);
		}
	}
	LOG("============ OK " << tm);
}
