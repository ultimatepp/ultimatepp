#ifndef _Android_Android_h_
#define _Android_Android_h_

#include <Core/Core.h>
#include <ide/Core/Core.h>

#include "Executables.h"
#include "Devices.h"

namespace Upp {

class Android {
public:
	static String GetScriptExt();
	static String GetCmdExt();
	
	static void NormalizeVersions(Vector<String>& versions);
	static void RemoveVersionsNormalization(Vector<String>& versions);
	
private:
	static String FindVersionsPrefix(const Vector<String>& versions);
	static String FindLongestVersion(const Vector<String>& versions);
	
private:
	Android();
	Android(const Android&);
	Android& operator=(const Android&);
	virtual ~Android();
};

class AndroidSDK {
public:
	static String GetDownloadUrl();
	
public:
	AndroidSDK();
	AndroidSDK(const String& path, bool deduceValues = false);
	virtual ~AndroidSDK();
	
	void DeducePathReleatedValues();
	void DeducePlatform();
	void DeduceBuildToolsRelease();
	
	bool Validate() const;
	bool ValidatePlatform() const;
	bool ValidateBuildTools() const;
	
	Vector<String> FindPlatforms() const;
	Vector<String> FindBuildToolsReleases() const;
	String         FindDefaultPlatform() const;
	String         FindDefaultBuildToolsRelease() const;
	
	Vector<AndroidDevice>        FindDevices() const;
	Vector<AndroidVirtualDevice> FindVirtualDevices() const;
	
public:
	Adb MakeAdb() const { return Adb(AdbPath()); }
	
public:
	String BinDir() const;
	String BuildToolsDir() const;
	String PlatformsDir() const;
	String ConcreteBuildToolsDir() const;
	String ConcretePlatformDir() const;
	String PlatformToolsDir() const;
	String ToolsDir() const;
	
	bool HasD8() const;
	
	String AaptPath() const       { return ConcreteBuildToolsDir() + DIR_SEPS + "aapt" + GetExeExt(); }
	String ApksignerPath() const  { return ConcreteBuildToolsDir() + DIR_SEPS + "apksigner" + Android::GetScriptExt(); }
	String DxPath() const         { return ConcreteBuildToolsDir() + DIR_SEPS + "dx" + Android::GetScriptExt(); }
	String D8Path() const         { return ConcreteBuildToolsDir() + DIR_SEPS + "d8" + Android::GetScriptExt(); }
	String ZipalignPath() const   { return ConcreteBuildToolsDir() + DIR_SEPS + "zipalign" + GetExeExt(); }
	String AndroidJarPath() const { return ConcretePlatformDir() + DIR_SEPS + "android.jar"; }
	String AdbPath() const        { return PlatformToolsDir() + DIR_SEPS + "adb" + GetExeExt(); }
	String AndroidPath() const    { return ToolsDir() + DIR_SEPS + "android" + Android::GetScriptExt(); }
	String EmulatorPath() const   { return ToolsDir() + DIR_SEPS + "emulator" + GetExeExt(); }
	String MonitorPath() const    { return ToolsDir() + DIR_SEPS + "monitor" + Android::GetScriptExt(); }
	
public:
	String GetLauchSDKManagerCmd() const { return NormalizeExePath(AndroidPath()) + " sdk"; }
	String GetLauchAVDManagerCmd() const { return NormalizeExePath(AndroidPath()) + " avd"; }
	
public:
	String GetPath() const              { return this->path; }
	String GetPlatform() const          { return this->platform; }
	String GetBuildToolsRelease() const { return this->buildToolsRelease; }
	
	void SetPath(const String& path)                           { this->path = path; }
	void SetPlatform(const String& platform)                   { this->platform = platform; }
	void SetBuildToolsRelease(const String& buildToolsRelease) { this->buildToolsRelease = buildToolsRelease; }
	
private:
	String path;
	String platform;
	String buildToolsRelease;
};

class AndroidNDK {
public:
	static String GetDownloadUrl();
	
public:
	AndroidNDK();
	AndroidNDK(const String& path);
	virtual ~AndroidNDK();
	
	bool Validate() const;
	
	String FindDefaultPlatform() const;
	String FindDefaultToolchain() const;
	String FindDefaultCppRuntime() const;
	
	Vector<String> FindPlatforms() const;
	Vector<String> FindToolchains() const;
	Vector<String> FindCppRuntimes() const;
	
public:
	String GetIncludeDir() const;
	String GetCppIncludeDir(const String& cppRuntime) const;

	String GetPlatformsDir() const  { return path + DIR_SEPS + "platforms"; }
	String GetToolchainsDir() const { return path + DIR_SEPS + "toolchains"; }
	String GetSourcesDir() const    { return path + DIR_SEPS + "sources"; }
	
	String GetNdkBuildPath() const  { return path + DIR_SEPS + "ndk-build" + Android::GetCmdExt(); }
	String GetGdbPath() const       { return path + DIR_SEPS + "ndk-gdb"; }
	
public:
	String GetPath() const { return this->path; }
	
	void SetPath(const String& path) { this->path = path; }
		
private:
	String path;
};

class Apk {
public:
	Apk(const String& path, const AndroidSDK& androidSDK);
	virtual ~Apk();
	
	String FindPackageName() const;
	String FindLaunchableActivity() const;
	
private:
	String FindValueInAndroidManifest(const String& badge, const String& tag) const;
	String FindBadgeTagValue(const String& badge, const String& tag) const;
	
private:
	String path;
	AndroidSDK sdk;
};

class AndroidManifest {
public:
	AndroidManifest();
	virtual ~AndroidManifest();
	
private:
	
};

}

#endif
