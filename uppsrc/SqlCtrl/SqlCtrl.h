#ifndef SQLCTRL_H
#define SQLCTRL_H

#include <Sql/Sql.h>
#include <CtrlLib/CtrlLib.h>


namespace Upp {

int  SqlError(const char *text, const char *error, const char *statement, bool retry = false);
int  SqlError(const char *text, const SqlSession& session, bool retry = false);
int  SqlError(const char *text, const Sql& sql APPSQLCURSOR, bool retry = false);

#ifdef PLATFORM_WIN32
int  SqlError(HWND parent, const char *text, const char *error, const char *statement, bool retry = false);
int  SqlError(HWND parent, const char *text, const SqlSession& session, bool retry = false);
int  SqlError(HWND parent, const char *text, const Sql& sql APPSQLCURSOR, bool retry = false);
#endif

bool   DisplayError(const SqlSession& session, const char *msg = NULL);
bool   ShowError(SqlSession& session, const char *msg = NULL);
bool   ErrorRollback(SqlSession& session, const char *emsg = NULL);
bool   OkCommit(SqlSession& session, const char *emsg = NULL);

bool   DisplayError(const Sql& sql, const char *msg = NULL);
bool   ShowError(Sql& sql, const char *msg = NULL);
bool   ErrorRollback(Sql& sql, const char *emsg = NULL);
bool   OkCommit(Sql& sql, const char *emsg = NULL);

#ifndef NOAPPSQL
bool   DisplayError(const char *msg = NULL);
bool   ShowError(const char *msg = NULL);
bool   ErrorRollback(const char *emsg = NULL);
bool   OkCommit(const char *emsg = NULL);
#endif

void SqlLoad(MapConvert& cv, Sql& sql);
void SqlLoad(MapConvert& cv, const SqlSelect& set, SqlSession& ss APPSQLSESSION);
#ifndef NOAPPSQL
void operator*=(MapConvert& cv, const SqlSelect& set);
#endif

void SqlLoad(DropList& dl, Sql& sql);
void SqlLoad(DropList& dl, const SqlSelect& set, SqlSession& ss APPSQLSESSION);
#ifndef NOAPPSQL
void operator*=(DropList& cv, const SqlSelect& set);
#endif

class SqlOption : public Option {
public:
	virtual void   SetData(const Value& data);
	virtual Value  GetData() const;
};

class SqlNOption : public SqlOption {
	virtual Value  GetData() const;
};

class SqlArray : public ArrayCtrl {
public:
	virtual void  SetData(const Value& v);
	virtual Value GetData() const;
	virtual bool  Accept();

protected:
	virtual bool  UpdateRow();
	virtual void  RejectRow();

private:
	SqlSession *ssn;
	SqlId       table;
	Value       fkv;
	SqlId       fk;
	SqlBool     where;
	SqlSet      orderby;
	int         querytime;
	int         count;
	int64       offset;
	bool        lateinsert;
	bool        goendpostquery;
	bool        autoinsertid;
	bool        updatekey;

	SqlBool     GetWhere();
#ifndef NOAPPSQL
	SqlSession& Session()                                  { return ssn ? *ssn : SQL.GetSession(); }
#else
	SqlSession& Session()                                  { ASSERT(ssn); return *ssn; }
#endif

	bool        PerformInsert();
	bool        PerformDelete();

	void      Inserting();

public:
	Callback                           WhenPreQuery;
	Callback                           WhenPostQuery;
	Gate<const VectorMap<Id, Value>&>  WhenFilter;

	void      StdBar(Bar& menu);
	bool      CanInsert() const;
	void      StartInsert();
	void      StartDuplicate();
	void      DoRemove();

	void      SetSession(SqlSession& _session)             { ssn = &_session; }

	void      Join(SqlId fk, ArrayCtrl& master);
	void      Join(ArrayCtrl& master);

	void      Query();
	void      AppendQuery(SqlBool where);
	void      Query(SqlBool where)                         { SetWhere(where); Query(); }
	void      ReQuery();
	void      ReQuery(SqlBool where)                       { SetWhere(where); ReQuery(); }

	void      Limit(int _offset, int _count)               { offset = _offset; count = _count; }
	void      Limit(int count)                             { Limit(0, count); }

	SqlArray& SetTable(SqlId _table)                       { table = _table; return *this; }
	SqlArray& SetTable(SqlId table, SqlId key)             { AddKey(key); return SetTable(table); }
	SqlArray& SetWhere(SqlBool _where)                     { where = _where; return *this;  }
	SqlArray& SetOrderBy(SqlSet _orderby)                  { orderby = _orderby; return *this; }
	SqlArray& SetOrderBy(const SqlVal& a)                  { return SetOrderBy(SqlSet(a)); }
	SqlArray& SetOrderBy(const SqlVal& a, const SqlVal& b) { return SetOrderBy(SqlSet(a, b)); }
	SqlArray& SetOrderBy(const SqlVal& a, const SqlVal& b, const SqlVal& c)
	                                                       { return SetOrderBy(SqlSet(a, b, c)); }
	SqlArray& GoEndPostQuery(bool b = true)                { goendpostquery = b; return *this; }
	SqlArray& AutoInsertId(bool b = true)                  { autoinsertid = b; return *this; }
	SqlArray& AppendingAuto()                              { Appending(); return AutoInsertId(); }
	SqlArray& UpdateKey(bool b =  true)                    { updatekey = b; return *this; }

	void      Clear();
	void      Reset();

	typedef   SqlArray CLASSNAME;

	SqlArray();
};

class SqlCtrls : public IdCtrls {
public:
	SqlCtrls& operator()(SqlId id, Ctrl& ctrl)       { Add(id, ctrl); return *this; }
	void      Table(Ctrl& dlg, SqlId table);
	SqlCtrls& operator()(Ctrl& dlg, SqlId table)     { Table(dlg, table); return *this; }
	SqlSet    Set() const;
	operator  SqlSet() const                         { return Set(); }
	void      Read(Sql& sql);
	bool      Fetch(Sql& sql);
	bool      Load(Sql& sql, SqlSelect select)       { sql * select; return Fetch(sql); }
	bool      Load(Sql& sql, SqlId table, SqlBool where);
#ifndef NOAPPSQL
	bool      Fetch()                                { return Fetch(SQL); }
	bool      Load(SqlSelect select)                 { return Load(SQL, select); }
	bool      Load(SqlId table, SqlBool where);
#endif
	void      Insert(SqlInsert& insert) const;
	void      Update(SqlUpdate& update) const;
	void      UpdateModified(SqlUpdate& update) const;
	SqlInsert Insert(SqlId table) const;
	SqlUpdate Update(SqlId table) const;
	SqlUpdate UpdateModified(SqlId table) const;

	SqlId       operator()(int i) const              { return item[i].id; }
	SqlId       GetKey(int i) const                  { return item[i].id; }

//deprecated:
	Callback  operator<<=(Callback cb);
};

class SqlDetail : public StaticRect {
public:
	virtual void  SetData(const Value& v);
	virtual Value GetData() const;
	virtual bool  Accept();

private:
	SqlSession *ssn;
	SqlCtrls    ctrls;
	SqlId       table;
	Value       fkval;
	SqlId       fk;
	bool        present;
	bool        autocreate;

#ifndef NOAPPSQL
	SqlSession& Session()                          { return ssn ? *ssn : SQL.GetSession(); }
#else
	SqlSession& Session()                          { ASSERT(ssn); return *ssn; }
#endif

	void        Query();

public:
	Callback    WhenPostQuery;

	bool        IsPresent() const                  { return present; }
	bool        Create();
	bool        Delete();

	void        SetSession(SqlSession& _session)   { ssn = &_session; }

	SqlDetail&  Add(SqlId id, Ctrl& ctrl);
	SqlDetail&  operator()(SqlId id, Ctrl& ctrl)   { return Add(id, ctrl); }

	SqlDetail&  Join(SqlId fk, ArrayCtrl& master);
	SqlDetail&  Join(ArrayCtrl& master);
	SqlDetail&  SetTable(SqlId _table)             { table = _table; return *this; }
	SqlDetail&  AutoCreate(bool b = true)          { autocreate = b; return *this; }

	void        Reset();

	SqlDetail();
};

void        SqlViewValue(const String& title, const String& value);

void        SQLCommander(SqlSession& session);
#ifndef NOAPPSQL
inline void SQLCommander() { SQLCommander(SQL.GetSession()); }
#endif
void        SQLObjectTree(SqlSession& session APPSQLSESSION);

INITIALIZE(SqlCtrl)

}

#endif
