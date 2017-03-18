#ifndef _ide_Android_Executable_h_
#define _ide_Android_Executable_h_

#include <Core/Core.h>

#include <ide/Core/Core.h>

namespace Upp {

class LogCat;

class Adb : public Moveable<Adb> {
public:
	Adb(const String& path);
	virtual ~Adb() {}
	
public:
	String GetPath() const               { return this->path; }
	String GetSerial() const             { return this->serial; }
	
	void SetPath(const String& path)     { this->path = path; }
	void SetSerial(const String& serial) { this->serial = serial; }
	
public:
	int GetPid(const String& packageName) const;
	
public:
	String MakeListDevicesCmd() const;
	
	String MakeCmd() const;
	
	String MakeInstallCmd(const String& apkPath) const;
	String MakeInstallOnDefaultDeviceCmd(const String& apkPath) const;
	String MakeInstallOnDefaultEmulatorCmd(const String& apkPath) const;
	
	String MakeLaunchOnDeviceCmd(const String& packageName, const String& activityName) const;
	
	String MakeGetAllProcessesCmd() const;
	
private:
	String path;
	String serial;
};

class LogCat : public Moveable<LogCat> {
public:
	LogCat();
	virtual ~LogCat();

public:
	String MakeCmdByTag(const String& packageName) const;
	String MakeCmdByTag(const Adb& adb, const String& packageName) const;
	
private:
	
};

class NDKBuild {
public:
	NDKBuild(const String& path);
	virtual ~NDKBuild() {}
	
	void SetJobs(int jobs)                       { this->jobs = jobs; }
	void SetWorkingDir(const String& workingDir) { this->workingDir = workingDir; }
	void EnableVerbose(bool verbose = true)      { this->verbose = verbose; }
	
	String MakeCmd() const;
	
private:
	String path;
	String workingDir;
	int    jobs;
	bool   verbose;
};

}

#endif
