#include <RichText/RichText.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	TimeStop tm;

	StdLogSetup(LOG_COUT|LOG_FILE);

	for(FindFile ff(GetDataFile("*.qtf")); ff; ff.Next()) {
		String qtf = LoadFile(ff.GetPath());
		LOG(ff.GetName() << ' ' << qtf.GetCount());
		ASSERT(qtf.GetLength() > 10000);
		String qtf2 = AsQTF(ParseQTF(qtf));
		ASSERT(qtf2 == qtf);
	}
	
	LOG("========== OK in " << tm << " s");
}
