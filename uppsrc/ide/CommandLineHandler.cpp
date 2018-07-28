#include "CommandLineHandler.h"

#include <Draw/Draw.h>
#include <ide/Common/CommandLineOptions.h>
#include <ide/Debuggers/GdbUtils.h>

using namespace Upp;

CommandLineHandler::CommandLineHandler(const Vector<String>& args)
	: args(clone(args))
{}

bool CommandLineHandler::Handle()
{
	if(HandleManipulators())
		return true;
	
	if(HandleHelp())
		return true;
	
	if(HandleDebugBreakProcess())
		return true;
	
	return false;
}

bool CommandLineHandler::HandleManipulators()
{
	if(HandleScale())
		return true;
	
	return false;
}

bool CommandLineHandler::HandleScale()
{
	if(args.GetCount() < 2 || !args[0].IsEqual(COMMAND_LINE_SCALE_OPTION))
		return false;
	
	int scale = StrInt(args[1]);
	if(IsNull(scale)) {
		Cout() << "Scale should be numeric value.";
		return true;
	}
	
	Font::SetStdFont(StdFont().Height(GetStdFontCy() * minmax(scale, 50, 400) / 100));
	
	args.Remove(0, 2);
	
	return false;
}

bool CommandLineHandler::HandleHelp() const
{
	if(args.IsEmpty() || findarg(args[0], "?", "--help", "-h", "-?", "/?") < 0)
		return false;
		
	Cout() << "Usage: theide assembly package\n"
			  "       theide assembly package build_method [-[a][b][e][r][s][S][v][1][2][m][d][M][l][x][X][Hn]] [+FLAG[,FLAG]...] [out]\n"
			  "       theide -f [file..]\n"
			  "       theide [file..] // autodetection mode\n\n";
	
	Cout() << "Common options:\n"
			  "    -h or --help - displays this site.\n\n";
	
	Cout() << "Advanced options:\n"
			  "    " << COMMAND_LINE_SCALE_OPTION << " $percent - scale interface by \"percent\" represented by parameter x.\n\n";
	
	Cout() << "Internal options (Should not be called by the user):\n"
	          "    " << COMMAND_LINE_GDB_DEBUG_BREAK_PROCESS_OPTION << " $pid - breaks debug process represented by \"pid\".\n";
	
	return true;
}

bool CommandLineHandler::HandleDebugBreakProcess() const
{
	if(args.GetCount() < 2 || !args[0].IsEqual(COMMAND_LINE_GDB_DEBUG_BREAK_PROCESS_OPTION))
		return false;
	
	int pid = StrInt(args[1]);
	if(IsNull(pid)) {
		Cout() << "PID should be numeric value.";
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
