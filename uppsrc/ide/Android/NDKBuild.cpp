#include "Executables.h"

NAMESPACE_UPP

NDKBuild::NDKBuild(const String& path)
{
	this->path = path;
	this->jobs = 0;
}

NDKBuild::~NDKBuild()
{
	
}

String NDKBuild::ToString() const
{
	String cmd = NormalizeExePath(path);
	if(!workingDir.IsEmpty())
		cmd << " -C " << workingDir;
	if(jobs > 0)
		cmd << " -j " << jobs;
	
	return cmd;
}

END_UPP_NAMESPACE
