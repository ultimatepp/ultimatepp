#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
#ifdef PLATFORM_WIN32
	FindFile ff("u:/client/*.*");
#else
	FindFile ff(AppendFileName(GetHomeDir(), "*.*"));
#endif
	while(ff) {
		LOG(ff.GetName());
		LOG(ff.GetPath());
		ff.Next();
	}
	LOG("================================================================");
	ff.Search("./*.*");
	while(ff) {
		LOG(ff.GetName());
		LOG(ff.GetPath());
		ff.Next();
	}
	LOG("================================================================");
	ff.Search("u:/client/zlib1.dll");
	while(ff) {
		LOG(ff.GetName());
		LOG(ff.GetPath());
		ff.Next();
	}
}
