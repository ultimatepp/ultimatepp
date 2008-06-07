#ifndef _SQLApp_SQLApp_h
#define _SQLApp_SQLApp_h

#include <SqlCtrl/SqlCtrl.h>
#include <plugin/sqlite3/Sqlite3.h>

using namespace Upp;

#define LAYOUTFILE <SQLApp/SQLApp.lay>
#include <CtrlCore/lay.h>

#define SCHEMADIALECT <plugin/sqlite3/Sqlite3Schema.h>
#define MODEL <SQLApp/SQLApp.sch>
#include "Sql/sch_header.h"

Convert& DateIntConvert();

struct SQLApp : public WithSQLAppLayout<TopWindow> {
	WithQueryLayout<TopWindow> query;
	void Serialize(Stream& s);

	void Query();
	void Query(int q);
	void SetupQuery();
	void ClearQuery();

	void NewBook();
	void EditBook();
	void DeleteBook();
	void BookMenu(Bar& bar);

	void NewBorrow();
	void EditBorrow();
	void DeleteBorrow();
	void ReturnedToday();
	void BorrowMenu(Bar& bar);

	void SetupMenu(Bar& bar);
	void MainMenu(Bar& bar);

	typedef SQLApp CLASSNAME;
	SQLApp();
};

#endif
