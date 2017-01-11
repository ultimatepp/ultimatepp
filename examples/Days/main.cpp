#include "Days.h"

Days::Days()
{
	CtrlLayout(*this, "Days");
	date1 ^= date2 ^= [=] {
		result = IsNull(date1) || IsNull(date2) ? "" :
		         Format("There is %d day(s) between %` and %`", abs(Date(~date1) - Date(~date2)), ~date1, ~date2);
	};
}

GUI_APP_MAIN
{
	Days().Run();
}
