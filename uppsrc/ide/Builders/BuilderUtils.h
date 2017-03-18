#ifndef _ide_Builders_BuilderUtils_h_
#define _ide_Builders_BuilderUtils_h_

#include <Core/Core.h>

namespace Upp {
	
class BuilderUtils final {
public:
	BuilderUtils() = delete;
	BuilderUtils(BuilderUtils&) = delete;
	BuilderUtils& operator=(BuilderUtils&) = delete;
	~BuilderUtils() = delete;
	
	static bool IsJavaFile(const String& path);
	static bool IsHeaderFile(const String& path);
	static bool IsCFile(const String& path);
	static bool IsCppFile(const String& path);
	static bool IsCppOrCFile(const String& path);
	static bool IsObjectFile(const String& path);
	static bool IsXmlFile(const String& path);
	
	static bool IsTranslationFile(const String& path);
	
private:
	static bool   HasExt(const String& path, const Index<String>& exts);
	static String NormalizeAndGetFileExt(const String& path);
};
	
}

#endif
