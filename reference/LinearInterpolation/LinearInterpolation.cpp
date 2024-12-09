#include <Core/Core.h>

using namespace Upp;

// Generic Lerp and specializations

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
    DUMP(Lerp(10, 20, 0.5));
    DUMP(Lerp(10.0, 20.0, 0.25));
    DUMP(Lerp(Point(10, 20), Point(30, 40), 0.5));
    DUMP(Lerp(Size(100, 200), Size(300, 400), 0.5));
    DUMP(Lerp(Rect(0, 0, 100, 100), Rect(100, 100, 200, 200), 0.5));

    DUMP(Lerp(Color(100, 200, 150), Color(200, 100, 120), 0.5));
    DUMP(Lerp(Color(100, 200, 150), Color(200, 100, 120), 1.5));
    DUMP(Lerp(Color(100, 200, 150), Color(200, 100, 120), -1));
}



