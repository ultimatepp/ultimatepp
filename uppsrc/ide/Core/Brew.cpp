#include "Core.h"

Value GetBrewJson(Gate<int, int> progress)
{
	String path = ConfigFile("brew.json.gz");
	static Value json;
	if(IsNull(json)) {
		if(FileExists(path) && FileGetTime(path) > GetSysTime() - 3600 * 24) // update daily
			json = ParseJSON(GZDecompress(LoadFile(path)));
		else {
		    HttpRequest http("https://formulae.brew.sh/api/formula.jws.json");
		    http.MaxContentSize(200000000);
			http.WhenWait = http.WhenDo = [&] {
				if(progress((int)http.GetContent().GetCount(), (int)http.GetContentLength()))
					http.Abort();
			};
		    String data = ParseJSON(http.Execute())["payload"];
			if(http.IsError())
				return ErrorValue();

		    data = ~ParseJSON(http.Execute())["payload"];
		    
		    SaveFile(path, GZCompress(data));
		    json = ParseJSON(data);
		}
	}

	return json;

}