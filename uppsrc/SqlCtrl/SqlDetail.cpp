#include "SqlCtrl.h"

namespace Upp {

SqlDetail::SqlDetail()
{
	present = false;
	autocreate = false;
	ssn = NULL;
}

SqlDetail& SqlDetail::Add(SqlId id, Ctrl& ctrl)
{
	ctrls(id, ctrl);
	ctrl.Disable();
	return *this;
}

SqlDetail& SqlDetail::Join(SqlId id, ArrayCtrl& master)
{
	master.AddCtrlAt(0, *this);
	fk = id;
	return *this;
}

SqlDetail& SqlDetail::Join(ArrayCtrl& master)
{
	Join(master.GetKeyId(), master);
	return *this;
}

void SqlDetail::Query()
{
	present = false;
	ctrls.Disable();
	ctrls.SetNull();
	if(fk.IsNull() || IsNull(fkval)) return;
	Sql sql(Session());
	sql.ClearError();
	sql * Select(ctrls).From(table).Where(fk == fkval);
	if(ShowError(sql)) return;
	present = ctrls.Fetch(sql);
	ctrls.Enable(present || autocreate);
	ctrls.ClearModify();
	WhenPostQuery();
}

bool SqlDetail::Create()
{
	if(IsPresent() || fk.IsNull() || IsNull(fkval)) return false;
	ClearModify();
	Sql sql(Session());
	sql.ClearError();
	sql * ctrls.Insert(table)(fk, fkval);
	if(ShowError(sql)) return false;
	present = true;
	ctrls.Enable();
	return true;
}

bool SqlDetail::Delete()
{
	if(!IsPresent() || fk.IsNull() || IsNull(fkval)) return false;
	Sql sql(Session());
	sql * UPP::Delete(table).Where(fk == fkval);
	if(ShowError(sql)) return false;
	present = false;
	return true;
}

void  SqlDetail::SetData(const Value& v)
{
	fkval = v;
	Query();
}

Value SqlDetail::GetData() const
{
	return fkval;
}

bool  SqlDetail::Accept()
{
	ClearModify();
	if(fk.IsNull() || IsNull(fkval)) return true;
	if(!ctrls.IsModified()) return true;
	if(IsPresent()) {
		Sql sql(Session());
		sql.ClearError();
		sql * ctrls.UpdateModified(table).Where(fk == fkval);
		return !ShowError(sql);
	}
	return autocreate ? Create() : true;
}


}
