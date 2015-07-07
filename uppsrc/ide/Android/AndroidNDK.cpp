#include "Android.h"

NAMESPACE_UPP

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

String AndroidNDK::FindDefaultToolchain() const
{
	Vector<String> toolchains = FindToolchains();
	Sort(toolchains, StdGreater<String>());
	
	return toolchains.GetCount() ? toolchains[toolchains.GetCount() - 1] : "";
}

String AndroidNDK::FindDefaultCppRuntime() const
{
	return "gnustl_shared";
}

Vector<String> AndroidNDK::FindToolchains() const
{
	Vector<String> toolchains;
	
	for(FindFile ff(AppendFileName(GetToolchainsDir(), "*")); ff; ff.Next()) {
		if(!ff.IsHidden() && ff.IsFolder()) {
			String name = ff.GetName();
			if(name.StartsWith("llvm"))
				continue;
			
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

END_UPP_NAMESPACE
