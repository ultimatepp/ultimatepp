#include "CommandLineHandler.h"

#include <Draw/Draw.h>

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
	if(args.GetCount() < 2 || args[0].Compare("--scale") != 0)
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
			  "    --scale $percent - scale interface by \"percent\" represented by parameter x.\n\n";
	
	Cout() << "Internal options (Should not be called by the user):\n"
	          "    --debug_break_process $pid - breaks debug process represented by \"pid\" (MS Windows only).\n";
	
	return true;
}
