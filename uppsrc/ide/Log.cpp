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
	String title = GetFileTitle(target);
	String pp = GetFileFolder(GetUserConfigDir(GetFileFolder(target))) + "/.local/state/u++/log/" + title + ".log";
	if(FileExists(pp))
		return pp;
	String h = GetFileFolder(target);
	while(h.GetCount() > 1 && DirectoryExists(h)) {
		String p = AppendFileName(h, ".config");
		FindFile ff(p);
		if(ff && ff.IsFolder() && ff.CanWrite()) {
			pp = p;
			break;
		}
		h = GetFileFolder(h);
	}
	if(IsNull(pp))
		pp = GetEnv("XDG_CONFIG_HOME");
	if(IsNull(pp) || !DirectoryExists(pp))
		pp = GetHomeDirFile(".config");
	pp << "/u++/" << title << '/' << title << ".log";
	return FileExists(pp) ? pp : GetHomeDirFile(".upp/" + title + "/" + title + ".log");
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
