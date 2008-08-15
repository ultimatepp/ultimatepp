#include "ataxx.h"

long willekeur (int max) {
	// pseudoramdom getal genereren mbv cosinus en teller
	// (cmath bibliotheek nodig)
	static long int getal = max/2;	// te bepalen willekeuig getal
	static int teller = 0;		// elke nieuwe aanroep onthouden
	teller++;
	getal = (long int) ( ( cos( (double)getal ) * cos( (double)teller ) )
		* (max+1) );
	if (getal<0) getal *= -1;
	return getal;
}

bool oneven (int getal) {
	if ( getal % 2 > 0 ) return true;
	else return false;
}

void spelerwissel (int & speler) {
	// wisselen van speler 1 en 2
	if (speler==1) speler=2;
	else speler=1;
}
