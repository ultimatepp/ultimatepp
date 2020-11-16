#include <Core/Core.h>

#include <vector>

using namespace Upp;

CONSOLE_APP_MAIN
{
	const int N = 2000;
	
	Vector<String> data;
	Buffer<std::string> sdata(N);
	
	for(int i = 0; i < N; i++) {
		data << AsString(i);
		sdata[i] = data[i].ToStd();
	}
	
	RDUMP(sizeof(std::string));
	RDUMP(sizeof(String));
	
	for(int q = 1000; q--;) {
		{
			RTIMING("vector<string>::push_back");
			std::vector<std::string> v;
			for(int i = 0; i < N; i++)
				v.push_back(sdata[i]);
		}
		{
			RTIMING("vector<String>::push_back");
			std::vector<String> v;
			for(int i = 0; i < N; i++)
				v.push_back(data[i]);
		}
		{
			RTIMING("Vector<String>::Add");
			Vector<String> v;
			for(int i = 0; i < N; i++)
				v.Add(data[i]);
		}
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
		{
			std::vector<std::string> v;
			for(int i = 0; i < N; i++)
				v.insert(v.begin(), sdata[i]);
			RTIMING("vector<string>::erase");
			while(v.size())
				v.erase(v.begin());
		}
		{
			std::vector<String> v(~sdata, ~sdata + N);
			RTIMING("vector<String>::erase");
			while(v.size())
				v.erase(v.begin());
		}
		{
			Vector<String> v;
			for(int i = 0; i < N; i++)
				v.Insert(0, data[i]);
			RTIMING("Vector<String>::Remove");
			while(v.GetCount())
				v.Remove(0);
		}
		for(int j = 0; j < 100; j++) {
			{
				std::vector<std::string> v(~sdata, ~sdata + N);
				RTIMING("std::remove_if");
				v.erase(std::remove_if(v.begin(), v.end(), [](const std::string& h) { return h[0] == '1'; }), v.end());
			}
			{
				Vector<String> v = clone(data);
				RTIMING("Vector::RemoveIf");
				v.RemoveIf([&](int i) { return v[i][0] == '1'; });
			}
		}
	}
}
