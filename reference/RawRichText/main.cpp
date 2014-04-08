#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	RichText txt;
	{
		RichPara para;
		RichPara::CharFormat fmt;
		(Font&)fmt = Serif(100);
		para.Cat("Hello world!", fmt);
		txt.Cat(para);
	}
	{
		RichPara para;
		{
			RichPara::CharFormat fmt;
			(Font&)fmt = Monospace(150).Bold();
			fmt.ink = Red;
			fmt.capitals = true;
			para.Cat("Monospace-bold-red-capitals, ", fmt);
		}
		{
			RichPara::CharFormat fmt;
			(Font&)fmt = SansSerif(100);
			fmt.link = "http://www.ultimatepp.org";
			para.Cat("This is some link", fmt);
		}
		txt.Cat(para);
	}
	{
		RichPara para;
		RichPara::CharFormat fmt;
		para.format.align = ALIGN_CENTER;
		para.Cat("Centered", fmt);
		txt.Cat(para);
	}
	{
		RichPara para;
		RichPara::CharFormat fmt;
		para.format.before = 100;
		para.format.lm = 300;
		para.Cat("Before and left margin", fmt);
		txt.Cat(para);
	}
	{
		RichPara para;
		RichPara::CharFormat fmt;
		RichObject obj = CreatePNGObject(CtrlImg::exclamation(), 200, 200);	
		para.Cat("Object: ", fmt);
		para.Cat(obj, fmt);
		txt.Cat(para);
	}
	{
		RichTable table;
		table.AddColumn(1);
		table.AddColumn(1);
		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 2; j++) {
				RichText celltext;
				RichPara para;
				RichPara::CharFormat fmt;
				para.Cat(AsString(i) + ':' + AsString(j), fmt);
				celltext.Cat(para);
				table.SetPick(i, j, pick(celltext));
			}
		txt.CatPick(pick(table));
	}
	
	RichTextView view;
	view.Pick(pick(txt));
	TopWindow win;
	win.Add(view.SizePos());
	win.Run();
}
