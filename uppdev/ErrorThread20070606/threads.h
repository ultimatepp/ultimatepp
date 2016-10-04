#ifndef _ErrorThread_threads_h_
#define _ErrorThread_threads_h_

struct CallerInfo {
	App *app;
};

void DownloadThread(CallerInfo caller)
{
	caller.app->isProgressCanceled = false;

	String result;
	result << "Download started, timeout is set to 30 seconds...";
	caller.app->downloadResult.Set(result);
	caller.app->tab.RefreshFrame();
	
	HttpClient http;
	http.TimeoutMsecs(30000);
	http.URL("http://www.ccl.net/cca/software/UNIX/apache/httpd.conf.3.1b1.txt");
	result = http.Execute(callback(caller.app, &App::ProgressCanceled));

	if (!caller.app->isProgressCanceled) {
		if(result.IsEmpty()) {
			result << "Error: Connection timeout!";
		}
		caller.app->downloadResult.Set(result);
		caller.app->tab.RefreshFrame();
	}
	caller.app->ActionDownloadStop(false);
}

#endif
