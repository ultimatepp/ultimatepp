#include "BuilderUtils.h"

using namespace Upp;

bool BuilderUtils::IsJavaFile(const String& path)
{
	return HasExt(path, { "java" });
}

bool BuilderUtils::IsHeaderFile(const String& path)
{
	return HasExt(path, { "i", "h", "hpp", "hxx" });
}

bool BuilderUtils::IsCFile(const String& path)
{
	return HasExt(path, { "c" });
}

bool BuilderUtils::IsCppFile(const String& path)
{
	return HasExt(path, { "icpp", "cpp", "cc", "cxx" });
}

bool BuilderUtils::IsCppOrCFile(const String& path)
{
	return HasExt(path, { "c", "icpp", "cpp", "cc", "cxx" });
}

bool BuilderUtils::IsXmlFile(const String& path)
{
	return HasExt(path, { "xml" });
}

bool BuilderUtils::IsObjectFile(const String& path)
{
	return HasExt(path, { "o" });
}

bool BuilderUtils::IsTranslationFile(const String& path)
{
	return HasExt(path, { "t" });
}

bool BuilderUtils::HasExt(const String& path, const Index<String>& exts)
{
	return exts.Find(NormalizeAndGetFileExt(path)) != -1;
}

String BuilderUtils::NormalizeAndGetFileExt(const String& path)
{
	String ext = ToLower(GetFileExt(path));
	if (ext.StartsWith(".")) {
		ext.Remove(0);
	}
	return ext;
}
