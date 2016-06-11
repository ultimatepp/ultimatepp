#include "CheckCompile.h"

CheckCompile::CheckCompile()
{
	CtrlLayout(*this, "Window title");
}

GUI_APP_MAIN
{

	{
		// Checks ambiguity issue with Lambda assignment vs operator<<(String&&, T)
		EditString es;
		es << [=] { LOG("HERE"); };
		String() << AsString(Random());
	}
}
