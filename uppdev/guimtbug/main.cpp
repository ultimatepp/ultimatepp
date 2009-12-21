#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class TestThread : public TopWindow
{
	protected:
		bool stop;
		void buttonCb(void);
		Thread thr;
		virtual void thrCb(void);
		
		void Stop();
		
		ProgressIndicator progress;
		Button button;
		StatusBar status;
		
	public:
		typedef TestThread CLASSNAME;

		TestThread();

};

void TestThread::thrCb(void)
{
	{
		GuiLock __;
		status.Set("Running, " + FormatInt(thr.GetCount()) + " threads");
	}
	for(;;)
	{
		{
			GuiLock __;
			if(progress < 100)
				progress++;
			else
				progress = 0;
			if(stop)
				break;
		}
		Sleep(1000);
	}
	{
		GuiLock __;
		status.Set("Idle....");
	}
	PostCallback(THISBACK(Stop));
}

void TestThread::Stop()
{
	button.SetLabel("START");
}


void TestThread::buttonCb(void)
{
	if(stop)
	{
		stop = false;
		button.SetLabel("STOP");
		thr.Run(THISBACK(thrCb));
	}
	else
		stop = true;
}

TestThread::TestThread()
{
	SetRect(0, 0, 300, 150);
	Add(button);
	button.TopPos(90, 30).HCenterPos(100);
	button.SetLabel("START");
	Add(progress);
	progress.TopPos(30, 40).HCenterPos(250);
	progress.Set(0, 100);
	AddFrame(status);
	status.Set(" ");

	stop = true;
	button <<= THISBACK(buttonCb);
		
}

GUI_APP_MAIN
{
	TestThread().Run();
}
