#ifndef _ataxx_attax_h_
#define _ataxx_attax_h_

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <ataxx/bordselectie.lay>
#include <CtrlCore/lay.h>


long willekeur (int max);
bool oneven (int getal);
void spelerwissel (int & speler);

struct zet {
	// datastructuur om een zet te beschrijven
	int speler;		// speler die de zet doet
	int xbron;		// x coordinaat van het virus waarmee gespeeld wordt
	int ybron;		// y coordinaat van het virus waarmee gespeeld wordt
	int xdoel;		// x coordinaat van het voelvakje
	int ydoel;		// y coordinaat van het voelvakje
	int aantal;		// hulpvariabele: getelde aantal mogelijke zetten
					// (om te kunnen communiceren tussen functies)
};

class ataxxbord {
	// klasse die het ataxx speelbord en acties daarop omschrijft
public:
	struct zet randomzet (int speler);		// bedenk een willekeurige zet
	struct zet strategiezet (int speler);	// bedenk een slimme zet
	struct zet allezetten (int speler, int maxzet);	// doorloopt alle zetten
	bool toegestanezet (zet test);	// controleer of een zet toegestaan is
	void doezet (zet doen);			// voert een zet uit
	void beginstand();				// initialiseer de beginstand
	void omwisselen();			// wisselt rood en blauw om (valsspelen)
	int telvirussen (int speler);	// telt aantal virussen van een speler
	bool bestaanmogelijkezetten (int speler);	// kan speler nog zetten?
	ataxxbord (int aantalkolommen, int aantalrijen);	// constructor
	~ataxxbord();		// destructor
	int ** veld;		// pointer naar twee-dimensionale dynamische array
						// die het speelveld bevat: 0 = leeg veld,
						// 1 = bezet door speler 1, 2 = bezet door speler 2
	int maxkolom;		// breedte
	int maxrij;			// hoogte
};

struct linkedlist {
	// beschrijft een linked list voor ataxx speelborden
	ataxxbord * stelling;				// het ataxx bord
	linkedlist * vorige;				// eerder bord
	linkedlist * volgende;			// volgend bord
	linkedlist();							// standaard constructor
	linkedlist (int kolommen, int rijen);	// constructor (overloaded)
	~linkedlist();							// destructor
};

class stapel {
	// de stapel bestaat uit een linked list van ataxx bordobjecten
	// en een aantal functies om borden toe te voegen of weg te halen
public:
	void zetopstapel (const ataxxbord * bord);	// voeg een bord toe
	void haallaatstevanstapel();				// verwijder het laatste bord
	bool isstapelleeg();						// test of de stapel leeg is
	linkedlist * lijst;							// de lijst met borden
	stapel();									// constructor
	~stapel();									// destructor
};

#endif
