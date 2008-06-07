#include "SQLApp.h"

struct DateIntConvertCls : Convert {
	virtual Value Format(const Value& q) const;
	virtual Value Scan(const Value& text) const;
	virtual int Filter(int chr) const;
};

Value DateIntConvertCls::Format(const Value& q) const
{
	return IsNull(q) ? String() : ::Format(Date(1970, 1, 1) + (int)q);
}

Value DateIntConvertCls::Scan(const Value& text) const
{
	String txt = text;
	if(IsNull(txt))
		return Null;
	Date d;
	if(StrToDate(d, txt))
		return d - Date(1970, 1, 1);
	return ErrorValue("Invalid date!");
}

int DateIntConvertCls::Filter(int chr) const
{
	return CharFilterDate(chr);
}

Convert& DateIntConvert()
{
	return Single<DateIntConvertCls>();
}

class BorrowDlg : public WithBorrowLayout<TopWindow> {
public:
	SqlCtrls ctrls;

	typedef BorrowDlg CLASSNAME;

	BorrowDlg();
};

BorrowDlg::BorrowDlg()
{
	CtrlLayoutOKCancel(*this, "Book borrow record");
	ctrls
		(BORROWED, borrowed)
		(RETURNED, returned)
		(PERSON, person)
	;
	borrowed.SetConvert(DateIntConvert());
	borrowed <<= GetSysDate() - Date(1970, 1, 1);
	returned.SetConvert(DateIntConvert());
}

void SQLApp::NewBorrow()
{
	if(!book.IsCursor())
		return;
	SQL * Select(ID).From(BORROW_RECORD)
	      .Where(BOOK_ID == book.GetKey() && IsNull(RETURNED));
	if(SQL.Fetch() && !PromptYesNo("The book is still borrowed!&Continue?")) {
		borrowed.FindSetCursor(SQL[0]);
		return;
	}
	BorrowDlg dlg;
	if(dlg.Run() != IDOK)
		return;
	SQL * dlg.ctrls.Insert(BORROW_RECORD)(BOOK_ID, book.GetKey());
	borrowed.Query();
	borrowed.FindSetCursor(SQL.GetInsertedId());
}

void SQLApp::EditBorrow()
{
	if(!book.IsCursor())
		return;
	int q = borrowed.GetKey();
	BorrowDlg dlg;
	SQL * Select(dlg.ctrls).From(BORROW_RECORD).Where(ID == q);
	if(!dlg.ctrls.Fetch(SQL))
		return;
	if(dlg.Run() != IDOK)
		return;
	SQL * dlg.ctrls.Update(BORROW_RECORD).Where(ID == q);
	borrowed.Query();
	borrowed.FindSetCursor(q);
}

void SQLApp::DeleteBorrow()
{
	if(book.IsCursor() && PromptYesNo("Delete the borrow record?")) {
		SQL * Delete(BORROW_RECORD).Where(ID == book.GetKey());
		book.Query();
	}
}

void SQLApp::ReturnedToday()
{
	if(borrowed.IsCursor() && IsNull(borrowed.Get(RETURNED)) && PromptYesNo("Mark as returned today?")) {
		int q = borrowed.GetKey();
		SQL * SqlUpdate(BORROW_RECORD)
				(RETURNED, GetSysDate() - Date(1970, 1, 1))
		      .Where(ID == q && IsNull(RETURNED));
		borrowed.Query();
		borrowed.FindSetCursor(q);
	}
}

void SQLApp::BorrowMenu(Bar& bar)
{
	bar.Add(book.IsCursor(), "New borrow record..", THISBACK(NewBorrow))
	   .Key(K_ENTER);
	bar.Add(borrowed.IsCursor(), "Edit borrow record..", THISBACK(EditBorrow));
	bar.Add(borrowed.IsCursor(), "Delete borrow record", THISBACK(DeleteBorrow))
	   .Key(K_DELETE);
	bar.Add(borrowed.IsCursor() && IsNull(borrowed.Get(RETURNED)), "Returned today",
	        THISBACK(ReturnedToday))
	   .Key(K_SPACE);
}
