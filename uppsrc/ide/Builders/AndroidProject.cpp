#include "Android.h"

namespace Upp {

AndroidProject::AndroidProject()
{
	
}

AndroidProject::AndroidProject(const String& dir)
{
	this->dir = dir;
}

AndroidProject::~AndroidProject()
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
	return this->dir + DIR_SEPS + "classes";
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

void AndroidProject::SetDir(const String& dir)
{
	this->dir = dir;
}

bool AndroidProject::HasDir() const
{
	return !dir.IsEmpty();
}

}
