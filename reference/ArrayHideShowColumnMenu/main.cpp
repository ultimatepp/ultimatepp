#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <ArrayHideShowColumnMenu/ArrayHideShowColumnMenu.lay>
#include <CtrlCore/lay.h>


class MainWindow : public WithMainWindowLayout<TopWindow> {
public:
	MainWindow();
	void Serialize(Stream& s);
protected:
	void FillTestData();
	void AddName(int i, const char * a_name);
	void OnArrayBar(Bar &menu);
	void ShowColumn(int index, bool show);
};

MainWindow::MainWindow()
{
	CtrlLayout(*this, "Array Hide/Show Column Menu demonstration");
	Sizeable();
	m_array.WhenBar = [=](Bar& menu) { OnArrayBar(menu); };
	m_array.AllSorting();


	//Showing columns
	//m_array.AddColumn("Zero Column");
	
	//hidden by default:
	int i = m_array.AddColumn("First Column").HeaderTab().GetIndex();
	m_array.HeaderObject().ShowTab(i, false);
	
	m_array.AddColumn("Second Column");
	m_array.AddColumn("Third Column");
	//m_array.AddColumn("Fourth Column");
	FillTestData();
}

void MainWindow::Serialize(Stream& s)
{
	int version = 1;
	s / version;
	SerializePlacement(s);
	m_array.SerializeSettings(s);
}

void MainWindow::FillTestData()
{
	AddName(0, "Zero");
	AddName(1, "One");
	AddName(2, "Two");
	AddName(3, "Three");
	AddName(4, "Four");
}

void MainWindow::AddName(int i, const char * a_name)
{
	Vector<Value> vec;
	vec.Add(i);
	vec.Add(a_name);
	vec.Add(i*10);
	m_array.Add(vec);
}

void MainWindow::OnArrayBar(Bar &menu)
{
	HeaderCtrl& header = m_array.HeaderObject();
	for(int i=0; i<header.GetCount(); i++){
		bool visible = header.IsTabVisible(i);
		String label = header[i].GetText();
		menu.Add(label, [=] { ShowColumn(i, !visible); }).Check(visible);
	}
}

void MainWindow::ShowColumn(int index, bool show)
{
	m_array.HeaderObject().ShowTab(index, show);
}



GUI_APP_MAIN
{
	MainWindow mw;
	LoadFromFile(mw);
	mw.Run();
	StoreToFile(mw);
}

