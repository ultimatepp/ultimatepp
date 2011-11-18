#include <Map/Map.h>

class TestWindow : public TopWindow
{
	typedef TestWindow CLASSNAME;
	MapView View;

public:
	TestWindow()
	{
		Sizeable().MinimizeBox().MaximizeBox();
		Add(View.SizePos());

		View.LoadMap(AppendFileName(AppendFileName(GetFileDirectory(GetExeFilePath()), "Mipmaps"),
			"marcos.map"));
		View.HighQuality(false);
		View.ShowSearch();

		// here link to callback
		PolygonItem::WhenClick = THISBACK(OnRoomClick);
	}

	// your method or function to react on clicks
	void OnRoomClick(Ptr<PolygonItem> room)
	{
		Point p = GetRect().TopLeft() + ViewToScene(~room, room->GetCenter());

		TopWindow popup;
		popup.NoCenter();
		popup.SetRect(Rect(p, Size(150, 50)));

		Label text;
		popup.Add(text.SizePos());
		text = "Name: " + room->GetName();

		popup.Execute();
	}
};

GUI_APP_MAIN
{
	TestWindow().Run();
}
