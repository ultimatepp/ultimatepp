#include <Core/Core.h>

#include <vector>

using namespace Upp;

CONSOLE_APP_MAIN
{
	const int N = 2500;
	
	Buffer<String> data(N);
	Buffer<std::string> sdata(N);
	
	for(int i = 0; i < N; i++) {
		data[i] = AsString(i);
		sdata[i] = data[i].ToStd();
	}
	
	RDUMP(sizeof(std::string));
	RDUMP(sizeof(String));
	
	for(int j = 0; j < N; j++) {
		{
			RTIMING("vector<string>::push_back");
			std::vector<std::string> v;
			for(int i = 0; i < j; i++)
				v.push_back(sdata[i]);
		}
		{
			RTIMING("vector<String>::push_back");
			std::vector<String> v;
			for(int i = 0; i < j; i++)
				v.push_back(data[i]);
		}
		{
			RTIMING("Vector<String>::Add");
			Vector<String> v;
			for(int i = 0; i < j; i++)
				v.Add(data[i]);
		}
	}
	
	for(int j = 0; j < 100; j++) {
		{
			RTIMING("vector<string>::insert");
			std::vector<std::string> v;
			for(int i = 0; i < N; i++)
				v.insert(v.begin(), sdata[i]);
		}
		{
			RTIMING("vector<String>::insert");
			std::vector<String> v;
			for(int i = 0; i < N; i++)
				v.insert(v.begin(), sdata[i]);
		}
		{
			RTIMING("Vector<String>::Insert");
			Vector<String> v;
			for(int i = 0; i < N; i++)
				v.Insert(0, data[i]);
		}
	}
}
