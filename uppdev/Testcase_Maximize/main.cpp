#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class MainWindow : public TopWindow
{
	public:
		MainWindow();
		void LeftUp(Point p, dword keyflags);
		void Paint(Draw &w);
		
	protected:
		bool	m_clicked;
		Point	m_point;
};

MainWindow::MainWindow()
: m_clicked(false), m_point(0, 0)
{}

void MainWindow::LeftUp(Point p, dword keyflags)
{
	m_clicked = true;
	m_point = p;
	Refresh();
	PromptOK("!!!");
}

void MainWindow::Paint(Draw &w)
{
	w.DrawRect(GetWorkArea(), Color(255, 255, 255));
	
	if(m_clicked)
		w.DrawImage(m_point.x, m_point.y, CtrlImg::arrow);
}

GUI_APP_MAIN
{
	MainWindow mw;
	mw.Title("Doubleclick Titlebar").Sizeable().MaximizeBox().MinimizeBox()
		.SetRect(0, 0, 800, 600);
	mw.Run();
}
