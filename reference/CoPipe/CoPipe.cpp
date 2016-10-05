#include <Core/Core.h>

using namespace Upp;

struct ReadIds {
	CoWork        co;
	Index<String> out;
	
	enum {
		PROCESSLINE,
		PROCESSID,
	};

	void Do(const char *path)
	{
		FileIn in(path);
		if(!in)
			return;
		while(!in.IsEof()) {
			String line = in.GetLine();
			co.Pipe(PROCESSLINE, [=] { SplitLine(line); });
		}
		co.Finish();
	}
	
	void SplitLine(const String& l)
	{
		const char *s = l;
		while(*s)
			if(IsAlpha(*s)) {
				const char *b = s++;
				while(IsAlNum(*s))
					s++;
				String w(b, s);
				co.Pipe(PROCESSID, [=] { ProcessId(w); });
			}
			else
				s++;
	}
	
	void ProcessId(const String& w)
	{
		out.FindAdd(w);
	}
};

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	String fn;
	int argc = CommandLine().GetCount();
	const Vector<String>& argv = CommandLine();
	if(argc < 1)
		fn = GetDataFile("CoPipe.cpp");
	else
		fn = argv[0];
	
	ReadIds h;
	h.Do(fn);
	
	LOG(h.out);
}
