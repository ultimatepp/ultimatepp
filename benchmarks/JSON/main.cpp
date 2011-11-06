#include "JSON.h"

#ifdef _DEBUG
#define N 1
#else
#define N 10000
#endif

CONSOLE_APP_MAIN
{
	String j0, j1, j2;
	for(int i = 0; i < N; i++) {
		RTIMING("Automated");
		JsonArray a;
		for(int i = 0; i < 100; i++)
			a << Json("first", i)("second", AsString(i));
		j0 = a;
	}
	RDUMP(j0);
	for(int i = 0; i < N; i++) {
		RTIMING("SemiOptimized");
		String s = "[";
		for(int i = 0; i < 100; i++) {
			if(i)
				s << ',';
			s << '{' << AsCString("first") << ':' << i << ',' << AsCString("second") << ':' << AsCString(AsString(i)) << '}';
		}
		s << ']';
		j2 = s;
	}
	RDUMP(j2);
	for(int i = 0; i < N; i++) {
		RTIMING("Optimized");
		String s = "[";
		for(int i = 0; i < 100; i++) {
			if(i)
				s << ',';
			s << "{\"first\":" << i << ",\"second\":" << AsCString(AsString(i)) << '}';
		}
		s << ']';
		j1 = s;
	}
	RDUMP(j1);
	RDUMP(j1 == j0 && j2 == j0);
	
	RDUMP(AsJSON(ParseJSON(j1), true));
}
