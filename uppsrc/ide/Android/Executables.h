#ifndef _ide_Android_Executable_h_
#define _ide_Android_Executable_h_

#include <Core/Core.h>

#include <ide/Core/Core.h>

NAMESPACE_UPP

class Adb : public Moveable<Adb> {
public:
	Adb(const String& path);
	virtual ~Adb();
	
public:
	String MakeListDevicesCmd() const;

	String MakeInstallCmd(const String& serial, const String& apkPath) const;
	String MakeInstallOnDeviceCmd(const String& apkPath) const;
	String MakeInstallOnEmulatorCmd(const String& apkPath) const;
	
private:
	String path;
	
};

class NDKBuild {
public:
	NDKBuild(const String& path);
	virtual ~NDKBuild();
	
	void SetJobs(int jobs)                       { this->jobs = jobs; }
	void SetWorkingDir(const String& workingDir) { this->workingDir = workingDir; }
	
	String ToString() const;
	
private:
	String path;
	String workingDir;
	int    jobs;
};

END_UPP_NAMESPACE

#endif
