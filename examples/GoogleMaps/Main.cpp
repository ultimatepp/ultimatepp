#include "GoogleMaps.h"

GUI_APP_MAIN {
	Pointf p(0, 51.477222);
	if(MapDlg(p))
		Exclamation("Marker position " + FormatGPS(p));
}
