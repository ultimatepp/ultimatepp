#include "ide.h"

String Ide::GetAndroidSdkPath()
{
	VectorMap<String, String> bm = GetMethodVars(method);
	if(bm.Get("BUILDER", "") == "ANDROID")
		return bm.Get("SDK_PATH", String());
	return String();
}
