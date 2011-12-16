#ifndef _SqlCtrls_SqlCtrls_h
#define _SqlCtrls_SqlCtrls_h

#include <CtrlLib/CtrlLib.h>
#include <SqlCtrl/SqlCtrl.h>
#include <plugin/sqlite3/Sqlite3.h>

using namespace Upp;

#define LAYOUTFILE <SqlCtrls/SqlCtrls.lay>
#include <CtrlCore/lay.h>

#define SCHEMADIALECT <plugin/sqlite3/Sqlite3Schema.h>
#define MODEL <SqlCtrls/Model.sch>
#include "Sql/sch_header.h"

class PersonsDlg : public WithPersonsLayout<TopWindow> {
	typedef PersonsDlg CLASSNAME;


	void Create();
	void Edit();
	void Remove();

public:
	PersonsDlg();
};

#endif
