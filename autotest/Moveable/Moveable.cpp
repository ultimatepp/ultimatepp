#include <Core/Core.h>

using namespace Upp;


namespace Upp {

//template <>
//inline constexpr bool is_upp_guest<std::string> = true;

};

template <>
inline constexpr bool Upp::is_upp_guest<std::string> = true;

template<> inline hash_t Upp::GetHashValue(const std::string& a)
{
	return memhash(a.data(), a.length());
}


CONSOLE_APP_MAIN
{
	{
		Vector<String> h;
		for(int i = 0; i < 20; i++)
			h << AsString(i);
		RDUMP(h);
		Vector<int> rem = { 1, 2, 3 };
		h.Remove(rem);
		RDUMP(h);
		h.RemoveIf([&](int i) { return h[i].EndsWith("8"); });
		RDUMP(h);
		Vector<String> n = { "21", "22", "23" };
		h.Insert(2, pick(n));
		RDUMP(h);
		h.Remove(2, 3);
		RDUMP(h);
	}
	RLOG("=======================");
	{
		Vector<std::string> h;
		for(int i = 0; i < 20; i++)
			h << AsString(i).ToStd();
		RDUMP(h);
		Vector<int> rem = { 1, 2, 3 };
		h.Remove(rem);
		RDUMP(h);
		h.RemoveIf([&](int i) { return h[i].back() == '8'; });
		RDUMP(h);
		Vector<std::string> n = { "21", "22", "23" };
		h.Insert(2, n);
		RDUMP(h);
		h.Insert(2, pick(n));
		RDUMP(h);
		h.Remove(2, 3);
		RDUMP(h);
	}
	{
		Index<std::string> x { "one", "two", "three" };
		RDUMP(x);
		RDUMP(x.Find("two"));
	}
	
	CheckLogEtalon();
}
