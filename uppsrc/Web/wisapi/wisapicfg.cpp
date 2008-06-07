//////////////////////////////////////////////////////////////////////
// isapicfg: HTML-based configuration dialog.

#if 0

#include "wisapicfg.h"
#include "version.h"
#include <winsvc.h>

//////////////////////////////////////////////////////////////////////

static const char title[] = "Konfigurace konektoru internetového serveru";

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

struct ServiceInfo
{
	String display_name;
	String service_name;
	dword  state;
};
template class Moveable<ServiceInfo>;

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

static HtmlTag HtmlSimpleRow()
{
	return HtmlRow() % HtmlCell().ColSpan(2).Width(-100);
}

//////////////////////////////////////////////////////////////////////

static Htmls HtmlSplitRow(const char *tag, const Htmls& content)
{
	return HtmlRow() % (HtmlCell().Width(-30) / tag + "\n" + HtmlCell().Left().Width(-70) / content);
}

//////////////////////////////////////////////////////////////////////

static Htmls HtmlValueRow(const char *tag, const Value& value)
{
	return HtmlSplitRow(tag, HtmlBold() / value.Format());
}

//////////////////////////////////////////////////////////////////////

static Htmls HtmlMenu(bool active, const char *text, const char *url, int wd)
{
	HtmlTag table = HtmlTable().CellSpacing(0).Border(2)
		.BorderColor(Black).BgColor(active ? Yellow : WhiteGray);
	if(wd)
		table.Width(wd);
	return table % HtmlRow() % HtmlCell().Center()
		/ HtmlLink(url) / HtmlFontColor(Black) / (active ? HtmlBold() / text : Htmls(text));
}

//////////////////////////////////////////////////////////////////////

static Htmls HtmlHardSpace(int count = 1)
{
	StringBuffer s(count * 6);
	char *p = s;
	for(int i = 0; i < count; i++)
	{
		memcpy(p, "&nbsp;", 6);
		p += 6;
	}
	return s;
}

//////////////////////////////////////////////////////////////////////

static HtmlTag HtmlIsapiTable()
{
	return HtmlTable().Width(-70).CellSpacing(0).CellPadding(1).Border(1).BgColor(WhiteGray)
		% HtmlRow() % HtmlCell()
		% HtmlTable().Width(-100).CellSpacing(2).CellPadding(0).Border(0).BgColor(WhiteGray);
}

//////////////////////////////////////////////////////////////////////

IsapiCfg::IsapiCfg(IsapiClient& client, const char *url)
: map(url)
, client(client)
, config_url(client.isapi_url + "?configure")
, can_write(-1)
{
	default_host << "localhost@" << DEFAULT_PORT;
#ifdef _DEBUG
	for(int i = 0; i < map.GetCount(); i++)
		LOG("map[" << i << "] = (" << map.GetKey(i) << " / " << map[i]);
#endif
}

//////////////////////////////////////////////////////////////////////

bool IsapiCfg::Run(String& header, String& content, String& error)
{
	Htmls body = Body();

	content = HtmlTag("HTML") %
		(HtmlTag("HEAD") % (HtmlTag("TITLE") / title)
		+ HtmlTag("BODY").BgColor(White) % body);

	return true;
}

//////////////////////////////////////////////////////////////////////

Htmls IsapiCfg::Body()
{
	Htmls body;
	body
		<< HtmlTag("H1") / title
		<< HtmlTag("SMALL") / "Umístìní knihovny: "
		<< HtmlTag("TT") / ToHtml(client.isapi_path) << "<BR>\n"
		<< HtmlTag("SMALL") / "Verze: <B>" ISAPI_VERSION "</B>, datum výroby: <B>"
		ISAPI_DATE "</B><BR>\n" ISAPI_COPYRIGHT "<P>\n";

	String s = map.GetString(ID_CONFIGURE);
	enum { CONFIG, STAT /*, SERV */ } page = CONFIG;
	switch(*s)
	{
	case 's': page = STAT; break;
//	case 'r': page = SERV; break;
	}

	Htmls menu;
	menu
		<< HtmlCell() / HtmlMenu(page == CONFIG, "Konfigurace", config_url, 120)
		<< HtmlCell() / HtmlHardSpace(2)
		<< HtmlCell() / HtmlMenu(page == STAT, "Statistika", config_url + "=s", 120)
//		<< HtmlCell() / HtmlHardSpace(2)
//		<< HtmlCell() / HtmlMenu(page == SERV, "Služby", config_url + "=r", 120)
	;

	body << HtmlTable().CellSpacing(0) % HtmlRow() % menu
		<< "<P>\n";

	switch(page)
	{
	case CONFIG: body << Configuration(); break;
	case STAT:   body << Statistics(); break;
//	case SERV:   body << Services(); break;
	}

	return body;
}

//////////////////////////////////////////////////////////////////////

Htmls IsapiCfg::Configuration()
{
	Htmls content;
	content << Login()
		<< HtmlSimpleRow() << Timing()
		<< HtmlSimpleRow() << Servers();
	content // must be the last one as Timing() and Servers() might modify some client data
		<< HtmlSimpleRow() << Commit();
	if(!IsNull(error))
		content = HtmlSimpleRow() / HtmlFontColor(LtRed) / (HtmlBig() / ("Chyba: " + HtmlBold() / error))
		+ HtmlSimpleRow() + content;
	return HtmlTag("FORM").Attr("METHOD", "POST").Attr("ACTION", config_url) % HtmlIsapiTable() % content;
}

//////////////////////////////////////////////////////////////////////

Htmls IsapiCfg::Timing()
{
	Htmls s;
	s << HtmlSimpleRow() % HtmlBig() / "Èasové konstanty"
		<< HtmlSplitRow("Pøedání dotazu (s):", ~Edit(ID_QUERY_TIMEOUT, client.query_timeout))
		<< HtmlSplitRow("Výsledek dotazu (s):", ~Edit(ID_ANSWER_TIMEOUT, client.answer_timeout));
	return s;
}

//////////////////////////////////////////////////////////////////////

HtmlTag IsapiCfg::Edit(Id id, int& value)
{
	int vsec = value / 1000;
	int new_value = map.GetInt(id);
	if(!IsNull(new_value) && new_value != vsec)
	{
		if(new_value >= 1 && new_value <= 600)
			value = new_value * 1000;
		else
			error = "Hodnota je mimo povolený rozsah (1 - 600 s).";
	}
	if(IsNull(new_value))
		new_value = vsec;
	return HtmlEdit(id).Data(IntStr(new_value)).Size(6);
}

//////////////////////////////////////////////////////////////////////

static Htmls HtmlTableRow(const Htmls& host, const Htmls& state, const Htmls& modf, Color c)
{
	Htmls cells;
	cells << HtmlCell().Width(-50) % host;
	if(!IsNull(modf))
		cells
			<< HtmlCell().Width(-10) % state
			<< HtmlCell().Width(-40) % modf;
	else
		cells
			<< HtmlCell().ColSpan(2).Width(-50) % state;
	return HtmlRow().BgColor(c) % cells;
}

//////////////////////////////////////////////////////////////////////

static const char no_gen[] = "V tuto chvíli nejsou pøipojeny žádné generátory.";

static Htmls NoGenMsg()
{
	return HtmlFontColor(LtBlue) / HtmlTag("I") / no_gen;
}

//////////////////////////////////////////////////////////////////////

Htmls IsapiCfg::Servers()
{
	int count = map.GetInt(ID_HOSTCOUNT);
	if(!IsNull(count))
	{
		for(int i = 0; i < count; i++)
			if(map.Find("RMHOST" + IntStr(i)) >= 0)
			{ // remove this host
				String hostid = map.GetString("HOST" + IntStr(i));
				if(CanWrite())
					error = client.Remove(hostid);
				break;
			}
	}

	if(map.Find(ID_ADDHOST) >= 0 && CanWrite()
		&& IsNull(error = client.Add(map.GetString(ID_NEW_HOST))))
	{ // advance current port number
		String s = map.GetString(ID_NEW_HOST);
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
		map.Set(ID_NEW_HOST, s);
	}

	Htmls table;
	table << HtmlTableRow(HtmlBold() / "Generátor", HtmlBold() / "Akt. stav", "", Yellow);

	// output connection parameters
	client.LockConnections();
	count = client.GetCount();
	table << HtmlHidden(ID_HOSTCOUNT, count) << "\n";
	for(int i = 0; i < count; i++)
	{
		const IsapiClient::Connection& conn = client[i];
		table << HtmlHidden("HOST" + IntStr(i), conn.GetHost()) << "\n";
		String status;
		if(conn.IsBusy())
			status = "pracuje";
		else if(conn.IsError())
			status = "chyba";
		else
			status = "èeká";
		String host = conn.GetHost();
		table << HtmlTableRow(host, status,
			~HtmlSubmit("Smazat").Name((const char *)("RMHOST" + IntStr(i))), White);
	}
	client.UnlockConnections();

	Htmls new_host;
	new_host << "Pøidat generátor: " << HtmlEdit(ID_NEW_HOST).Data(map(ID_NEW_HOST, default_host)) << "  "
		<< HtmlSubmit("  Vložit  ").Name(ID_ADDHOST);

	Htmls s;
	s << HtmlSimpleRow() / HtmlBig() / "Aktivní generátory";
	if(count > 0)
		s << HtmlSimpleRow()
		% (HtmlTable().CellSpacing(0).CellPadding(0)
		.BgColor(White).Border(0).Center().Width(-100) % table);
	else
		s << HtmlSimpleRow() / NoGenMsg();
	s << HtmlSimpleRow() / new_host;
	return s;
}

//////////////////////////////////////////////////////////////////////

Htmls IsapiCfg::Login()
{
	Htmls s;
	s << HtmlSimpleRow() / HtmlBig() / "Heslo pro zápis"
		<< HtmlSplitRow("Heslo:", ~HtmlInput("PASSWORD").Name(ID_PASSWORD).Data(map(ID_PASSWORD, Null)));
	return s;
}

//////////////////////////////////////////////////////////////////////

Htmls IsapiCfg::Commit()
{
	Htmls text;
	text << HtmlSimpleRow()
	<< HtmlSimpleRow().Left() % HtmlSubmit("Zapsat konfiguraci").Name(ID_SAVE_CONFIG);

	if(map.Find(ID_SAVE_CONFIG) >= 0 && CanWrite())
		if(client.Save())
			text << HtmlSimpleRow()
			<< HtmlSimpleRow() % HtmlFontColor(LtBlue)
			/ HtmlBig() / "Konfiguraèní soubor byl korektnì uložen.";
		else
			error = "Chyba pøi zápisu konfiguraèního souboru.";
	return text;
}

//////////////////////////////////////////////////////////////////////

static Htmls HtmlStatGenRow(const Value& gen, const Value& num, const Value& avg, const Value& max, const Value& err,
							const HtmlTag& tag = HtmlTag())
{
	return Htmls()
		<< HtmlCell().Width(-32) / tag / gen.Format()
		<< HtmlCell().Width(-17) / tag / num.Format()
		<< HtmlCell().Width(-17) / tag / avg.Format()
		<< HtmlCell().Width(-17) / tag / max.Format()
		<< HtmlCell().Width(-17) / tag / err.Format();
}

//////////////////////////////////////////////////////////////////////

Htmls IsapiCfg::Statistics()
{
	Htmls content;

	client.LockStatistics();

	double duration = GetSysTime() - client.run_time;
	int id = fround(duration);
	String dd;
	if(id < 60)
		dd << id << " s";
	else if(id < 60 * 60)
		dd << id / 60 << " min, " << id % 60 << " s";
	else
	{
		int hours = ffloor(duration / 3600);
		id = fround(duration - 3600.0 * hours);
		dd << hours << " h, " << id / 60 << " min, " << id % 60 << " s";
	}

	content
		<< HtmlSimpleRow() % HtmlBig() / "Celková statistika"
		<< HtmlValueRow("Spuštìní serveru:", client.run_time)
		<< HtmlValueRow("Doba bìhu:", dd)
		<< HtmlValueRow("Poèet dotazù:", client.num_requests)
		<< HtmlValueRow("Poèet chyb:", client.num_errors)
		<< HtmlValueRow("Prùmìrná odezva (ms):",
			client.total_ticks / max(client.num_requests - client.num_errors, 1))
		<< HtmlValueRow("Nejdelší odezva (ms):", client.max_ticks)
		<< HtmlSimpleRow()
		<< HtmlSimpleRow() % HtmlBig() / "Statistika po generátorech"
		<< HtmlSimpleRow();

	client.UnlockStatistics();

	client.LockConnections();
	int count = client.GetCount();
	if(count == 0)
		content << HtmlSimpleRow() % NoGenMsg();
	else
	{
		Htmls table;
		table << HtmlRow().BgColor(Yellow)
			% HtmlStatGenRow("Generátor", "Dotazy", "Odezva (ms)", "Max (ms)", "Chyby", HtmlBold());
		for(int i = 0; i < count; i++)
		{
			IsapiClient::Connection& conn = client[i];
			conn.Enter();
			table << HtmlRow().BgColor(White)
				% HtmlStatGenRow(conn.GetHost(), conn.num_requests,
				conn.total_ticks / max(conn.num_requests - conn.num_errors, 1), conn.max_ticks, conn.num_errors);
			conn.Leave();
		}
		content << HtmlSimpleRow() % HtmlTable().CellSpacing(0).CellPadding(0)
			.BgColor(White).Border(0).Center().Width(-100) % table;
	}
	client.UnlockConnections();

	return HtmlIsapiTable() % content;
}

//////////////////////////////////////////////////////////////////////

bool IsapiCfg::CanWrite()
{
	if(can_write < 0)
	{
		String pwd = CsToLower(map.GetString(ID_PASSWORD));
		if(IsNull(pwd))
		{
			can_write = 0;
			error = "Pøi zápisu zmìn musíte zadat platné heslo.";
		}
		else
			if(!(can_write = (pwd == CsToLower(client.admin_pwd))))
				error = "Neplatné heslo; zkontrolujte nastavení klávesnice.";
	}
	return !!can_write;
}

//////////////////////////////////////////////////////////////////////

/*
Htmls IsapiCfg::Services()
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

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

/*
Htmls IsapiCfg::ServiceTable()
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

//////////////////////////////////////////////////////////////////////

#endif
