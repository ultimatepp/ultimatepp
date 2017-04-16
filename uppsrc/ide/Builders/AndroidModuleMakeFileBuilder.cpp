#include "AndroidBuilder.h"

#define METHOD_NAME "AndroidModuleMakeFileCreator::" << UPP_FUNCTION_NAME << "(): "

namespace Upp {

AndroidModuleMakeFileCreator::AndroidModuleMakeFileCreator(const Index<String>& builderConfig)
	: config(builderConfig)
{
}

void AndroidModuleMakeFileCreator::AddSources(Vector<String>& sources)
{
	for(const String& source : sources) {
		makeFile.AddSourceFile(source);
	}
}

void AndroidModuleMakeFileCreator::AddSources(Index<String>& sources)
{
	for(const String& source : sources) {
		makeFile.AddSourceFile(source);
	}
}

void AndroidModuleMakeFileCreator::AddInclude(const String& path)
{
	makeFile.AddInclude(path);
}

void AndroidModuleMakeFileCreator::AddIncludes(const Array<OptItem>& uses)
{
	for(const OptItem& use : uses) {
		makeFile.AddInclude(AndroidBuilderUtils::GetAssemblyDir(use.text));
	}
}

void AndroidModuleMakeFileCreator::AddFlags(const Array<OptItem>& flags)
{
	for(const OptItem& flag : flags) {
		makeFile.AddCppFlag(flag.text);
	}
}

void AndroidModuleMakeFileCreator::AddLdLibraries(const Array<OptItem>& libraries)
{
	Vector<String> libs = Split(Gather(libraries, config.GetKeys()), ' ');
	for(const String& lib : libs) {
		makeFile.AddLdLibrary(lib);
	}
}

void AndroidModuleMakeFileCreator::AddStaticModuleLibrary(Array<OptItem>& staticLibraries)
{
	Vector<String> slibs = Split(Gather(staticLibraries, config.GetKeys()), ' ');
	for(const String& slib : slibs) {
		makeFile.AddStaticModuleLibrary(slib);
	}
}

void AndroidModuleMakeFileCreator::AddSharedLibraries(const Array<OptItem>& uses)
{
	for(const OptItem& use : uses) {
		makeFile.AddSharedLibrary(use.text);
	}
}

bool AndroidModuleMakeFileCreator::Save(const String& path)
{
	String directory = GetFileDirectory(path);
	if (!RealizeDirectory(directory)) {
		Loge() << METHOD_NAME << "Creating module directory failed \"" << directory << "\".";
		return false;
	}
	
	String data = Create();
	if (FileExists(path) && LoadFile(path) == data) {
		Logi() << METHOD_NAME << "Following file \"" << path << "\" content is the same as previous one.";
		return true;
	}
	
	if (!SaveFile(path, Create())) {
		Loge() << METHOD_NAME << "Saving module file failed \"" << path << "\".";
		return false;
	}
	
	return true;
}

}
