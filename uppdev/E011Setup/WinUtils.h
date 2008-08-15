#ifndef E011Setup_utils_h
#define E011Setup_utils_h

bool InstallDesktopIcon(const char *exe, const char *lnk, const char *desc);
bool InstallProgramGroup(const char *groupname, const char *exe, const char *args, const char *lnk, const char *desc, const char *fileicon, int icon = -1);
void InstallUninstall(const char *name, const char *dname, const char *cmdline);
String GetShellFolder(const char *name, HKEY type);
bool CreateShellLink(const char *filepath, const char *args, const char *linkpath, const char *desc, const char *fileicon, int icon);
void DelKey(const char *dir, const char *key);

#endif
