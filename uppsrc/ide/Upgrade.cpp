#include "ide.h"

void Ide::UpgradeTheIDE()
{
	String idepath = GetExeFilePath();
	String newpath = idepath + ".new";
	String bakpath(~idepath, GetFileExtPos(idepath));
	bakpath << ".bak.exe";
	
	int tbak = targetmode;
	String tmbak = StoreAsString(release);

	release.target_override = true;
	release.target = newpath;
#ifdef PLATFORM_WIN32
	release.linkmode = 0;
#else
	release.linkmode = 1;
#endif
	release.createmap = false;
	release.version.Clear();
	release.def.blitz = false;
	release.def.debug = false;
	release.package.Clear();
	
	targetmode = 1;

	Clean();
	if(Build()) {
		FileDelete(bakpath);
		if(FileExists(bakpath))
			Exclamation("Unable to delete&" + bakpath);
		else {
			PutVerbose("Moving " + idepath + " to " + bakpath);
			FileMove(idepath, bakpath);
			PutVerbose("Moving " + newpath + " to " + idepath);
			FileMove(newpath, idepath);
			PutConsole("Upgrade finished, please restart theide.");
		}
	}

	LoadFromString(release, tmbak);
	targetmode = tbak;
}
