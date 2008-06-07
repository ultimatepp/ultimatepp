#pragma BLITZ_APPROVE

#define TABLE(n)         void Build##n(DataTable& table);
#define TABLE_(n)        const String& n(); TABLE(n)
#define COLUMN(t, n, f)
#define COLUMN_(t, n, f) const String& n();
#define SCOLUMN(t, n)
#define SCOLUMN_(t, n)   const String& n();
#define NINDEX()
#define TINDEX()
#define SINDEX()
#define LINK(rt)
#define END_TABLE()

#ifdef BUILDPROC
void BUILDPROC(DataBase& db);
#else
void BuildAllTables(DataBase& db);
#endif

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
#undef LINK
#undef END_TABLE

#ifndef KEEP
#undef TABLEFILE
#ifdef BUILDPROC
#undef BUILDPROC
#endif
#endif
