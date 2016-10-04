#ifndef _HomeBudget_HomeBudget_h
#define _HomeBudget_HomeBudget_h

#include <CtrlLib/CtrlLib.h>
#include <DateTimeCtrl/DateTimeCtrl.h>
#include <GridCtrl/GridCtrl.h>
#include <plugin/Sqlite3/lib/Sqlite3.h>
#include <plugin/Sqlite3/Sqlite3.h>

#define IMAGEFILE <HomeBudget/HomeBudget.iml>
#define IMAGECLASS Images
#include <Draw/iml_header.h>

#define SCHEMADIALECT <plugin/Sqlite3/Sqlite3Schema.h>
#define MODEL <HomeBudget/HomeBudget.sch>
#include "Sql/sch_header.h"

#define LAYOUTFILE <HomeBudget/HomeBudget.lay>
#include <CtrlCore/lay.h>

Sqlite3Session db;

class HomeBudgetHelp : public HelpWindow
{
	public:
		HomeBudgetHelp();
		virtual Topic AcquireTopic(const String& topic);
};

class HomeBudget : public WithHomeBudgetLayout<TopWindow>
{
	public:
		typedef HomeBudget CLASSNAME;

		GridCtrl groups;
		GridCtrl categories;
		GridCtrl money;
		GridCtrl mostpr;
		GridCtrl mostcat;
		Splitter spl;
		DateTimeCtrl dt;
		DropList dlpm, category, months, yesno, plusminus;
		EditDouble defval, val;
		EditString es, eg, ec;
		FrameRight<Button> newcat;

		int dtid;
		bool dosummary;
		int lang;

		HomeBudget();

		void NewGroup();
		void NewCategory();

		void InsertGroup();
		void UpdateGroup();
		void RemoveGroup();
		void ChangeGroup();

		void InsertCategory();
		void UpdateCategory();
		void RemoveCategory();
		void LoadCategories(int group_id);
		void UpdateCategories();

		void InsertMoney();
		void UpdateMoney();
		void RemoveMoney();

		void InsertDate();
		void UpdateDate();
		void RemoveDate();
		void ChangeDate();
		void NewDate();
		void AcceptDate();

		void LoadDates();
		void LoadGroups();

		void UpdateSummary();
		void ClearSummary();
		void UpdateValue();
		void GenMonthList(int year);
		void Serialize(Stream &s);
		void Options();
		void About();
		void Setup();

		void Help();

		void SetRest(StaticText &rest, float r);
		int GetCategorySign();

		void ClearAll();


		void LoadMoney(int did);
		void EnableMoney(int cnt = -1);

};

#endif
