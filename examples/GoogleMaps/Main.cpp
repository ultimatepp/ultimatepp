#include "GoogleMaps.h"

GUI_APP_MAIN {
	// Following key was obtained for U++ project, please get your owen key if you want to use
	// GoogleMaps API here (it is free): http://code.google.com/intl/cs/apis/maps/signup.html
	SetGoogleMapsKey("ABQIAAAAXHmSFgmVIbMZDJ5RhfPINBSBetkRueiarolywVmVT7jJWZRGvBQFIvrtty50ivBLt4YApjpEVXW8Hw");
	
	Pointf p(0, 51.477222);
	if(MapDlg(p))
		Exclamation("Marker position " + FormatGPS(p));
}
