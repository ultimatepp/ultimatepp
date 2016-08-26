#include "Android.h"

namespace Upp {

String AndroidNDK::GetDownloadUrl()
{
	return "https://developer.android.com/ndk/downloads/index.html";
}

AndroidNDK::AndroidNDK()
{
	
}

AndroidNDK::AndroidNDK(const String& path)
{
	this->path = path;
}

AndroidNDK::~AndroidNDK()
{
	
}

bool AndroidNDK::Validate() const
{
	if(!DirectoryExists(path) || !FileExists(GetNdkBuildPath()))
		return false;
	
	return true;
}

String AndroidNDK::FindDefaultPlatform() const
{
	Vector<String> platforms = FindPlatforms();
	
	Android::NormalizeVersions(platforms);
	Sort(platforms, StdGreater<String>());
	Android::RemoveVersionsNormalization(platforms);
	
	return !platforms.IsEmpty() ? platforms[0] : "";
}

String AndroidNDK::FindDefaultToolchain() const
{
	Vector<String> toolchains = FindToolchains();
	Sort(toolchains, StdGreater<String>());
	
	return toolchains[toolchains.GetCount() - 1];
}

String AndroidNDK::FindDefaultCppRuntime() const
{
	return "gnustl_shared";
}

Vector<String> AndroidNDK::FindPlatforms() const
{
	Vector<String> platforms;
	
	for(FindFile ff(AppendFileName(GetPlatformsDir(), "*")); ff; ff.Next()) {
		if(!ff.IsHidden() && ff.IsFolder())
			platforms.Add(ff.GetName());
	}
	
	return platforms;
}

Vector<String> AndroidNDK::FindToolchains() const
{
	Vector<String> toolchains;
	
	for(FindFile ff(AppendFileName(GetToolchainsDir(), "*")); ff; ff.Next()) {
		if(!ff.IsHidden() && ff.IsFolder()) {
			String name = ff.GetName();
			if(name.StartsWith("llvm")) {
				toolchains.Add("clang");
				continue;
			}
			
			Vector<String> nameParts = Split(name, "-");
			if(nameParts.GetCount() > 1) {
				String toolchain = nameParts[nameParts.GetCount() - 1];
				if(FindIndex(toolchains, toolchain) == -1)
					toolchains.Add(toolchain);
			}
		}
	}

	return toolchains;
}

Vector<String> AndroidNDK::FindCppRuntimes() const
{
	Vector<String> runtimes;
	
	// Values from ndk documentation
	runtimes.Add("system");
	runtimes.Add("gabi++_static");
	runtimes.Add("gabi++_shared");
	runtimes.Add("stlport_static");
	runtimes.Add("stlport_shared");
	runtimes.Add("gnustl_static");
	runtimes.Add("gnustl_shared");
	runtimes.Add("c++_static");
	runtimes.Add("c++_shared");
	
	return runtimes;
}

String AndroidNDK::GetIncludeDir() const
{
	String dir;
	dir << GetPlatformsDir() << DIR_SEPS << FindDefaultPlatform() << DIR_SEPS;
	// TODO: decide how to implement architecture selection.
	dir << "arch-arm" << DIR_SEPS;
	dir << "usr" << DIR_SEPS << "include";
	
	return DirectoryExists(dir) ? dir : "";
}

String AndroidNDK::GetCppIncludeDir(const String& cppRuntime) const
{
	String nest = GetSourcesDir() + DIR_SEPS + "cxx-stl" + DIR_SEPS;
	if(cppRuntime == "system")
		return nest + "system" + DIR_SEPS + "include";
	else
	if(cppRuntime.StartsWith("gabi++"))
		return nest + "gabi++" + DIR_SEPS + "include";
	else
	if(cppRuntime.StartsWith("stlport"))
		return nest + "stlport" + DIR_SEPS + "stlport";
	else
	if(cppRuntime.StartsWith("gnustl")) {
		// TODO: implement selection of library version
		String versionsDir = nest + "gnu-libstdc++";
		
		FindFile ff(nest + "gnu-libstdc++/*.*");
		String inc;
		double ver = 0;
		while(ff) {
			if(ff.IsFolder()) {
				double h = atof(ff.GetName());
				if(h > ver) {
					ver = h;
					inc = ff.GetPath();
				}
			}
			ff.Next();
		}
		return inc + '/' + "include";
	}
	else
	if(cppRuntime.StartsWith("c++")) {
		return nest + "llvm-libc++" + DIR_SEPS + "libcxx" + DIR_SEPS + "include";
	}
	
	return "";
}

}
