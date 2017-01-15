#include "AndroidBuilder.h"

#define METHOD_NAME       "AndroidBuilderUtils::" + String(__FUNCTION__) + "(): "
#define ERROR_METHOD_NAME "[ERROR] " METHOD_NAME

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
	
	return assemblyDir != packageDir ? assemblyDir : "";
}

bool AndroidBuilderUtils::IsJavaFile(const String& path)
{
	return HasExt(path, { "java" });
}

bool AndroidBuilderUtils::IsHeaderFile(const String& path)
{
	return HasExt(path, { "i", "h", "hpp", "hxx" });
}

bool AndroidBuilderUtils::IsCFile(const String& path)
{
	return HasExt(path, { "c" });
}

bool AndroidBuilderUtils::IsCppFile(const String& path)
{
	return HasExt(path, { "icpp", "cpp", "cc", "cxx" });
}

bool AndroidBuilderUtils::IsCppOrCFile(const String& path)
{
	return HasExt(path, { "c", "icpp", "cpp", "cc", "cxx" });
}

bool AndroidBuilderUtils::IsXmlFile(const String& path)
{
	return HasExt(path, { "xml" });
}

bool AndroidBuilderUtils::IsObjectFile(const String& path)
{
	return HasExt(path, { "o" });
}

bool AndroidBuilderUtils::IsTranslationFile(const String& path)
{
	return HasExt(path, { "t" });
}

bool AndroidBuilderUtils::HasExt(const String& path, const Index<String>& exts)
{
	return exts.Find(NormalizeAndGetFileExt(path)) != -1;
}

String AndroidBuilderUtils::NormalizeAndGetFileExt(const String& path)
{
	String ext = ToLower(GetFileExt(path));
	if (ext.StartsWith(".")) {
		ext.Remove(0);
	}
	return ext;
}

}
