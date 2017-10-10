#include "SqlCtrl.h"

namespace Upp {

void SqlLoad(MapConvert& cv, Sql& sql) {
	cv.Clear();
	while(sql.Fetch())
		cv.Add(sql[0], sql[1]);
}

void SqlLoad(MapConvert& cv, const SqlSelect& set, SqlSession& ss) {
	Sql sql(ss);
	ss.ClearError();
	sql * set;
	if(ShowError(sql)) return;
	SqlLoad(cv, sql);
}

#ifndef NOAPPSQL
void operator*=(MapConvert& cv, const SqlSelect& set) {
	SqlLoad(cv, set);
}
#endif

void SqlLoad(DropList& dl, Sql& sql) {
	dl.ClearList();
	while(sql.Fetch())
		if(sql.GetColumns() == 1)
			dl.Add(sql[0]);
		else
			dl.Add(sql[0], sql[1]);
}

void SqlLoad(DropList& dl, const SqlSelect& set, SqlSession& ss) {
	Sql sql(ss);
	ss.ClearError();
	sql * set;
	if(ShowError(sql)) return;
	SqlLoad(dl, sql);
}

#ifndef NOAPPSQL
void operator*=(DropList& dl, const SqlSelect& set) {
	SqlLoad(dl, set);
}
#endif

void  SqlOption::SetData(const Value& data) {
	String s = data;
	if(IsThreeState() && IsNull(data))
		Set(Null);
	else
		Set(!(IsNull(s) || s == "0"));
}

Value SqlOption::GetData() const {
	if(IsThreeState() && IsNull(Get()))
		return String();
	return Get() ? "1" : "0";
}

Value SqlNOption::GetData() const
{
	if(Get()) return "1";
	return Null;
}

void SqlCtrls::Table(Ctrl& dlg, SqlId table)
{
	Vector<String> col = GetSchColumns(~table);
	for(Ctrl *q = dlg.GetFirstChild(); q; q = q->GetNext()) {
		String id = ToUpper(q->GetLayoutId());
		if(!dynamic_cast<Button *>(q) && !dynamic_cast<Label *>(q) && FindIndex(col, id) >= 0)
			Add(id, *q);
	}
}

SqlSet SqlCtrls::Set() const {
	SqlSet set;
	for(int i = 0; i < item.GetCount(); i++)
		set.Cat(SqlId(item[i].id));
	return set;
}

void SqlCtrls::Read(Sql& sql)
{
	for(int i = 0; i < item.GetCount(); i++) {
		Item& m = item[i];
		m.ctrl->SetData(sql[SqlId(m.id)]);
	}
}

bool SqlCtrls::Fetch(Sql& sql) {
	if(!sql.Fetch()) return false;
	Read(sql);
	return true;
}

void SqlCtrls::Insert(SqlInsert& insert) const {
	for(int i = 0; i < item.GetCount(); i++)
		insert(item[i].id, item[i].ctrl->GetData());
}

void SqlCtrls::Update(SqlUpdate& update) const {
	for(int i = 0; i < item.GetCount(); i++)
		update(item[i].id, item[i].ctrl->GetData());
}

void SqlCtrls::UpdateModified(SqlUpdate& update) const {
	for(int i = 0; i < item.GetCount(); i++)
		if(item[i].ctrl->IsModified())
			update(item[i].id, item[i].ctrl->GetData());
}

SqlInsert SqlCtrls::Insert(SqlId table) const {
	SqlInsert insert(table);
	Insert(insert);
	return insert;
}

SqlUpdate SqlCtrls::Update(SqlId table) const {
	SqlUpdate update(table);
	Update(update);
	return update;
}

SqlUpdate SqlCtrls::UpdateModified(SqlId table) const {
	SqlUpdate update(table);
	UpdateModified(update);
	return update;
}

Callback SqlCtrls::operator<<=(Callback cb)
{
	for(int i = 0; i < item.GetCount(); i++)
		item[i].ctrl->WhenAction = cb;
	return cb;
}

bool SqlCtrls::Load(Sql& sql, SqlId table, SqlBool where)
{
	sql * Select(*this).From(table).Where(where);
	return Fetch(sql);
}

#ifndef NOAPPSQL
bool SqlCtrls::Load(SqlId table, SqlBool set)
{
	return Load(SQL, table, set);
}
#endif

}
