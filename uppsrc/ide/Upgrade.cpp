#include "ide.h"

void Ide::UpgradeTheIDE()
{
	String idepath, newpath, bakpath;
#ifdef PLATFORM_COCOA
	idepath = GetAppFolder();
	String name = GetFileTitle(idepath);
	idepath = GetFileFolder(idepath);
	String target_dir = idepath + "/" + name + ".new";
	DeleteFolderDeep(target_dir);
	RealizeDirectory(target_dir);

	newpath = target_dir + "/" + name + ".app";
	bakpath = idepath + "/" + name + ".bak.app";
	idepath << "/" << name << ".app";
#else
	idepath = GetExeFilePath();
	newpath = idepath + ".new";
	bakpath = String(~idepath, GetFileExtPos(idepath));
	bakpath << ".bak" << GetExeExt();
#endif

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
	release.def.debug = false;
	release.def.blitz = MapFlag(GetMethodVars(method), "RELEASE_BLITZ");

	targetmode = 1;

	if(!GetCtrl())
		Clean();

	if(Build()) {
		#ifdef PLATFORM_COCOA
			DeleteFolderDeep(bakpath);
		#else
			FileDelete(bakpath);
		#endif
		if(FileExists(bakpath))
			Exclamation("Unable to delete&" + bakpath);
		else {
			PutVerbose("Moving " + idepath + " to " + bakpath);
			FileMove(idepath, bakpath);
			PutVerbose("Moving " + newpath + " to " + idepath);
			FileMove(newpath, idepath);
		#ifdef PLATFORM_COCOA
			DeleteFolderDeep(target_dir);
		#endif
			PutConsole("Upgrade finished, please restart theide.");
		}
	}


	LoadFromString(release, tmbak);
	targetmode = tbak;
}

void Ide::InstallDesktop()
{
	String apps = GetHomeDirFile(".local/share/applications");
	if(PromptYesNo("Write theide.desktop to&[* \1" + apps + "\1]?&"
	               "That should make theide available through desktop menu.")) {
		String icon = ConfigFile("theide.png");
		PNGEncoder().SaveFile(icon, IdeImg::PackageLarge());
		String path = apps + "/theide.desktop";
		RealizePath(path);
		Upp::SaveFile(path,
			"[Desktop Entry]\n"
			"Encoding=UTF-8\n"
			"Name=TheIDE\n"
			"GenericName=TheIDE\n"
			"Comment=U++ IDE\n"
			"MimeType=application/x-upp;\n"
			"Exec=" + GetExeFilePath() + "\n"
			"Icon=" + icon + "\n"
			"Terminal=false\n"
			"Type=Application\n"
			"Categories=Development;IDE\n"
			"StartupNotify=false\n"
		);
	}
}