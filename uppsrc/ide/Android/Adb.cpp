#include "Android.h"

namespace Upp {

Adb::Adb(const String& path)
	: path(path)
{
}

int Adb::GetPid(const String& packageName) const
{
	int pid = -1;
	
	String out;
	if(Sys(MakeGetAllProcessesCmd(), out) == 0) {
		Vector<String> lines = Split(out, "\n");
		for(int i = 0; i < lines.GetCount(); i++) {
			if(lines[i].Find(packageName) >= 0) {
				Vector<String> parts = Split(lines[i], " ");
				if(parts.GetCount() >= 2)
					pid = StrInt(parts[1]);
			}
		}
	}
	
	return pid;
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
	return MakeCmd() + " install -r " + apkPath;
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

String Adb::MakeGetAllProcessesCmd() const
{
	return MakeCmd() + " shell ps";
}

}
