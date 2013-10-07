#include <RichEdit/RichEdit.h>
#include <Sql/Sql.h>

using namespace Upp;

#define MODEL <SqlS_fn/Schema.sch>
#define SCHEMADIALECT <plugin/sqlite3/Sqlite3Schema.h>
#include <Sql/sch_header.h>
#include <Sql/sch_source.h>

struct MyFieldOperator : FieldOperator {
	virtual void Field(const char *name, Ref f) {
		LOG("Field " << name << ", value: " << (Value)f);
		if(name == String("NAME"))
			f = "Carl";
	}
};

CONSOLE_APP_MAIN
{
	S_PERSON p;
	
	p.ID = 100;
	p.NAME = "John";
	p.LASTNAME = "Smith";

	DUMP(p);

	DUMP(p.TableName);

	DUMP(~p.ColumnSet());
	DUMP(~p.ColumnSet("PREFIX_"));
	DUMP(~p.Of(PERSON));

	DUMPC(p.GetColumnIds());

	DUMP(p.Get(ID));
	DUMP(p.Get(LASTNAME));
	
	DUMP(p.Get());	

	S_PERSON p2;
	p2 = p;
	DUMP(p == p2);
	p2.ID = 101;
	DUMP(p == p2);
	
	MyFieldOperator fo;
	p.FieldLayout(fo);
	
	DUMP(p.NAME);
}
