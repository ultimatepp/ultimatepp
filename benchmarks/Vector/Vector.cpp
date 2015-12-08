#include <Core/Core.h>

#include <vector>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Buffer<String> data(2000);
	Buffer<std::string> sdata(2000);
	
	for(int i = 0; i < 2000; i++) {
		data[i] = AsString(i);
		sdata[i] = data[i].ToStd();
	}
	
	for(int j = 0; j < 10000; j++) {
		{
			RTIMING("vector<string>::push_back");
			std::vector<std::string> v;
			for(int i = 0; i < 2000; i++)
				v.push_back(sdata[i]);
		}
		{
			RTIMING("vector<String>::push_back");
			std::vector<String> v;
			for(int i = 0; i < 2000; i++)
				v.push_back(data[i]);
		}
		{
			RTIMING("Vector<String>::Add");
			Vector<String> v;
			for(int i = 0; i < 2000; i++)
				v.Add(data[i]);
		}
	}

	for(int j = 0; j < 100; j++) {
		{
			RTIMING("vector<string>::insert");
			std::vector<std::string> v;
			for(int i = 0; i < 2000; i++)
				v.insert(v.begin(), sdata[i]);
		}
		{
			RTIMING("vector<String>::insert");
			std::vector<String> v;
			for(int i = 0; i < 2000; i++)
				v.insert(v.begin(), sdata[i]);
		}
		{
			RTIMING("Vector<String>::Insert");
			Vector<String> v;
			for(int i = 0; i < 2000; i++)
				v.Insert(0, data[i]);
		}
	}
}
