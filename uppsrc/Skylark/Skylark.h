#ifndef _Skylark_Skylark_h
#define _Skylark_Skylark_h

#include <Sql/Sql.h>

namespace Upp {
	
namespace Ini {
extern IniBool skylark_log;
};

String GetThreadName();

#define SKYLARKLOG(x) LOG_(Ini::skylark_log, GetThreadName() << ' ' << x)

class Renderer;
class Http;

struct SessionConfig {
	String cookie;
	String dir;
	int    format;
	SqlId  table, id_column, data_column, lastwrite_column;
	int    expire;
	
	SessionConfig();
};

struct AuthExc : Exc {
	AuthExc(const String& s) : Exc(s) {}
};

struct BadRequestExc : Exc {};

struct SkylarkConfig {
	String                    root;
	VectorMap<String, String> view_var;
	String                    path;
	String                    static_dir;
	SessionConfig             session;
	int                       threads;
	int                       port;
	int                       prefork;
	int                       timeout;
	bool                      use_caching;
};

class SkylarkApp : protected SkylarkConfig {
	TcpSocket    server;
	Mutex        accept_mutex;
	int          main_pid;
	Vector<int>  child_pid;
	bool         quit;
	

	void         ThreadRun();
	void         Broadcast(int signal);
	void         Signal(int signal);
	static void  SignalHandler(int signal);
	void         Main();

	void         FinalizeViews();

	static SkylarkApp *app;

#ifdef PLATFORM_WIN32
	static BOOL WINAPI CtrlCHandlerRoutine(__in  DWORD dwCtrlType);
#endif

	typedef SkylarkApp CLASSNAME;
	
	friend class Http;

public:
	virtual void SqlError(Http& http);
	virtual void InternalError(Http& http);
	virtual void NotFound(Http& http);
	virtual void Unauthorized(Http& http);
	virtual void BadRequest(Http& http);

	virtual void WorkThread();
	
	void RunThread();

	void Run();
	
	static SkylarkApp& TheApp();
	static const SkylarkConfig& Config();

	SkylarkApp();
	virtual ~SkylarkApp();
};

#include "Witz.h"
#include "Http.h"

};

#endif
