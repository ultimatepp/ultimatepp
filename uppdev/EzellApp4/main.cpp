#include <CtrlLib/CtrlLib.h>
#include <SqlCtrl/SqlCtrl.h>
#include <plugin/Sqlite3/Sqlite3.h>

#define SCHEMADIALECT <plugin/Sqlite3/Sqlite3Schema.h>
#define MODEL <EzellApp4/EzellApp4.sch>
#include "Sql/sch_header.h"

// Update Schema when in debug mode
#ifdef _DEBUG
#include <Sql/sch_schema.h>
#endif

// This seems to do some magic schema defining
#include <Sql/sch_source.h>

// Include the layout
#define LAYOUTFILE <EzellApp4/EzellApp4.lay>
#include <CtrlCore/lay.h>

class EzellApp4 : public WithEzellApp4Layout<TopWindow> {
	WithListLayout<ParentCtrl> list;
	WithDetailLayout<ParentCtrl> detail;

	// For editing the Jobs SqlArray
	EditString editDate;
	EditString editDesc;
	EditString editCost;

public:
	typedef EzellApp4 CLASSNAME;
	void AddCustomer();
	void SaveRecord();
	void SwitchView();
	void CrossUpdate();
	void Test();
	void Console();
	EzellApp4();
};

// Prompts user to add a new customer record
// TODO: fix
void EzellApp4::AddCustomer()
{
	// This isnt working right
	list.CListView.StartInsert();
	tab.Set(1);
}

// Saves the current record
// TODO: fix (implement!)
void EzellApp4::SaveRecord()
{
	// I don't know how to manually save! UpdateRow() is a proteted function!
}

// Moves to the second tab
void EzellApp4::SwitchView()
{
	tab.Set(1);
}

// Function: Updates list.CListView with changes on detail (since row does not update automatically)
// Problem: this will "set" list.CListView every tab navigation after modification until a different row is selected
//			But this will not happen in "real world" situation
//			Possible fix comes from learning to actually update DB on command
void EzellApp4::CrossUpdate()
{
	int cur_row;

	if(detail.strName.IsModified() || detail.strAddress.IsModified() ||
		detail.strCSZ.IsModified() || detail.strPhone1.IsModified())
	{
		PromptOK("Mods have been made that have not been saved to the DB.");
		// Gets row ID and updates list.CListView
		cur_row=list.CListView.GetCursor();
		if (cur_row < 0) return;
		if ( (~detail.strName) == Null) {
			Exclamation("The Name field cannot be empty!");
			return;
		}
		list.CListView.Set( cur_row, 1, ~detail.strName);
		list.CListView.Set( cur_row, 2, ~detail.strAddress);
		list.CListView.Set( cur_row, 3, ~detail.strCSZ);
		list.CListView.Set( cur_row, 4, ~detail.strPhone1);
		// We can't clear the modifications b/c it's used to update DB!
		//detail.strName.ClearModify();
		//detail.strAddress.ClearModify();
		//detail.strCSZ.ClearModify();
		//detail.strPhone1.ClearModify();
	}
}

// A test callback not implemented in normal source
void EzellApp4::Test()
{
	PromptOK("We updated a row");
}

EzellApp4::EzellApp4()
{
	// Setup the layouts
	CtrlLayout(*this, "Bean Heat and Air- Customer Files");
	CtrlLayout(list);
	CtrlLayout(detail);

	// Allow app to resize
	Sizeable();
	Zoomable();

	// Tell tab control what slave ctrls to use
	tab.Add(list, "Customers");
	tab.Add(detail, "Details");

	// Setup buttons
	list.btnAddCustomer.WhenPush = THISBACK(AddCustomer);
	detail.btnSave.WhenPush = THISBACK(SaveRecord);

	// Custom Callback, called when SqlArray actually updates the database
	// Mirek, I've used this to see when the DB is actually updated.
	// It is when you navigate to a different row,
	//list.CListView.WhenUpdateRow = THISBACK(Test);

	// When navigating tabs, run CrossUpdate function to sync data
	// may become unnecessary if I can actually update the DB on command
	tab.WhenSet = THISBACK(CrossUpdate);

	// Setup primary customer list
	list.CListView.SetTable(CUSTOMERS);
	list.CListView.AddColumn(ID, "Id");
	//list.CListView.HeaderObject().HideTab(0);
	list.CListView.AddColumn(NAME,"Name");
	list.CListView.AddColumn(ADDRESS,"Address");
	list.CListView.AddColumn(CSZ,"City, State Zip");
	list.CListView.AddColumn(PHONE1,"Phone #");
	list.CListView.ColumnWidths("10 167 181 208 130");
	list.CListView.AppendingAuto().Removing();

	// And on the other tab
	list.CListView.AddCtrl(NAME, detail.strName);
	list.CListView.AddCtrl(ADDRESS, detail.strAddress);
	list.CListView.AddCtrl(CSZ, detail.strCSZ);
	list.CListView.AddCtrl(PHONE1, detail.strPhone1);
	list.CListView.AddCtrl(PHONE2, detail.strPhone2);
	list.CListView.AddCtrl(UNIT_INFO, detail.strUnitInfo);
	list.CListView.AddCtrl(NOTES, detail.strNotes);
	list.CListView.AddCtrl(CURRENT, detail.chkCurrent);
	list.CListView.AddCtrl(SERVICE, detail.chkService);

	// Order customers by name
	list.CListView.SetOrderBy(NAME);

	// Set double click to go to details
	list.CListView.WhenLeftDouble = THISBACK(SwitchView);

	// Job detail list
	detail.detailList.SetTable(JOBS);
	detail.detailList.AddColumn(JOBID,"Id");
	detail.detailList.HeaderObject().HideTab(0);
	detail.detailList.AddColumn(DONEDATE,"Date").Edit(editDate);
	detail.detailList.AddColumn(DESCRIPTION,"Description").Edit(editDesc);
	detail.detailList.AddColumn(COST,"Cost").Edit(editCost);
	detail.detailList.ColumnWidths("0 79 523 84");
	detail.detailList.SetOrderBy(JOBID);
	detail.detailList.Join(CUSTOMER_ID, list.CListView);
	detail.detailList.Removing().Appending();

	// Go!
	list.CListView.Query();

	console <<= THISBACK(Console);

}

void EzellApp4::Console()
{
	SQLCommander();
}

GUI_APP_MAIN
{
	// Open the SqLite Session
	Sqlite3Session session;

	// Read in the database
	if(!session.Open(ConfigFile("simple2.db"))) {
		Exclamation("Error opening the database file");
		return;
	}

	// Log errors if we are debugging
	#ifdef _DEBUG
		session.SetTrace();
		session.LogErrors(true);
	#endif

	// Set default SQL session
	SQL = session;

	// If we are in debug mode
	// Update the schema to match the schema described in "EzellApp4.sch"
#ifdef _DEBUG
	SqlSchema sch(SQLD_SQLITE3);
	StdStatementExecutor se(session);
	All_Tables(sch);
	if(sch.ScriptChanged(SqlSchema::UPGRADE))
		Sqlite3PerformScript(sch.Upgrade());
	if(sch.ScriptChanged(SqlSchema::ATTRIBUTES)) {
		Sqlite3PerformScript(sch.Attributes());
	}
	if(sch.ScriptChanged(SqlSchema::CONFIG)) {
		Sqlite3PerformScript(sch.ConfigDrop());
		Sqlite3PerformScript(sch.Config());
	}
	sch.SaveNormal();
#endif

	// Run the program
	EzellApp4().Run();
}