#include <CtrlLib/CtrlLib.h>
#include <Web/Web.h>
#include <Geom/Coords/GeomCoords.h>

using namespace Upp;

#define LLOG(x) // LOG(x)

String apikey = "";

void SetGoogleMapsKey(const char *key)
{
	apikey = key;
}

String GetGoogleMap(double center_x, double center_y, int zoom, int cx, int cy,
                    const char *format = "png", String *error = NULL)
{
	String request;
	request << "http://maps.google.com/maps/api/staticmap?center=" <<
		       AsString(center_y) << ',' << AsString(center_x) <<
		       "&zoom=" << zoom <<
		       "&size=" << cx << 'x' << cy <<
		       "&format=" << format <<
		       "&sensor=false&key=" << apikey;
	return HttpClientGet(request, NULL, error);
}

Image GetGoogleMapImage(double center_x, double center_y, int zoom, int cx, int cy,
                    const char *format = "png", String *error = NULL)
{
	return StreamRaster::LoadStringAny(GetGoogleMap(center_x, center_y, zoom, cx, cy, format, error));
}

double CvDeg(double deg, double minutes, double seconds)
{
	return deg + (double)minutes / 60 + seconds / 3600;
}

// 50°7'57.96"N; 14°22'21.75"E

static void sFetch(double& a, double *x, int& ii)
{
	DDUMP(x[0]);
	DDUMP(x[1]);
	DDUMP(x[2]);
	a = CvDeg(x[0], x[1], x[2]);
	ii = 0;
}

int CharFilterH(int c)
{
	return c == 'e' || c == 'E' ? 'r' : c == '\'' || c == '\"' ? '@' : c;
}

Pointf ScanGPS(const char *s)
{
	String h = Filter(s, CharFilterH);
	Pointf r = Null;
	double x[3];
	CParser p(h);
	x[0] = x[1] = x[2] = 0;
	int ii = 0;
	while(!p.IsEof()) {
		if(p.IsDouble()) {
			if(ii < 3)
				x[ii++] = p.ReadDouble();
			else {
				if(IsNull(r.y))
					sFetch(r.y, x, ii);
				else {
					sFetch(r.x, x, ii);
					break;
				}
				x[ii++] = p.ReadDouble();
			}
		}
		else
		if(p.Char('W') || p.Char('w')) {
			if(x[0] > 0)
				x[0] = -x[0];
			sFetch(r.x, x, ii);
		}
		else
		if(p.Char('r'))
			sFetch(r.x, x, ii);
		else
		if(p.Char('S') || p.Char('s')) {
			if(x[0] > 0)
				x[0] = -x[0];
			sFetch(r.y, x, ii);
		}
		else
		if(p.Char('N') || p.Char('n'))
			sFetch(r.y, x, ii);
		else
			p.SkipTerm();
	}
	if(!IsNull(r.y) && IsNull(r.x))
		sFetch(r.x, x, ii);
	return r;
}

String FormatGPS(Pointf p)
{
	return FormatDegree(p.y, 2) + (p.y < 0 ? "S " : "N ") +
	       FormatDegree(p.x, 2) + (p.x < 0 ? "W " : "E ");
}

static const double sOffset = 268435456;
static const double sRadius = sOffset / M_PI;

static int LToX_(double x)
{
	return int(sOffset + sRadius * x * M_PI / 180);
}

static int LToY_(double y)
{
	return int(sOffset - sRadius * log((1 + sin(y * M_PI / 180))/(1 - sin( y * M_PI / 180))) / 2);
}

static double XToL_(int x)
{
	return ((x - sOffset) / sRadius) * 180 / M_PI;
}

static double YToL_(int y)
{
	return (M_PI / 2 - 2 * atan(exp((y - sOffset) / sRadius))) * 180 / M_PI;
}

Pointf GoogleMapsPixelToGps(Pointf center, int zoom, Point diff)
{
	return Pointf(XToL_(LToX_(center.x) + (diff.x << (21 - zoom))),
	              YToL_(LToY_(center.y) + (diff.y << (21 - zoom))));
}

Pointf GoogleMapsPixelToGps(Pointf center, int zoom, Size sz, Point p)
{
	return GoogleMapsPixelToGps(center, zoom, p - sz / 2);
}

Pointf GoogleMapsGpsToPixelDiff(Pointf center, int zoom, Pointf gps)
{
	return Pointf((LToX_(center.x) - LToX_(gps.x)) >> (21 - zoom),
	              (LToY_(center.y) - LToY_(gps.y)) >> (21 - zoom));
}

Pointf GoogleMapsGpsToPixel(Pointf center, int zoom, Size sz, Pointf gps)
{
	return sz / 2 - GoogleMapsGpsToPixelDiff(center, zoom, gps);
}

struct App : public TopWindow {
	Pointf home;
	Pointf center;
	String error;
	Image  map;
	int    zoom;
	
	virtual void LeftDown(Point p, dword)
	{
		center = GoogleMapsPixelToGps(center, zoom, Size(640, 640), p);
		map = GetGoogleMapImage(center.x, center.y, zoom, 640, 640, "png", &error);
		Refresh();
	}
	
	virtual void Paint(Draw& w) {
		Size sz = GetSize();
		String error;
		w.DrawRect(sz, SColorPaper());
		if(IsNull(map))
			w.DrawText(0, 0, error);
		else {
			w.DrawImage(0, 0, map);
			w.DrawImage(320, 320, CtrlImg::arrow());
			Point hp = GoogleMapsGpsToPixel(center, zoom, Size(640, 640), home);
			w.DrawImage(hp.x, hp.y, CtrlImg::arrow());
		}
	}

	App() {
		zoom = 15;
		home = center = Pointf(CvDeg(14, 22, 21.75), CvDeg(50, 7, 57.96));
		map = GetGoogleMapImage(center.x, center.y, zoom, 640, 640, "png", &error);
	}
};

GUI_APP_MAIN
{
	SetGoogleMapsKey("ABQIAAAAXHmSFgmVIbMZDJ5RhfPINBSBetkRueiarolywVmVT7jJWZRGvBQFIvrtty50ivBLt4YApjpEVXW8Hw");

	DDUMP(Pointf(CvDeg(14, 22, 21.75), CvDeg(50, 7, 57.96)));
	DDUMP(ScanGPS("50°7'57.96\"N; 14°22'21.75\"E"));
	DDUMP(ScanGPS("50°7'57.96 14°22'21.75"));
	DDUMP(ScanGPS("14°22'21.75\"E50°7'57.96\"N;"));
	DDUMP(ScanGPS("14°22'21.75\"E 50°7'57.96\"N;"));
	DDUMP(ScanGPS("14°22 E 50°7'57.96\"N;"));
	DDUMP(FormatGPS(ScanGPS("14°22 E 50°7'57.96\"N;")));
	
//	App().Run();
}

