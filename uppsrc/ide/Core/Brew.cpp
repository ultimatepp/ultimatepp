#include "Core.h"

Value GetBrewJson(Gate<int, int> progress)
{
	String path = ConfigFile("brew.json.gz");
	static String data;
	if(data.GetCount() == 0) {
		if(FileExists(path) && FileGetTime(path) > GetSysTime() - 3600 * 24) // update daily
			data = GZDecompress(LoadFile(path));
		else {
		    HttpRequest http("https://formulae.brew.sh/api/formula.jws.json");
		    http.MaxContentSize(200000000);
			http.WhenWait = http.WhenDo = [&] {
				if(progress((int)http.GetContent().GetCount(), (int)http.GetContentLength()))
					http.Abort();
			};
		    data = ParseJSON(http.Execute())["payload"];
			if(http.IsError())
				return ErrorValue();
		    
		    SaveFile(path, GZCompress(data));
		}
	}

	return ParseJSON(data);
}