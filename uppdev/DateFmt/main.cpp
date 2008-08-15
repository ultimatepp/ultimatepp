#include <Core/Core.h>



CONSOLE_APP_MAIN
{
	SetLanguage(LNG_CZECH);

	SetDateFormat("%4:Day %2:Month-%3:d %1:d");
	DUMP(GetSysDate());

	for(int i = 0; i < 100; i++)
		DUMP(Format("%1:d %1:tw %1:0tw %1:month %1:mon %1:day %1:dy", i));
	DUMP(StdConvertDate().Scan("15.6.2005"));
	SetDateScan("myd");
	DUMP(StdConvertDate().Scan("6.2005.15"));
	SetDateScan("mdy");
	DUMP(StdConvertDate().Scan("MAY 15 2005"));
	DUMP(StdConvertDate().Scan("MAY-15 2005"));
	Date d = Date(2004, 12, 24);
	DUMP(d);
	SetDateFormat("%2:02d/%3:02d/%1:4d");
	DUMP(d);
	SetDateFormat("%2:Month %3:02d %1:4d");
	DUMP(d);
	SetDateFormat("%2:month %3:02d %1:4d");
	DUMP(d);
	SetDateFormat("%2:MONTH %3:02d %1:4d");
	DUMP(d);
	SetDateFormat("%2:Mon %3:02d %1:4d");
	DUMP(d);
	SetDateFormat("%2:mon %3:02d %1:4d");
	DUMP(d);
	SetDateFormat("%2:MON %3:02d %1:4d");
	DUMP(d);
}
