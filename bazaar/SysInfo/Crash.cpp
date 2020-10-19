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

#if defined(PLATFORM_WIN32)	
LONG WINAPI CrashHandler::UnhandledHandler(EXCEPTION_POINTERS *exceptionPtrs) { 
	Panic("Default exception");
	return EXCEPTION_EXECUTE_HANDLER;
}
void __cdecl CrashHandler::SEHHandler(unsigned u, EXCEPTION_POINTERS* p) {
	switch(u) {
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		case EXCEPTION_INT_DIVIDE_BY_ZERO:
		Panic("Floating point exception");
		break;
	default:
		Panic("SEH exception");	
	}
}
#endif 

void __cdecl CrashHandler::TerminateHandler() {
	Panic("Terminate exception");
}

void __cdecl CrashHandler::UnexpectedHandler() {
 	Panic("Unexpected exception");
}

void __cdecl CrashHandler::PureCallHandler() {
	Panic("Pure virtual function call");
}

void __cdecl CrashHandler::InvalidParameterHandler(const wchar_t* expression, const wchar_t *function, 
	const wchar_t* file, unsigned int line, uintptr_t) {
	if (line == 0)
		Panic("Invalid parameter");
	else
		Panic(Format("Invalid parameter in %s, function %s, file %s, line %d", AsString(expression),
			AsString(function), AsString(file), int(line)));
}

void __cdecl CrashHandler::NewHandler() {
	Panic("Not enough memory available");
}

void CrashHandler::SigabrtHandler(int) {
	Panic("SIGABRT: Process has aborted");
}

void CrashHandler::SigfpeHandler(int) {
	Panic("SIGFPE: Floating point error");
}

void CrashHandler::SigillHandler(int) {
	Panic("SIGILL: Executable code seems corrupted");
}

void CrashHandler::SigintHandler(int) {
	Panic("SIGINT: Process has been asked to terminate by user");
}

void CrashHandler::SigsegvHandler(int) {
	Panic("SIGSEGV: Trying to read or write from/to a memory area that your process does not have access to");
}

void CrashHandler::SigtermHandler(int) {
	Panic("SIGTERM: Process has been asked to terminate by other application");
}

};