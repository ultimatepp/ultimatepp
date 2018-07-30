#ifndef _ide_Command_Line_Handler_h_
#define _ide_Command_Line_Handler_h_

#include <Core/Core.h>

namespace Upp {

// TODO: All TheIDE command line arguments should be handle in this class.
class CommandLineHandler {
public:
	CommandLineHandler(const Vector<String>& args);
	
	bool Handle();
	
public:
	Vector<String> GetArgs() { return clone(args); }
	
private:
	bool HandleManipulators();
	bool HandleScale();
	
	bool HandleVersion() const;
	bool HandleHelp() const;
	
	bool HandleDebugBreakProcess() const;
	
private:
	Vector<String> args;
};

}

#endif
