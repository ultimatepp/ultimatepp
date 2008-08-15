#ifndef _utils_h_
#define _utils_h_

// uninstall.cpp
String GetShellFolder(const char *name, HKEY type);
void DelKey(const char *dir, const char *key);
void RemoveWindowsItems();
void Uninstall();

#endif
