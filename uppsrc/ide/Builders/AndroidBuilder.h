#ifndef _ide_Builders_AndroidBuilder_h_
#define _ide_Builders_AndroidBuilder_h_

#include "Android.h"

#include <ide/Core/Core.h>

namespace Upp {

class AndroidBuilderUtils final {
public:
	AndroidBuilderUtils() = delete;
	AndroidBuilderUtils(AndroidBuilderUtils&) = delete;
	AndroidBuilderUtils& operator=(AndroidBuilderUtils&) = delete;
	~AndroidBuilderUtils() = delete;
	
	static String GetAssemblyDir(const String& package);
	static String GetAssemblyDir(const String& packageDir, const String& package);
};

class AndroidModuleMakeFileCreator {
public:
	AndroidModuleMakeFileCreator(const Index<String>& builderConfig);
	virtual ~AndroidModuleMakeFileCreator() {}
	
	void SetModuleName(const String& name) { makeFile.SetName(name); }

	void AddSources(Vector<String>& sources);
	void AddInclude(const String& path);
	void AddIncludeWithSubdirs(const String& path);
	
	void AddIncludes(const Array<OptItem>& uses);
	void AddFlags(const Array<OptItem>& flags);
	void AddLdLibraries(const Array<OptItem>& libraries);
	void AddStaticModuleLibrary(Array<OptItem>& staticLibraries);
	void AddSharedLibraries(const Array<OptItem>& uses);
	
	String Create() { return makeFile.ToString(); }
	
private:
	AndroidModuleMakeFile makeFile;
	const Index<String>&  config;
};
	
}

#endif
