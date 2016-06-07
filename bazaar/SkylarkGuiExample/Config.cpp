#include "Config.h"

using namespace Upp;

namespace Config{

	INI_INT(WebInterfacePort,	1080, "Webinteface http port number");
	INI_BOOL(WebInterfaceTrace,	true, "skylark trace");

}