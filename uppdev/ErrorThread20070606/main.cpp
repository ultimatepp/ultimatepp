#include "main.h"
#include "threads.h"

App::App()
{
	/* Begin: Main settings */
	Title("ErrorThread20070606");
	SetDefaultCharset(CHARSET_UTF8);
	SetRect(0, 0, 800, 500);
	/* End */

	/* Begin: Main Ctrls */
	AddFrame(menu);
	MenuContent(menu);
	Add(tab);	
	TabContent(tab);
	AddFrame(status);
	StatusContent(status);
	/* End */

	/* Begin: Main actions */
	WhenClose = THISBACK(ActionExit);
	/* End */
}

void App::MenuContent(MenuBar& menu)
{
	menu.Add("File", THISBACK(MenuContent_File));
}

void App::MenuContent_File(Bar& bar)
{
	bar.Add("Exit", THISBACK(ActionExit))
	   .Key(K_ESCAPE);
}
  
void App::TabContent(TabCtrl& tab)
{
	tab.SizePos();
	
	Content_Download(contentDownload);
	tab.Add(contentDownload, "Home page");
}

void App::Content_Download(StaticRect& ctrl)
{
	ctrl.SizePos();
	ctrl.Color(SWhite);
	ctrl.SetFrame(InsetFrame());
	
	downloadButton.SetLabel("Download START (~46kB)");
	downloadButton.LeftPos(10, 150).TopPos(10, 20);
	downloadButton <<= THISBACK(ActionDownloadStart);
	ctrl.Add(downloadButton);
	
	cancelDownloadButton.SetLabel("Download STOP");
	cancelDownloadButton.LeftPos(10, 150).TopPos(40, 20);
	cancelDownloadButton.Disable();
	cancelDownloadButton <<= THISBACK1(ActionDownloadStop, true);
	ctrl.Add(cancelDownloadButton);
	
	downloadResult.LeftPos(10, 750).TopPos(75, 330);
	ctrl.Add(downloadResult);
}

void App::ActionDownloadStart()
{
	if (isProgressCanceled) {
		cancelDownloadButton.Enable();
		CallerInfo caller;
		caller.app = this;
		httpThread.Run(callback1(DownloadThread, caller));
		SetTimeCallback(-50, callback(this, &App::StatusUpdate), 0);
		StatusUpdate();
	}
}

void App::ActionDownloadStop(bool clearResult = false)
{
	isProgressCanceled = true;
	cancelDownloadButton.Disable();
	statusInfo = "";
	if (clearResult) {
		String result = "";
		downloadResult.Set(result);
	}
	progressDisplay.x = -95;
	KillTimeCallback(0);
}

bool App::ProgressCanceled(int pos, int size)
{
	return isProgressCanceled;
}

void App::StatusContent(StatusBar& status)
{
	status = GetTitle().ToString();
	statusInfo.SetDefault(" ");
	statusInfo.Set("");	
	status.AddFrame(statusInfo.Right(150));
	progressDisplay.x = -100;
	isProgressCanceled = true;
}

void App::StatusUpdate()
{
	PaintRect rect(progressDisplay);
	statusInfo.Set(rect);
	if (progressDisplay.x >= 150 - 5) {
		progressDisplay.x = -95;
	} else {
		progressDisplay.x += 5;
	}
}

void App::ActionExit()
{
	isProgressCanceled = true;
	Close();
}

bool App::Key(dword key, int count)
{
	/*if(key == K_ESCAPE) {
		ActionExit();
		return true;
	}*/
	return TopWindow::Key(key, count);
}


GUI_APP_MAIN
{
	App app;
	app.Run();
}
