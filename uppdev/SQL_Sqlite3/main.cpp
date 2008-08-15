#include <Core/Core.h>
#include <plugin/sqlite3/Sqlite3.h>
using namespace Upp;

CONSOLE_APP_MAIN
{
	Sqlite3Session session;
	session.LogErrors(true);
	if (!session.Open("data"))
		Cout() << "Error opening SQL";
}//crash here!
