#include "DerivedLayout.h"

Base::Base()
{
	CtrlLayout(*this);
}

Derived::Derived()
{
	SetLayout_DerivedLayout(*this, true);
}

GUI_APP_MAIN
{
	Derived().Run();
}
