#include "Updater.h"

#include <SysExec/SysExec.h>

NAMESPACE_UPP

#ifdef PLATFORM_POSIX

// scans for theme folders on which put/delete the mimetype icons
// that's needed because if themed icons aren't available, the system
// uses fallback ones instead of going through hicolor ones
// btw...that must be some ill-brained people there
// the routine builds a list of folders containing 32x32 icons
// it would be of course better with svg, but Upp don't support them
Vector<String>Updater::ScanTheme(void)
{
	// gathers all subfolders of /usr/share/icons that contains 32x32 subfolder
	Vector<String>subFolders;
	FindFile ff("/usr/share/icons/*");
	while(ff)
	{
		if(
			ff.IsFolder() &&
			ff.GetName() != "." &&
			ff.GetName() != ".." &&
			DirectoryExists("/usr/share/icons/" + ff.GetName() + "/32x32/")
		)
			subFolders.Add("/usr/share/icons/" + ff.GetName() + "/");
		ff.Next();
	}
	return subFolders;
}

// save an image as png inside multiple theme folders
// retrieved with ScanTheme
void Updater::InstallThemeIcons(Image const &img, String const &name)
{
	Vector<String>folders = ScanTheme();
	String imgs = PNGEncoder().SaveString(img);
	for(int i = 0; i < folders.GetCount(); i++)
	{
		SaveFile(folders[i] + "32x32/mimetypes/" + name + ".png", imgs);
		SysStart("gtk-update-icon-cache", "--quiet " + folders[i]);
	}
}

// deletes all icons with given name inside folders retrieved with ScanTheme
void Updater::RemoveThemeIcons(String const &name)
{
	Vector<String>folders = ScanTheme();
	for(int i = 0; i < folders.GetCount(); i++)
	{
		FileDelete(folders[i] + "32x32/mimetypes/" + name + ".png");
		SysStart("gtk-update-icon-cache", "--quiet " + folders[i]);
	}
}

// links application to OS shell
// (i.e., add icon, menu entry, mimetype....)
bool Updater::ShellLink(void)
{
	bool success = true;
	
	// build desktop file
	String desktop =
		"[Desktop Entry]\n"
		"Type=Application\n"
		"Encoding=UTF-8\n"
	;
	desktop += "Name=" + appName + "\n";
	if(comment != "")
		desktop += "Comment=" + comment + "\n";
	if(cathegory != "")
		desktop += "Categories=" + cathegory + "\n";
	desktop += "Exec=" + appName + " %f\n";
	if(!IsNull(icon))
		desktop += "Icon=" + appName + "\n";
	desktop += "MimeType=application/x-" + appName + ";\n";
	desktop += "Terminal=false\n";
	
	// stores desktop file in global applications folder
	success = SaveFile("/usr/share/applications/" + appName + ".desktop", desktop);
	
	// places icon in /usr/share/icons/hicolor/32x32/apps
	// maybe we should look for some better handling of icons...
	// by now, we support only 32x32 ones in default theme
	if(!IsNull(icon))
	{
		success &= PNGEncoder().SaveFile("/usr/share/icons/hicolor/32x32/apps/" + appName + ".png", icon);
		InstallThemeIcons(icon, "application-x-" + appName);
	}
	
	// add mime type for supported extensions
	String mime =
		"<?xml version=\"1.0\"?>\n"
		"<mime-info xmlns='http://www.freedesktop.org/standards/shared-mime-info'>\n"
			"<mime-type type=\"application/x-" + appName + "\">\n";
	if(comment != "")
		mime += "<comment>" + comment + "</comment>\n";
	for(int i = 0; i < extensions.GetCount(); i++)
		mime += "<glob pattern=\"" + extensions[i] + "\"/>\n";
	mime +=
			"</mime-type>\n"
		"</mime-info>\n"
	;
	success &= SaveFile("/usr/share/mime/packages/" + appName + ".xml", mime);
	success &= (SysStart("update-mime-database",  "/usr/share/mime") != -1);
	
	// updates desktop database to make it sense the changes in mimetype and application
	success &= (SysStart("update-desktop-database", "") != -1);
	
	return success;
}

// unlinks application
bool Updater::ShellUnlink(void)
{
	bool success = true;
	
	// remove desktop file
	FileDelete("/usr/share/applications/" + appName + ".desktop");
	
	// remove icon
	bool appIcon = FileExists("/usr/share/icons/hicolor/32x32/apps/" + appName + ".png");
	if(appIcon)
		FileDelete("/usr/share/icons/hicolor/32x32/apps/" + appName + ".png");
	RemoveThemeIcons("application-x-" + appName);

	if(FileExists("/usr/share/mime/packages/" + appName + ".xml"))
	{
		FileDelete("/usr/share/mime/packages/" + appName + ".xml");
		SysStart("update-mime-database",  "/usr/share/mime");
	}

	// updates desktop database to make it sense the changes in mimetype and application
	SysStart("update-desktop-database", "");
	
	return success;
}
		
#endif

END_UPP_NAMESPACE
