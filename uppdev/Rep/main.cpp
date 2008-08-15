#include <CtrlLib/CtrlLib.h>
#include <Report/Report.h>

GUI_APP_MAIN
{
	String qtf;
	Vector<String> data;
	int rows=4;
	int cols=3;
	int i;

	double leftMargin=1.0;
	double topMargin=.5;

	// 8.5" x 600 dots/in = 5100 dots
	// 11" x 600 dots/in = 6600 dots
	Size sz(5100-(1200*leftMargin),6600-(1200*topMargin));
	Report report(sz);
	int rowheight=(sz.cy - 4 * (rows - 1) - 20) /rows;

	for(i=1;i<(rows*cols+1);i++)
		data.Add("cell "+AsString(i));

	qtf << "[ $$0,0#00000000000000000000000000000000:Default]";
	qtf << "[{_}\r\n";
	qtf << "[ {{500";
	for(i=1;i<cols;i++)
		qtf << ":500";
	qtf << "H" << rowheight << "l/0r/0t/0b/0 ";

	// Cell one doesn't start with ::
	qtf << "[s0;= " << data[0] << "]\r\n";

	for(i=1;i<data.GetCount();i++)
	{
		qtf << ":: [s0;= " << data[i] << "]\r\n";
	}
	qtf << "}}]\r\n";

	report << qtf;
	Perform(report);
}
