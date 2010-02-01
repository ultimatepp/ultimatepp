#include <CtrlLib/CtrlLib.h>
#include <Web/Web.h>

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
		       AsString(center_x) << ',' << AsString(center_y) <<
		       "&zoom=" << zoom <<
		       "&size=" << cx << 'x' << cy <<
		       "&format=" << format <<
		       "&sensor=false&key=" << apikey;
	LLOG(request);
	return HttpClientGet(request, NULL, error);
}

Image GetGoogleMapImage(double center_x, double center_y, int zoom, int cx, int cy,
                    const char *format = "png", String *error = NULL)
{
	return StreamRaster::LoadStringAny(GetGoogleMap(center_x, center_y, zoom, cx, cy, format, error));
}

double CvDeg(int deg, int minutes, double seconds)
{
	return deg + (double)minutes / 60 + seconds / 3600;
}

Pointf CvGps(const *gps)

struct App : public TopWindow {
	virtual void Paint(Draw& w) {
		Size sz = GetSize();
		String error;
		w.DrawRect(sz, SColorPaper());
//		Image m = GetGoogleMapImage(40.714728, -73.998672, 15, 640, 640, "png", &error);
		Image m = GetGoogleMapImage(CvDeg(50, 7, 57.96), CvDeg(14, 22, 21.75), 15, 640, 640, "png", &error);
		if(IsNull(m))
			w.DrawText(0, 0, error);
		else
			w.DrawImage(0, 0, m);
	}
};

// 50°7'57.96"N; 14°22'21.75"E

GUI_APP_MAIN
{
	SetGoogleMapsKey("ABQIAAAAXHmSFgmVIbMZDJ5RhfPINBROL4Fm4DQZqQc98wIV24vMfAeDoRShQOCxXcf5YZU_t63bo6-F5Joq4A");
	
	App().Run();
}

