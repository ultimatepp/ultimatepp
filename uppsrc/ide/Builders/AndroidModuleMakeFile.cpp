#include "Android.h"

namespace Upp {

AndroidModuleMakeFile::AndroidModuleMakeFile()
{
	
}

AndroidModuleMakeFile::AndroidModuleMakeFile(const String& name)
{
	this->name = name;
}

AndroidModuleMakeFile::~AndroidModuleMakeFile()
{
	
}

void AndroidModuleMakeFile::Clear()
{
	name.Clear();
	sourceFiles.Clear();
	includes.Clear();
	cppFlags.Clear();
	ldLibraries.Clear();
	staticLibraries.Clear();
	sharedLibraries.Clear();
}

String AndroidModuleMakeFile::ToString() const
{
	String makeFile;
	
	makeFile << "include $(CLEAR_VARS)\n";
	AppendName(makeFile);
	AppendSourceFiles(makeFile);
	AppendIncludes(makeFile);
	AppendCppFlags(makeFile);
	AppendLdLibraries(makeFile);
	AppendStaticLibraries(makeFile);
	AppendSharedLibraries(makeFile);
	makeFile << "include $(BUILD_SHARED_LIBRARY)\n";
	AppendModules(makeFile);
	
	return makeFile;
}

void AndroidModuleMakeFile::AddSourceFile(const String& path)
{
	sourceFiles.Add(path);
}

void AndroidModuleMakeFile::AddInclude(const String& includePath)
{
	includes.Add(includePath);
}

void AndroidModuleMakeFile::AddCppFlag(const String& name, const String& value)
{
	cppFlags.Add(name, value);
}

void AndroidModuleMakeFile::AddLdLibrary(const String& ldLibrary)
{
	ldLibraries.Add(ldLibrary);
}

void AndroidModuleMakeFile::AddStaticLibrary(const String& staticLibrary)
{
	staticLibraries.Add(staticLibrary);
}

void AndroidModuleMakeFile::AddStaticModuleLibrary(const String& staticModuleLibrary)
{
	staticModuleLibraries.Add(staticModuleLibrary);
}

void AndroidModuleMakeFile::AddSharedLibrary(const String& sharedLibrary)
{
	sharedLibraries.Add(sharedLibrary);
}

void AndroidModuleMakeFile::AppendName(String& makeFile) const
{
	makeFile << "LOCAL_MODULE := " << name << "\n";
}

void AndroidModuleMakeFile::AppendSourceFiles(String& makeFile) const
{
	AndroidMakeFile::AppendStringVector(makeFile, sourceFiles, "LOCAL_SRC_FILES");
}

void AndroidModuleMakeFile::AppendIncludes(String& makeFile) const
{
	AndroidMakeFile::AppendStringVector(makeFile, includes, "LOCAL_C_INCLUDES");
}

void AndroidModuleMakeFile::AppendCppFlags(String& makeFile) const
{
	if(!cppFlags.IsEmpty()) {
		makeFile << "LOCAL_CPPFLAGS := ";
		for(int i = 0; i < cppFlags.GetCount(); i++) {
			String value = cppFlags[i];
			makeFile << "-D" << cppFlags.GetKey(i);
			if(!value.IsEmpty())
				makeFile << "=" << value;
			if(i + 1 < sourceFiles.GetCount())
				makeFile << " ";
		}
		makeFile << "\n";
	}
}

void AndroidModuleMakeFile::AppendLdLibraries(String& makeFile) const
{
	AndroidMakeFile::AppendStringVector(makeFile, ldLibraries, "LOCAL_LDLIBS", "-l");
}

void AndroidModuleMakeFile::AppendStaticLibraries(String& makeFile) const
{
	Vector<String> allLibs;
	allLibs.Append(staticLibraries);
	allLibs.Append(staticModuleLibraries);
	
	AndroidMakeFile::AppendStringVector(makeFile, allLibs, "LOCAL_STATIC_LIBRARIES");
}

void AndroidModuleMakeFile::AppendSharedLibraries(String& makeFile) const
{
	AndroidMakeFile::AppendStringVector(makeFile, sharedLibraries, "LOCAL_SHARED_LIBRARIES");
}

void AndroidModuleMakeFile::AppendModules(String& makeFile) const
{
	if(staticModuleLibraries.GetCount()) {
		for(int i = 0; i < staticModuleLibraries.GetCount(); i++) {
			if(i == 0)
				makeFile << "\n";
			
			const String androidPrefix = "android_";
			
			String module = staticModuleLibraries[i];
			if(module.StartsWith(androidPrefix))
				module.Remove(0, androidPrefix.GetCount());
			module = "android/" + module;
			
			makeFile << "$(call import-module, " << module << ")\n";
		}
	}
}

}
