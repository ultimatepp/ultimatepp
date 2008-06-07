#include "SQLApp.h"

void SQLApp::Query()
{
	SqlBool where;
	SqlSet borrowed = Select(BOOK_ID).From(BORROW_RECORD).Where(IsNull(RETURNED));
	if(query.status == 1)
		where = ID != borrowed;
	if(query.status == 2)
		where = ID == borrowed;
	SqlBool bdate;
	if(!IsNull(query.borrowed_from))
		bdate = BORROWED >= ~query.borrowed_from;
	if(!IsNull(query.borrowed_to))
		bdate = bdate && BORROWED <= ~query.borrowed_to;
	if(!bdate.IsEmpty())
		where = where && ID == Select(BOOK_ID).From(BORROW_RECORD).Where(bdate);
	book.Query(where);
}

void SQLApp::ClearQuery()
{
	query.status = 0;
	query.borrowed_from <<= query.borrowed_to <<= Null;
}

void SQLApp::SetupQuery()
{
	if(query.Execute() == IDOK)
		Query();
}

void SQLApp::Query(int q)
{
	book.Query();
	if(book.FindSetCursor(q))
		return;
	ClearQuery();
	book.Query();
	book.FindSetCursor(q);
}

void SQLApp::Serialize(Stream& s)
{
	int version = 0;
	s / version;
	s % query.status
	  % query.borrowed_from
	  % query.borrowed_to;
}
