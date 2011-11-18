#ifndef FORM_EDITOR_COMMON__LOGGER_H
#define FORM_EDITOR_COMMON__LOGGER_H

#include "Utils.h"

#define LLOG(text) ToLog(text)
#define LLOGF(text) ToLog(text, __FILE__, __LINE__, __FUNCTION__)
#define LDUMP(text) ToLog(String(#text) + " = " + AsString(text))
#define LERROR(text) ToLog(String(t_("ERROR: ")) + text, __FILE__, __LINE__, __FUNCTION__)
#define LWARNING(text) ToLog(String(t_("WARNING: ")) + text, __FILE__, __LINE__, __FUNCTION__)

static StaticMutex __LogMutex;
static bool __Created = false;
inline bool CreateLogFile()
{
	Mutex::Lock __(__LogMutex);
	if (__Created) return true;
	FileOut f;
	if (!f.Open(ConfigFileEx(GetFileTitle(GetExeFilePath()) + "Log.txt")))
	{
		LOG(t_("ERROR: LOG-file \"FormEditorLog.txt\" not created!"));
		return false;
	}
	f.PutLine(t_("LOG created."));
	f.Close();
	return __Created = true;
}

inline String ToLog(const char *text)
{
	Mutex::Lock __(__LogMutex);
	CreateLogFile();
	FileAppend f;
	if (!f.Open(ConfigFileEx(GetFileTitle(GetExeFilePath()) + "Log.txt")))
		return String(text);
	f.PutLine(FormatTime(GetSysTime(), "DD.MM.YYYY hh:mm:ss ") + text);
	f.Close();
	return String(text);
}

inline String ToLog(const char *text, const char* file, int line,  const char* method)
{
	Mutex::Lock __(__LogMutex);
	CreateLogFile();
	FileAppend f;
	if (!f.Open(ConfigFileEx(GetFileTitle(GetExeFilePath()) + "Log.txt")))
		return String(text);
	f.PutLine(FormatTime(GetSysTime(), "DD.MM.YYYY hh:mm:ss ") + text
		+ NFormat(t_(" (file \"%s\", line %d: \"%s\")"), file, line, method));
	f.Close();
	return String(text) + " (доп. информация в лог-файле)";
}

#endif
