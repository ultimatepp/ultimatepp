#include "Android.h"

#include <plugin/pcre/Pcre.h>

namespace Upp {

String AndroidSDK::GetDownloadUrl()
{
	return "https://developer.android.com/studio/index.html";
}

AndroidSDK::AndroidSDK()
{
}

AndroidSDK::AndroidSDK(const String& path, bool deduceValues)
{
	this->path = path;
	if(deduceValues)
		DeducePathReleatedValues();
}

AndroidSDK::~AndroidSDK()
{
}

void AndroidSDK::DeducePathReleatedValues()
{
	DeducePlatform();
	DeduceBuildToolsRelease();
}

void AndroidSDK::DeducePlatform()
{
	platform = FindDefaultPlatform();
}

void AndroidSDK::DeduceBuildToolsRelease()
{
	build_tools_release = FindDefaultBuildToolsRelease();
}

bool AndroidSDK::Validate() const
{
	if(IsNull(path) || !DirectoryExists(path) || !FileExists(AndroidPath())) return false;
	
	return true;
}

bool AndroidSDK::ValidatePlatform() const
{
	return DirectoryExists(ConcretePlatformDir());
}

bool AndroidSDK::ValidateBuildTools() const
{
	return DirectoryExists(ConcreteBuildToolsDir());
}

Vector<String> AndroidSDK::FindPlatforms() const
{
	Vector<String> platforms;
	
	for(FindFile ff(AppendFileName(PlatformsDir(), "*")); ff; ff.Next()) {
		if(!ff.IsHidden() && ff.IsFolder())
			platforms.Add(ff.GetName());
	}
	
	return platforms;
}

Vector<String> AndroidSDK::FindBuildToolsReleases() const
{
	Vector<String> buildTools;
	
	for(FindFile ff(AppendFileName(BuildToolsDir(), "*")); ff; ff.Next()) {
		if(!ff.IsHidden() && ff.IsFolder())
			buildTools.Add(ff.GetName());
	}
	
	return buildTools;
}

Vector<AndroidDevice> AndroidSDK::FindDevices() const
{
	Vector<AndroidDevice> devices;
	
	String out;
	if(Sys(NormalizeExePath(AdbPath()) + " devices -l", out) == 0) {
		Vector<String> lines;
		Vector<String> outputLines = Split(out, "\n");
		for(int i = 0; i < outputLines.GetCount(); i++) {
			if(!outputLines[i].StartsWith("*") && !outputLines[i].StartsWith(" "))
				lines.Add(outputLines[i]);
		}
		
		for(int i = 1; i < lines.GetCount(); i++) {
			AndroidDevice device;
			
			Vector<String> elements = Split(lines[i], " ");
			for(int j = 0; j < elements.GetCount(); j++) {
				if(j == 0)
					device.SetSerial(elements[j]);
				else if(j == 1)
					device.SetState(elements[j]);
				else {
					Vector<String> element = Split(elements[j], ":");
					if(element.GetCount() == 2) {
						String tag  = element[0];
						String data = element[1];
						if(tag == "usb")
							device.SetUsb(data);
						else
						if(tag == "model")
							device.SetModel(data);
					}
				}
			}
			devices.Add(device);
		}
	}
	
	return devices;
}

Vector<AndroidVirtualDevice> AndroidSDK::FindVirtualDevices() const
{
	Vector<AndroidVirtualDevice> avdes;
	
	String out;
	if(Sys(NormalizeExePath(AndroidPath()) + " list avd", out) == 0) {
		Vector<String> lines = Split(out, "\n");
		
		AndroidVirtualDevice avd;
		for(int i = 0; i < lines.GetCount(); i++) {
			Vector<String> line = Split(lines[i], ":");
			if(line.GetCount() == 2) {
				String tag  = line[0];
				String data = line[1];
				if(data.StartsWith(" "))
					data.Remove(0);
				if(tag.Find("Name") > -1) {
					if(!avd.GetName().IsEmpty() && avd.GetName() != data)
						avdes.Add(avd);
					avd.SetName(data);
				}
				else
				if(tag.Find("Device") > -1)
					avd.SetDeviceType(data);
				else
				if(tag.Find("Path") > -1)
					avd.SetPath(data);
				else
				if(tag.Find("Target") > -1)
					avd.SetTarget(data);
				else
				if(tag.Find("Tag/ABI") > -1)
					avd.SetAbi(data);
				
				// TODO: implement all possible tags
			}
		}
		
		if(!avd.GetName().IsEmpty())
			avdes.Add(avd);
	}
	
	return avdes;
}

String AndroidSDK::FindDefaultPlatform() const
{
	Vector<String> platforms = FindPlatforms();
	if(platforms.GetCount()) {
		Android::NormalizeVersions(platforms);
		Sort(platforms, StdGreater<String>());
		Android::RemoveVersionsNormalization(platforms);
		
		int idx = 0;
		for(int i = 0; i < platforms.GetCount(); i++) {
			if(RegExp("^android-[0-9]*$").Match(platforms[i])) {
				idx = i;
				break;
			}
		}
		return platforms[idx];
	}
	return "";
}

String AndroidSDK::FindDefaultBuildToolsRelease() const
{
	Vector<String> releases = FindBuildToolsReleases();
	if(releases.GetCount()) {
		Sort(releases, StdGreater<String>());
		int idx = 0;
		for(int i = 0; i < releases.GetCount(); i++) {
			if(RegExp("^[1-9][0-9.]*$").Match(releases[i])) {
				idx = i;
				break;
			}
		}
		return releases[idx];
	}
	return "";
}

// ---------------------------------------------------------------

String AndroidSDK::BinDir() const
{
	return path + DIR_SEPS + "bin";
}

String AndroidSDK::BuildToolsDir() const
{
	return path + DIR_SEPS + "build-tools";
}

String AndroidSDK::PlatformsDir() const
{
	return path + DIR_SEPS + "platforms";
}

String AndroidSDK::ConcreteBuildToolsDir() const
{
	return BuildToolsDir() + DIR_SEPS + build_tools_release;
}

String AndroidSDK::ConcretePlatformDir() const
{
	return PlatformsDir() + DIR_SEPS + platform;
}

String AndroidSDK::PlatformToolsDir() const
{
	return path + DIR_SEPS + "platform-tools";
}

String AndroidSDK::ToolsDir() const
{
	return path + DIR_SEPS + "tools";
}

// ---------------------------------------------------------------

bool AndroidSDK::HasD8() const
{
	return FileExists(D8Path());
}

}
