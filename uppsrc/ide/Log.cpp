#include "ide.h"

#define METHOD_NAME "Ide::" << UPP_FUNCTION_NAME << "(): "

String Ide::GetIdeLogPath()
{
	return GetStdLogPath();
}

String Ide::GetTargetLogPath()
{
	if(target.GetCount() == 0)
		return Null;
#ifdef PLATFORM_WIN32
	return ForceExt(target, ".log");
#else
	String p = GetFileTitle(target);
	return GetHomeDirFile(".upp/" + p + "/" + p + ".log");
#endif
}

void Ide::OpenLog(const String& logFilePath)
{
	String normalizedPath = NormalizePath(logFilePath);
	if(!designer && normalizedPath == editfile) {
		History(-1);
		return;
	}
	AddHistory();
	if(!FileExists(logFilePath)) {
		Loge() << METHOD_NAME << "Following log file doesn't exist: \"" << logFilePath << "\".";
		return;
	}
	
	EditFile(logFilePath);
}
