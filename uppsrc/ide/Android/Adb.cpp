#include "Android.h"

NAMESPACE_UPP

Adb::Adb(const String& path) : path(path)
{
	
}

Adb::~Adb()
{
	
}

String Adb::MakeCmd() const
{
	String cmd = NormalizeExePath(path);
	if(!serial.IsEmpty())
		cmd << " -s " << serial;
	
	return cmd;
}

String Adb::MakeInstallCmd(const String& apkPath) const
{
	return NormalizeExePath(path) + " -s " + serial + " install -r " + apkPath;
}

String Adb::MakeInstallOnDefaultDeviceCmd(const String& apkPath) const
{
	return NormalizeExePath(path) + " -d install -r " + apkPath;
}

String Adb::MakeInstallOnDefaultEmulatorCmd(const String& apkPath) const
{
	return NormalizeExePath(path) + " -e install -r " + apkPath;
}

String Adb::MakeLaunchOnDeviceCmd(const String& packageName, const String& activityName) const
{
	String cmd = NormalizeExePath(path);
	cmd << " shell am start";
	cmd << " -n " << packageName << "/" << activityName;
	
	return cmd;
}

END_UPP_NAMESPACE
