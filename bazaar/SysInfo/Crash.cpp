#include <Core/Core.h>
#include "SysInfo_in.h"
#include <signal.h>
#include <exception>
#include <fenv.h>
#if defined(PLATFORM_WIN32) 
#include <psapi.h>
#include <rtcapi.h>
#include <Shellapi.h>
	#ifdef COMPILER_MSC
	#include <verrsrc.h>
	#endif
#include <dbghelp.h>
#include <eh.h>
#include <memory>
#endif

namespace Upp {

#if defined(PLATFORM_WIN32)
#pragma float_control(except, on)
#endif

void ExceptionHandler(const char *msg) {
#if defined(_DEBUG) 
	static CrashHandler crash;
	crash.Init(msg);
#endif
}

void CrashHandler::Init(const char *msg) {
	message = String(msg);
	
#if defined(PLATFORM_WIN32)
	_clearfp();
	_controlfp(_controlfp(0, 0) & ~(_EM_INVALID | _EM_ZERODIVIDE | _EM_OVERFLOW), _MCW_EM);
	
	SetUnhandledExceptionFilter(UnhandledHandler);    
	_set_purecall_handler(PureCallHandler);    
	_set_invalid_parameter_handler(InvalidParameterHandler); 
	_set_abort_behavior(_CALL_REPORTFAULT, _CALL_REPORTFAULT);
#else
	feenableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW);
#endif

	std::set_new_handler(NewHandler);
	std::set_terminate(TerminateHandler);
	std::set_unexpected(UnexpectedHandler);
	
	signal(SIGABRT, SigabrtHandler);  
	signal(SIGINT, SigintHandler);     
	signal(SIGTERM, SigtermHandler);          
	signal(SIGFPE, SigfpeHandler);     
	signal(SIGILL, SigillHandler);     
	signal(SIGSEGV, SigsegvHandler); 
}

String CrashHandler::message = ""; 

void CrashHandler::ErrorHandler(const String &str) {
	SetConsoleColor(CONSOLE_COLOR::LTYELLOW);	
	PanicMessageBox(t_("Program error"), str + "\n" + message);
	SetConsoleColor(CONSOLE_COLOR::RESET);	
}

#if defined(PLATFORM_WIN32)	
LONG WINAPI CrashHandler::UnhandledHandler(EXCEPTION_POINTERS *exceptionPtrs) { 
	ErrorHandler(t_("Default exception"));
	_exit(1);
	return EXCEPTION_EXECUTE_HANDLER;
}
void __cdecl CrashHandler::SEHHandler(unsigned u, EXCEPTION_POINTERS* p) {
	switch(u) {
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		case EXCEPTION_INT_DIVIDE_BY_ZERO:
		ErrorHandler(t_("Floating point exception"));
		break;
	default:
		ErrorHandler(t_("SEH exception"));	
	}
	_exit(1);	
}
#endif 

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#endif

void __cdecl CrashHandler::TerminateHandler() {
	ErrorHandler(t_("Terminate exception"));
	_exit(1);
}

void __cdecl CrashHandler::UnexpectedHandler() {
 	ErrorHandler(t_("Unexpected exception"));
	_exit(1);
}

void __cdecl CrashHandler::PureCallHandler() {
	ErrorHandler(t_("Pure virtual function call"));
	_exit(1);
}

void __cdecl CrashHandler::InvalidParameterHandler(const wchar_t* expression, const wchar_t *function, 
	const wchar_t* file, unsigned int line, uintptr_t) {
	if (line == 0)
		ErrorHandler(t_("Invalid parameter"));
	else
		ErrorHandler(Format(t_("Invalid parameter in %s, function %s, file %s, line %d"), AsString(expression),
			AsString(function), AsString(file), int(line)));
	_exit(1);
}

void __cdecl CrashHandler::NewHandler() {
	ErrorHandler(t_("Not enough memory available"));
	_exit(1);
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

void CrashHandler::SigabrtHandler(int) {
	ErrorHandler(t_("SIGABRT: Process has aborted"));
	_exit(1);
}

void CrashHandler::SigfpeHandler(int) {
	ErrorHandler(t_("SIGFPE: Floating point error"));
	_exit(1);
}

void CrashHandler::SigillHandler(int) {
	ErrorHandler(t_("SIGILL: Executable code seems corrupted"));
	_exit(1);    
}

void CrashHandler::SigintHandler(int) {
	ErrorHandler(t_("SIGINT: Process has been asked to terminate by user"));
	_exit(1);
}

void CrashHandler::SigsegvHandler(int) {
	ErrorHandler(t_("SIGSEGV: Trying to read or write from/to a memory area that your process does not have access to"));
	_exit(1); 
}

void CrashHandler::SigtermHandler(int) {
	ErrorHandler(t_("SIGTERM: Process has been asked to terminate by other application"));
	_exit(1);
}

};