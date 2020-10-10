#include "Android.h"

namespace Upp {

static const String TOOLCHAIN_CLANG = "clang";

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
	Index<String> toolchains(FindToolchains());
	
	int clangIdx = toolchains.Find(TOOLCHAIN_CLANG);
	if (clangIdx >= 0) {
		return toolchains[clangIdx];
	}
	
	SortIndex(toolchains, StdGreater<String>());
	return !toolchains.IsEmpty() ? toolchains[toolchains.GetCount()] : "";
}

String AndroidNDK::FindDefaultCppRuntime() const
{
	return "c++_shared";
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
				toolchains.Add(TOOLCHAIN_CLANG);
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
	
	// Values from NDK documentation
	runtimes.Add("c++_static");
	runtimes.Add("c++_shared");
	runtimes.Add("system");
	runtimes.Add("none");
	
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
	if(cppRuntime == "system") {
		return nest + "system" + DIR_SEPS + "include";
	}
	else
	if(cppRuntime.StartsWith("c++")) {
		return nest + "llvm-libc++" + DIR_SEPS + "include";
	}
	
	return "";
}

}
