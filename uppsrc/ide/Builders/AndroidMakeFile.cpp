#include "Android.h"

namespace Upp {

void AndroidMakeFile::AppendString(
	String& makeFile,
    const String& variable,
    const String& variableName)
{
	if(!variable.IsEmpty())
		makeFile << variableName << " := " << variable << "\n";
}

void AndroidMakeFile::AppendStringVector(
	String& makeFile,
    const Vector<String>& vec,
    const String& variableName,
    const String& variablePrefix,
    const String& variableSuffix)
{
	if(vec.IsEmpty()) {
		return;
	}
	
	makeFile << variableName << " := ";
	for(int i = 0; i < vec.GetCount(); ++i) {
		makeFile << variablePrefix << vec[i] << variableSuffix;
		if(i + 1 < vec.GetCount())
			makeFile << " ";
	}
	makeFile << "\n";
}

AndroidMakeFile::AndroidMakeFile()
{
	hasHeader = false;
}

AndroidMakeFile::~AndroidMakeFile()
{
	
}

bool AndroidMakeFile::IsEmpty() const
{
	return ToString().IsEmpty();
}

void AndroidMakeFile::Clear()
{
	this->hasHeader = false;
}

void AndroidMakeFile::AddHeader()
{
	this->hasHeader = true;
}

void AndroidMakeFile::AddInclusion(const String& inclusion)
{
	inclusions.Add(inclusion);
}

void AndroidMakeFile::AddModuleImport(const String& moduleName)
{
	importedModules.Add(moduleName);
}

String AndroidMakeFile::ToString() const
{
	String makeFile;
	
	if(hasHeader)
		AppendHeader(makeFile);
	AppendInclusions(makeFile);
	AppendImportedModules(makeFile);
	
	return makeFile;
}

void AndroidMakeFile::AppendHeader(String& makeFile) const
{
	makeFile << "LOCAL_PATH := $(call my-dir)\n";
}

void AndroidMakeFile::AppendInclusions(String& makeFile) const
{
	if(!inclusions.IsEmpty())
		makeFile << "\n";
	for(int i = 0; i < inclusions.GetCount(); i++)
		makeFile << "include $(LOCAL_PATH)" << DIR_SEPS << inclusions[i] << "\n";
}

void AndroidMakeFile::AppendImportedModules(String& makeFile) const
{
	if(!importedModules.IsEmpty())
		makeFile << "\n";
	for(int i = 0; i < importedModules.GetCount(); i++)
		makeFile << "$(call import-module, " << importedModules[i] << ")\n";
}

}
