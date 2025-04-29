#include "VerticalLabel.h"

VerticalLabel::VerticalLabel()
{
	CtrlLayout(*this, "Window title");
}

GUI_APP_MAIN
{
	VerticalLabel().Run();
}
