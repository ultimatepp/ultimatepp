#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class MyApp : public TopWindow {
public:
	typedef MyApp CLASSNAME;

	ColumnList list;

	void DropInsert(int i, PasteClip& d)
	{
		if(AcceptText(d))
			list.Insert(i, GetString(d));
		if(AcceptInternal<ColumnList>(d, "mydrag"))
			list.InsertDrop(i, d);
	}

	void DropSum(int ii, PasteClip& d) {
		if(AcceptInternal<ColumnList>(d, "mydrag")) {
			const ColumnList& src = GetInternal<ColumnList>(d);
			int sum = 0;
			for(int i = 0; i < src.GetCount(); i++)
				if(src.IsSel(i))
					sum += atoi(~String(src[i]));
			list.Set(ii, AsString(sum));
			d.SetAction(DND_COPY);
			list.SetCursor(ii);
			PromptOK("Finished!");
		}
	}

	void Drag()
	{
		if(list.DoDragAndDrop(InternalClip(list, "mydrag"), CtrlImg::ibeam0(), DND_ALL|DND_EXACTIMAGE) == DND_MOVE)
			list.RemoveSelection();
	}

	MyApp()
	{
		SetRect(0, 0, 228, 356);
		Add(list.SizePos());

		list.Columns(3);
		list.MultiSelect();
		list.WhenDropInsert = THISBACK(DropInsert);
		list.WhenDropItem = THISBACK(DropSum);

		list.WhenDrag = THISBACK(Drag);

		for(int i = 0; i < 500; i++)
			list.Add(AsString(i));
		RLOG("HERE!");
		Sizeable();
	}
};

GUI_APP_MAIN
{
	MyApp w;

	w.Run();
}
