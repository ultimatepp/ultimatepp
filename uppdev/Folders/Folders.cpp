#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	DUMP(GetDesktopFolder());
	DUMP(GetProgramsFolder());
	DUMP(GetAppDataFolder());
	DUMP(GetMusicFolder());
	DUMP(GetPicturesFolder());
	DUMP(GetVideoFolder());
	DUMP(GetDocumentsFolder());
	DUMP(GetTemplatesFolder());
	DUMP(GetDownloadFolder());
}

