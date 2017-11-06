#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Vector<bool> data;
	for(int i = 0; i < 1000; i++)
		data.Add(Random() & 1);
	
	int N = 100000;
	for(int k = 0; k < N; k++) {
		{
			RTIMING("Vector<bool>::At");
			Vector<bool> h;
			for(int j = 0; j < data.GetCount(); j++)
				h.At(j, false) = data[j];
		}
		{
			RTIMING("Vector<bool>::Add");
			Vector<bool> h;
			for(int j = 0; j < data.GetCount(); j++)
				h.Add(data[j]);
		}
		{
			RTIMING("Vector<bool>::At / Reserve");
			Vector<bool> h;
			h.Reserve(data.GetCount());
			for(int j = 0; j < data.GetCount(); j++)
				h.At(j, false) = data[j];
		}
		{
			RTIMING("Vector<bool>::Add / Reserve");
			Vector<bool> h;
			h.Reserve(data.GetCount());
			for(int j = 0; j < data.GetCount(); j++)
				h.Add(data[j]);
		}
		{
			RTIMING("Bits");
			Bits h;
			for(int j = 0; j < data.GetCount(); j++)
				h.Set(j, data[j]);
		}
		{
			RTIMING("Bits / Reserve");
			Bits h;
			h.Reserve(data.GetCount());
			for(int j = 0; j < data.GetCount(); j++)
				h.Set(j, data[j]);
		}
	}
}
