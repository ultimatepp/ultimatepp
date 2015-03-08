#include <CtrlLib/CtrlLib.h>

using namespace Upp;

static void sLoadImage(const String& path, Image& result)
{
	result = StreamRaster::LoadFileAny(path);
}

GUI_APP_MAIN
{
	FileSel fs;
	fs.AllFilesType();
	fs.WhenIconLazy = sLoadImage;
	fs.ExecuteOpen();
}
