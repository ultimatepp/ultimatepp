#include <Core/Core.h>
#include <SystemLog/SystemLog.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	// opens the log
	SysLog.Open(GetExeTitle());
	
	// enables/disables output streams
	SysLog.EnableCerr();
	SysLog.EnableCout();
	SysLog.EnableUppLog();
	SysLog.EnableSysLog(false);
	
	// enables all log levels
	SysLog.SetLevels(SystemLog::LALLDEBUG);
	
	// logs some values there
	SysLog(SystemLog::LWARNING) << "a message on WARNING level " << "on a single line\n\n\n";
	SysLog(SystemLog::LWARNING) << "another message on WARNING level";
	SysLog << " on a single line";
	SysLog(SystemLog::LERROR) << "a message on ERROR level\non 2 lines";
	
	// shows behaviour of enabled/disabled log levels
	SysLog.DisableLevels(SystemLog::LDEBUG);
	SysLog(SystemLog::LDEBUG) << "that one gets lost....";
	SysLog.EnableLevels(SystemLog::LDEBUG);
	SysLog(SystemLog::LDEBUG) << "that one logged on DEBUG level";
	
	// shows behaviour of setting last output level
	SysLog(SystemLog::LINFO) << "that message goes on " << "INFO" << " level\n";
	SysLog << "that one goes also on INFO level, since it was last level selected\n";
	SysLog(SystemLog::LWARNING);
	SysLog << "here 3 messages on WARNING level\n";
	SysLog << "second one\n";
	SysLog << "third one\n";
	SysLog(SystemLog::LNOTICE) << "That's all folks !\n";
}

