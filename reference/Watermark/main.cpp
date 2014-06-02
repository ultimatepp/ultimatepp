#include <Report/Report.h>

using namespace Upp;

void DrawWatermark(Report *report)
{ // Draw something to the background
	report->DrawText(1000, 1000, 3000, "Watermark", Serif(1000), WhiteGray());
}


GUI_APP_MAIN
{
	Report report;
	report.OnPage(callback1(DrawWatermark, &report)); // called after new page is created
	for(int i = 0; i < 100; i++)
		report << "Report with watermark";
	Perform(report);
}
