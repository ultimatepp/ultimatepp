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
		}
	}
	
	void Drag()
	{
		if(list.DoDragAndDrop(InternalClip(list, "mydrag"), list.GetDragSample()) == DND_MOVE)
			list.RemoveSelection();
	}

	MyApp()
	{
		SetRect(0, 0, 228, 356);
		Add(list.SizePos());

		list.Columns(3);
		list.MultiSelect();
		list.WhenDropInsert = [=](int i, PasteClip& d) { DropInsert(i, d); };
		list.WhenDropItem = [=](int i, PasteClip& d) { DropSum(i, d); };
		
		list.WhenDrag = [=] { Drag(); };

		for(int i = 0; i < 500; i++)
			list.Add(AsString(i));
	}
};

GUI_APP_MAIN
{
	MyApp w;

	w.Run();
}
