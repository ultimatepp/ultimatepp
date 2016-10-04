//////////////////////////////////////////////////////////////////////
// smallprg: small (in terms of requirements) progress window.

#ifndef __INCLUDE_SMALLPRG_H__
#define __INCLUDE_SMALLPRG_H__

#include <RichText/RichText.h>
#include <plugin/bz2/bz2.h>

#define LAYOUTFILE <Installer/Installer.lay>
#include <CtrlCore/lay.h>

bool InstallDesktopIcon(const char *exe, const char *lnk, const char *desc);
bool InstallProgramGroup(const char *file, const char *group, const char *lnk, int icon = -1);
void InstallUninstall(const char *name, const char *dname, const char *cmdline);
void InstallUninstall(const char *exe, const char *name);

#endif//__INCLUDE_SMALLPRG_H__
