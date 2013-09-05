#include <OleDB/OleDB.h>
#include <SqlCtrl/SqlCtrl.h>

#define HAVE_ORACLE

#ifndef flagNOSQLLITE
	#define HAVE_SQLLITE
	#include <plugin/sqlite3/Sqlite3.h>
#endif

#ifndef flagNOMYSQL
	#define HAVE_MYSQL
#endif

#ifndef flagNOPOSTGRESQL
	#define HAVE_POSTGRESQL
	#include <PostgreSQL/PostgreSQL.h>
#endif

#if defined(PLATFORM_WIN32) && defined(COMPILER_MSC)
	#define HAVE_OLEDB
#endif

#ifdef HAVE_ORACLE
	#include <Oracle/Oracle7.h>
	#include <Oracle/Oracle8.h>
#endif

#ifdef HAVE_MYSQL
	#ifndef flagMT
		#error Must compile MySQL in multithreaded mode!
	#endif
	#include <MySql/MySql.h>
#endif

using namespace Upp;

#define LAYOUTFILE <SqlCommander/login.lay>
#include <CtrlCore/lay.h>

class SqlCommanderApp {
public:
	SqlCommanderApp();

	void Run();
	void Serialize(Stream& s);

private:
	WithLoginLayout<TopWindow> login;
	enum SERVER_TYPE
	{
		SERVER_ORACLE,
#ifdef HAVE_MYSQL
		SERVER_MYSQL
#endif
	};
	One<SqlSession> session;
};

SqlCommanderApp::SqlCommanderApp() {
	CtrlLayoutOKCancel(login, "Connect to server");
	login.password.Password();
	login.save_password = true;
#ifdef HAVE_ORACLE
	login.connection.Add("OCI7");
	login.connection.Add("OCI8");
#endif
#ifdef HAVE_MYSQL
	login.connection.Add("MYSQL");
#endif
#ifdef HAVE_POSTGRESQL
	login.connection.Add("POSTGRESQL");
#endif
#ifdef HAVE_SQLLITE
	login.connection.Add("SQLLITE");
#endif
#ifdef HAVE_OLEDB
	Array<OleDBSession::Provider> providers = OleDBSession::EnumProviders();
	for(int i = 0; i < providers.GetCount(); i++)
		login.connection.Add("OLEDB:" + providers[i].name);
#endif
}

void SqlCommanderApp::Serialize(Stream& s) {
	int version = 3;
	s / version;
	if(version < 3)
	{
		s.SetError();
		return;
	}
	String pwd;
	if(s.IsStoring() && login.save_password)
		pwd = Garble(~login.password);
	s % login.username % login.database % login.server % login.connection % login.save_password % pwd;
	if(s.IsLoading() && login.save_password)
		login.password <<= Garble(pwd);
}

void SqlCommanderApp::Run() {

	LoadFromFile(*this);

	do {
		if(login.Run() != IDOK) return;
		String un = ~login.username;
		String pw = ~login.password;
		String db = ~login.database;
		String sv = ~login.server;
		String co = ~login.connection;
		String connect = un;
		if(!pw.IsEmpty()) connect << '/' << pw;
		if(!sv.IsEmpty()) connect << '@' << sv;
#ifdef HAVE_ORACLE
		if(co == "OCI7") {
			One<Oracle7> oci7 = new Oracle7;
			if(!oci7->Open(connect)) {
				Exclamation(NFormat("OCI7 login failed: [* \1%s\1].", oci7->GetLastError()));
				continue;
			}
			session = -oci7;
		}
		if(co == "OCI8") {
			One<Oracle8> oci8 = new Oracle8;
			if(!oci8->Open(connect, false)) {
				Exclamation(NFormat("OCI8 login failed: [* \1%s\1].", oci8->GetLastError()));
				continue;
			}
			session = -oci8;
		}
#endif
#ifdef HAVE_MYSQL
		if(co == "MYSQL") {
			One<MySqlSession> mysql = new MySqlSession;
			if(!mysql->Connect(un, pw, db, sv)) {
				Exclamation(NFormat("MySql login failed: [* \1%s\1].", mysql->GetLastError()));
				continue;
			}
			session = -mysql;
		}
#endif
#ifdef HAVE_POSTGRESQL
		if(co == "POSTGRESQL") {
			One<PostgreSQLSession> postgres = new PostgreSQLSession;
			String conninfo;
			conninfo << "user='" << un << "' password='" << pw << "' ";
			if(!IsNull(sv)) {
				String port;
				int f = sv.Find(':');
				if(f >= 0) {
					port = sv.Mid(f + 1);
					sv.Trim(f);
				}
				if(!IsNull(sv))
					conninfo << "host='" << sv << "' ";
				if(!IsNull(port))
					conninfo << "port=" << port << ' ';
			}
			if(!IsNull(db))
				conninfo << "dbname='" << db << "' ";
			postgres->Open(conninfo);
			session = -postgres;
		}
#endif
#ifdef HAVE_SQLLITE
		if(co == "SQLLITE") {
			One<Sqlite3Session> sqlite = new Sqlite3Session;
			if(!sqlite->Open(db)) {
				Exclamation(NFormat("SQLite3 open failed, (db file = [* \1%s\1]): [* \1%s\1].", db, sqlite->GetLastError()));
				continue;
			}
			session = -sqlite;
		}
#endif
#ifdef HAVE_OLEDB
		if(!memcmp(co, "OLEDB:", 6)) {
			String conn = co.Mid(6);
			One<OleDBSession> oledb = new OleDBSession;
			if(!oledb->Open(un, pw, sv, conn)) {
				Exclamation(NFormat("OleDB login failed: [* \1%s\1].", oledb->GetLastError()));
				continue;
			}
			session = -oledb;
		}
#endif
		if(!session)
			Exclamation(NFormat("Unsupported connection type: [* \1%s\1].", co));
	}
	while(!session);
	login.Close();
	SQLCommander(*session);
	StoreToFile(*this);
}

#ifdef flagMAIN
GUI_APP_MAIN
{
//	SetLanguage(LNGC_('E', 'N', 'U', 'S', CHARSET_WIN1252));
	SetLanguage(LNGC_('C', 'S', 'C', 'Z', CHARSET_WIN1250));
//	SetDefaultCharset(CHARSET_WIN1250);
//	Draw::XLFDArialFont = "-*-helvetica-*-*-*-*-*-*-*-*-*-*-*-*";
//	Draw::SetStdFont(Arial(12));
	SqlCommanderApp().Run();
}
#endif//flagMAIN
