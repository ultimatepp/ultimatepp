#include "SqlCtrls.h"

#ifdef _DEBUG
#include <Sql/sch_schema.h>
#endif

#include <Sql/sch_source.h>

struct PersonDlg : public WithPersonLayout<TopWindow> {
	typedef PersonDlg CLASSNAME;

	SqlCtrls ctrls;

	PersonDlg();
};

PersonDlg::PersonDlg()
{
	CtrlLayoutOKCancel(*this, "Person");
#ifdef flagSCH
	ctrls(*this, PERSON); // matches widgets to columns based on Layout and schema introspection
#else
	ctrls // "manual" variant
		(NAME, name)
		(LASTNAME, lastname)
		(AGE, age)
	;
#endif
}

void PersonsDlg::Create()
{
	PersonDlg dlg;
	dlg.Title("New person");
	if(dlg.Execute() != IDOK)
		return;
	SQL * dlg.ctrls.Insert(PERSON);
	int id = SQL.GetInsertedId();
	list.ReQuery();
	list.FindSetCursor(id);
}

void PersonsDlg::Edit()
{
	int id = list.GetKey();
	if(IsNull(id))
		return;
	PersonDlg dlg;
	dlg.Title("Edit person");
	if(!dlg.ctrls.Load(PERSON, ID == id))
		return;
	if(dlg.Execute() != IDOK)
		return;
	SQL * dlg.ctrls.Update(PERSON).Where(ID == id);
	list.ReQuery();
	list.FindSetCursor(id);
}

void PersonsDlg::Remove()
{
	int id = list.GetKey();
	if(IsNull(id) || !PromptYesNo("Delete row?"))
		return;
	SQL * SqlDelete(PERSON).Where(ID == id);
	list.ReQuery();
}

PersonsDlg::PersonsDlg()
{
	CtrlLayout(*this, "Persons");

	create << THISFN(Create);
	edit << THISFN(Edit);
	remove << THISFN(Remove);
	
	list.SetTable(PERSON);
	list.AddKey(ID);
	list.AddColumn(NAME, "Name");
	list.AddColumn(LASTNAME, "Last Name");
	list.AddColumn(AGE, "Age");
	list.WhenLeftDouble = THISFN(Edit);
	list.SetOrderBy(AGE, NAME);
	list.Query();
}

GUI_APP_MAIN
{
	Sqlite3Session sqlite3;
	if(!sqlite3.Open(GetHomeDirFile("persons.db"))) {
		LOG("Can't create or open database file\n");
		return;
	}
	sqlite3.SetTrace();

	SQL = sqlite3;

	// Update the schema to match the schema described in "Model.sch"
#ifdef _DEBUG
	SqlSchema sch(SQLITE3);
	All_Tables(sch);
	if(sch.ScriptChanged(SqlSchema::UPGRADE))
		SqlPerformScript(sch.Upgrade());
	if(sch.ScriptChanged(SqlSchema::ATTRIBUTES))
		SqlPerformScript(sch.Attributes());
	sch.SaveNormal();
#endif

	PersonsDlg().Run();
}
