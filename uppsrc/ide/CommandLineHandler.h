#ifndef _ide_Command_Line_Handler_h_
#define _ide_Command_Line_Handler_h_

#include <Core/Core.h>

// TODO: All TheIDE command line arguments should be consume in this file.

namespace Upp {

class ACommandLineHandler {
public:
	ACommandLineHandler(const Vector<String>& args);
	
	Vector<String> GetArgs() { return clone(args); }
	
public:
	virtual bool Handle() = 0;
	
protected:
	Vector<String> args;
};

class BaseCommandLineHandler final : public ACommandLineHandler {
public:
	BaseCommandLineHandler(const Vector<String>& args);
	
public:
	bool Handle() override;
	
private:
	bool HandleVersion() const;
	bool HandleHelp() const;
	
	bool HandleDebugBreakProcess() const;
};

class MainCommandLineHandler final : public ACommandLineHandler {
public:
	MainCommandLineHandler(const Vector<String>& args);
	
public:
	bool Handle() override;
	
private:
	bool HandleManipulators();
	bool HandleScale();
};

}

#endif
