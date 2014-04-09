#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	const Index<String>& h = GetMIMETypes();
	for(int i = 0; i < h.GetCount(); i++) {
		String ext = MIMEToFileExt(h[i]);
		String m2 = FileExtToMIME(ext);
		LOG(h[i] << " -> " << ext << ", " << ext << " -> " << m2);
		ASSERT(h[i] == m2);
	}
	
	LOG("============== OK");
}
