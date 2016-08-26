#include "Android.h"

namespace Upp {

String Android::GetScriptExt()
{
#if defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64)
	return ".bat";
#else
	return "";
#endif
}

String Android::GetCmdExt()
{
#if defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64)
	return ".cmd";
#else
	return "";
#endif
}

void Android::NormalizeVersions(Vector<String>& versions)
{
	String prefix = FindVersionsPrefix(versions);
	String longestVersion = FindLongestVersion(versions);
	
	int longestVersionCount = longestVersion.GetCount();
	for(int i = 0; i < versions.GetCount(); i++) {
		int diff = longestVersionCount - versions[i].GetCount();
		if(diff <= 0)
			continue;
			
		String version = versions[i];
		if(!prefix.IsEmpty())
			version.Replace(prefix, "");
		for(int j = 0; j < diff; j++)
			version = "0" + version;
		versions[i] = prefix + version;
	}
}

void Android::RemoveVersionsNormalization(Vector<String>& versions)
{
	String prefix = FindVersionsPrefix(versions);
	for(int i = 0; i < versions.GetCount(); i++) {
		String version = versions[i];
		if(!prefix.IsEmpty())
			version.Replace(prefix, "");
		for(;;) {
			if(!version.StartsWith("0"))
				break;
			version.Remove(0);
		}
		versions[i] = prefix + version;
	}
}

String Android::FindVersionsPrefix(const Vector<String>& versions)
{
	String prefix;
	for(int i = 0; i < versions.GetCount(); i++) {
		String currentPrefix;
		String version = versions[i];
		for(int j = 0; j < 10; j++) {
			int idx = version.Find(IntStr(j));
			if(idx < 0)
				continue;
			String left = version.Left(idx);
			if(currentPrefix.IsEmpty() || left.GetCount() < currentPrefix.GetCount())
				currentPrefix = left;
		}
		if(i == 0)
			prefix = currentPrefix;
		else
		if(i > 0 && currentPrefix != prefix)
			return "";
	}
	
	return prefix;
}

String Android::FindLongestVersion(const Vector<String>& versions)
{
	String longest;
	for(int i = 0; i < versions.GetCount(); i++) {
		String current = versions[i];
		if(current.GetCount() > longest.GetCount())
			longest = current;
	}
	return longest;
}

}
