#include <Core/Core.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <libgen.h>

#include <sqlfront.h>	/* sqlfront.h always comes first */
#include <sybdb.h>	/* sybdb.h is the only other file you need */

int err_handler(DBPROCESS*, int, int, int, char*, char*) {}
int msg_handler(DBPROCESS*, DBINT, int, int, char*, char*, char*, int) {}

using namespace Upp;

extern char *optarg;
extern int optind;

const static char syntax[] = 
		"syntax: example -S server -D db -U user -P passwd\n";

struct Options {
	char *appname, *servername, *dbname, *username, *password;
} options = {0,0,0,0,0};


CONSOLE_APP_MAIN
{
	int i, ch;
	LOGINREC *login;
	DBPROCESS *dbproc;
	RETCODE erc;
	
	options.appname = "Holly";
	
	options.servername = strdup("mssql");
	options.dbname = strdup("Holly");

	if (dbinit() == FAIL) {
		LOG("dbinit failed");
		return;
	}
	
	dberrhandle(err_handler);
	dbmsghandle(msg_handler);

	if ((login = dblogin()) == NULL) {
		LOG("dblogin failed");
		return;
	}

	DBSETLUSER(login, options.username);
	DBSETLPWD(login, options.password);
	
	if ((dbproc = dbopen(login, options.servername)) == NULL) {
		LOG("unable to connect");
		return;
	}

	if (options.dbname  && (erc = dbuse(dbproc, options.dbname)) == FAIL) {
		LOG("unable to use db");
		return;
	}

	LOG("Success!");	
}
