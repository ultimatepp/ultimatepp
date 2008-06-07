#include "SQLApp.h"

struct BookDlg : public WithBookLayout<TopWindow> {
	SqlCtrls ctrls;

	BookDlg() {
		CtrlLayoutOKCancel(*this, "Book");
		ctrls
			(AUTHOR, author)
			(TITLE, title)
		;
	}
};

void SQLApp::NewBook()
{
	BookDlg dlg;
	if(dlg.Run() != IDOK)
		return;
	SQL * dlg.ctrls.Insert(BOOK);
	Query(SQL.GetInsertedId());
}

void SQLApp::EditBook()
{
	if(!book.IsCursor())
		return;
	BookDlg dlg;
	int q = book.GetKey();
	SQL * Select(dlg.ctrls).From(BOOK).Where(ID == q);
	if(!dlg.ctrls.Fetch(SQL))
		return;
	if(dlg.Run() != IDOK)
		return;
	SQL * dlg.ctrls.Update(BOOK).Where(ID == q);
	Query(q);
}

void SQLApp::DeleteBook()
{
	if(book.IsCursor() && PromptYesNo("Delete the book?")) {
		SQL * Delete(BORROW_RECORD).Where(BOOK_ID == book.GetKey());
		SQL * Delete(BOOK).Where(ID == book.GetKey());
		Query();
	}
}

void SQLApp::BookMenu(Bar& bar)
{
	bar.Add("New book..", THISBACK(NewBook));
	bar.Add(book.IsCursor(), "Edit book..", THISBACK(EditBook));
	bar.Add(book.IsCursor(), "Delete book..", THISBACK(DeleteBook));
	bar.Separator();
	bar.Add("Query..", THISBACK(SetupQuery))
	   .Key(K_CTRL_F);
}
