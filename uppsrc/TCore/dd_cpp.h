#pragma BLITZ_APPROVE

#define __EMP __EMP2
#define __EMP2 __EMP3
#define __EMP3 __EMP4
#define __EMP4

#define TABLE(n)
#define TABLE_(n) const String& n()        { static String s = #n; return s; }
#define COLUMN(t, n, f)
#define COLUMN_(t, n, f) const String& n() { static String s = #n; return s; }
#define SCOLUMN(t, n)
#define SCOLUMN_(tn, n)  const String& n() { static String s = #n; return s; }
#define NINDEX()
#define TINDEX()
#define SINDEX()
#define LINK(rt)
#define END_TABLE()

#include TABLEFILE

#undef TABLE
#undef TABLE_
#undef COLUMN
#undef COLUMN_
#undef SCOLUMN
#undef SCOLUMN_
#undef NINDEX
#undef TINDEX
#undef SINDEX
#undef END_TABLE

#define TABLE(n) void Build##n(DataTable& table) {
#define TABLE_(n) TABLE(n)
#define COLUMN(t, n, f)  if(table.FindColumn(n()) < 0) table.AddColumn(n(), ValueTypeNo((const t*)0), 0 f);
#define COLUMN_(t, n, f) if(table.FindColumn(n()) < 0) table.AddColumn(n(), ValueTypeNo((const t*)0), 0 f);
#define SCOLUMN(t, n) COLUMN(t, n, __EMP)
#define SCOLUMN_(t, n) COLUMN(t, n, __EMP)
#define NINDEX | DataTableColumn::NUMBER_INDEX
#define TINDEX | DataTableColumn::DATETIME_INDEX
#define SINDEX | DataTableColumn::STRING_INDEX
#define END_TABLE() }

#include TABLEFILE

#undef TABLE
#undef COLUMN
#undef COLUMN_
#undef NINDEX
#undef TINDEX
#undef SINDEX
#undef END_TABLE

#ifdef BUILDPROC
void BUILDPROC(DataBase& db)
#else
void BuildAllTables(DataBase& db)
#endif
{

#define TABLE(n) Build##n(db.GetAdd(n()));
#define COLUMN(t, n, f)
#define COLUMN_(t, n, f)
#define NINDEX __EMP
#define TINDEX __EMP
#define SINDEX __EMP
#define END_TABLE()

#include TABLEFILE

#undef TABLE
#undef COLUMN
#undef COLUMN_
#undef LINK
#undef END_TABLE

#define TABLE(n) { DataTable& table = db[n()]; {
#define COLUMN(t, n, f) } { const String& c = n(); f
#define COLUMN_(t, n, f) } { const String& c = n(); f
#define LINK(rt) table.Column(c).Link(db[rt()]);
#define END_TABLE() } }

#include TABLEFILE

}

#undef TABLE
#undef TABLE_
#undef COLUMN
#undef COLUMN_
#undef SCOLUMN
#undef SCOLUMN_
#undef LINK
#undef NINDEX
#undef TINDEX
#undef SINDEX
#undef END_TABLE
#undef TABLEFILE
#undef __EMP
#undef __EMP2
#undef __EMP3
#undef __EMP4
#ifdef BUILDPROC
#undef BUILDPROC
#endif
