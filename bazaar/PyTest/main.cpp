#include "PyTest.h"

void SimpleCall()
{
	Cout() << "Invoking a python statement:" << EOL;
	PyRun_SimpleString(	"from time import time,ctime\n"
						"print 'Today is',ctime(time())\n");
	PyRun_SimpleString(String("print 'python can calculate', 2+3"));
}

void StartConsole()
{
	PyRun_SimpleString(	"import code\n"
						"code.interact()\n");
}

CONSOLE_APP_MAIN
{
	Cout() << "Starting Python..." << EOL;

	SimpleCall();

	EmbedPython();
	ExtendPython();

	StartConsole();

	//leftover	
	DestroyNewModule();
	
	Cout() << "DONE." << EOL;
}

