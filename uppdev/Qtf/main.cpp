#include <Report/Report.h>

using namespace Upp;

GUI_APP_MAIN
{
	String r_title = "[t This] is [MY TITLE!]";

	String qtf;
	qtf << " && [*= " << DeQtf(r_title)
	    << "&"
	    << "[l500 ahoj&ahoj&]&"
	    << "[P &]&"
	    << "[H10hBl200r200 &]"
	    << "[_= Analysis [t Report]]&"
	    << "[_= Analysis [t Report]]&"
	    << "[_= Analysis [t Report]]&"
	;
	
	Report r;
	r.Header("[A2> Page $$P");
	r << qtf;
	Perform(r);
}

void Foo()
{
	DrawingDraw dw(100, 100);
	dw.DrawRect(0, 0, 100, 100, White);
	dw.DrawEllipse(10, 10, 80, 80, Red, 5, Blue);
	dw.DrawRect(50, 50, 50, 50, Yellow);
	QtfRichObject pict(CreateDrawingObject(dw.GetResult(), Size(500, 500), Size(500, 500)));
	String qtf;
	qtf << "[A6 This is some [t drawing] in QTF: " << pict << "&";
	qtf << "[C3*@B And now in the table: {{1:1 " << pict << "::H100 Another cell " << pict << "}}";
	qtf << "[A6ph Some text ajdsfh akjds fakjs fjkash dfalsd fhal fakhjds fkahjds fka fkajhs dfka&]";
	qtf << "[A6pd Some text ajdsfh akjds fakjs fjkash dfalsd fhal fakhjds fkahjds fka fkajhs dfka&]";
	qtf << "[A6po Some text ajdsfh akjds fakjs fjkash dfalsd fhal fakhjds fkahjds fka fkajhs dfka&]";
	PromptOK(qtf);
}
