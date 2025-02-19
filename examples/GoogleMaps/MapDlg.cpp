#include "GoogleMaps.h"

#define IMAGECLASS GoogleMapsImg
#define IMAGEFILE <GoogleMaps/GoogleMaps.iml>
#include <Draw/iml_source.h>

struct MapImage : public Ctrl {
	Image  map;
	String error;
	Point  home;
	
	Callback1<Point> WhenLeftClick;
	
	virtual void LeftDown(Point p, dword)
	{
		WhenLeftClick(p);
	}
	
	virtual void Paint(Draw& w) {
		Size sz = GetSize();
		w.DrawRect(sz, SColorPaper());
		if(IsNull(map))
			w.DrawText(0, 0, error);
		else {
			w.DrawImage(0, 0, map);
			Point p = GoogleMapsImg::Pin().GetHotSpot();
			w.DrawImage(home.x - p.x, home.y - p.y, GoogleMapsImg::Pin());
		}
	}
	
	MapImage() { SetFrame(ViewFrame()); BackPaint(); }
};

struct MapDlgDlg : public WithMapDlgLayout<TopWindow> {
	typedef MapDlgDlg CLASSNAME;

	Pointf   home;
	Pointf   center;
	MapImage map;
	
	void LoadMap();
	void SetHome();
	void MapClick(Point p);
	
	void ZoomIn();
	void ZoomOut();
	void Move(int x, int y);
	
	void   Set(Pointf p);
	Pointf Get() { return home; }
	
	MapDlgDlg();
};

void MapDlgDlg::SetHome()
{
	map.home = GoogleMapsGpsToPixel(center, ~zoom, Size(640, 640), home);
	gpsy.SetLabel("  " + FormatGPSY(home.y));
	gpsx.SetLabel("  " + FormatGPSX(home.x));
	map.Refresh();
}

void MapDlgDlg::LoadMap()
{
	map.map = GetGoogleMapImage(center.x, center.y, ~zoom, 640, 640, "png", &map.error);
	SetHome();
}

void MapDlgDlg::Move(int x, int y)
{
	center = GoogleMapsPixelToGps(center, ~zoom, Point(250 * x, 250 * y));
	LoadMap();
}

void MapDlgDlg::MapClick(Point p)
{
	home = GoogleMapsPixelToGps(center, ~zoom, Size(640, 640), p);
	SetHome();
}

void MapDlgDlg::Set(Pointf p)
{
	home = center = p;
	LoadMap();
}

void MapDlgDlg::ZoomIn()
{
	zoom <<= min((int)~zoom + 1, 21);
	LoadMap();
}

void MapDlgDlg::ZoomOut()
{
	zoom <<= max((int)~zoom - 1, 0);
	LoadMap();	
}

MapDlgDlg::MapDlgDlg()
{
	CtrlLayoutOKCancel(*this, "");
	
	Size sz = GetSize();
	Size msz = map.GetSize();
	sz += Size(640 - msz.cx, 640 - msz.cy);
	SetRect(sz);

	for(int i = 0; i < 22; i++)
		zoom.Add(i);
	zoom <<= 17;
	zoom <<= THISBACK(LoadMap);
	zoomin <<= THISBACK(ZoomIn);
	zoomout <<= THISBACK(ZoomOut);
	
	left <<= THISBACK2(Move, -1, 0);
	left.SetImage(CtrlImg::SmallLeft());
	right <<= THISBACK2(Move, 1, 0);
	right.SetImage(CtrlImg::SmallRight());
	up <<= THISBACK2(Move, 0, -1);
	up.SetImage(CtrlImg::SmallUp());
	down <<= THISBACK2(Move, 0, 1);
	down.SetImage(CtrlImg::SmallDown());
	
	map.WhenLeftClick = THISBACK(MapClick);
}

bool MapDlg(Pointf& p)
{
	MapDlgDlg dlg;
	dlg.Set(p);
	if(dlg.Run() == IDOK) {
		p = dlg.Get();
		return true;
	}
	return false;
}
