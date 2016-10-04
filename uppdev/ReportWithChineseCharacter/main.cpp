#include <Report/Report.h>

using namespace Upp;

GUI_APP_MAIN
{
	Report r;
	r.Header("[A2< Header Page $$P");
	r.Footer("[A2> Footer Page $$P");
	r << "This is some [* QTF text";
	r << t_("顯示[* QTF 文字");
	r << "&&Let's report some table:";
	r << t_("輸出表格式報表：");
	String tab;
	tab << "{{1:1:1:1:1:1:2 A:: B:: C:: D:: E:: F:: G (wider)";
	for(int row = 0; row < 200; row++)
		for(int column = 0; column < 7; column++)
			tab << ":: " << row << ":" << column << t_("測試");
	r << tab;
	
	r.NewPage();
	int y = r.GetY();
	r.DrawEllipse(300, y + 50, 3000, 2000, LtRed, 20);
	r.DrawText(500, y + 50 + 950, "Some free drawing in this page!", Arial(100));
	r.DrawText(500, y + 50 + 1100, t_("在這頁隨便畫一些東西!"), Arial(100));
	r.SetY(y + 50 + 3000);
	r << "More of [/ QTF] [C6* text";
	
	Perform(r);
}
