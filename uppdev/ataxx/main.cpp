/*
	Evert Mouw
	emouw@liacs.nl
	S 0332291
	Programmeermethoden
	4e programmeeropdracht, ATAXX
	2006-12-06
*/

#include "ataxx.h"

//#include <cmath>		// om pseudorandom getallen te krijgen dmv cosinus

// globals om te kunnen gebruiken in alle (modal) forms
int kolommen, rijen;		// invoer grootte speelbord

struct MyAppWindow : TopWindow {
	// spelgegevens
	int speler;					// aan de beurt; speler 1 begint altijd
	bool slim;					// random zetten of slimme zetten?
	ataxxbord * bord;			// class ataxxbord
	stapel geheugen;			// om stellingen te onthouden
	zet invoer;					// ingevoerde zet (struct)
	bool invoerstatus;			// 1e klik (false) of 2e klik (true)
	bool invoermogelijk;		// na einde spel op false zetten
	// GUI
	String bordtekst;
	MenuBar menu;
	StatusBar status;
	int vakjegrootte;
	int virusgrootte;
	int lijndikte;
	bool bordwissel;
	int oplichtx;
	int oplichty;
	typedef MyAppWindow CLASSNAME;
	void BordInit();			// startvariabelen vullen
	void Nieuw();				// code om nieuw spel te starten
	void ZetTerug();
	void Omwisselen();
	void ComputerZet();
	void ToonLegaleZetten();
	void VerbergLegaleZetten();
	void OplichtenUit();
	void Evalueer();
	void Stoppen();
	void Bordgrootte();
	void SpelerKeuze1();
	void SpelerKeuze2();
	void SpeelstijlRandom();
	void SpeelstijlSlim();
	void Spelregels();
	void Informatie();
	void MainMenu(Bar& bar);
	void SubMenu1(Bar& bar);
	void SubMenu2(Bar& bar);
	void SubMenu2b(Bar& bar);
	void SubMenu2c(Bar& bar);
	void SubMenu3(Bar& bar);
	virtual void Paint(Draw& w);
	virtual void RightDown(Point, dword flags);
	virtual void LeftDown(Point p, dword flags);
	MyAppWindow();					// constructor
	~MyAppWindow();					// destructor
};

class bordselectie : public WithDlgLayout<TopWindow> {
public:
	typedef bordselectie CLASSNAME;
	bordselectie(int xdefault, int ydefault);
	void vullen(int xdefault, int ydefault);
	void annuleren();
	void OK();
};

void MyAppWindow::BordInit() {
	vakjegrootte = 50;
	lijndikte = 2;
	virusgrootte = 40;
	kolommen = 7;
	rijen = 7;
	speler = 1;
	bordwissel = true;
	invoerstatus = false;
	invoermogelijk = true;
	slim = false;
	bordtekst = "";
	oplichtx = -1;
	oplichty = -1;
	// maak een nieuw ataxxbord object
	bord = new ataxxbord (kolommen,rijen);
}

void MyAppWindow::Nieuw() {
	// code om nieuw spel te starten
	bord->beginstand();
	while ( geheugen.lijst->stelling != NULL ) {
		geheugen.haallaatstevanstapel();
	}
	invoermogelijk = true;
	bordtekst = "";
	VerbergLegaleZetten();
	oplichtx = -1;
	oplichty = -1;
	Refresh();
}

void MyAppWindow::ZetTerug() {
	// stelling terughalen vanuit het geheugen
	int x,y;
	if ( geheugen.lijst->stelling != NULL ) {
		for (x=0;x<kolommen;x++) {
			for (y=0;y<rijen;y++) {
				bord->veld[x][y] =
					geheugen.lijst->stelling->veld[x][y];
			}
		}
		geheugen.haallaatstevanstapel();
	}
	else PromptOK("Kan niet verder terug.");
	invoerstatus = false;
	invoermogelijk = true;
	bordtekst = "";
	VerbergLegaleZetten();
	oplichtx = -1;
	oplichty = -1;
	Refresh();
}

void MyAppWindow::Stoppen() {
	Break();
}

void MyAppWindow::Bordgrootte() {
	int hulpx = kolommen;
	int hulpy = rijen;
	bordselectie invoerraam(kolommen,rijen);
	invoerraam.Run(true);
	if (hulpx != kolommen || hulpy != rijen ) {
		while ( geheugen.lijst->stelling != NULL ) {
			geheugen.haallaatstevanstapel();
		}
		delete bord;
		bord = new ataxxbord (kolommen,rijen);
		bordwissel = true;
		Nieuw();
	}
}

void MyAppWindow::SpelerKeuze1() {
	speler = 1;
	Nieuw();
}

void MyAppWindow::SpelerKeuze2() {
	speler = 2;
	Nieuw();
}

void MyAppWindow::SpeelstijlRandom() {
	slim = false;
	Refresh();
}

void MyAppWindow::SpeelstijlSlim() {
	slim = true;
	Refresh();
}

void MyAppWindow::Spelregels() {
	String regels;
	regels = "";
	regels += "[*A3 spelregels]";
	regels += "&[A2 Een virus kan 1 stap doen (cloneren) of]";
	regels += "&[A2 twee stappen doen (verlaatsen) in iedere]";
	regels += "&[A2 willekeurige richting, behalve naar vakjes]";
	regels += "&[A2 die al bezet zijn. Bij elke clonatie of]";
	regels += "&[A2 verplaatsing komen de aangrenzende]";
	regels += "&[A2 virussen van de vijand in eigen bezit.]";
	PromptOK(regels);
}

void MyAppWindow::Informatie() {
	String regels;
	regels = "";
	regels += "[A4 ATAXX]";
	regels += "&[A3* Evert Mouw]";
	regels += "&[A3 S0332291]";
	regels += "&[A2/ emouw@liacs.nl]";
	regels += "&[A2 2006-12-06]";
	PromptOK(regels);
}

void MyAppWindow::MainMenu(Bar& bar) {
	bar.Add("Spel", THISBACK(SubMenu1));
	bar.Add("Instellingen", THISBACK(SubMenu2));
	bar.Add("Hulp", THISBACK(SubMenu3));
}

void MyAppWindow::SubMenu1(Bar& bar) {
	bar.Add("Nieuw", THISBACK(Nieuw));
	bar.Add("ZetTerug", THISBACK(ZetTerug));
	bar.Add("Omwisselen", THISBACK(Omwisselen));
	bar.Add("Stoppen", THISBACK(Stoppen));
}

void MyAppWindow::SubMenu2(Bar& bar) {
	bar.Add("Bordgrootte", THISBACK(Bordgrootte));
	bar.Add("Spelerkeuze", THISBACK(SubMenu2b));
	bar.Add("Speelstijl", THISBACK(SubMenu2c));
}

void MyAppWindow::SubMenu3(Bar& bar) {
	bar.Add("Spelregels", THISBACK(Spelregels));
	bar.Add("Informatie", THISBACK(Informatie));
}

void MyAppWindow::SubMenu2b(Bar& bar) {
	// sub van Spelerkeuze
	bar.Add("Speler 1 (blauw)", THISBACK(SpelerKeuze1));
	bar.Add("Speler 2 (rood)", THISBACK(SpelerKeuze2));
}

void MyAppWindow::SubMenu2c(Bar& bar) {
	// sub van Speelstijl
	bar.Add("Willekeurig", THISBACK(SpeelstijlRandom));
	bar.Add("Slim", THISBACK(SpeelstijlSlim));
}

void MyAppWindow::Paint(Draw& w) {
	int x,y;
	int startx,starty;
	String statusregel;
	Point centrumpunt;
	int raambreedte, raamhoogte;
	int centreerx, centreery;
	int vijand;

	if (speler==1) vijand=2;
	else vijand=1;

	w.DrawRect(GetSize(), Black); // hele achtergrond zwart
	// alle virussen tonen
	for (x=0;x<kolommen;x++) {
		for (y=0;y<rijen;y++) {
			startx = x * vakjegrootte;
			starty = y * vakjegrootte;
			w.DrawRect(startx, starty, vakjegrootte-lijndikte, vakjegrootte-lijndikte, White());
			startx += (vakjegrootte-virusgrootte-lijndikte)/2;
			starty += (vakjegrootte-virusgrootte-lijndikte)/2;
			if ( bord->veld[x][y]==9 )
				w.DrawEllipse(startx, starty, virusgrootte, virusgrootte, LtYellow(), 2, Gray);
			if ( bord->veld[x][y]==8 )
				w.DrawEllipse(startx, starty, virusgrootte, virusgrootte, Yellow(), 2, Gray);
			if ( bord->veld[x][y]==1 )
				w.DrawEllipse(startx, starty, virusgrootte, virusgrootte, LtBlue(), 2, Gray);
			if ( bord->veld[x][y]==2 )
				w.DrawEllipse(startx, starty, virusgrootte, virusgrootte, LtRed(), 2, Gray);
		}
	}
	// geselecteerde kandidaatvirus laten opvallen
	if ( invoerstatus && bord->veld[invoer.xbron][invoer.ybron]==speler ) {
		startx = invoer.xbron * vakjegrootte;
		starty = invoer.ybron * vakjegrootte;
		startx += (vakjegrootte-(virusgrootte/2)-lijndikte)/2;
		starty += (vakjegrootte-(virusgrootte/2)-lijndikte)/2;
		w.DrawEllipse(startx, starty, virusgrootte/2, virusgrootte/2, Yellow());
	}
	// laatst gezette computervirus ander kleurtje geven
	if ( oplichtx > -1 && oplichty > -1 ) {
		startx = oplichtx * vakjegrootte;
		starty = oplichty * vakjegrootte;
		startx += (vakjegrootte-virusgrootte-lijndikte)/2;
		starty += (vakjegrootte-virusgrootte-lijndikte)/2;
		if (speler==1)
			w.DrawEllipse(startx, starty, virusgrootte, virusgrootte, LtMagenta(), 2, Gray);
		if (speler==2)
			w.DrawEllipse(startx, starty, virusgrootte, virusgrootte, LtCyan(), 2, Gray);
	}
	w.DrawText(vakjegrootte, (rijen-1)*(vakjegrootte), 400, bordtekst, Roman(24).Underline().Bold(), Red);
	// statusregel
		status.Height(20);
		// statusregel menselijke speler
			statusregel = "";
			//statusregel += "Speler " + Format("%i",speler) + " ";
			if (speler==1) statusregel += "blauw";
			else statusregel += "rood";
			statusregel += " (";
			statusregel += "mens";
			statusregel += ") ";
			statusregel += "[" + Format("%i",bord->telvirussen(speler)) + "]";
		statusregel += " <--> ";
		// statusregel computerspeler
			//statusregel += "Speler " + Format("%i",vijand) + " ";
			if (vijand==1) statusregel += "blauw";
			else statusregel += "rood";
			statusregel += " (";
			if (slim) statusregel += "slim";
			else statusregel += "willekeurig";
			statusregel += ") ";
			statusregel += "[" + Format("%i",bord->telvirussen(vijand)) + "]";
		status = statusregel;
	// stel grootte en positie van hoofdraam in (indien 1e start of verandering grootte bord)
	if (bordwissel) {
		raambreedte = vakjegrootte*kolommen;
		raamhoogte = vakjegrootte*rijen + 40;
		// bereken het midden van het scherm
		centrumpunt = GetWorkArea().CenterPoint();
		centreerx = centrumpunt.x-(int)(.5*raambreedte);
		centreery = centrumpunt.y-(int)(.5*raamhoogte);
	//	SetRect(centreerx, centreery, raambreedte, raamhoogte);
		bordwissel = false;
	}
}

void MyAppWindow::RightDown(Point, dword flags) {
	// menu ook via rechtermuisknop
	MenuBar::Execute(THISBACK(MainMenu));
}

void MyAppWindow::LeftDown(Point p, dword flags) {
	int x = -1;
	int y = -1;
	int i;
	if ( ! invoermogelijk ) return;
	for (i=0;i<kolommen;i++) {
		if ( p.x>i*vakjegrootte && p.x < i*vakjegrootte+vakjegrootte )
			x = i;
	}
	for (i=0;i<rijen;i++) {
		if ( p.y>i*vakjegrootte && p.y < i*vakjegrootte+vakjegrootte )
			y = i;
	}
	if ( x>-1 && y>-1 ) {
		if (invoerstatus) {
			// 2e klik
			VerbergLegaleZetten();
			invoer.xdoel = x;
			invoer.ydoel = y;
			invoer.speler = speler;
			if (bord->toegestanezet(invoer)) {
				geheugen.zetopstapel (bord);
				bord->doezet(invoer);
				spelerwissel (speler);
				ComputerZet();
			}
			invoerstatus = false;	// weer nieuwe zetinvoer mogelijk maken
		}
		else {
			// 1e klik
			invoer.xbron = x;
			invoer.ybron = y;
			invoer.speler = speler;
			ToonLegaleZetten();
			invoerstatus = true;
		}
		Refresh();
	}
}

void MyAppWindow::ComputerZet() {
	// laat de computer spelen als die aan de beurt is
	if ( bord->telvirussen(speler) > 0 && bord->bestaanmogelijkezetten(speler) ) {
		if (slim) invoer = bord->strategiezet(speler);
		else invoer = bord->randomzet(speler);
		if ( bord->toegestanezet(invoer) ) {
			bord->doezet (invoer);
			// vertraging in het weergeven van de computerzet
			oplichtx = invoer.xdoel;
			oplichty = invoer.ydoel;
			SetTimeCallback(1500, THISBACK(OplichtenUit));
		}
	}
	spelerwissel (speler);
	Evalueer();
}

void MyAppWindow::OplichtenUit() {
	// het veld even van kleur laten veranderen - uitzetten
	oplichtx = -1;
	oplichty = -1;
	Refresh();
}

void MyAppWindow::Omwisselen() {
	bord->omwisselen();
	Refresh();
}

void MyAppWindow::Evalueer() {
	bool kanspeler = false;
	bool kanvijand = false;
	bool eindespel = false;
	int vijand;

	if (speler==1) vijand=2;
	else vijand=1;

	// testen op mogelijkheid tot doen van een zet
	if ( bord->telvirussen(speler) > 0 && bord->bestaanmogelijkezetten(speler) ) {
		kanspeler = true;
	}
	if ( bord->telvirussen(vijand) > 0 && bord->bestaanmogelijkezetten(vijand) ) {
		kanvijand = true;
	}
	if ( kanspeler==false && kanvijand==false ) eindespel = true;

	// testen op het hebben van virussen
	if ( bord->telvirussen(speler) == 0 ) eindespel = true;
	if ( bord->telvirussen(vijand) == 0 ) eindespel = true;

	if (eindespel) {
		if ( bord->telvirussen(speler) > bord->telvirussen(vijand) ) {
			PromptOK ("U heeft gewonnen!!!");
			bordtekst = "gewonnen";
		}
		if ( bord->telvirussen(speler) < bord->telvirussen(vijand) ) {
			PromptOK ("U heeft verloren...");
			bordtekst = "verloren";
		}
		if ( bord->telvirussen(speler) == bord->telvirussen(vijand) ) {
			PromptOK ("Gelijk spel.");
			bordtekst = "gelijk spel";
		}
		invoermogelijk = false;
	}

	// computer speelt opnieuw als speler niet kan
	if ( bord->telvirussen(speler) > 0 && kanspeler==false && kanvijand==true) {
		spelerwissel (speler);
		ComputerZet();
	}
}

void MyAppWindow::ToonLegaleZetten() {
	int dx,dy;			// afstanden over x en y die een zet doet
	int dxtest, dytest;
	int afstand;
	zet test;
	test.speler = invoer.speler;
	test.xbron = invoer.xbron;
	test.ybron = invoer.ybron;
	for (dx=-2;dx<=2;dx++) {
		for (dy=-2;dy<=2;dy++) {
			test.xdoel = test.xbron + dx;
			test.ydoel = test.ybron + dy;
			if ( bord->toegestanezet(test) ) {
				// verplaatsingen gaan over afstand 2, clonaties over afstand 1
				// bepaald delta(x) en delta(y)
				dxtest = dx;
				dytest = dy;
				if ( dxtest < 0 ) dxtest *= -1;
				if ( dytest < 0 ) dytest *= -1;
				// bepaal nu de afstand
				if ( dxtest >= dytest ) afstand = dxtest;
				else afstand = dytest;
				// kleur in door bordwaarde op 8 / 9 te zetten
				if (afstand==1)
					bord->veld[test.xdoel][test.ydoel] = 8;
				if (afstand==2)
					bord->veld[test.xdoel][test.ydoel] = 9;
			}
		}
	}
}

void MyAppWindow::VerbergLegaleZetten() {
	int x,y;		// hulpvariabelen voor iteraties door veld
	for (x=0;x<kolommen;x++) {
		for (y=0;y<rijen;y++) {
			if ( bord->veld[x][y] == 8 ) bord->veld[x][y] = 0;
			if ( bord->veld[x][y] == 9 ) bord->veld[x][y] = 0;
		}
	}
}

MyAppWindow::MyAppWindow() {
	BordInit();
	Title("ataxx").MinimizeBox();
	AddFrame(menu);
	menu.Set(THISBACK(MainMenu));
	AddFrame(status);
}

MyAppWindow::~MyAppWindow() {
	// destructor
	delete bord;
}

GUI_APP_MAIN {
	MyAppWindow ataxx;
	ataxx.Run();
}

bordselectie::bordselectie(int xdefault, int ydefault) {
	CtrlLayout(*this, "");
	vullen(xdefault, ydefault);
	Bannuleren <<= THISBACK(annuleren);
	BOK <<= THISBACK(OK);
}

void bordselectie::vullen(int xdefault, int ydefault) {
	int i;
	int k;
	for (i=5;i<30;i++) {
		if ( oneven(i) ) {
			x.Add(i,Format("%i",i));
			y.Add(i,Format("%i",i));
		}
	}
	// standaard waarde op huidige bordgrootte zetten
	// x
	k = x.FindKey(xdefault);
	if (k==-1) PromptOK("kolommen niet gevonden");
	x.SetIndex(k);
	// y
	k = y.FindKey(ydefault);
	if (k==-1) PromptOK("rijen niet gevonden");
	y.SetIndex(k);
}

void bordselectie::annuleren() {
	Break();
}

void bordselectie::OK() {
	kolommen = x.GetData();
	rijen = y.GetData();
	Break();
}
