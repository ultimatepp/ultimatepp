#include <MySql/MySql.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	MySqlSession session;
	// substitute your 'username' and 'password' here:
	if(!session.Connect("root", "heslo", "test", "10.0.0.19")) {
		printf("Can't connect with MySql\n");
		return;
	}
	Sql sql(session);
	sql.Execute("use test");
	sql.Execute("show tables");
	while(sql.Fetch())
		Cout() << (String)sql[0] << '\n';
	SaveFile("u:/lego.sch", ExportSch(session, "test"));
}
