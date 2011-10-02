//////////////////////////////////////////////////////////////////////
// DlgServerGui: ISAPI web server connector.

#include "WebCtrl.h"

#pragma hdrstop

#define LNGFILE    <Web/Ctrl/DlgHttpServer.lay>
#define LANGUAGE   LNG_CZECH
#include           <Core/lng_header.h>

#define LNGFILE    <Web/Ctrl/DlgHttpServer.lay>
#define LNGMODULE  Web_Ctrl_DlgHttpServer_lay
#define LANGUAGE   LNG_CZECH
#include           <Core/lng_master.h>
#define LANGUAGE   LNG_ENGLISH
#include           <Core/lng_version.h>

#define LAYOUTFILE <Web/Ctrl/DlgHttpServer.lay>
#include           <CtrlCore/lay.h>

#undef LNGFILE
#undef LNGMODULE

#define LNGFILE    <Web/Ctrl/DlgHttpServer.lng>
#define LANGUAGE   LNG_CZECH
#include           <Core/lng_header.h>
#define LNGFILE    <Web/Ctrl/DlgHttpServer.lng>
#define LNGMODULE  Web_Ctrl_DlgHttpServer
#define LANGUAGE   LNG_CZECH
#include           <Core/lng_master.h>
#define LANGUAGE   LNG_ENGLISH
#include           <Core/lng_version.h>

#include "WebImg.h"

#define IMAGECLASS WebImg
#define IMAGEFILE  <Web/Ctrl/web.iml>
#include           <Draw/iml_source.h>

//////////////////////////////////////////////////////////////////////

void DeleteMask(const char *path, const char *mask)
{
	Vector<String> files;
	FindFile ff;
	if(ff.Search(AppendFileName(path, mask)))
		do
			if(!ff.IsDirectory())
				files.Add(ff.GetName());
		while(ff.Next());
	while(!files.IsEmpty())
		DeleteFile(AppendFileName(path, files.Pop()));
}

//////////////////////////////////////////////////////////////////////

void DeleteMask(const char *mask)
{
	DeleteMask(GetFileDirectory(mask), GetFileNamePos(mask));
}

//////////////////////////////////////////////////////////////////////

template class Moveable<HttpServer *>;

class DlgServerGui : public WithHttpServerLayout<TopWindow>
{
	friend class DlgHttpServer;

public:
	typedef DlgServerGui CLASSNAME;
	DlgServerGui();

	bool                           Run(int timeout);

	void                           AddServer(HttpServer& server);
	void                           RemoveServer(HttpServer& server);
	int                            GetServerCount() const;
	HttpServer&                    GetServer(int i) const;

	One<HttpRequest>               GetRequest();

private:
	void                           OnMinimize();
	void                           OnStop();

protected:
	HttpServer                    *activeserver;
	Index<HttpServer *>            servers;
	GradientDisplay                gradient;
	bool                           minimized;
	bool                           stopped;
	Time                           start_time;
};

//////////////////////////////////////////////////////////////////////

RegisterHelpTopicObjectTitle(DlgHttpServer, s_(DlgHttpServerTitle))

DlgServerGui::DlgServerGui()
: gradient(Point(0, -1), Color(192, 255, 192), Point(-1, 0), White)
{
	activeserver = NULL;
	minimized = false;

	CtrlLayoutOKCancel(*this, DlgHttpServerHelpTitle());
	HelpTopic("DlgHttpServer");
	run_time.SetReadOnly();
	response.SetReadOnly();
	trailing.SetReadOnly();
	elapsed_time.SetReadOnly();
	hits.SetReadOnly();

	Icon(WebImg::small_web_server(), WebImg::large_web_server());

	ActiveFocus(ok);
	Background(gradient);
	MinimizeBox();

	ok <<= THISBACK(OnMinimize);
	cancel <<= WhenClose = THISBACK(OnStop);
	run_time <<= Format(start_time = GetSysTime());
}

//////////////////////////////////////////////////////////////////////

/*
static void LogExc(const Exc& e, WebConfig& cfg)
{
	cfg.Log("exception at " + Format(GetSysTime()) + ": " + e);
}
*/

//////////////////////////////////////////////////////////////////////

bool DlgServerGui::Run(int timeout_msec)
{
	stopped = false;
	activeserver = NULL;
	try
	{
//		if(context.LogRequests)
//			context.Log("normal startup at " + Format(dog_time = GetSysTime()) + "\r\n");

		if(!IsOpen())
		{
			TopWindow::OpenMain();
			if(minimized)
				Minimize(false);
		}

/*
		if(!IsNull(context.dlg_position.x) && context.dlg_position.y >= 0)
		{
			Size dlgSize = dialog.GetScreenRect().Size();
			dialog.SetRect(Rect(minmax<Point>(context.dlg_position, Point(0, 0),
				Point(ScreenInfo().GetPagePixels() - dlgSize)), dlgSize));
		}
#ifdef PLATFORM_MFC
		CWinApp *app = AfxGetApp();
		bool set_main_wnd = false;
		if(app && !app -> m_pMainWnd)
		{
			set_main_wnd = true;
			app -> m_pMainWnd = GetCWnd();
		}
#endif//PLATFORM_MFC
*/

		String text;
		int delta = fround(GetSysTime() - start_time);
		if(delta < 60)
			text << delta << " s";
		else if(delta < 3600)
			text = Format(s_(DlgHttpServerFormatMinSec), delta / 60, delta % 60);
		else
			text = Format(s_(DlgHttpServerFormatHourMinSec), delta / 3600, delta / 60 % 60, delta % 60);
		elapsed_time = text;

		double s_hits = 0, s_response = 0, s_trailing = 0;
		for(int i = 0; i < servers.GetCount(); i++)
		{
			s_hits += servers[i] -> GetHitCount();
			s_response += servers[i] -> GetAvgTime();
			s_trailing += servers[i] -> GetAvgLagTime();
		}
		int ndiv = max(servers.GetCount(), 1);
		hits = AsString(s_hits, 0);
		response = AsString(s_response / ndiv, 0);
		trailing = AsString(s_trailing / ndiv, 0);

		semaphore.SetImage(WebImg::semaphore_green());
		Show();

		ProcessEvents();

		activeserver = HttpServer::Wait(servers.GetKeys(), min(timeout_msec, 100));
		if(activeserver)
		{
			semaphore.SetImage(WebImg::semaphore_red());
			semaphore.Sync();
		}
		return !stopped;
	}
	catch(Exc e)
	{
//		LogExc(e, context);
#ifdef _DEBUG
		PromptOK(e);
#endif//_DEBUG
//		context.exit_code = Null;
		return false;
	}
}

//////////////////////////////////////////////////////////////////////

void DlgServerGui::AddServer(HttpServer& server)
{
	servers.FindAdd(&server);
}

//////////////////////////////////////////////////////////////////////

void DlgServerGui::RemoveServer(HttpServer& server)
{
	int i = servers.Find(&server);
	if(i >= 0)
		servers.Remove(i);
}

//////////////////////////////////////////////////////////////////////

int DlgServerGui::GetServerCount() const
{
	return servers.GetCount();
}

//////////////////////////////////////////////////////////////////////

HttpServer& DlgServerGui::GetServer(int i) const
{
	return *servers[i];
}

//////////////////////////////////////////////////////////////////////

One<HttpRequest> DlgServerGui::GetRequest()
{
	if(!activeserver)
		return NULL;
	One<HttpRequest> request = activeserver -> GetRequest();
	activeserver = NULL;
	return request;
}

//////////////////////////////////////////////////////////////////////

void DlgServerGui::OnMinimize()
{
	Minimize();
}

//////////////////////////////////////////////////////////////////////

void DlgServerGui::OnStop()
{
	stopped = true;
}

//////////////////////////////////////////////////////////////////////
// DlgHttpServer::

DlgHttpServer::DlgHttpServer()
{
	gui = new DlgServerGui;
}

//////////////////////////////////////////////////////////////////////

DlgHttpServer::~DlgHttpServer()
{
}

//////////////////////////////////////////////////////////////////////

void DlgHttpServer::Title(const char *t)
{
	gui -> Title(t);
}

//////////////////////////////////////////////////////////////////////

void DlgHttpServer::Icon(Image small_icon, Image large_icon)
{
	gui -> Icon(small_icon, large_icon);
}

//////////////////////////////////////////////////////////////////////

void DlgHttpServer::Minimize(bool min)
{
	if(!gui -> IsOpen())
		gui -> minimized = min;
	else if(min)
		gui -> Minimize();
	else
		gui -> Restore();
}

//////////////////////////////////////////////////////////////////////

bool DlgHttpServer::IsMinimized() const
{
	return gui -> IsOpen() ? gui -> IsMinimized() : gui -> minimized;
}

//////////////////////////////////////////////////////////////////////

void DlgHttpServer::Position(Point pt)
{
	if(gui -> IsOverlapped())
	{
		Rect old_rc = gui -> GetRect();
		Rect work_rc = Ctrl::GetWorkArea();
		Size rc = old_rc.Size();
		pt.x += (pt.x >= 0 ? work_rc.left : work_rc.right  + pt.x - rc.cx);
		pt.y += (pt.y >= 0 ? work_rc.top  : work_rc.bottom + pt.y - rc.cy);
		gui -> SetRect(old_rc + (pt - old_rc.TopLeft()));
	}
}

//////////////////////////////////////////////////////////////////////

Point DlgHttpServer::GetPosition() const
{
	return gui -> GetRect().TopLeft();
}

//////////////////////////////////////////////////////////////////////

bool DlgHttpServer::Wait(int msec)
{
	return gui -> Run(msec);
}

//////////////////////////////////////////////////////////////////////

One<HttpRequest> DlgHttpServer::GetRequest()
{
	return gui -> GetRequest();
}

//////////////////////////////////////////////////////////////////////

void DlgHttpServer::AddServer(HttpServer& server)
{
	gui -> AddServer(server);
}

//////////////////////////////////////////////////////////////////////

void DlgHttpServer::RemoveServer(HttpServer& server)
{
	gui -> RemoveServer(server);
}

//////////////////////////////////////////////////////////////////////

int DlgHttpServer::GetServerCount() const
{
	return gui -> GetServerCount();
}

//////////////////////////////////////////////////////////////////////

HttpServer& DlgHttpServer::GetServer(int i) const
{
	return gui -> GetServer(i);
}

//////////////////////////////////////////////////////////////////////
