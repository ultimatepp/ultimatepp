#include "GoogleMaps.h"

using namespace Upp;

#define LLOG(x) // LOG(x)

String apikey = "";

void SetGoogleMapsKey(const char *key)
{
	apikey = key;
}

String GetGoogleMap(double center_x, double center_y, int zoom, int cx, int cy,
                    const char *format, String *error)
{
	String request;
	request << "https://maps.google.com/maps/api/staticmap?center=" <<
		       AsString(center_y) << ',' << AsString(center_x) <<
		       "&zoom=" << zoom <<
		       "&size=" << cx << 'x' << cy <<
		       "&format=" << format <<
		       "&sensor=false&key=" << apikey;
	HttpRequest r(request);
	String h = r.Execute();
	if(r.IsFailure())
		*error = r.GetErrorDesc();
	return h;
}

Image GetGoogleMapImage(double center_x, double center_y, int zoom, int cx, int cy,
                        const char *format, String *error)
{
	return StreamRaster::LoadStringAny(GetGoogleMap(center_x, center_y, zoom, cx, cy, format, error));
}

double CvDeg(double deg, double minutes, double seconds)
{
	return deg + (double)minutes / 60 + seconds / 3600;
}

static void sFetch(double& a, double *x, int& ii)
{
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

String FormatDegree(double d, int decimals)
{
	if(IsNull(d))
		return Null;
	d = modulo(d + 180, 360) - 180;
	char sign = (d < 0 ? '-' : '+');
	if(d < 0) d = -d;
	int deg = ffloor(d);
	String cd = ToCharset(CHARSET_DEFAULT, "%c%d°", CHARSET_UTF8);
	if(decimals <= -2)
		return NFormat(cd, sign, deg);
	d = (d - deg) * 60;
	int min = ffloor(d);
	if(decimals <= -1)
		return NFormat(cd + " %02d\'", sign, deg, min);
	d = (d - min) * 60;
	String sec = FormatDoubleFix(d, decimals);
	if(!IsDigit(sec[1]))
		sec.Insert(0, '0');
	return NFormat(cd + " %02d\' %s\"", sign, deg, min, sec);
}

String FormatGPSX(double x)
{
	return IsNull(x) ? String() : FormatDegree(x, 2) + (x < 0 ? "W" : "E");
}

String FormatGPSY(double y)
{
	return IsNull(y) ? String() : FormatDegree(y, 2) + (y < 0 ? "S" : "N");
}

String FormatGPS(Pointf p)
{
	return FormatGPSY(p.y) + ' ' + FormatGPSX(p.x);
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
	return (Sizef)sz / 2.0 - GoogleMapsGpsToPixelDiff(center, zoom, gps);
}
