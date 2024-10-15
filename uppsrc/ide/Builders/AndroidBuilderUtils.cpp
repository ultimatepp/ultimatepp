#include "AndroidBuilder.h"

namespace Upp {

String AndroidBuilderUtils::GetAssemblyDir(const String& package)
{
	String packageManifest = PackagePath(package);
	String packageDir = GetFileFolder(packageManifest);
	
	return GetAssemblyDir(packageDir, package);
}

String AndroidBuilderUtils::GetAssemblyDir(const String& packageDir, const String& package)
{
	String assemblyDir = packageDir;
	
	int pos = packageDir.GetCount() - package.GetCount() - 1;
	if (pos >= 0 && pos < assemblyDir.GetCount())
		assemblyDir.Trim(pos);
	
	return assemblyDir != packageDir ? assemblyDir : String();
}

}
