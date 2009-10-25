#include <Report/Report.h>

using namespace Upp;

struct TableRowTracer : RichTextLayoutTracer {
	VectorMap<int, int> pgr;

	virtual void TableCell(const Rect& page, PageY py, int i, int j, const RichTable& table, PageY npy)
	{
		if(j == 0)
			for(int i = py.page; i <= npy.page; i++)
				pgr.GetAdd(i, 0)++;
	}
};

GUI_APP_MAIN
{
	Report r;
	r.Footer("[R1 ");
	TableRowTracer t;
	r.SetRichTextLayoutTracer(t);
	String qtf = LoadFile("D:\\log.qtf");
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
