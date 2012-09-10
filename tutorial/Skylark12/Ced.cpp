#include "Skylark12.h"

void CreateEditDelete::Create(Http& http)
{
	http("ACTION", THISLINK(SubmitCreate))
	.RenderResult(dialog);
}

void CreateEditDelete::SubmitCreate(Http& http)
{
	SQL * http.Insert(table);
	http.Redirect(back);
}

void CreateEditDelete::Edit(Http& http)
{
	int id = http.Int(0);
	http
		(Select(columns).From(table).Where(key == id))
		("ID", id)
		("ACTION", THISLINK(SubmitEdit), id)
	.RenderResult(dialog);
}

void CreateEditDelete::SubmitEdit(Http& http)
{
	SQL * http.Update(table).Where(key == http.Int(0));
	http.Redirect(back);
}

void CreateEditDelete::Delete(Http& http)
{
	SQL * SqlDelete(table).Where(key == atoi(http[0]));
	http.Redirect(back);
}

void CreateEditDelete::Use()
{
	SKYLARK_METHOD(Create, "create");
	SKYLARK_METHOD(SubmitCreate, "create_submit:POST");
	SKYLARK_METHOD(Edit, "edit/*");
	SKYLARK_METHOD(SubmitEdit, "submit_edit/*:POST");
	SKYLARK_METHOD(Delete, "delete/*");
}
