#include "ataxx.h"
// stapel.cpp

linkedlist::linkedlist() {
	// standaard constructor
	vorige = NULL;
	volgende = NULL;
	stelling = NULL;
}

linkedlist::linkedlist (int kolommen, int rijen) {
	// constructor (overloaded)
	vorige = NULL;
	volgende = NULL;
	stelling = new ataxxbord (kolommen, rijen);
}

linkedlist::~linkedlist() {
	// destructor
	delete stelling;
}

stapel::stapel() {
	// constructor
	lijst = new linkedlist;
}

stapel::~stapel() {
	// destructor
	delete lijst;
}

void stapel::zetopstapel (const ataxxbord * bord) {
	// zet een bordstelling op de stapel

	int x,y;				// hulpvariabelen om door het veld te itereren
	linkedlist * hulp;		// hulpvariabele

	// voeg het bord toe aan de linked list
	hulp = lijst;
	lijst = new linkedlist (bord->maxkolom, bord->maxrij);
	hulp->volgende = lijst;
	lijst->vorige = hulp;
	// en vul ook de velden goed in
	// (dat gebeurt automatisch want het is een dynamische array)
	for (x=0;x<bord->maxkolom;x++) {
		for (y=0;y<bord->maxrij;y++) {
			lijst->stelling->veld[x][y] = bord->veld[x][y];
		}
	}
}

void stapel::haallaatstevanstapel() {
	// haal het laatste element (bord/stelling) van de stapel

	linkedlist * hulp;		// hulpvariabele

	// alleen als de pointers naar echte objecten verwijzen
	// ook daadwerkelijk tot het verwijderen overgaan
	if ( lijst != NULL ) {
		if ( lijst->vorige != NULL ) {
			hulp = lijst->vorige;
			delete lijst;
			lijst = hulp;
		}
	}
}

bool stapel::isstapelleeg() {
	// test of een stapel leeg is
	if ( lijst == NULL ) return true;
	else return false;
}
