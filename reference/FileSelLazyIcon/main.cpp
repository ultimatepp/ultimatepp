#include <CtrlLib/CtrlLib.h>
#include <plugin/jpg/jpg.h>

using namespace Upp;

GUI_APP_MAIN
{
	FileSel fs;
	fs.AllFilesType();
	fs.WhenIconLazy = [](const String& path, Image& result) {
		result = StreamRaster::LoadFileAny(path);
	};
	fs.ExecuteOpen();
}
