#include "Spin.h"

Spin::Spin()
{
	CtrlLayout(*this, "Window title");
	
	int_2.SetInc(5);
	int64_2.SetInc(5);
	double_2.SetInc(5);
	date_2.SetInc(5);
	time_2.SetInc(5);

	int_3.RoundFromMin().SetInc(5).Min(3);
	int64_3.RoundFromMin().SetInc(5).Min(3);
	double_3.RoundFromMin().SetInc(5).Min(3);
	date_3.RoundFromMin().SetInc(5).Min(Date(1970, 1, 1));
	time_3.RoundFromMin().SetInc(5).Min(Time(1970, 1, 1));
}

GUI_APP_MAIN
{
	Spin().Run();
}
