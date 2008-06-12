//////////////////////////////////////////////////////////////////////
// wisapi: WISAPI extension library.

#include <Web/Web.h>
#include <TCore/TCore.h>
#include <httpext.h>
#include <wininet.h>
#pragma hdrstop
#include "version.h"

using namespace Upp;

#define TFILE "wisapi.t"
#include <Core/t.h>

enum { DEFAULT_PORT = 1571 };

static void AddHeaders(HttpQuery& query, const char *h)
{
	while(*h)
		if((byte)*h > ' ') {
			String var;
			for(; *h && *h != ':'; h++)
				if(IsAlNum(*h))
					var.Cat(ToUpper(*h));
				else if(*h == '-' && !var.IsEmpty() && *var.Last() != '_')
					var.Cat('_');
			String value;
			LOG("var = " << var);
			if(*h == ':') {
				while((byte)*++h <= ' ' && *h && *h != '\n')
					;
				const char *b = h;
				while(*h && *h++ != '\n')
					;
				const char *e = h;
				while(e > b && (byte)e[-1] < ' ')
					e--;
				value = String(b, e);
				LOG("value = " << value);
			}
			if(!IsNull(var)) {
				var.Insert(0, "$$");
				String old = query.GetString(var);
				if(!IsNull(old) && !IsNull(value)) {
					old << ", " << value;
					value = old;
				}
				LOG("AddHeaders: " << var << "=" << value);
				query.Set(var, value);
			}
		}
		else
			h++;
}

class CheckedSection {
public:
	CheckedSection() : lock(0) { InitializeCriticalSection(&section); }
	~CheckedSection()          { DeleteCriticalSection(&section); }

	void            Enter()    { EnterCriticalSection(&section); AtomicInc(lock); }
	void            Leave()    { LeaveCriticalSection(&section); AtomicDec(lock); }

	int             Get() const { return lock; }

	class Lock
	{
	public:
		Lock(CheckedSection& cs) : cs(cs) { cs.Enter(); }
		~Lock()                           { cs.Leave(); }

	private:
		CheckedSection& cs;
	};

private:
	CRITICAL_SECTION section;
	Atomic           lock;
};

/*
class SHandle
{
public:
	SHandle(SC_HANDLE handle = 0) : handle(handle) {}
	~SHandle() { if(handle) CloseServiceHandle(handle); }

	operator SC_HANDLE () const             { return handle; }

	void           Clear()                  { Attach(0); }

	SC_HANDLE      Detach()                 { SC_HANDLE h = handle; handle = 0; return h; }
	void           Attach(SC_HANDLE h)      { if(handle) CloseServiceHandle(handle); handle = h; }

	SHandle&       operator = (SC_HANDLE h) { Attach(h); return *this; }

	bool           Query(SERVICE_STATUS& s) { return handle && QueryServiceStatus(handle, &s); }

private:
	SC_HANDLE      handle;
};

struct ServiceInfo
{
	String display_name;
	String service_name;
	dword  state;
};
template class Moveable<ServiceInfo>;

static Array<ServiceInfo> EnumServices(SHandle& manager)
{
	dword resume_handle = 0;
	dword more = 0;
	bool cont;
	Array<ServiceInfo> list;
	do
	{
		dword done;
		union
		{
			ENUM_SERVICE_STATUS status[1];
			char                buffer[10000];
		};
		bool result = EnumServicesStatus(manager, SERVICE_WIN32,
			SERVICE_STATE_ALL, status, sizeof(buffer), &more, &done, &resume_handle);
		cont = (!result && GetLastError() == ERROR_MORE_DATA);
		if(!result && !cont)
			break;
		for(int i = 0; i < done; i++)
		{
			ServiceInfo& info = list.Add();
			info.display_name = status[i].lpDisplayName;
			info.service_name = status[i].lpServiceName;
			info.state        = status[i].ServiceStatus.dwCurrentState;
		}
	}
	while(cont);
	return list;
}
*/

static HtmlTag HtmlSimpleRow()
{
	return HtmlRow() % HtmlCell().ColSpan(2).Width(-100);
}

static Htmls HtmlSplitRow(const char *tag, const Htmls& content)
{
	return HtmlRow() % (HtmlCell().Width(-30) / tag + "\n" + HtmlCell().Left().Width(-70) / content);
}

static Htmls HtmlValueRow(const char *tag, const Value& value)
{
	return HtmlSplitRow(tag, HtmlBold() / StdFormat(value));
}

static Htmls HtmlMenu(bool active, const char *text, const char *url, int wd)
{
	HtmlTag table = HtmlTable().CellSpacing(0).Border(2)
		.BorderColor(Black).BgColor(active ? Yellow : WhiteGray);
	if(wd)
		table.Width(wd);
	return table % HtmlRow() % HtmlCell().Center()
		/ HtmlLink(url) / HtmlFontColor(Black) / (active ? HtmlBold() / text : Htmls(text));
}

static Htmls HtmlHardSpace(int count = 1)
{
	StringBuffer s(count * 6);
	char *p = s;
	for(int i = 0; i < count; i++) {
		memcpy(p, "&nbsp;", 6);
		p += 6;
	}
	return String(s);
}

static HtmlTag HtmlIsapiTable()
{
	return HtmlTable().Width(-100).CellSpacing(0).CellPadding(1).Border(1).BgColor(WhiteGray)
		% HtmlRow() % HtmlCell()
		% HtmlTable().Width(-100).CellSpacing(2).CellPadding(0).Border(0).BgColor(WhiteGray);
}

static Htmls HtmlButton(const char *text, const char *id)
{
	return HtmlSubmit(text).Name(id);
}

static String ReadServerVariable(EXTENSION_CONTROL_BLOCK *ecb, const char *varname)
{
	byte buffer[1024];
	DWORD needed = __countof(buffer);
	if(ecb->GetServerVariable(ecb->ConnID, (char *)varname, buffer, &needed))
		return String(buffer, (int)needed - 1);
	StringBuffer strbuf(needed);
	if(ecb->GetServerVariable(ecb->ConnID, (char *)varname, strbuf, &needed)) {
		strbuf.SetLength((int)needed - 1);
		return strbuf;
	}
	return String::GetVoid();
}

static void WriteHeader(EXTENSION_CONTROL_BLOCK *ecb, const char *response, const char *header)
{
//	LOGHEXDUMP(header, strlen(header));
	LOG("WriteHeader(response = " << (response ? response : "(null)") << ", headers = " << (header ? header : "(null)") << ")");
	ecb->ServerSupportFunction(ecb->ConnID, HSE_REQ_SEND_RESPONSE_HEADER, (void *)response, 0, (dword *)header);
}

static void WriteClient(EXTENSION_CONTROL_BLOCK *ecb, const void *data, int count)
{
	dword c = (count >= 0 ? count : (int)strlen((const char *)data));
//	LOG("client output (" << (int)c << "bytes): " << (const char *)data << "(eocd)");
//	LOGHEXDUMP(data, count);
//	LOG("(eocd raw)");
	ecb->WriteClient(ecb->ConnID, (void *)data, &c, HSE_IO_SYNC);
}

static void WriteClient(EXTENSION_CONTROL_BLOCK *ecb, const String& data)
{
	WriteClient(ecb, data, data.GetLength());
}

static HINSTANCE       dll_module;
static String          dll_filename;
static bool            log_isapi = true;
static bool            show_headers = false;
static bool            remote_admin = true;
static bool            isapi_english = true;
static int             isapi_timeout = 5000;
static long            log_id = 0;
static CheckedSection  log_lock;
static int             retry_msec = 1000;

static void WriteLog(const String& s)
{
	CheckedSection::Lock guard(log_lock);
	Stream& strm = VppLog();
	int i = GetTickCount();
	int l = InterlockedIncrement(&log_id);
	String out = NFormat("%d.%d, #%d, T%d: %s\n", i / 1000, (i / 100) % 10, l, (int)GetCurrentThreadId(), s);
	strm.Put(out);
	strm.Flush();
}

#define ISAPILOG(x) if(log_isapi) WriteLog(String() << x); else ;
#define ISAPIDEBUG(x) if(log_isapi) { ISAPILOG("<< " #x); x; ISAPILOG(">> " #x); } else { x; }

//timeout_title[] = "Chyba: server je pøetížen",
//timeout_desc[]  = "Pøipojení k internetovému serveru se nezdaøilo,\n"
//"protože server je plnì vytížen vyøizováním požadavkù ostatních klientù.\n"
//"\n"
//"<P>\n"
//"Zkuste se prosím pøipojit pozdìji.\n";

static String GetError(const char *title, const char *desc)
{
	return String() <<
		"\n"
		"<HTML>\n"
		"<HEAD>\n"
		"<TITLE>" << title << "</TITLE>\n"
		"</HEAD>\n"
		"<BODY>\n"
		"<H1>" << title << "</H1>\n"
		"<P>\n"
		<< desc <<
		"\n"
		"</BODY>\n"
		"</HTML>\n";
}

static String Trim(const char *s, const char *e)
{
	while(s < e && (byte)*s <= ' ')
		s++;
	while(e > s && (byte)e[-1] <= ' ')
		e--;
	return String(s, e);
}

class WIsapiClient
{
public:
	WIsapiClient();
	~WIsapiClient();

	dword             Run(int cid, EXTENSION_CONTROL_BLOCK *ecb);

private:
	void              ReadPostData(HttpQuery& query, EXTENSION_CONTROL_BLOCK *ecb);
	bool              Run(int cid, const HttpQuery& query, EXTENSION_CONTROL_BLOCK *ecb);

	Htmls             Config(const HttpQuery& query);
	Htmls             Body(String english);
	Htmls             Configuration(String english);
	Htmls             Login();
	Htmls             English();
	Htmls             Logging();
	Htmls             ShowHeaders();
	Htmls             Timing();
	Htmls             RemoteAdmin();
//	HtmlTag           Edit(Id id, int& value);
	Htmls             Servers();
//	Htmls             Commit();

	Htmls             Statistics();

	Htmls             Services();
	Htmls             ServiceTable();

	bool              CanWrite();

	void              AddHost(const String& host);
	void              RemoveHost(const String& host);

	void              LoadConfig();
	void              SaveConfig();

private:
	class Connection
	{
	public:
		Connection(WIsapiClient& owner, const String& raw_host);

		bool        Run(int conn_id, const HttpQuery& query, One<CheckedSection::Lock>& owner_lock, EXTENSION_CONTROL_BLOCK *ecb);
		String      RawRun(const HttpQuery& query, EXTENSION_CONTROL_BLOCK *ecb, int& data_length);
		void        Kill();
		void        Status();
		void        CONNLOG(const String& log);

	public:
		// configuration
		String          raw_host;
		String          host;
		int             port;
		String          status;
		dword           retry_ticks;
		bool            is_error;
		bool            is_busy;

		// statistics
		Time            run_time;
		double          hit_count;
		int             last_msecs;
		double          total_msecs;
		double          total_bytes;
		int             max_msecs;
		double          error_count;

		// state parameters
		int             conn_id;
		CheckedSection  conn_lock;
		WIsapiClient&   owner;
		HttpQuery       query;
		Socket          socket; // server connection
	};
	friend class Connection;

	Array<Connection> connections;
	CheckedSection    client_lock;
	Event             free_event;

	Time              run_time;
	double            hit_count;
	double            total_msecs;
	double            total_bytes;
	double            max_msecs;
	double            error_count;

	HttpQuery         cfg_query;
	String            cfg_filename;
	String            cfg_default_host;
	String            cfg_error;
	String            cfg_url;
	String            cfg_password;
	int               cfg_can_write;
};

WIsapiClient::Connection::Connection(WIsapiClient& owner, const String& raw_host)
: owner(owner), raw_host(raw_host)
{
	const char *p = raw_host;
	while(*p && *p != '@')
		p++;
	host = String(raw_host.Begin(), p);
	port = DEFAULT_PORT;
	if(*p++ == '@' && IsDigit(*p))
		port = stou(p);
	hit_count = total_msecs = total_bytes = error_count = 0;
	max_msecs = 0;
	last_msecs = 0;
	run_time = GetSysTime();
	is_busy = false;
	is_error = false;
	retry_ticks = GetTickCount();
	Status();
}

void WIsapiClient::Connection::CONNLOG(const String& log)
{
	ISAPILOG("Connection(" << conn_id << "): " << log);
}

void WIsapiClient::Connection::Status()
{
	ISAPILOG("Connection::Status->OpenClient");
	dword ticks = GetTickCount();
	String response;
	ISAPILOG("Connection::Status->OpenClient, opening socket");
	if(ClientSocket(socket, host, port)) {
		ISAPILOG("Connection::Status->ClientSocket open, sending STAT");
		socket.Write("STAT\n");
		response = socket.ReadUntil('\n');
		if(!socket.Close())
		{
			status = Socket::GetErrorText();
			ISAPILOG("Connection::Status->" << status);
			is_error = true;
		}
		else
		{
			ISAPILOG("-> response=" << response);
			if(!strnicmp(response, "READY ", 6))
			{
				status = response.Mid(6);
				is_error = false;
				return;
			}
			status = t_("Invalid server response: ") + response;
		}
	}
	else {
		ISAPILOG("Connection::Status->ClientSocket failed to open, " << Socket::GetErrorText());
		status = NFormat(t_("Error opening socket %s:%d: %s"), host, port, Socket::GetErrorText());
	}
	is_error = true;
	dword new_ticks = GetTickCount();
	retry_ticks = new_ticks + minmax(retry_msec, 10 * (int)(new_ticks - ticks), 60000);
	ISAPILOG("retry ticks = " << (int)retry_ticks);
}

bool WIsapiClient::Connection::Run(int cid, const HttpQuery& new_query, One<CheckedSection::Lock>& owner_lock, EXTENSION_CONTROL_BLOCK *ecb)
{
	{
		CheckedSection::Lock conn_guard(conn_lock);

		if(is_busy) {
			CONNLOG("connection busy, ignoring new request " + FormatInt(cid));
			return false;
		}

		if(is_error) {
			int delta = GetTickCount() - retry_ticks;
			ISAPILOG("WIsapiClient::Connection; retry in " << delta);
			if(delta >= 0)
				Status();
			if(is_error)
				return false;
		}

		conn_id = cid;
		is_busy = true;
	}

	CONNLOG(String() << "before leaving client lock: " << owner.client_lock.Get());
	CONNLOG(String() << "owner lock = " << FormatIntHex(~owner_lock));
	owner_lock.Clear();
//	owner.client_lock.Leave();
	CONNLOG(String() << "after leaving client lock: " << owner.client_lock.Get());

	dword start_ticks = GetTickCount();

	int data_length = 0;
	String error = RawRun(new_query, ecb, data_length);

	CONNLOG(String() << "before re-entering client lock: " << owner.client_lock.Get());
	owner_lock = new CheckedSection::Lock(owner.client_lock);
	CONNLOG(String() << "after re-entering client lock: " << owner.client_lock.Get());
//	owner.client_lock.Enter();

	bool is_ok = IsNull(error);
	if(is_error = !is_ok)
	{
		CONNLOG(error);
		socket.Close();
	}

	{
		CheckedSection::Lock conn_guard(conn_lock);
		hit_count++;
		owner.hit_count++;
		if(is_ok)
		{
			last_msecs = GetTickCount() - start_ticks;
			total_msecs += last_msecs;
			owner.total_msecs += last_msecs;

			total_bytes += data_length;
			owner.total_bytes += data_length;

			if(last_msecs > max_msecs) max_msecs = last_msecs;
			if(last_msecs > owner.max_msecs) owner.max_msecs = last_msecs;
		}
		else
		{
			error_count++;
			owner.error_count++;
			is_busy = false;
		}
	}

	if(is_ok)
		owner.free_event.Set();

	return true;
}

String WIsapiClient::Connection::RawRun(const HttpQuery& new_query, EXTENSION_CONTROL_BLOCK *ecb, int& data_length)
{
	CONNLOG("OpenClient");
	if(!ClientSocket(socket, host, port))
		return Socket::GetErrorText();
	CONNLOG(String() << "OpenClient->socket " << socket.GetNumber());
	query = new_query;
	StringStream out;
	out.Put("SAPI");
	out.PutIL(0);
	out % query;
	out.Seek(4);
	out.PutIL((int)(out.GetSize() - 8));
	socket.Write(out.GetResult());
//		socket.StopWrite();

	CONNLOG("executing");
	String line = socket.ReadUntil('\n');
	if(socket.IsError())
		return Socket::GetErrorText();
	CONNLOG("-> response=" + line);
	int trim = line.GetLength();
	while(trim > 0 && line[trim - 1] <= ' ')
		trim--;
	line.Trim(trim);
	if(trim < 5 || memicmp(line, "HTTP/", 5))
		return NFormat(t_("Invalid server response: %s\n"
	"<br>...\n"), HtmlTag("TT") / ToHtml(line));

	const char *p = line.Begin() + 5;
	while(*p && *p != ' ')
		p++;
	String status_line;
	if(*p == ' ' && IsDigit(*++p)) {
		status_line = p;
	}
	int content_length = 0;
	String headers;
	for(;;)
	{
		line = socket.ReadUntil('\n');
		if(socket.IsError())
			return Socket::GetErrorText();

		CONNLOG(String() << "-> header=" << line);
		for(p = line; *p && (byte)*p <= ' '; p++)
			;
		const char *b = p, *e = line.End();
		while(e > b && (byte)e[-1] < ' ')
			e--;
		if(b >= e)
		{
			headers.Cat("\r\n");
			break;
		}
		static const char cl[] = "content-length:";
		enum
		{
			CL_LENGTH = sizeof(cl) - 1,
			MAX_DATA_SIZE = 10000000,
		};
		if(!strnicmp(p, cl, CL_LENGTH))
		{
			for(p += CL_LENGTH; *p == ' '; p++)
				;
			if(IsDigit(*p))
				content_length = minmax<int>(stou(p), 0, MAX_DATA_SIZE);
		}
		headers.Cat(b, (int)(e - b));
		headers.Cat("\r\n");
	}
	String body;
	while(body.GetLength() < content_length) {
		String part = socket.Read(isapi_timeout);
		if(part.IsEmpty())
			return NFormat("Partial input received, %d out of %d for #%d", body.GetLength(), content_length, conn_id);
		body.Cat(part);
	}
	CONNLOG(String() << "-> transmitted " << body.GetLength() << " bytes");

	socket.Close();
	int old_id = conn_id;

	{
		CheckedSection::Lock guard(conn_lock);
		is_busy = false;
		CONNLOG("-> connection freed");
	}
	owner.free_event.Set();

	WriteHeader(ecb, !IsNull(status_line) ? ~status_line : NULL, headers);
	WriteClient(ecb, body);

	data_length = headers.GetLength() + body.GetLength();

	return Null;
}

WIsapiClient::WIsapiClient()
{
	RLOGBLOCK("WIsapiClient constructor");
	cfg_filename = ForceExt(dll_filename, ".cfg");
	cfg_default_host << "localhost@" << (int)DEFAULT_PORT;
	hit_count = total_msecs = total_bytes = max_msecs = error_count = 0;
	run_time = GetSysTime();
	show_headers = false;
	LoadConfig();
}

WIsapiClient::~WIsapiClient()
{
}

void WIsapiClient::ReadPostData(HttpQuery& query, EXTENSION_CONTROL_BLOCK *ecb)
{
	unsigned left = ecb->cbTotalBytes;
	unsigned length = left;
	StringBuffer b(length);
	char *out = b;
	if(ecb->cbAvailable)
	{
		unsigned count = min<unsigned>(ecb->cbAvailable, left);
		memcpy(out, ecb->lpbData, count);
		out += count;
		left -= count;
	}
	while(left > 0)
	{
		dword count = left;
		if(!ecb->ReadClient(ecb->ConnID, out, &count) || count > left)
		{
			ISAPILOG(">>> HttpExtensionProc: POST fetch error");
			return; // error reading client data
		}
		left -= count;
		out += count;
	}
	String buffer = b;
	if(buffer.Find('\n') < 0)
	{
		query.SetURL(buffer);
		return;
	}
	GetHttpPostData(query, buffer);
}

bool WIsapiClient::Run(int cid, const HttpQuery& query, EXTENSION_CONTROL_BLOCK *ecb)
{
	dword ticks = GetTickCount();

	ISAPILOG("WIsapiClient(" << cid << "): assigning request");
	enum { ASSIGN_TIMEOUT = 60000 };
	do {
		ISAPILOG("WIsapiClient(" << cid << "): before entering client lock: " << client_lock.Get());
		One<CheckedSection::Lock> lock = new CheckedSection::Lock(client_lock);
		ISAPILOG("WIsapiClient(" << cid << "): after entering client lock: " << client_lock.Get());
//		client_lock.Enter();
		if(connections.IsEmpty())
			return false;
		int repcnt = connections.GetCount();
		int i = 0;
		if(repcnt > 0) {
			int ticks = connections[0].last_msecs;
			for(int t = 1; t < connections.GetCount(); t++)
				if(connections[t].last_msecs < ticks)
					ticks = connections[i = t].last_msecs;
		}
		while(--repcnt >= 0) {
			Connection& conn = connections[i];
			ISAPILOG("WIsapiClient(" << cid << "): trying connection #" << i);
			if(conn.Run(cid, query, lock, ecb)) {
//				client_lock.Leave();
				ISAPILOG("WIsapiClient(" << cid << "): request completed, lock = " << client_lock.Get());
				return true;
			}
			if(++i >= connections.GetCount())
				i = 0;
		}
		ISAPILOG("WIsapiClient(" << cid << "): before leaving client lock: " << client_lock.Get());
		lock.Clear();
//		client_lock.Leave();
		ISAPILOG("WIsapiClient(" << cid << "): after leaving client lock: " << client_lock.Get());
		ISAPIDEBUG(free_event.Wait(500);)
	}
	while(int(GetTickCount() - ticks) <= ASSIGN_TIMEOUT);
	ISAPILOG("WIsapiClient(" << cid << "): request assignment timeout\n");
	return false;
}

dword WIsapiClient::Run(int cid, EXTENSION_CONTROL_BLOCK *ecb)
{
/*	bool empty = false;
	for(int i = 0; i < connections.GetCount(); i++)
		if(!connections[i].ecb)
		{
			empty = true;
			break;
		}
	if(!empty)
	{
		ISAPILOG("ECB busy");
		return HSE_STATUS_SUCCESS;
	}
*/
	int start_time = GetTickCount();

	ISAPILOG("WIsapiClient(" << cid << ")");

	ecb->dwHttpStatusCode = HTTP_STATUS_OK;

//	ISAPILOG("ALL_HTTP:\n" << ReadServerVariable(ecb, "ALL_HTTP"));
//	ISAPILOG("ALL_RAW:\n" << ReadServerVariable(ecb, "ALL_RAW"));
//	ISAPILOG("AUTH_PASSWORD:\n" << ReadServerVariable(ecb, "AUTH_PASSWORD"));
//	ISAPILOG("AUTH_TYPE:\n" << ReadServerVariable(ecb, "AUTH_TYPE"));
//	ISAPILOG("AUTH_USER:\n" << ReadServerVariable(ecb, "AUTH_USER"));
//	ISAPILOG("LOCAL_ADDR:\n" << ReadServerVariable(ecb, "LOCAL_ADDR"));

	HttpQuery query;

	AddHeaders(query, ReadServerVariable(ecb, "ALL_RAW"));

	query.Set("$$IPADDR", ReadServerVariable(ecb, "REMOTE_ADDR"));
	query.Set("$$WISAPI", dll_filename);
	query.Set("$$METHOD", ecb->lpszMethod);
	query.Set("$$QUERY",  ecb->lpszQueryString);
	query.SetInt("$$HTTP_VERSION", 1000);
	query.SetURL(ecb->lpszQueryString);
	if(ecb->lpszPathInfo && *ecb->lpszPathInfo)
		query.Set("$$PATH", ecb->lpszPathInfo);
	else
//		query.Set(WID__PATH, "http://www.wmap.cz:1571/atlaszp");
		query.Set("$$PATH", GetFileName(dll_filename));
	query.Set("$$CONTENT_TYPE", ecb->lpszContentType);

	if(!stricmp(ecb->lpszMethod, "POST"))
		ReadPostData(query, ecb);

	ISAPILOG("WIsapiClient(" << cid << "): query = " << query);

	String out;

	if(query.Find("CONFIGURE") >= 0)
	{ // config page
		out = Config(query);
		String header = HttpContentType(HttpTextHtml(CHARSET_DEFAULT));
		header << "Content-length: " << out.GetLength() << "\r\n\r\n";
		WriteHeader(ecb, NULL, header);
		WriteClient(ecb, out);
		return HSE_STATUS_SUCCESS;
	}

	if(Run(cid, query, ecb)) {
		ISAPILOG("end: status = " << (int)ecb->dwHttpStatusCode);
		return HSE_STATUS_SUCCESS;
//			ISAPILOG("-> accepted");
//			ecb->dwHttpStatusCode = HTTP_STATUS_ACCEPTED;
//			return HSE_STATUS_PENDING;
	}

	CheckedSection::Lock guard(client_lock);
	if(connections.IsEmpty())
		out << t_("Currently there are no servers connected.\n"
	"You can connect servers using ")
		<< HtmlLink(query.GetString("$$PATH") + "?configure") / t_("the configuration page")
		<< ".";
	else
		out << t_("No connected server can process the current request.");

	ISAPILOG("WIsapiClient(" << cid << "):->error page: " << out);
	out = GetHttpErrorPage(query, out, show_headers);
	String header = HttpContentType(HttpTextHtml(CHARSET_DEFAULT));
	header << "Content-length: " << out.GetLength() << "\r\n\r\n";
	WriteHeader(ecb, NULL, header);
	WriteClient(ecb, out);
	return HSE_STATUS_SUCCESS;
}

static const char *Title() { return t_("Web server connector configuration"); }

Htmls WIsapiClient::Config(const HttpQuery& query)
{
	CheckedSection::Lock guard(client_lock);

	cfg_url = GetHttpPath(cfg_query = query) + "?configure";
	cfg_can_write = -1;
	cfg_error = Null;

	String eng = English();
	return HtmlTitlePage(Title(), Body(eng));
}

#if defined(CPU_AMD64)
	#define ARCHITECTURE "x64"
#elif defined(CPU_IA32)
	#define ARCHITECTURE "x86"
#else
	#define ARCHITECTURE "unknown"
#endif

Htmls WIsapiClient::Body(String eng)
{
	Htmls body;
	body
		<< HtmlTag("SMALL") / t_("Architecture: ")
		<< HtmlTag("TT") / ToHtml(ARCHITECTURE) << "<BR>\n"
		<< HtmlTag("SMALL") / t_("Library location: ")
		<< HtmlTag("TT") / ToHtml(dll_filename) << "<BR>\n"
		<< HtmlTag("SMALL") / (String() << t_("Version: <B>")
			<< WISAPI_VERSION << t_("</B>, release date: <B>")
			<< WISAPI_DATE << "</B><BR>\n" WISAPI_COPYRIGHT "<P>\n");

	String s = cfg_query.GetString("CONFIGURE");
	enum { CONFIG, STAT /*, SERV */ } page = CONFIG;
	switch(*s)
	{
	case 's': page = STAT; break;
//	case 'r': page = SERV; break;
	}
	Htmls menu;
	if(remote_admin)
	{
		menu
			<< HtmlCell() / ::HtmlMenu(page == CONFIG, t_("Configuration"), cfg_url, 120)
			<< HtmlCell() / HtmlHardSpace(2);
	}
	else
		page = STAT;
	menu
		<< HtmlCell() / ::HtmlMenu(page == STAT, t_("Statistics"), cfg_url + "=s", 120)
//		<< HtmlCell() / HtmlHardSpace(2)
//		<< HtmlCell() / HtmlMenu(page == SERV, "Služby", cfg_url + "=r", 120)
	;

	body << HtmlTable().CellSpacing(0) % HtmlRow() % menu
		<< "<P>\n";

	switch(page)
	{
	case CONFIG: body << Configuration(eng); break;
	case STAT:   body << Statistics(); break;
//	case SERV:   body << Services(); break;
	}

	return body;
}

Htmls WIsapiClient::Configuration(String eng)
{
	Htmls content;
	content << Login()
		<< HtmlSimpleRow() << eng
		<< HtmlSimpleRow() << Logging()
		<< HtmlSimpleRow() << ShowHeaders()
		<< HtmlSimpleRow() << Timing()
		<< HtmlSimpleRow() << RemoteAdmin()
		<< HtmlSimpleRow() << Servers();
//	content // must be the last one as Timing() and Servers() might modify some client data
//		<< HtmlSimpleRow() << Commit();
	if(!IsNull(cfg_error))
		content = HtmlSimpleRow() / HtmlFontColor(LtRed) / (HtmlBig() / (t_("Error: ") + HtmlBold() / cfg_error))
		+ HtmlSimpleRow() + content;
	return HtmlTag("FORM").Attr("METHOD", "POST").Attr("ACTION", cfg_url) % HtmlIsapiTable() % content;
}

Htmls WIsapiClient::English()
{
	int engstate = Null;
	if(!cfg_query.IsEmpty("ENGLISH"))
		engstate = 1;
	else if(!cfg_query.IsEmpty("CZECH"))
		engstate = 0;
	if(!IsNull(engstate) && CanWrite()) {
		bool new_eng = !!engstate;
		if(new_eng != isapi_english)
		{
			isapi_english = new_eng;
			SaveConfig();
			SetLanguage(isapi_english ? LNG_ENGLISH : LNG_CZECH);
		}
	}
	return HtmlSplitRow(HtmlBig() / (t_("ISAPI library language: ")
		+ HtmlFontColor(LtRed) / t_("english")),
		HtmlButton(isapi_english ? "  Czech " : " Anglicky ", isapi_english ? "CZECH" : "ENGLISH"));
}

Htmls WIsapiClient::Logging()
{
	int logstate = Null;
	if(!cfg_query.IsEmpty("LOGON"))
		logstate = 1;
	else if(!cfg_query.IsEmpty("LOGOFF"))
		logstate = 0;
	if(!IsNull(logstate) && CanWrite())
	{
		bool new_log = !!logstate;
		if(new_log != log_isapi)
		{
			log_isapi = new_log;
			SaveConfig();
		}
	}
	return HtmlSplitRow(HtmlBig() / (t_("Log-file: ")
		+ HtmlFontColor(LtRed) / (log_isapi ? t_("yes") : t_("no"))),
		HtmlButton(log_isapi ? t_("Disable") : t_("Enable"), log_isapi ? "LOGOFF" : "LOGON"));
}

Htmls WIsapiClient::ShowHeaders()
{
	int hdrstate = Null;
	if(!cfg_query.IsEmpty("HDRON"))
		hdrstate = 1;
	else if(!cfg_query.IsEmpty("HDROFF"))
		hdrstate = 0;
	if(!IsNull(hdrstate) && CanWrite())
	{
		bool new_hdr = !!hdrstate;
		if(new_hdr != show_headers)
		{
			show_headers = new_hdr;
			SaveConfig();
		}
	}
	return HtmlSplitRow(HtmlBig() / (t_("Show headers on error: ")
		+ HtmlFontColor(LtRed) / (show_headers ? t_("yes") : t_("no"))),
		HtmlButton(show_headers ? t_("Disable") : t_("Enable"), show_headers ? "HDROFF" : "HDRON"));
}

Htmls WIsapiClient::RemoteAdmin()
{
	bool noadmin = false;
	if(!cfg_query.IsEmpty("NOADMIN1") && CanWrite())
	{
		remote_admin = false;
		SaveConfig();
		return HtmlSplitRow(HtmlBig() / (t_("Remote administration: ") + HtmlFontColor(LtRed) / t_("off")), Null);
	}
	if(!cfg_query.IsEmpty("NOADMIN") && CanWrite())
		return HtmlSplitRow(HtmlBig() / HtmlFontColor(LtRed) / (t_("After confirmation the remote "
	"administration option will be disabled. You can re-enable remote "
	"administration by removing the line ADMIN=0 from the "
	"configuration file ")
			+ cfg_filename + "."), HtmlButton(t_(" Confirm "), "NOADMIN1"));
	return HtmlSplitRow(HtmlBig() / (t_("Remote administration: ") + HtmlFontColor(LtRed) / t_("on")),
		HtmlButton(t_("  Disable  "), "NOADMIN"));
}

Htmls WIsapiClient::Timing()
{
	if(!cfg_query.IsEmpty("SAVE_TIMEOUT") && CanWrite())
	{
		isapi_timeout = cfg_query.GetInt("TIMEOUT", 1000, 60000, 1000);
		SaveConfig();
	}
	return HtmlSplitRow(HtmlBig() / (t_("Internal timeout (msec):")),
		HtmlEdit("TIMEOUT").Data(cfg_query.GetString("TIMEOUT", IntStr(isapi_timeout)))
		+ "&nbsp;&nbsp;" + HtmlButton(t_(" Commit "), "SAVE_TIMEOUT"));
}

/*
HtmlTag WIsapiClient::Edit(Id id, int& value)
{
	int vsec = value / 1000;
	int new_value = cfg_query.GetInt(id);
	if(!IsNull(new_value) && new_value != vsec)
	{
		if(new_value >= 1 && new_value <= 600)
			value = new_value * 1000;
		else
			cfg_error = "Hodnota je mimo povolený rozsah (1 - 600 s).";
	}
	if(IsNull(new_value))
		new_value = vsec;
	return HtmlEdit(id).Data(IntStr(new_value)).Size(6);
}
*/

static Htmls HtmlTableRow(const Htmls& host, const Htmls& state, const Htmls& modf, Color c)
{
	Htmls cells;
	cells << HtmlCell().Width(-30) % host;
	if(!modf.IsEmpty())
		cells
			<< HtmlCell().Width(-60) % state
			<< HtmlCell().Width(-10) % modf;
	else
		cells
			<< HtmlCell().ColSpan(3).Width(-100) % state;
	return HtmlRow().BgColor(c) % cells;
}

static const char *NoGenerators() { return t_("Currently no generators are connected."); }

static Htmls NoGenMsg()
{
	return HtmlFontColor(LtBlue) / HtmlTag("I") / NoGenerators();
}

Htmls WIsapiClient::Servers()
{
	int count = cfg_query.GetInt("HOSTCOUNT");
	if(!IsNull(count))
	{
		for(int i = 0; i < count; i++)
			if(!cfg_query.IsEmpty("RMHOST" + IntStr(i)))
			{ // remove this host
				String hostid = cfg_query.GetString("HOST" + IntStr(i));
				if(CanWrite())
					try
					{
						RemoveHost(hostid);
					}
					catch(Exc e)
					{
						cfg_error = e;
					}
				break;
			}
	}

	if(!cfg_query.IsEmpty("ADDHOST"))
	{
		String ah = cfg_query.GetString("ADDHOST");
		ISAPILOG("ADDHOST=<" << ah << "> (" << ah.GetLength() << " bytes)");
		if(CanWrite())
			try
			{
				// advance current port number
				String s = cfg_query.GetString("NEW_HOST");
				AddHost(s);
				int i = s.Find('@');
				int port = DEFAULT_PORT;
				if(i < 0)
					i = s.GetLength();
				else
				{
					port = stou(s.Begin() + i + 1);
					if(port == -1)
						port = DEFAULT_PORT;
				}
				s.Trim(i);
				s << '@' << (port + 1);
				cfg_query.Set("NEW_HOST", s);
			}
			catch(Exc e)
			{
				cfg_error = e;
			}
	}

	Htmls table;
	table << HtmlTableRow(HtmlBold() / t_("Generator"), HtmlBold() / t_("Active state"), "", Yellow);

	// output connection parameters
	count = connections.GetCount();
	table << HtmlHidden("HOSTCOUNT", count) << "\n";
	for(int i = 0; i < count; i++)
	{
		Connection& conn = connections[i];
		CheckedSection::Lock guard(conn.conn_lock);
		table << HtmlHidden("HOST" + IntStr(i), conn.raw_host) << "\n";
		String status;
		if(conn.is_busy)
			status = t_("(working)") + conn.status;
		else if(conn.is_error)
			status = conn.status;
		else
			status = conn.status;
		table << HtmlTableRow(conn.raw_host, status,
			~HtmlSubmit(t_("Remove")).Name((const char *)("RMHOST" + IntStr(i))), White);
	}

	DUMP(cfg_query.GetString("NEW_HOST"));
	DUMP(cfg_default_host);
	Htmls new_host;
	new_host << t_("Add generator: ") << HtmlEdit("NEW_HOST").Data(cfg_query.GetString("NEW_HOST", cfg_default_host))
		<< "  " << HtmlSubmit(t_("  Insert  ")).Name("ADDHOST");

	Htmls s;
	s << HtmlSimpleRow() / HtmlBig() / t_("Active generators");
	if(count > 0)
		s << HtmlSimpleRow()
		% (HtmlTable().CellSpacing(0).CellPadding(0)
		.BgColor(White).Border(0).Center().Width(-100) % table);
	else
		s << HtmlSimpleRow() / NoGenMsg();
	s << HtmlSimpleRow() / new_host;
	return s;
}

Htmls WIsapiClient::Login()
{
	Htmls s;
	s << HtmlSimpleRow() / HtmlBig() / t_("Admin password")
		<< HtmlSplitRow(t_("Password:"), ~HtmlInput("PASSWORD").Name("PASSWORD").Data(cfg_query.GetString("PASSWORD", Null)));
	return s;
}

/*
Htmls WIsapiClient::Commit()
{
	Htmls text;
	text << HtmlSimpleRow()
	<< HtmlSimpleRow().Left() % HtmlSubmit("Zapsat konfiguraci").Name(WID_SAVE_CONFIG);

	if(map.Find(ID_SAVE_CONFIG) >= 0 && CanWrite())
		if(client.Save())
			text << HtmlSimpleRow()
			<< HtmlSimpleRow() % HtmlFontColor(LtBlue)
			/ HtmlBig() / "Konfiguraèní soubor byl korektnì uložen.";
		else
			error = "Chyba pøi zápisu konfiguraèního souboru.";
	return text;
}
*/

static Htmls HtmlStatGenRow(const Value& gen, const Value& num, const Value& byt, const Value& avg, const Value& max, const Value& err,
							const HtmlTag& tag = HtmlTag())
{
	return Htmls()
		<< HtmlCell().Width(-25) / tag / StdFormat(gen)
		<< HtmlCell().Width(-15) / tag / StdFormat(num)
		<< HtmlCell().Width(-15) / tag / StdFormat(byt)
		<< HtmlCell().Width(-15) / tag / StdFormat(avg)
		<< HtmlCell().Width(-15) / tag / StdFormat(max)
		<< HtmlCell().Width(-15) / tag / StdFormat(err);
}

Htmls WIsapiClient::Statistics()
{
	Htmls content;

	double duration = (double)(GetSysTime() - run_time);
	int id = fround(duration);
	String dd;
	if(id < 60)
		dd << id << t_(" sec");
	else if(id < 60 * 60)
		dd << id / 60 << t_(" min, ") << id % 60 << t_(" hrs, ");
	else
	{
		int hours = ffloor(duration / 3600);
		id = fround(duration - 3600.0 * hours);
		dd << hours << t_(" hrs, ") << id / 60 << t_(" min, ") << id % 60 << t_(" sec");
	}

	content
		<< HtmlSimpleRow() % HtmlBig() / t_("Overall statistics")
		<< HtmlValueRow(t_("Server started:"), run_time)
		<< HtmlValueRow(t_("Running time:"), dd)
		<< HtmlValueRow(t_("Hit count:"), hit_count)
		<< HtmlValueRow(t_("Error count:"), error_count)
		<< HtmlValueRow(t_("Transferred data (KB):"), FormatDouble(total_bytes * 1e-3, 4, FD_REL))
		<< HtmlValueRow(t_("Average response (msecs):"), FormatDouble(total_msecs / max<double>(hit_count - error_count, 1), 4, FD_REL))
		<< HtmlValueRow(t_("Slowest response (msecs):"), max_msecs)
		<< HtmlSimpleRow()
		<< HtmlSimpleRow() % HtmlBig() / t_("Statistics by generators")
		<< HtmlSimpleRow();

	int count = connections.GetCount();
	if(count == 0)
		content << HtmlSimpleRow() % NoGenMsg();
	else
	{
		Htmls table;
		table << HtmlRow().BgColor(Yellow)
			% HtmlStatGenRow(t_("Generator"),
				t_("Requests"),
				t_("Data (KB)"),
				t_("Response (msec)"),
				t_("Max (msec)"), t_("Errors"), HtmlBold());
		for(int i = 0; i < count; i++)
		{
			Connection& conn = connections[i];
			CheckedSection::Lock guard(conn.conn_lock);
			table << HtmlRow().BgColor(White)
				% HtmlStatGenRow(conn.raw_host, conn.hit_count, floor(conn.total_bytes * 1e-3),
				conn.total_msecs / max<double>(conn.hit_count - conn.error_count, 1),
				conn.max_msecs, conn.error_count);
		}
		content << HtmlSimpleRow() % HtmlTable().CellSpacing(0).CellPadding(0)
			.BgColor(White).Border(0).Center().Width(-100) % table;
	}

	return HtmlIsapiTable() % content;
}

bool WIsapiClient::CanWrite()
{
	if(cfg_can_write < 0)
	{
		String pwd = ToLower(cfg_query.GetString("PASSWORD"));
		if(pwd.IsEmpty())
		{
			DUMP(cfg_query.GetString("PASSWORD"));
			cfg_can_write = 0;
			cfg_error = t_("To commit changes you must "
	"enter the password.");
		}
		else
			if(!(cfg_can_write = (pwd == ToLower(cfg_password))))
				cfg_error = t_("Password invalid; check keyboard "
	"settings.");
	}
	return !!cfg_can_write;
}

/*
Htmls WIsapiClient::Services()
{
	Htmls content;
	content << Login()
		<< HtmlSimpleRow() % HtmlBig("Instalované služby")
		<< HtmlSimpleRow() % ServiceTable();

	if(!IsNull(error))
		content = HtmlSimpleRow() / HtmlFontColor(LtRed) / (HtmlBig("Chyba: " + HtmlBold(error)))
		+ HtmlSimpleRow() + content;
	return HtmlTag("FORM").Attr("METHOD", "POST").Attr("ACTION", config_url + "=r")
		% HtmlIsapiTable() % content;
}
*/

/*
static Htmls HtmlServiceRow(Htmls cell1, Htmls cell2, Htmls cell3)
{
	Htmls s;
	s << HtmlCell().Width(-50) / cell1
		<< HtmlCell().Width(-25) / cell2
		<< HtmlCell().Width(-25) / (cell3 + "&nbsp;");
	return s;
}
*/

/*
Htmls WIsapiClient::ServiceTable()
{
	SHandle mgr = OpenSCManager(0, 0, GENERIC_READ | GENERIC_WRITE);
	if(!mgr)
	{
		error = "Pøístup ke správci služeb byl odmítnut.";
		return Htmls();
	}
	Array<ServiceInfo> services = EnumServices(mgr);
	SortX(services, XField(&ServiceInfo::display_name), StdCsNumSort);

	Htmls table = HtmlRow().BgColor(Yellow) % HtmlServiceRow(HtmlBold("Název služby"), HtmlBold("Stav"), Null);

	for(int i = 0; i < services.GetCount(); i++)
	{
		const ServiceInfo& s = services[i];
		const char *state = "(?)";
		switch(s.state)
		{
		case SERVICE_START_PENDING:    state = "spouští se"; break;
		case SERVICE_RUNNING:          state = "bìží"; break;
		case SERVICE_STOP_PENDING:     state = "vypíná se"; break;
		case SERVICE_STOPPED:          state = "vypnutá"; break;
		case SERVICE_CONTINUE_PENDING: state = "obnovuje se"; break;
		case SERVICE_PAUSE_PENDING:    state = "pozastavuje se"; break;
		case SERVICE_PAUSED:           state = "pozastavena"; break;
		}
		table << HtmlRow() % HtmlServiceRow(ToHtml(s.display_name), state, Null);
	}
	return HtmlTable().Width(-100).CellSpacing(0).CellPadding(0).Border(0).BgColor(White) % table;
}
*/

void WIsapiClient::AddHost(const String& host)
{
	CheckedSection::Lock guard(client_lock);
	if(FindFieldIndex(connections, &Connection::raw_host, host) >= 0)
		throw Exc(NFormat(t_("Generator '%s' is already "
	"connected."), host));
	connections.Add(new Connection(*this, host));
	SaveConfig();
}

void WIsapiClient::RemoveHost(const String& host)
{
	CheckedSection::Lock guard(client_lock);
	int x = FindFieldIndex(connections, &Connection::raw_host, host);
	if(x < 0)
		throw Exc(NFormat(t_("Server '%s' not found."), host));
	{
		CheckedSection::Lock conn_guard(connections[x].conn_lock);
		if(connections[x].is_busy)
			throw Exc(NFormat(t_("Cannot disconnect server '%s' "
	"while it's processing a request."), host));
	}
	connections.Remove(x);
}

void WIsapiClient::SaveConfig()
{
	CheckedSection::Lock guard(client_lock);
	StringStream cfg;
	cfg <<
	"PASSWORD=" << cfg_password << "\n"
	"LOGGING=" << (log_isapi ? '1' : '0') << "\n"
	"HEADERS=" << (show_headers ? '1' : '0') << "\n"
	"ENGLISH=" << (isapi_english ? '1' : '0') << "\n"
	"TIMEOUT=" << isapi_timeout << "\n"
	"ADMIN=" << (remote_admin ? '1' : '0') << "\n"
//	"RETRY=" << retry_msec << "\n"
	;
	for(int i = 0; i < connections.GetCount(); i++)
		cfg << "HOST=" << connections[i].raw_host << '\n';
	DeleteFile(cfg_filename);
	if(!SaveFile(cfg_filename, cfg))
		throw Exc(NFormat(t_("Error saving file '%s'."), cfg_filename));
	SetFileAttributes(cfg_filename, FILE_ATTRIBUTE_HIDDEN);
}

void WIsapiClient::LoadConfig()
{
	CheckedSection::Lock guard(client_lock);
	ASSERT(connections.IsEmpty());
	VectorMap<String, String> cfg = LoadKeyMapFile(cfg_filename);
	log_isapi = (*cfg.Get("LOGGING", Null) == '1');
	show_headers = (*cfg.Get("HEADERS", "1") == '1');
	remote_admin = (*cfg.Get("ADMIN", "1") == '1');
	isapi_english = (*cfg.Get("ENGLISH", "1") == '1');
	cfg_password = cfg.Get("PASSWORD", "wisapi");
//	retry_msec = ::minmax(stoi(cfg("RETRY", "1000")), 0, 60000);
	isapi_timeout = ::minmax(ScanInt(cfg.Get("TIMEOUT", "1000")), 1000, 60000);
	SetLanguage(isapi_english ? LNG_ENGLISH : LNG_CZECH);
	for(int i = cfg.Find("HOST"); i >= 0; i = cfg.FindNext(i))
		connections.Add(new Connection(*this, cfg[i]));
//	LOG("WIsapiClient::LoadConfig: retry = " << retry_msec);
	LOG("WIsapiClient::LoadConfig: timeout = " << isapi_timeout);
}

GLOBAL_VAR(One<WIsapiClient>, client)

extern "C" dword WINAPI HttpExtensionProc(EXTENSION_CONTROL_BLOCK *ecb)
{
	try
	{
		static long epid = 0;
		int cid = InterlockedIncrement(&epid);
		ISAPILOG("HttpExtensionProc, epid = " << cid << ", thread " << (int)GetCurrentThreadId());
		return client()->Run(cid, ecb);
	}
	catch(Exc e)
	{
		ISAPILOG("HttpExtensionProc->Exc(" << e << ")");
		ecb->dwHttpStatusCode = HTTP_STATUS_SERVER_ERROR;
		return HSE_STATUS_ERROR;
	}
	catch(...)
	{
		ISAPILOG("HttpExtensionProc->Exc (unknown)");
		ecb->dwHttpStatusCode = HTTP_STATUS_SERVER_ERROR;
		return HSE_STATUS_ERROR;
	}
}

extern "C" BOOL WINAPI GetExtensionVersion(HSE_VERSION_INFO *pVer)
{
	pVer->dwExtensionVersion = MAKELONG(HSE_VERSION_MINOR, HSE_VERSION_MAJOR);
	strcpy(pVer->lpszExtensionDesc, "ISAPI extension: Copyright © 1999-2003 Ultimate Development spol. s r.o.");
	return TRUE;
}

extern "C" BOOL WINAPI DllMain(HANDLE module, DWORD reason, LPVOID lpReserved)
{
	if(reason == DLL_PROCESS_ATTACH) {
		SetLanguage(LNG_ENGLISH);
		SetDefaultCharset(CHARSET_WIN1250);
		dll_module = (HINSTANCE)module;
		dll_filename = GetModuleFileName(dll_module);
		String lfn = ForceExt(dll_filename, "_log.txt");
		DeleteFile(lfn);
		SetVppLogName(lfn);
		SetVppLogSizeLimit(500000);
		ISAPILOG("WISAPI DllMain/DLL_PROCESS_ATTACH: creating WIsapiClient object");
		client() = new WIsapiClient;
		ISAPILOG("WISAPI DllMain/DLL_PROCESS_ATTACH: done");
	}
	else if(reason == DLL_PROCESS_DETACH) {
		ISAPILOG("WISAPI DllMain/DLL_PROCESS_DETACH: destroying WIsapiClient object");
		client().Clear();
		ISAPILOG("WISAPI DllMain/DLL_PROCESS_DETACH: done");
	}
	else if(reason == DLL_THREAD_ATTACH) {
		ISAPILOG("WISAPI DllMain/DLL_THREAD_ATTACH");
	}
	else if(reason == DLL_THREAD_DETACH) {
		ISAPILOG("WISAPI DllMain/DLL_THREAD_DETACH");
	}
	return TRUE;
}

#ifndef flagDLL
CONSOLE_APP_MAIN
{
}
#endif
