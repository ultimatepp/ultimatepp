#include <Core/Core.h>
#include <UnitTest++/UnitTest++.h>
#include <UnitTest++/TestReporterStdout.h>

CONSOLE_APP_MAIN
{
	UPP::SetExitCode( UnitTest::RunAllTests() );
}

