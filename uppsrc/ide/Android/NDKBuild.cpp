#include "Executables.h"

namespace Upp {

NDKBuild::NDKBuild(const String& path)
	: path(path)
	, jobs(0)
	, verbose(false)
{
}

String NDKBuild::MakeCmd() const
{
	String cmd = NormalizeExePath(path);
	if(verbose)
		cmd << " V=1 ";
	if(!workingDir.IsEmpty())
		cmd << " -C " << workingDir;
	if(jobs > 0)
		cmd << " -j " << jobs;
	
	return cmd;
}

}
