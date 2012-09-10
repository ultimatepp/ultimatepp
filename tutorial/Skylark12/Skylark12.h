#ifndef _Skylark12_Skylark12_h
#define _Skylark12_Skylark12_h

#include <Skylark/Skylark.h>
#include <plugin/sqlite3/Sqlite3.h>

using namespace Upp;

#define  MODEL         <Skylark12/Model.sch>
#define  SCHEMADIALECT <plugin/sqlite3/Sqlite3Schema.h>
#include <Sql/sch_header.h>

struct CreateEditDelete : SkylarkPack {
	HandlerId   back;
	SqlId       table;
	SqlId       key;
	SqlSet      columns;
	String      dialog;

	void Create(Http& http);
	void SubmitCreate(Http& http);
	void Edit(Http& http);
	void SubmitEdit(Http& http);
	void Delete(Http& http);

	typedef CreateEditDelete CLASSNAME;

	void Use();
	
	CreateEditDelete() { key = SqlId("ID"); columns = SqlSet(SqlAll()); }
};

class Skylark12 : public SkylarkApp {
public:
	virtual void WorkThread();

	typedef Skylark12 CLASSNAME;
	Skylark12();
};

#endif
