#include "AndroidBuilder.h"

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

void AndroidModuleMakeFileCreator::AddInclude(const String& path)
{
	makeFile.AddInclude(path);
}

void AndroidModuleMakeFileCreator::AddIncludeWithSubdirs(const String& path)
{
	BiVector<String> dirs { path };
	while(!dirs.IsEmpty()) {
		for(FindFile ff(AppendFileName(dirs.Head(), "*")); ff; ff.Next()) {
			if(ff.IsHidden() || ff.IsSymLink() || !ff.IsFolder()) {
				continue;
			}
			
			String name = ff.GetName();
			if (name.EndsWith(".tpp")) {
				continue;
			}
				
			dirs.AddTail(ff.GetPath());
		}
		
		AddInclude(dirs.Head());
		dirs.DropHead();
	}
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

}
