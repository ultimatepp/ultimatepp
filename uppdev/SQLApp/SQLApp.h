#ifndef _SQLApp_SQLApp_h
#define _SQLApp_SQLApp_h

#include <CtrlLib/CtrlLib.h>

#include <MySql/MySql.h>
#include <SqlCtrl/SqlCtrl.h>

#define LAYOUTFILE <SQLApp/SQLApp.lay>
#include <CtrlCore/lay.h>

#define SCHEMADIALECT <MySql/MySqlSchema.h>
#define MODEL <SQLApp/SQLApp.sch>
#include "Sql/sch_schema.h"
#include "Sql/sch_header.h"
#include "Sql/sch_source.h"

class Data;

class SQLApp : public WithSQLAppLayout<TopWindow> {
public:
	typedef SQLApp CLASSNAME;
	SQLApp();
	~SQLApp();

	void CreateTables();

	void OnAddBT();
	void OnUpdateBT();
	void OnDeleteBT();
	void OnFindBT();
	void OnClearBT();
	void OnShowBT();

private:
//	Sql sql;
	Data *DATA;
	int key;
	SqlId *SID;
};

class Data
{
public:
	int Key;
	String Data1;
	String Data2;
	String Data3;

	Data() { Key=0;Data1="";Data2="";Data3=""; };
};

#endif
