#include <CtrlLib/CtrlLib.h>
#include <MtAlt/MtAlt.h>
#include "Worker.h"
using namespace Upp;

#define LAYOUTFILE <MtAltExample1/MainWindow.lay>
#include <CtrlCore/lay.h>

//thread GUI (main application thread)
class MainWindow : public WithMainWindowLayout<TopWindow>, public CallbackQueue, public Worker::Notify
{
public: //thread GUI
	MainWindow();
	
public: //thread Worker
	virtual void OnWork2Result(int x1, int x2, String s) {Request(&MainWindow::Work2Result,x1,x2,s);}

private:
	void Work1() {worker.Request(&Worker::DoWork1);}
	
	void Work2() {worker.Request(&Worker::DoWork2);}
	void Work2Result(int x1, int x2, String s)
	{
		log.Set(log.Get()+Format("Worker::Work2 %d, %d, %s\n", x1,x2,s));
	}
	
	void DoTasksAndStatistics();
	
	Worker worker;
};


MainWindow::MainWindow()
	:worker(this)
{
	CtrlLayout(*this);
	button1 <<= callback(this, &MainWindow::Work1);
	button2 <<= callback(this, &MainWindow::Work2);
	
	SetTimeCallback(-200, callback(this, &MainWindow::DoTasksAndStatistics));
	worker.Start();
}

void MainWindow::DoTasksAndStatistics()
{
	info.SetLabel(Format(" GUI: %d    Worker: %d", GetTasksCount(), worker.GetTasksCount()));
	DoTasks();
}

GUI_APP_MAIN
{
	MainWindow().Sizeable().Run();
}

