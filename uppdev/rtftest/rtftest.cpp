#include <Report/Report.h>

using namespace Upp;

static void CatChart(RichText& out, String title, Drawing dwg)
{
	RichTable table;
	table.AddColumn(1);
	table.SetQTF(0, 0, title);
	table[0][0].format.color = GrayColor(240);
	RichObject object = CreateDrawingObject(dwg, dwg.GetSize(), dwg.GetSize());
	RichPara para;
	para.format.align = ALIGN_CENTER;
	para.Cat(object, para.format);
	RichTxt txt;
	txt.Cat(para, out.GetStyles());
	table.SetPick(1, 0, txt);
	table.format.keep = true;
	out.CatPick(table);
	out.Cat(RichPara());
}

static void PaintLegend(Draw& draw, Rect& outer, const Vector<String>& linie, bool do_rtext)
{
	Vector<int> order = GetSortOrder(linie, GetLanguageInfo());

	static const int LINE_WD = 150;
	static const int TEXT_HT = 80;
	static const int LEG_OUTER_GAP = 100;
	static const int LEG_INNER_GAP = 30;

	RichPara legend_para;
	RichPara::CharFormat fmt;
	fmt.Face(Font::ARIAL).Height(TEXT_HT);
	for(int o = 0; o < order.GetCount(); o++) {
		String l = linie[order[o]];
		WString ltext = l.ToWString();

		DrawingDraw ddraw(LINE_WD, TEXT_HT);
		ddraw.DrawLine(0, TEXT_HT / 2, LINE_WD, TEXT_HT / 2, 5, LtGreen());

		if(o) {
			legend_para.Cat(WString(0xa0, 1), fmt);
			legend_para.Cat(WString(0xa0, 1), fmt);
			legend_para.Cat(WString(0xa0, 1), fmt);
			legend_para.Cat(WString(' ', 1), fmt);
		}

		legend_para.Cat(CreateDrawingObject(ddraw, LINE_WD, TEXT_HT), fmt);
		legend_para.Cat(WString(0xa0, 1), fmt);
		legend_para.Cat(WString(0xa0, 1), fmt);
		for(int i = 0; i < ltext.GetLength(); i++)
			if(ltext[i] == ' ')
				ltext.Set(i, 0xA0);
		legend_para.Cat(ltext, fmt);
	}
	RichText rtext;
	rtext.Cat(legend_para);
	rtext.Normalize();

	DDUMP(AsQTF(rtext));

	int wd = min(rtext.GetWidth(), outer.Width() - 2 * LEG_INNER_GAP);
	int ht = rtext.GetHeight(wd);

	Rect leg_rc;
	leg_rc.left = (outer.left + outer.right - wd - 2 * LEG_INNER_GAP) >> 1;
	leg_rc.right = leg_rc.left + wd + 2 * LEG_INNER_GAP;
	leg_rc.top = outer.top;
	leg_rc.bottom = leg_rc.top + ht + 2 * LEG_INNER_GAP;

	DrawFatFrame(draw, leg_rc, Black(), 10);
	if(do_rtext)
		rtext.Paint(draw, leg_rc.left + LEG_INNER_GAP, leg_rc.top + LEG_INNER_GAP, wd);

	outer.top = leg_rc.bottom + LEG_OUTER_GAP;
}

void VHBChartPrekroceniPrutoku(RichText& out, bool do_rtext)
{
	Size dwg_size = iscale(Size(140, 100), 6000, 254);

	DrawingDraw draw(dwg_size);

	Rect outer(dwg_size);
	outer.Deflate(50);
	
	Vector<String> linie;
	for(int i = 0; i < 10; i++)
		linie.Add(NFormat("testovaci linie [%d]", i));

	PaintLegend(draw, outer, linie, do_rtext);

	Rect inner = outer, fixed = outer;
	
	while(inner.Width() >= 100 && inner.Height() >= 100) {
		DrawFatFrame(draw, inner, LtRed(), 20);
		inner.Deflate(100);
	}

	CatChart(out, "[A1 [= VHBChartPrekroceniPrutoku", draw);
}

GUI_APP_MAIN {
	RichText richtext;

	VHBChartPrekroceniPrutoku(richtext, false);
	VHBChartPrekroceniPrutoku(richtext, true);

	WriteClipboard("Rich text format", EncodeRTF(richtext));

	Report report;
	report.Put(richtext);
	
	Perform(report);
}
