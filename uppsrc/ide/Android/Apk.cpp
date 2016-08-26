#include "Android.h"

namespace Upp {

Apk::Apk(const String& path, const AndroidSDK& androidSDK)
{
	this->path = path;
	this->sdk = androidSDK;
}

Apk::~Apk()
{
	
}

String Apk::FindPackageName() const
{
	return FindValueInAndroidManifest("package", "name");
}

String Apk::FindLaunchableActivity() const
{
	return FindValueInAndroidManifest("launchable-activity", "name");
}

String Apk::FindValueInAndroidManifest(const String& badge, const String& tag) const
{
	String out;
	if(Sys(sdk.AaptPath() + " dump badging " + path + " AndroidManifest.xml", out) == 0) {
		Vector<String> lines = Split(out, '\n');
		for(int i = 0; i < lines.GetCount(); i++) {
			if(lines[i].Find(badge) >= 0)
				return FindBadgeTagValue(lines[i], tag);
		}
	}
	
	return String();
}

String Apk::FindBadgeTagValue(const String& badge, const String& tag) const
{
	String tagValue;
	String tagBeginning = tag + "='";
	int pos = badge.Find(tagBeginning);
	if(pos >= 0) {
		for(int i = pos + tagBeginning.GetCount(); i < badge.GetCount(); i++) {
			if(badge[i] == '\'')
				break;
			tagValue += badge[i];
		}
	}
	
	return tagValue;
}

}
