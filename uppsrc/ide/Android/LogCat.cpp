#include "Executables.h"

NAMESPACE_UPP

LogCat::LogCat()
{
	
}

LogCat::~LogCat()
{
	
}

String LogCat::MakeCmdByTag(const String& packageName) const
{
	String cmd;
	
	cmd << "logcat " << packageName << ":I" << " *:S";
	
	return cmd;
}

String LogCat::MakeCmdByTag(const Adb& adb, const String& packageName) const
{	
	return adb.MakeCmd() + " " + MakeCmdByTag(packageName);
}

END_UPP_NAMESPACE
