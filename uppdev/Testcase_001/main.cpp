// Testcase
// Datapusher does not pop up in ArrayCtrl when only the last character is hidden (by one pixel)


// -- Includes --
#include <CtrlLib/CtrlLib.h>


using namespace Upp;


// -- Definitions --
class MainWindow : public TopWindow
{
	public:
		MainWindow();

	protected:
		ArrayCtrl report;
};


// -- Implementation --
MainWindow::MainWindow()
{
	Value key, text;

	report.NoHorzGrid();
	report.OddRowColor(Color(226, 234, 247), Color(0, 0, 0));
	report.SetLineCy(Draw::GetStdFontCy() + 8);
	report.Appending();

	report.AddColumn("Icon", 50);
	report.AddColumn("Nr", 50);
	report.AddColumn("Name", 100);
	report.AddColumn("Group", 100);
	report.AddColumn("Subgroup", 200);
	report.AddColumn("Printdate", 150);
	report.AddColumn("CD", 50);
	report.ColumnWidths("66 38 138 138 277 207 70");

	report.Add(text = "Ordner", text = "1.1.2");
	report.Add(text = "Ordner", text = "1.1.3");
	report.Add(text = "Ordner", text = "1.1.4");
	report.Add(text = "Ordner", text = "1.1.5");
	report.Add(text = "Ordner", text = "1.1.6");
	report.Add(text = "Ordner", text = "1.1.7");


	Add(report);
	report.HSizePos(0, 0).VSizePos(0, 0);
	report.Show();
}




GUI_APP_MAIN
{
	MainWindow main;
	main.Run();
}
