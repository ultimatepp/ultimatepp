#include "Core.h"

const Vector<BrewPackageInfo>& GetBrewPackages(Gate<int, int> progress)
{
	static Vector<BrewPackageInfo> pi;
	ONCELOCK {
		String ds = LoadFile(GetHomeDirFile(
	#ifdef PLATFORM_MACOS
			"Library/Caches"
	#else
			".cache"
	#endif
			"/Homebrew/descriptions.json"));
		if(ds.GetCount()) {
			Value v = ParseJSON(ds);
			if(IsValueMap(v)) {
				ValueMap m = v;
				for(int i = 0; i < m.GetCount(); i++) {
					BrewPackageInfo& r = pi.Add();
					r.name = m.GetKey(i);
					r.description = m.GetValue(i);
				}
			}
		}
		else {
		    HttpRequest http("https://formulae.brew.sh/api/formula.jws.json");
		    http.MaxContentSize(200000000);
			http.WhenWait = http.WhenDo = [&] {
				if(progress((int)http.GetContent().GetCount(), (int)http.GetContentLength()))
					http.Abort();
			};
			
			for(Value m : ParseJSON(~ParseJSON(http.Execute())["payload"])) {
				BrewPackageInfo& r = pi.Add();
				r.name = m["name"];
				r.description = m["desc"];
			}
		}
	}
	return pi;
}
