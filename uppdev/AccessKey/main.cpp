#include "AccessKey.h"

GUI_APP_MAIN
{
	Ctrl::AddFlags(Ctrl::AKD_CONSERVATIVE);
	AccessKey().Run(Accept());
	String s;
	s.Cat()
}
