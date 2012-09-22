#ifndef _GoogleMaps_GoogleMaps_h
#define _GoogleMaps_GoogleMaps_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <GoogleMaps/GoogleMaps.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS GoogleMapsImg
#define IMAGEFILE <GoogleMaps/GoogleMaps.iml>
#include <Draw/iml_header.h>

void   SetGoogleMapsKey(const char *key);
String GetGoogleMap(double center_x, double center_y, int zoom, int cx, int cy,
                    const char *format = "png", String *error = NULL);
Image  GetGoogleMapImage(double center_x, double center_y, int zoom, int cx, int cy,
                         const char *format = "png", String *error = NULL);
double CvDeg(double deg, double minutes, double seconds);

Pointf GoogleMapsPixelToGps(Pointf center, int zoom, Point diff);
Pointf GoogleMapsPixelToGps(Pointf center, int zoom, Size sz, Point p);
Pointf GoogleMapsGpsToPixelDiff(Pointf center, int zoom, Pointf gps);
Pointf GoogleMapsGpsToPixel(Pointf center, int zoom, Size sz, Pointf gps);

Pointf ScanGPS(const char *s);

String FormatGPSX(double x);
String FormatGPSY(double y);
String FormatGPS(Pointf p);

bool MapDlg(Pointf& p);

#endif
