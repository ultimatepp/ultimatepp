#include "Android.h"

#define METHOD_NAME "AndroidManifest::" << UPP_FUNCTION_NAME << "(): "

namespace Upp {

AndroidManifest::AndroidManifest(const String& path)
	: path(path)
{
}

AndroidManifest::~AndroidManifest()
{
}

bool AndroidManifest::Parse()
{
	String xml = LoadFile(path);
	if (xml.IsVoid()) {
		Loge() << METHOD_NAME << "Failed to load manifest file \"" + path + "\".";
		return false;
	}
	
	try {
		XmlParser p(xml);
		while(!p.IsTag()) {
			p.Skip();
		}
		p.PassTag("manifest");
		while (!p.End()) {
			if(p.TagE("uses-sdk")) {
				uses_sdk.Create();
				
				uses_sdk->minSdkVersion = p.Int("android:minSdkVersion");
				uses_sdk->targetSdkVersion = p.Int("android:targetSdkVersion");
				uses_sdk->maxSdkVersion = p.Int("android:maxSdkVersion");
			}
			
			p.Skip();
		}
	}
	catch(const XmlError& e) {
		Loge() << METHOD_NAME << "Failed to parse manifest file with error \"" + e + "\".";
		return false;
	}
	
	return true;
}

}
