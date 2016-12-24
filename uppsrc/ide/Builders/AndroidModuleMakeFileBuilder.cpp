#include "AndroidBuilder.h"

namespace Upp {

AndroidModuleMakeFileCreator::AndroidModuleMakeFileCreator(const Index<String>& builderConfig)
	: config(builderConfig)
{
}

void AndroidModuleMakeFileCreator::AddSources(Vector<String>& sources)
{
	for(int i = 0; i < sources.GetCount(); ++i) {
		makeFile.AddSourceFile(sources[i]);
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
	for(int i = 0; i < uses.GetCount(); ++i) {
		makeFile.AddInclude(AndroidBuilderUtils::GetAssemblyDir(uses[i].text));
	}
}

void AndroidModuleMakeFileCreator::AddFlags(const Array<OptItem>& flags)
{
	for(int i = 0; i < flags.GetCount(); ++i) {
		makeFile.AddCppFlag(flags[i].text);
	}
}

void AndroidModuleMakeFileCreator::AddLdLibraries(const Array<OptItem>& libraries)
{
	Vector<String> libs = Split(Gather(libraries, config.GetKeys()), ' ');
	for(int i = 0; i < libs.GetCount(); ++i) {
		makeFile.AddLdLibrary(libs[i]);
	}
}

void AndroidModuleMakeFileCreator::AddStaticModuleLibrary(Array<OptItem>& staticLibraries)
{
	Vector<String> slibs = Split(Gather(staticLibraries, config.GetKeys()), ' ');
	for(int i = 0; i < slibs.GetCount(); ++i) {
		makeFile.AddStaticModuleLibrary(slibs[i]);
	}
}

void AndroidModuleMakeFileCreator::AddSharedLibraries(const Array<OptItem>& uses)
{
	for(int i = 0; i < uses.GetCount(); i++) {
		makeFile.AddSharedLibrary(uses[i].text);
	}
}

}
