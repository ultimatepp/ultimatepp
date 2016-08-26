#include "Sql.h"

namespace Upp {

static String sPutId(const char *type, Index<String>& id, String n, int w = 0)
{
	String r = type;
	if(id.Find(n) < 0) {
		id.Add(n);
		r << '_';
	}
	r << String(' ', max(0, w - r.GetLength())) << '(' << n;
	return r;
}

String ExportSch(SqlSession& session, const String& database)
{
	String r;
	Vector<String> tab = session.EnumTables(database);
	Index<String> id;
	for(int i = 0; i < tab.GetCount(); i++) {
		r << sPutId("TABLE", id, ToUpper(tab[i])) << ")\r\n";
		Vector<SqlColumnInfo> c = session.EnumColumns(database, tab[i]);
		for(int i = 0; i < c.GetCount(); i++) {
			String type;
			int    width = Null;
			switch(c[i].type) {
			case INT_V:
				type = "INT";
				break;
			case DOUBLE_V:
				type = "DOUBLE";
				break;
			case DATE_V:
				type = "DATE";
				break;
			case TIME_V:
				type = "TIME";
				break;
			case STRING_V:
				type = "STRING";
				width = c[i].width;
				if(width < 0 || width > 40000)
					width = 2000;
				break;
			default:
				type = "STRING";
				width = 200;
				break;
			}
			r << '\t' << sPutId(type, id, ToUpper(c[i].name), 8);
			if(width > 0 && width < 40000)
				r << ", " << width;
			r << ")\r\n";
		}
		r << "END_TABLE\r\n\r\n";
	}
	return r;
}

#ifndef NOAPPSQL
String ExportSch(const String& database)
{
	return ExportSch(SQL.GetSession(), database);
}
#endif

static void sId(String& r, const String& id, Index<String>& done)
{
	String u = ToUpper(id);
	if(done.Find(u) >= 0)
		return;
	done.Add(u);
	if(u == id)
		r << "SQLID(" << id << ")\r\n";
	else
		r << "SQL_ID(" << u << ", " << id << ")\r\n";
}

String ExportIds(SqlSession& session, const String& database)
{
	String r;
	Vector<String> tab = session.EnumTables(database);
	Index<String> done;
	for(int i = 0; i < tab.GetCount(); i++) {
		sId(r, tab[i], done);
		Vector<SqlColumnInfo> c = session.EnumColumns(database, tab[i]);
		for(int i = 0; i < c.GetCount(); i++)
			sId(r, c[i].name, done);
	}
	return r;
}

#ifndef NOAPPSQL
String ExportIds(const String& database)
{
	return ExportIds(SQL.GetSession(), database);
}
#endif

}
