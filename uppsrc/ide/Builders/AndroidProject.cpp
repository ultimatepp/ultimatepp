#include "Android.h"

namespace Upp {

AndroidProject::AndroidProject(const String& dir, bool debug)
	: dir(dir)
	, debug(debug)
{
}

String AndroidProject::GetDir() const
{
	return this->dir;
}

String AndroidProject::GetJavaDir() const
{
	return this->dir + DIR_SEPS + "java";
}

String AndroidProject::GetJavaDir(const String& package) const
{
	return GetJavaDir() + DIR_SEPS + package;
}

String AndroidProject::GetJniDir() const
{
	return this->dir + DIR_SEPS + "jni";
}

String AndroidProject::GetJniDir(const String& package) const
{
	return GetJniDir() + DIR_SEPS + package;
}

String AndroidProject::GetLibsDir() const
{
	return this->dir + DIR_SEPS + "libs";
}

String AndroidProject::GetResDir() const
{
	return this->dir + DIR_SEPS + "res";
}

String AndroidProject::GetBuildDir() const
{
	return this->dir + DIR_SEPS + "build";
}

String AndroidProject::GetClassesDir() const
{
	return GetIntermediatesDir() + DIR_SEPS + "classes";
}

String AndroidProject::GetIntermediatesDir() const
{
	return this->dir + DIR_SEPS + "intermediates";
}

String AndroidProject::GetBinDir() const
{
	return this->dir + DIR_SEPS + "bin";
}

String AndroidProject::GetObjDir() const
{
	return this->dir + DIR_SEPS + "obj";
}

String AndroidProject::GetObjLocalDir() const
{
	return GetObjDir() + DIR_SEPS + "local";
}

// -------------------------------------------------------------------

String AndroidProject::GetManifestPath() const
{
	return this->dir + DIR_SEPS + "AndroidManifest.xml";
}

String AndroidProject::GetJniMakeFilePath() const
{
	return GetJniDir() + DIR_SEPS + "Android.mk";
}

String AndroidProject::GetJniApplicationMakeFilePath() const
{
	return GetJniDir() + DIR_SEPS + "Application.mk";
}

// -------------------------------------------------------------------

Vector<String> AndroidProject::GetClassessFiles() const
{
	BiVector<String> dirs = { GetClassesDir() };
	
	Vector<String> classesFiles;
	while(!dirs.IsEmpty())
	{
		for(FindFile ff(AppendFileName(dirs.Head(), "*")); ff; ff.Next()) {
			if (ff.IsHidden() || ff.IsSymLink()) {
				continue;
			}
			
			auto path = ff.GetPath();
			if (ff.IsFolder()) {
				dirs.AddTail(path);
				continue;
			}
			
			if (path.EndsWith(".class")) {
				classesFiles.Add(path);
				continue;
			}
		}
		dirs.DropHead();
	}
	
	return classesFiles;
}

// -------------------------------------------------------------------

bool AndroidProject::IsDebug() const
{
	return debug;
}

bool AndroidProject::IsRelease() const
{
	return !debug;
}

}
