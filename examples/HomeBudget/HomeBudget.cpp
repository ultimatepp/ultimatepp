#include <CtrlLib/CtrlLib.h>
#include <DropGrid/DropGrid.h>
#include <GridCtrl/GridCtrl.h>
#include <plugin/sqlite3/Sqlite3.h>

using namespace Upp;

#define IMAGEFILE <HomeBudget/HomeBudget.iml>
#define IMAGECLASS Images
#include <Draw/iml.h>

#define SCHEMADIALECT <plugin/sqlite3/Sqlite3Schema.h>
#define MODEL <HomeBudget/HomeBudget.sch>

#include <Sql/sch_schema.h>
#include <Sql/sch_header.h>
#include <Sql/sch_source.h>

#define LAYOUTFILE <HomeBudget/HomeBudget.lay>
#include <CtrlCore/lay.h>

#define TOPICFILE <HomeBudget/src.tpp/all.i>
#include <Core/topic_group.h>
#define TOPICFILE <HomeBudget/help.tpp/all.i>
#include <Core/topic_group.h>

#define TFILE <HomeBudget/HomeBudget.t>
#include <Core/t.h>

#include "HomeBudget.h"

struct ConvMonth : Convert
{
	Value Format(const Value &q) const
	{
		Date dt = (Date) q;
		return q.IsNull() ? "" : UPP::Format("%Month %.4d", dt.month, dt.year);
	}
};

struct ConvDouble : Convert
{
	Value Format(const Value &q) const
	{
		return q.IsNull() ? Null : UPP::Format("%.2f", q);
	}
};

struct DispPM : GridDisplay
{
	virtual void Paint(Draw &w, int x, int y, int cx, int cy, const Value &val, dword style,
			           Color &fg, Color &bg, Font &fnt, bool found, int fs, int fe)
	{
		if(!val.IsNull())
		{
			int t = int(val) < 0 ? 0 : 1;
			SetCenterImage(t ? Images::Plus : Images::Minus);
		}
		else
			SetCenterImage(Null);
		GridDisplay::Paint(w, x, y, cx, cy, Value(""), style, fg, bg, fnt, found, fs, fe);
	}
};

HomeBudget::HomeBudget()
{
	lang = 0;
}

void HomeBudget::Setup()
{
	CtrlLayout(*this, String(t_("Home budget")) + " 1.0");

	Icon(Images::SmallIcon());
	LargeIcon(Images::LargeIcon());

	tab.Add(money.SizePos(), t_("Expenses"));
	tab.Add(spl.Horz(groups, categories), t_("Categories"));
	spl.SetPos(2000);

	money.AddIndex(ID);
	money.AddColumn(CAT_ID, t_("Category")).Edit(category).SetConvert(category);
	money.AddColumn(PM, t_("Plus / Minus")).SetDisplay(Single<DispPM>()).Edit(plusminus);
	money.AddColumn(VALUE, t_("Value")).Edit(val).Default(0).SetConvert(Single<ConvDouble>());
	money.AddColumn(DT, t_("When")).Edit(dt).Default(GetSysDate());
	money.AddColumn(DESC, t_("Describe")).Edit(es);
	money.Appending().Removing().Editing().Accepting().Canceling();
	money.RejectNullRow();
	money.WhenInsertRow = THISBACK(InsertMoney);
	money.WhenUpdateRow = THISBACK(UpdateMoney);
	money.WhenRemoveRow = THISBACK(RemoveMoney);
	money.SetToolBar();

	Date dt = GetSysDate();
	dt.day = 1;

	month.AddIndex(ID);
	month.AddColumn(DT, t_("Month")).Edit(months).SetConvert(Single<ConvMonth>()).Default(dt);
	month.Appending().Removing().Editing().Accepting().Canceling();
	month.WhenInsertRow = THISBACK(InsertDate);
	month.WhenUpdateRow = THISBACK(UpdateDate);
	month.WhenRemoveRow = THISBACK(RemoveDate);
	month.WhenChangeRow = THISBACK(ChangeDate);
	month.WhenAcceptedRow = THISBACK(AcceptedDate);
	month.WhenNewRow    = THISBACK(NewDate);
	month.SetToolBar();

	groups.AddIndex(ID);
	groups.AddColumn(NAME, t_("Name")).Edit(eg);
	groups.Appending().Removing().Editing().Accepting().Canceling();
	groups.RejectNullRow();
	groups.SetToolBar();
	groups.WhenInsertRow = THISBACK(InsertGroup);
	groups.WhenUpdateRow = THISBACK(UpdateGroup);
	groups.WhenRemoveRow = THISBACK(RemoveGroup);
	groups.WhenChangeRow = THISBACK(ChangeGroup);

	categories.AddIndex(ID);
	categories.AddColumn(NAME, t_("Name")).Edit(ec);
	categories.AddColumn(DEFVALUE, t_("Default value")).Edit(defval).SetConvert(Single<ConvDouble>());
	categories.AddColumn(PM, t_("Plus / Minus")).Edit(dlpm).SetConvert(dlpm).Default(-1);
	categories.AddColumn(INNEWMONTH, t_("Default for a new month")).Edit(yesno).SetConvert(yesno).Default(0);
	categories.WhenInsertRow = THISBACK(InsertCategory);
	categories.WhenUpdateRow = THISBACK(UpdateCategory);
	categories.WhenRemoveRow = THISBACK(RemoveCategory);
	categories.WhenAcceptedRow = THISBACK(UpdateCategories);
	categories.Appending().Removing().Editing();
	categories.RejectNullRow();
	categories.SetToolBar();

	mostpr.AddColumn(t_("Name"));
	mostpr.AddColumn(t_("Amount")).SetConvert(Single<ConvDouble>()).AlignCenter();
	mostcat.AddColumn(t_("Name"));
	mostcat.AddColumn(t_("Amount")).SetConvert(Single<ConvDouble>()).AlignCenter();

	tabmost.Add(mostpr.SizePos(), t_("Product"));
	tabmost.Add(mostcat.SizePos(), t_("Category"));

	dlpm.Add(-1, t_("Minus")).Add(1, t_("Plus"));
	plusminus.Add(-1, t_("Minus")).Add(1, t_("Plus"));
	yesno.Add(0, t_("No")).Add(1, t_("Yes"));

	GenMonthList(dt.year);

	SQL * Delete(MONEY).Where(IsNull(CAT_ID));

	LoadDates();
	LoadGroups();

	UpdateCategories();

	if(month.GetCount() > 0)
	{
		month.GoEnd();
		UpdateSummary();
	}
	
	EnableMoney();
	Sizeable().Zoomable();

	category <<= THISBACK(UpdateValue);
	exit <<= THISBACK(Close);
	options <<= THISBACK(Options);
	about <<= THISBACK(About);
	help <<= THISBACK(Help);

	category.Resizeable(false).Header(false);
	category.AddPlus(THISBACK(NewCategory));
}

void HomeBudget::Serialize(Stream &s)
{
	int version = 1;
	s / version;
	SerializePlacement(s);
	s % spl;
	s % lang;
	//s % categories;
	//s % money;
}

void HomeBudget::GenMonthList(int year)
{
	months.Clear();
	
	int curr_year = GetSysDate().year;
	int max_year = max(year,curr_year);

	for(int scan_year = min(year, curr_year) - 1; scan_year <= max_year; scan_year++)
		for(int i = 0; i < 12; i++)
			months.Add(Date(scan_year, i + 1, 1), Format("%Month %.4d", i + 1, scan_year));
}

void HomeBudget::LoadMoney(int dtid)
{
	money.Clear();
	SQL * Select(ID, CAT_ID, PM, VALUE, DT, DESC).From(MONEY).Where(DT_ID == dtid);
	while(SQL.Fetch())
	{
		money.Add();
		money(ID) = SQL[ID];
		money(CAT_ID) = SQL[CAT_ID];
		money(PM) = SQL[PM];
		money(VALUE) = SQL[VALUE];
		money(DT) = SQL[DT];
		money(DESC) = SQL[DESC];
	}
}

void HomeBudget::LoadDates()
{
	month.Clear();
	SQL * Select(ID, DT).From(DATES);
	while(SQL.Fetch())
		month.Add(SQL);
}

void HomeBudget::LoadGroups()
{
	groups.Clear();
	SQL * Select(ID, NAME).From(GROUPS);
	while(SQL.Fetch())
		groups.Add(SQL);

	if(!groups.IsEmpty())
		groups.SetCursor(0);
	else
		categories.Disable();
}

void HomeBudget::LoadCategories(int group_id)
{
	categories.Clear();
	SQL * Select(ID, NAME, DEFVALUE, PM, INNEWMONTH).From(CATEGORIES).Where(GR_ID == group_id);
	while(SQL.Fetch())
		categories.Add(SQL);
}

int HomeBudget::GetCategorySign()
{
	SQL * Select(PM).From(CATEGORIES).Where(ID == money.Get(CAT_ID));
	Value v = SQL.Fetch() ? SQL[0] : Value(0);
	return v;
}

void HomeBudget::UpdateCategories()
{
	category.Clear();
	SQL & Select(ID.Of(CATEGORIES), NAME.Of(CATEGORIES), NAME.Of(GROUPS))
		.From(CATEGORIES, GROUPS)
		.Where(GR_ID == ID.Of(GROUPS))
		.OrderBy(NAME.Of(GROUPS), NAME.Of(CATEGORIES));

	while(SQL.Fetch())
		category.Add(SQL[0], Format("%s - %s", SQL[2], SQL[1]));
}

void HomeBudget::InsertGroup()
{
	try
	{
		SQL & Insert(GROUPS)
			(NAME, groups(1));
		groups(0) = SQL.GetInsertedId();

		categories.Clear();
		categories.Enable();
	}
	catch(SqlExc &e)
	{
		groups.CancelInsert();
		Exclamation("[* " + DeQtfLf(e) + "]");
	}

}

void HomeBudget::UpdateGroup()
{
	try
	{
		SQL & ::Update(GROUPS)
			(NAME, groups(1))
			.Where(ID == groups(0));
	}
	catch(SqlExc &e)
	{
		groups.CancelUpdate();
		Exclamation("[* " + DeQtfLf(e) + "]");
	}
}

void HomeBudget::RemoveGroup()
{
	if(categories.IsEmpty())
	{
		SQL & Delete(GROUPS).Where(ID == groups(ID));
		
		if(groups.GetCount() == 1)
			categories.Disable();
	}
	else
	{
		PromptOK(t_("You can't remove this group. It is not empty."));
		groups.CancelRemove();
	}
}

struct AddNewCat : WithNewCategoryLayout<TopWindow>
{
	typedef AddNewCat CLASSNAME;
	bool newgroup;

	AddNewCat()
	{
		newgroup = false;
		CtrlLayoutOKCancel(*this, t_("New item"));
		addgroup.SetImage(Images::SmallPlus());
		addgroup <<= THISBACK(NewGroup);
		groups <<= THISBACK(SelGroup);
		addgroup.NoWantFocus();

		int cnt = 0;
		SQL * Select(ID, NAME).From(GROUPS);
		while(SQL.Fetch())
		{
			groups.Add(SQL[ID], SQL[NAME]);
			cnt++;
		}

		bool isgroup = cnt > 0;
		if(isgroup)
			groups.SetIndex(0);
		else
			ok.Disable();

		pm = 1;
	}

	void SelGroup()
	{
		ok.Enable();
	}

	void NewGroup()
	{
		WithNewGroupLayout<TopWindow> dlg;
		CtrlLayoutOKCancel(dlg, t_("New category"));
		if(dlg.Execute() == IDOK)
		{
			try
			{
				SQL & Insert(GROUPS)(NAME, ~dlg.name);
				int64 id = SQL.GetInsertedId();
				groups.Add(id, ~dlg.name);
				groups <<= id;
				ok.Enable();
				newgroup = true;
			}
			catch(SqlExc &e)
			{
				Exclamation("[* " + DeQtfLf(e) + "]");
			}
		}
	}
	bool IsNewGroup() { return newgroup; }
};

void HomeBudget::NewCategory()
{
	AddNewCat dlg;

	if(dlg.Execute() == IDOK)
	{
		try
		{
			SQL & Insert(CATEGORIES)
			    (GR_ID, ~dlg.groups)
				(NAME, ~dlg.name)
				(DEFVALUE, 0)
				(PM, dlg.pm == 0 ? 1 : -1)
				(INNEWMONTH, 0);

			int64 id = SQL.GetInsertedId();
			UpdateCategories();
			money.Set(CAT_ID, id);
			UpdateValue();

			int gid = ~dlg.groups;

			if(dlg.IsNewGroup())
			{
				groups.Add(Value(gid), dlg.groups.GetValue());
				groups.GoEnd();
				categories.Enable();
			}
			int c = groups.GetCursor();
			if(c >= 0 && gid == groups(c, 0))
				LoadCategories(gid);
		}
		catch(SqlExc &e)
		{
			Exclamation("[* " + DeQtfLf(e) + "]");
		}
	}
}

void HomeBudget::InsertCategory()
{
	try
	{
		SQL & Insert(CATEGORIES)
		    (GR_ID, groups(ID))
			(NAME, categories(NAME))
			(DEFVALUE, categories(DEFVALUE))
			(PM, categories(PM))
			(INNEWMONTH, categories(INNEWMONTH));

		categories(0) = SQL.GetInsertedId();
	}
	catch(SqlExc &e)
	{
		categories.CancelInsert();
		Exclamation("[* " + DeQtfLf(e) + "]");
	}

}

void HomeBudget::UpdateCategory()
{
	try
	{
		SQL & ::Update(CATEGORIES)
			(NAME, categories(NAME))
			(DEFVALUE, categories(DEFVALUE))
			(PM, categories(PM))
			(INNEWMONTH, categories(INNEWMONTH))
			.Where(ID == categories(ID));

		UpdateSummary();
	}
	catch(SqlExc &e)
	{
		categories.CancelUpdate();
		Exclamation("[* " + DeQtfLf(e) + "]");
	}
}

void HomeBudget::ChangeGroup()
{
	LoadCategories(groups(ID));
}

void HomeBudget::RemoveCategory()
{
	try
	{
		SQL * Select(CAT_ID, NAME)
			.From(MONEY, CATEGORIES)
			.Where(CAT_ID == categories(ID) && ID.Of(CATEGORIES) == categories(ID))
			.Limit(1);

		if(SQL.Fetch())
		{
			PromptOK(Format(t_("Item '%s' can't be removed. It is used in calculations."), AsString(SQL[1])));
			categories.CancelRemove();
		}
		else
		{
			SQL & Delete(CATEGORIES).Where(ID == categories(ID));
			UpdateCategories();
		}
	}
	catch(SqlExc &e)
	{
		categories.CancelRemove();
		Exclamation("[* " + DeQtfLf(e) + "]");
	}
}

void HomeBudget::InsertMoney()
{
	try
	{
		SQL & Insert(MONEY)
			(DT_ID,  dtid)
			(CAT_ID, money(CAT_ID))
			(PM,     money(PM))
			(VALUE,  money(VALUE))
			(DT,     money(DT))
			(DESC,   money(DESC));

		money(ID) = SQL.GetInsertedId();

		UpdateSummary();
	}
	catch(SqlExc &e)
	{
		money.CancelInsert();
		Exclamation("[* " + DeQtfLf(e) + "]");
	}
}

void HomeBudget::UpdateMoney()
{
	try
	{
		SQL & ::Update(MONEY)
			(DT_ID,  dtid)
			(CAT_ID, money(CAT_ID))
			(PM,     money(PM))
			(VALUE,  money(VALUE))
			(DT,     money(DT))
			(DESC,   money(DESC))
			.Where(ID == money(ID));

		UpdateSummary();
	}
	catch(SqlExc &e)
	{
		money.CancelUpdate();
		Exclamation("[* " + DeQtfLf(e) + "]");
	}
}

void HomeBudget::RemoveMoney()
{
	try
	{
		SQL & Delete(MONEY).Where(ID == money(ID));
		UpdateSummary();
	}
	catch(SqlExc &e)
	{
		money.CancelRemove();
		Exclamation("[* " + DeQtfLf(e) + "]");
	}
}

void HomeBudget::InsertDate()
{
	try
	{
		SQL & Insert(DATES)(DT, month(DT));
		month(ID) = dtid = (int) SQL.GetInsertedId();
		EnableMoney();
	}
	catch(SqlExc &e)
	{
		month.CancelInsert();
		Exclamation("[* " + DeQtfLf(e) + "]");
	}
}

void HomeBudget::EnableMoney(int cnt)
{
	money.Enable(cnt < 0 ? month.GetCount() > 0 : cnt);
}

void HomeBudget::UpdateDate()
{
	try
	{
		SQL & ::Update(DATES)(DT, month(DT)).Where(ID == month(ID));
	}
	catch(SqlExc &e)
	{
		month.CancelUpdate();
		Exclamation("[* " + DeQtfLf(e) + "]");
	}
}

void HomeBudget::RemoveDate()
{
	try
	{
		SQL.Begin();
		SQL & Delete(MONEY).Where(DT_ID == month(ID));
		SQL & Delete(DATES).Where(ID == month(ID));
		SQL.Commit();
		bool en = month.GetCount() <= 1 ? false : true;
		EnableMoney(en);
		if(!en)
			money.Clear();
	}
	catch(SqlExc &e)
	{
		SQL.Rollback();
		month.CancelRemove();
		Exclamation("[* " + DeQtfLf(e) + "]");
	}

}

void HomeBudget::ChangeDate()
{
	dtid = month(ID);
	LoadMoney(dtid);
	EnableMoney();
	UpdateSummary();
	GenMonthList(((Date) month(DT)).year);
}

void HomeBudget::NewDate()
{
	money.Clear();
	money.Disable();
	UpdateSummary();
}

void HomeBudget::AcceptedDate()
{
	if(!month.IsNewRow())
		return;

	Sql q;
	SQL.Begin();
	SQL * Select(ID, DEFVALUE, PM).From(CATEGORIES).Where(INNEWMONTH == 1);
	while(SQL.Fetch())
	{
		money.Add();
		money(CAT_ID) = SQL[ID];
		money(VALUE) = SQL[DEFVALUE];
		money(PM) = SQL[PM];

		q * Insert(MONEY)
			(DT_ID,  dtid)
			(CAT_ID, money(CAT_ID))
			(PM,     money(PM))
			(VALUE,  money(VALUE))
			(DT,     money(DT))
			(DESC,   money(DESC));

		money(ID) = SQL.GetInsertedId();

		money.RefreshNewRow();
	}
	SQL.Commit();
	UpdateSummary();
}

void HomeBudget::UpdateSummary()
{
	if(!month.IsCursor())
		return;

	float p = 0, tp = 0;
	float m = 0, tm = 0;
	float r = 0, tr = 0;

	mostcat.Clear();
	mostpr.Clear();

	try
	{
		SQL & Select(PM, SqlSum(VALUE))
			.From(MONEY, DATES)
			.Where(DT.Of(DATES) < month(DT) && DT_ID == ID.Of(DATES) && NotNull(VALUE))
			.GroupBy(PM);

		while(SQL.Fetch())
		{
			int pm = SQL[PM];
			float v = (float) int(SQL[1]);
			if(pm < 0)
				tm = v;
			else
				tp = v;
		}

		tr = tp - tm;

		SQL & Select(PM, SqlSum(VALUE))
			.From(MONEY)
			.Where(DT_ID == dtid && NotNull(VALUE))
			.GroupBy(PM);

		while(SQL.Fetch())
		{
			int pm = SQL[PM];
			float v = (float) int(SQL[1]);
			if(pm < 0)
				m = v;
			else
				p = v;
		}

		SQL & Select(NAME, SqlId("sum(value) as val"))
			.From(MONEY, CATEGORIES)
			.Where(DT_ID == dtid && CAT_ID == ID.Of(CATEGORIES) && PM.Of(MONEY) < 0)
			.GroupBy(CAT_ID)
			.OrderBy(Descending(SqlId("val")));

		while(SQL.Fetch())
			mostpr.Add(SQL);

		SQL & Select(NAME.Of(GROUPS), SqlId("sum(value) as val"))
			.From(MONEY, GROUPS, CATEGORIES)
			.Where(DT_ID == dtid && PM.Of(MONEY) < 0 && CAT_ID == ID.Of(CATEGORIES) && GR_ID == ID.Of(GROUPS))
			.GroupBy(GR_ID)
			.OrderBy(Descending(SqlId("val")));

		while(SQL.Fetch())
			mostcat.Add(SQL);
	}
	catch(SqlExc &e)
	{
		Exclamation("[* " + DeQtfLf(e) + "]");
	}

	r = p - m;
	plus.SetText(Format("%.2f", p));
	minus.SetText(Format("%.2f", m));
	SetRest(rest, r);
	SetRest(prev_month, tr);
	SetRest(real_rest, r + tr);
}

void HomeBudget::ClearSummary()
{
	plus.SetText("");
	minus.SetText("");
	rest.SetText("");
	prev_month.SetText("");
	real_rest.SetText("");
	mostpr.Clear();
	mostcat.Clear();
}

void HomeBudget::SetRest(StaticText &rest, float r)
{
	if(r == 0)
	{
		rest.SetText("0.00");
		rest.SetInk(Black);
	}
	else if(r > 0)
	{
		rest.SetText(Format("+%.2f", r));
		rest.SetInk(Green);
	}
	else
	{
		rest.SetText(Format("%.2f", r));
		rest.SetInk(LtRed);
	}
}

void HomeBudget::UpdateValue()
{
	if(money.IsEmpty())
		return;
		
	try
	{
		SQL & Select(DEFVALUE, PM).From(CATEGORIES).Where(ID == money.Get(CAT_ID));
		Value v = SQL.Fetch() ? SQL[0] : Value(0);
		Value s = SQL.Fetch() ? SQL[1] : Value(-1);
		
		if(IsNull(money.Get(VALUE)))
			money.Set(VALUE, v);
		
		if(IsNull(money.Get(PM)))
			money.Set(PM, s);
	}
	catch(SqlExc &e)
	{
		Exclamation("[* " + DeQtfLf(e) + "]");
	}
}

void HomeBudget::Options()
{
	WithOptionsLayout<TopWindow> dlg;
	CtrlLayoutOK(dlg, t_("Options"));
	dlg.lang.Add(0, "English")
	        .Add(1, "Česky")
	        .Add(2, "Polski")
	        .SetIndex(lang);
	dlg.clear <<= THISBACK(ClearAll);
	if(dlg.Execute() != IDOK)
		return;
	lang = ~dlg.lang;
}

void HomeBudget::ClearAll()
{
	try
	{
		SQL & Delete(CATEGORIES);
		SQL & Delete(GROUPS);
		SQL & Delete(MONEY);
		SQL & Delete(DATES);
		//SQL.ExecuteX("VACUUM");

		LoadDates();
		LoadGroups();
		UpdateCategories();
		money.Clear();
		categories.Clear();
		EnableMoney();

		PromptOK(t_("Database was cleared"));
	}
	catch(SqlExc &e)
	{
		Exclamation("[* " + DeQtfLf(e) + "]");
	}
}

void HomeBudget::About()
{
	WithAboutLayout<TopWindow> dlg;
	CtrlLayoutCancel(dlg, t_("About"));
	dlg.info.NoSb();
	dlg.info.SetQTF(GetTopic(String("HomeBudget/src/About$") + (lang == 0 ? "en-us" : "pl-pl")), Zoom(150, 1400));
	dlg.info.SetZoom(Zoom(1, 1));
	dlg.Execute();
}

void HomeBudget::Help()
{
	PromptOK(t_("No help available yet"));
/*	HomeBudgetHelp dlg;
	dlg.AddTree(0, Images::Plus(), "HomeBudget/help/Introduction$pl-pl", "Wprowadzenie");
	dlg.AddTree(0, Images::Plus(), "HomeBudget/help/T0$pl-pl", "Instrukcja");
	dlg.AddTree(0, Images::Plus(), "HomeBudget/src/About$pl-pl", "ala");
	dlg.Execute();
*/
}

HomeBudgetHelp::HomeBudgetHelp()
{
}

Topic HomeBudgetHelp::AcquireTopic(const String& topic)
{
	Topic t;
	t = GetTopic(topic);
	t.text = (const char *) GetTopic(topic);
	//String path = topic;
	//if(FileExists(path))
	//	t.text = ReadTopic(LoadFile(path)) .text;
	return t;
}

GUI_APP_MAIN
{
	bool nodb = false;
	Sqlite3Session db;
	db.LogErrors(true);

	#ifdef flagDEBUG
	db.SetTrace();
	nodb = true;
	#endif

	FileIn fi("HomeBudget.db3");
	if(fi.IsError() || fi.GetSize() <= 0)
		nodb = true;
	fi.Close();

	if(!db.Open(ConfigFile("HomeBudget.db3")))
	{
		Exclamation(t_("Can't create or open database file"));
		return;
	}

	SQL = db;

	if(nodb)
	{
		SqlSchema sch(SQLITE3);
		StdStatementExecutor se(db);
		All_Tables(sch);
		if(sch.ScriptChanged(SqlSchema::UPGRADE))
			Sqlite3PerformScript(sch.Upgrade(), se);
		if(sch.ScriptChanged(SqlSchema::ATTRIBUTES))
		{
			Sqlite3PerformScript(sch.Attributes(), se);
		}
		if(sch.ScriptChanged(SqlSchema::CONFIG))
		{
			Sqlite3PerformScript(sch.ConfigDrop(), se);
			Sqlite3PerformScript(sch.Config(), se);
		}
		sch.SaveNormal();
	}

	HomeBudget hb;

	LoadFromFile(hb);

	switch(hb.lang) {
	case 1:
		SetLanguage(LNGC_('C','S','C','Z', CHARSET_UNICODE));
		break;
	case 2:
		SetLanguage(LNGC_('P','L','P','L', CHARSET_UNICODE));
		break;
	default:
		SetLanguage(LNGC_('E','N','U','S', CHARSET_UNICODE));
		break;
	}
	hb.Setup();
	hb.Run();
	StoreToFile(hb);
}
