// Testcase


// -- Includes --
#include <CtrlLib/CtrlLib.h>


using namespace Upp;


// -- Definitions --
class MainWindow : public TopWindow
{
	public:
		MainWindow();
		
	protected:
		void Message(void) {PromptOK("In Message()");}
	
		StatusBar	m_statusBar;
		MenuBar		m_menu;
		TreeCtrl	m_tree;
};


// -- Implementation --
MainWindow::MainWindow()
{
	AddFrame(m_menu);
	m_menu.Add(true, "Neu", Image(CtrlImg::new_doc), callback(this, &MainWindow::Message));
	
	AddFrame(m_statusBar);
	m_statusBar.Set(String(" "));
	
	
	Add(m_tree);
	m_tree.HSizePos(0, 0).VSizePos(0, 0);
	m_tree.Show();
}




GUI_APP_MAIN
{
	MainWindow main;
	main.Sizeable().Tip("GribError");
	main.Run();
}

