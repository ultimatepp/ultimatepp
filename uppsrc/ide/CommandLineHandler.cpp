#include "CommandLineHandler.h"

#include "About.h"

#include <Draw/Draw.h>
#include <ide/Common/CommandLineOptions.h>
#include <ide/Debuggers/GdbUtils.h>

using namespace Upp;

ACommandLineHandler::ACommandLineHandler(const Vector<String>& args)
	: args(clone(args))
{}

BaseCommandLineHandler::BaseCommandLineHandler(const Vector<String>& args)
	: ACommandLineHandler(args)
{}

bool BaseCommandLineHandler::Handle()
{
	if(HandleVersion())
		return true;
	if(HandleHelp())
		return true;
	
	if(HandleDebugBreakProcess())
		return true;
	
	return false;
}

bool BaseCommandLineHandler::HandleVersion() const
{
	if(args.IsEmpty() || findarg(args[0], "-v", "--version") < 0)
		return false;
	
	Cout() << SplashCtrl::GenerateVersionInfo(false) << "\n";
	
	return true;
}

bool BaseCommandLineHandler::HandleHelp() const
{
	if(args.IsEmpty() || findarg(args[0], "?", "--help", "-h", "-?", "/?") < 0)
		return false;
		
	Cout() << "Usage:\n"
			  "    theide [file..]             - opens given file in editor mode (Auto detection mode).\n"
			  "    theide [assembly] [package] - opens given package from given assembly.\n\n";
	
	Cout() << "Common options:\n"
	          "    -f $file        - opens given file in editor mode.\n"
	          "    -v or --version - displays information about version.\n"
			  "    -h or --help    - displays this site.\n\n";
	
	Cout() << "Advanced options:\n"
			  "    " << COMMAND_LINE_SCALE_OPTION << " $percent - scale interface by \"percent\" parameter.\n\n";
	
	Cout() << "Internal options (Should not be called by the user):\n"
	          "    " << COMMAND_LINE_GDB_DEBUG_BREAK_PROCESS_OPTION << " $pid - breaks debug process represented by \"pid\".\n";
	
	return true;
}

bool BaseCommandLineHandler::HandleDebugBreakProcess() const
{
	if(args.GetCount() < 2 || !args[0].IsEqual(COMMAND_LINE_GDB_DEBUG_BREAK_PROCESS_OPTION))
		return false;
	
	int pid = StrInt(args[1]);
	if(IsNull(pid)) {
		Cout() << "PID should be numeric value.\n";
		return true;
	}
	
	auto utils = GdbUtilsFactory().Create();
	auto error = utils->BreakRunning(pid);
	if(!error.IsEmpty()) {
		Cout() << error << "\n";
		SetExitCode(COMMAND_LINE_GDB_DEBUG_BREAK_PROCESS_FAILURE_STATUS);
	}
	
	return true;
}

MainCommandLineHandler::MainCommandLineHandler(const Vector<String>& args)
	: ACommandLineHandler(args)
{}

bool MainCommandLineHandler::Handle()
{
	if(HandleManipulators())
		return true;
	
	return false;
}

bool MainCommandLineHandler::HandleManipulators()
{
	if(HandleScale())
		return true;
	
	return false;
}

bool MainCommandLineHandler::HandleScale()
{
	if(args.GetCount() < 2 || !args[0].IsEqual(COMMAND_LINE_SCALE_OPTION))
		return false;
	
	int scale = StrInt(args[1]);
	if(IsNull(scale)) {
		Cout() << "Scale should be numeric value.\n";
		return true;
	}
	
	Font::SetStdFont(StdFont().Height(GetStdFontCy() * minmax(scale, 50, 400) / 100));
	
	args.Remove(0, 2);
	
	return false;
}
