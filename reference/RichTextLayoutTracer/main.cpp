#include <Report/Report.h>

using namespace Upp;

struct TableRowTracer : RichTextLayoutTracer {
	VectorMap<int, int> pgr;

	virtual void TableRow(const Rect& page, PageY y, int, const RichTable&) {
		pgr.GetAdd(y.page, 0)++;
	}	
};

GUI_APP_MAIN
{
	Report r;
	r.Footer("[R1 ");
	TableRowTracer t;
	r.SetRichTextLayoutTracer(t);
	String qtf = "&[R This is a demonstration of [* RichTextLayoutTracer]&&&{{1 Table";
	for(int i = 0; i < 100; i++)
		qtf << ":: " << i;
	qtf << "}}";
	r.Put(qtf);
	
	Report rr;
	for(int i = 0; i < r.GetCount(); i++) {
		if(i)
			rr.NewPage();
		rr.Footer("[R1 There is " + AsString(t.pgr.Get(i, 0)) + " table rows on this page");
		rr.DrawDrawing(r.GetPageSize(), r[i]);
	}
	
	Perform(rr);
}
