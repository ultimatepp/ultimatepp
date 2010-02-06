#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	String path = "C:/Documents and Settings/cxl/Plocha";
	FindFile ff(AppendFileName(path, "*.*"));
	while(ff) {
		if(ff.IsSymLink()) {
			String fp = AppendFileName(path, ff.GetName());
			LOG(fp << " -> " << GetSymLinkPath(fp));
		}
		ff.Next();
	}
}

