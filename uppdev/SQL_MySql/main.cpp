#include <MySql/MySql.h>

using namespace Upp;


CONSOLE_APP_MAIN
{
	MySqlSession session;
	// substitute your 'username' and 'password' here:
	if(!session.Connect("root", "Passw0rd", "mysql", "localhost")) {
		printf("Can't connect with MySql\n");
		return;
	}
	Sql sql(session);
	sql.Execute("use mysql");
	sql.Execute("show tables");
	while(sql.Fetch())
		Cout() << (String)sql[0] << '\n';
}
