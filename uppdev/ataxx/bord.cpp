#include "ataxx.h"

// bord.cpp

ataxxbord::ataxxbord (int aantalkolommen, int aantalrijen) {
	// constructor
	int i;							// hulpvariabele voor iteratie
	maxkolom = aantalkolommen;		// onthoud maxkolom als afmeting
	maxrij = aantalrijen;			// onthoud maxrij als afmeting
	// dynamische array maken
	veld = new int * [maxkolom];
	for (i=0;i<maxkolom;i++) {
		veld[i] = new int [maxrij];
	}
	// en het bord in de beginstand zetten
	beginstand();
}

ataxxbord::~ataxxbord() {
	// destructor
	int x;							// hulpvariabele voor iteratie
	for (x=0;x<maxkolom;x++) {
		delete [] veld[x];
	}
	delete [] veld;
	veld = NULL;
}

void ataxxbord::beginstand() {
	// het speelbord in de beginstand zetten
	// en startposities instellen
	int x,y;						// hulpvariabelen voor iteraties door veld
	for (x=0;x<maxkolom;x++) {
		for (y=0;y<maxrij;y++) {
			veld[x][y] = 0;
		}
	}
	veld[0][maxrij-1] = 1;		// beginstand speler 1
	veld[maxkolom-1][0] = 2;	// beginstand speler 2
}

bool ataxxbord::toegestanezet (zet test) {
	// als een zet toegestaan is, dan true retourneren

	int dx, dy;						// afstanden over x en y die een zet doet

	// test 1: blijft de zet wel binnen het bord?
	if ( test.xdoel > maxkolom-1 || test.ydoel > maxrij-1 ) return false;
	if ( test.xdoel < 0 || test.ydoel < 0 ) return false;

	// test 2: staat er op het bronveld wel een eigen virus?
	if ( veld[test.xbron][test.ybron] != test.speler ) return false;

	// test 3: is het doelvak wel vrij?
	if ( veld[test.xdoel][test.ydoel] != 0 ) return false;

	// test 4: afstand bron-doel moet 1 of 2 vakjes zijn, maar niet groter
	dx = test.xdoel - test.xbron;
	dy = test.ydoel - test.ybron;
	// absolute waarden van maken
	if ( dx < 0 ) dx *= -1;
	if ( dy < 0 ) dy *= -1;
	if ( dx > 2 || dy > 2 ) return false;

	// als alle tests doorstaan zijn, is de zet toegestaan
	return true;
}

void ataxxbord::doezet (zet doen) {
	// voer een zet uit

	int dx,dy,afstand;				// afstanden over x en y die een zet doet

	// nieuwe veld vullen met virus (zowel bij clonatie als verplaatsing)
	veld[doen.xdoel][doen.ydoel] = doen.speler;

	// aangrenzende (vijandige) virussen overnemen
	for ( dx=doen.xdoel-1; dx<=doen.xdoel+1; dx++ ) {
		for ( dy=doen.ydoel-1; dy<=doen.ydoel+1; dy++ ) {
			if ( dx>=0 && dy>=0 && dx<maxkolom && dy<maxrij) {
				if ( veld[dx][dy] > 0 ) veld[dx][dy] = doen.speler;
			}
		}
	}

	// voor het effect is het van belang of het gaat om een verplaatsing
	// of een clonering
	// verplaatsingen gaan over afstand 2, clonaties over afstand 1
	// bepaald delta(x) en delta(y)
	dx = doen.xdoel - doen.xbron;
	dy = doen.ydoel - doen.ybron;
	if ( dx < 0 ) dx *= -1;
	if ( dy < 0 ) dy *= -1;
	// bepaal nu de afstand
	if ( dx >= dy ) afstand = dx;
	else afstand = dy;
	// bij verplaatsing oude veld legen
	if ( afstand==2 ) veld[doen.xbron][doen.ybron] = 0;
}

int ataxxbord::telvirussen (int speler) {
	// telt het aantal virussen voor een speler

	int x,y;						// hulpvariabelen voor iteraties door veld
	int aantal=0;					// aantal gevonden virussen

	for (x=0;x<maxkolom;x++) {
		for (y=0;y<maxrij;y++) {
			if ( veld[x][y] == speler ) aantal++;
		}
	}
	return aantal;
}

bool ataxxbord::bestaanmogelijkezetten (int speler) {
	// kijkt of er uberhaupt wel zetten mogelijk zijn voor een speler

	int x,y;						// hulpvariabelen voor iteraties door veld
	int dx,dy;						// afstanden over x en y die een zet doet
	int aantal=0;					// aantal gevonden mogelijke zetten
	zet test;						// testzet (struct)

	// kijk voor alle virussen van de speler of er mee gezet kan worden
	for (x=0;x<maxkolom;x++) {
		for (y=0;y<maxrij;y++) {
			if ( veld[x][y] == speler ) {
				// kijk of er zetten mogelijk zijn met stap = 1 of stap = 2
				// met behulp van de functie toegestanezet
				test.speler = speler;
				test.xbron = x;
				test.ybron = y;
				for (dx=-2;dx<=2;dx++) {
					for (dy=-2;dy<=2;dy++) {
						test.xdoel = test.xbron + dx;
						test.ydoel = test.ybron + dy;
						if ( toegestanezet(test) ) aantal++;
					}
				}
			}
		}
	}

	if ( aantal>0) return true;
	else return false;
}

zet ataxxbord::allezetten (int speler, int maxzet) {
	// doorloopt alle mogelijke zetten, en geeft de laatste zet terug of
	// de de "maxzet"-ste zet (als maxzet==0 dan de laatste zet)

	int x,y;						// hulpvariabelen voor iteraties door veld
	int dx,dy;						// afstanden over x en y die een zet doet
	int aantal=0;					// aantal gevonden mogelijke zetten
	zet test;						// testzet (struct)
	zet mogenlijkezet;				// onthoud de beste mogelijke zet (struct)

	// initialiseren mogenlijkezet; mocht er geen mogenlijke zet
	// gevonden worden, dan wordt toch deze "lege" zet teruggegeven
	// om een compiler warning te voorkomen
	mogenlijkezet.speler = speler;
	mogenlijkezet.xbron = 0;
	mogenlijkezet.ybron = 0;
	mogenlijkezet.xdoel = 0;
	mogenlijkezet.ydoel = 0;
	mogenlijkezet.aantal = 0;

	// kijk nu voor alle virussen van de speler wat er mogelijk is
	for (x=0;x<maxkolom;x++) {
		for (y=0;y<maxrij;y++) {
			if ( veld[x][y] == speler ) {
				// kijk of er zetten mogelijk zijn met stap = 1 of stap = 2
				// met gebruik van de functie toegestanezet
				test.speler = speler;
				test.xbron = x;
				test.ybron = y;
				for (dx=-2;dx<=2;dx++) {
					for (dy=-2;dy<=2;dy++) {
						test.xdoel = test.xbron + dx;
						test.ydoel = test.ybron + dy;
						if ( toegestanezet(test) ) {
							aantal++;
							mogenlijkezet = test;
							mogenlijkezet.aantal = aantal;
				 			if ( aantal==maxzet ) return mogenlijkezet;
			 			}
					}
				}
			}
		}
	}
	// als we hier arriveren is er kennelijk geen zet mogelijk, of zijn er
	// minder dan maxzet zetten mogelijk. in dat laatste geval wordt een
	// eerdere mogelijke zet teruggegeven.
	return mogenlijkezet;
}

zet ataxxbord::randomzet (int speler) {
	// alleen aanroepen na een test op
	// bool ataxxbord::bestaanmogelijkezetten (int speler)
	// als schatting gaan we ervanuit dat er meestal
	// wel minstens 6 zetten mogelijk zijn
	int treffer = (int) willekeur(6);
	return allezetten(speler,treffer);
}

zet ataxxbord::strategiezet (int speler) {
	// voer de zet uit die op dit moment de meeste virussen oplevert

	zet kandidaatzet;				// geheugen voor een kandidaat zet
	int vijand;						// nummer van vijandige speler
	int i=0;						// hulpvariabele voor iteraties
	int x,y;						// hulpvariabelen voor iteraties door veld
	int score;						// gevonden score voor een kandidaatzet
	int maxscore=0, maxzet=0;		// maximale score bij de beste zet(nr)
	int defensie=2;				// mate van defensief spelen
	int agressie=1;				// mate van agressief spelen
	int ** tijdelijkveld;			// kopie van het echte veld (dynamisch)

	// vijand bepalen
	if (speler==1) vijand = 2;
	else vijand = 1;

	// dynamische array maken
	tijdelijkveld = new int * [maxkolom];
	for (x=0;x<maxkolom;x++) {
		tijdelijkveld[x] = new int [maxrij];
	}

	do {
		i++;
		kandidaatzet = allezetten(speler,i);
		// maak kopie echte bord naar tijdelijkveld
		for (x=0;x<maxkolom;x++) {
			for (y=0;y<maxrij;y++) {
				tijdelijkveld[x][y] = veld[x][y];
			}
		}
		// test de zet op het echte bord ...
		doezet (kandidaatzet);
		// ... en bereken een score (agressie is afhankelijk van deze scoreberekening)
		score = defensie * telvirussen(speler) - agressie * telvirussen(vijand);
		if ( score > maxscore ) {
			maxscore = score;
			maxzet = i;
		}
		// herstel het juiste bord
		for (x=0;x<maxkolom;x++) {
			for (y=0;y<maxrij;y++) {
				veld[x][y] = tijdelijkveld[x][y];
			}
		}
	}
	while ( kandidaatzet.aantal == i );

	// verwijder dynamisch array
	for (x=0;x<maxkolom;x++) {
		delete [] tijdelijkveld[x];
	}
	delete [] tijdelijkveld;
	tijdelijkveld = NULL;

	// geef de profitabele zet terug
	kandidaatzet = allezetten (speler, maxzet);
	return kandidaatzet;
}

void ataxxbord::omwisselen() {
	int x,y;						// hulpvariabelen voor iteraties door veld
	int hulp;
	for (x=0;x<maxkolom;x++) {
		for (y=0;y<maxrij;y++) {
			hulp = veld[x][y];
			if ( hulp == 1 ) veld[x][y] = 2;
			if ( hulp == 2 ) veld[x][y] = 1;
		}
	}
}
