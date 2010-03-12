#ifndef _HomeBudget_HomeBudget_h
#define _HomeBudget_HomeBudget_h

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
		DropDate dt;
		DropGrid dlpm, category, months, yesno, plusminus;
		EditDouble defval, val;
		EditStringNotNull eg, ec;
		EditString es;

		int dtid;
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
		void AcceptedDate();

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
