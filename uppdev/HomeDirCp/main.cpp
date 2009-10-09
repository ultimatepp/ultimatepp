#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN {
	SetDefaultCharset(CHARSET_UTF8);

	const VectorMap<String, String>& map = Environment();
	for(int i = 0; i < map.GetCount(); i++)
		LOG(map.GetKey(i) << "=" << map[i]);
	
	PromptOK(GetHomeDirFile("test"));
	
	FileSel fs;
	fs.AllFilesType();
	fs.ExecuteOpen();
}
