@ZAZNAM "Sociální záznam" _
    "Sociální záznam: <<PRIJETI>>, <<Osoby DITE>>, otec: <<Osoby OTEC>>, matka: <<Osoby MATKA>>"
    ^
;
osoby;
update;
PRIJETI    "Datum pøijetí do péèe"   date today;
/*
OTEC_ZAM   "Zamìstnání otce"         text +;
MATKA_ZAM  "Zamìstnání matky"        text;
OTEC_AZAM  "  adresa zamìstmavatele" text +;
MATKA_AZAM "  adresa zamìstmavatele" text;
OTEC_DUCH  "  poživatel dùchodu"     list ##ANONE = N +;
MATKA_DUCH "  poživatel dùchodu"     list ##ANONE = N;
OTEC_ZPS   "  ZPS" list ##ZPS = NE +;
MATKA_ZPS  "  ZPS" list ##ZPS = NE;
*/

@UKON "Záznam o úkonu" _
	^
	^
;

@ZAPIS "Jiný zápis" _
	"<<POPIS>>"
;
POPIS "Popis zápisu" text;

@NAVRH "Návrh" _
	"Návrh na <<POPIS>>"
	NAVRH "Návrh"
;
deti;
ZNACKA     "Naše znaèka" text;
OTEC       "Otec"        osoba OTEC;
MATKA      "Matka"       osoba MATKA;
POPIS      "Návrh na"    text;

@VYCHOVAVYZIVA     "Urèení výchovy a výživy" VV ##STDNAVRH;
@NEPROVDANAMATKA   "Urèení výživného pro neprovdanou matku" VV ##STDNAVRH;
@EXEKUCE           "Exekuce" VV ##STDNAVRH;
@URCENIVYZIVNEHO   "Urèení výživného" VV ##STDNAVRH;
@SNIZENIVYZIVNEHO  "Snížení výživného" VV ##STDNAVRH;
@VVROZVOD          "Urèení výchovy a výživy po rozvodu" VV ##STDNAVRH;
@ZMENAVYCHOVY      "Zmìna výchovy" VV ##STDNAVRH;
@ZRUSENIVYZIVNEHO  "Zrušení výživného" VV ##STDNAVRH;
@ZVYSENIVYZIVNEHO  "Zvýšení výživného" VV ##STDNAVRH;

@POPRENIOTCOVSTVI  "Popøení otcovství" VZTAH ##STDNAVRH;
@URCENIOTCOVSTVI   "Urèení otcovství" VZTAH ##STDNAVRH;
@ZMENAPRIJMENI     "Zmìna pøíjmení" VZTAH ##STDNAVRH;

@MIMOSOUDNISTYK    "Mimosoudní dohoda o styku" STYK ##STDNAVRH;
@NAVRHSTYK         "Úpravu styku" STYK ##STDNAVRH;

@PROTOKOLOSVOJENI "Protokol o osvojeni" VZTAH
	^
	^
;
deti;
OSVOJITEL   "Osvojitel"   osoba;
MATKA       "Matka"       osoba MATKA;
OTEC        "Otec"        osoba OTEC;

@OSVOJENI "Osvojeni manželem matky" VZTAH
    "Návrh na osvojeni: <<Osoba DITE>>, otec: <<Osoba MATKA>>, navrhovatel: <<Osoba NAVRHOVATEL>>"
	^
;
deti;
NAVRHOVATEL "Navrhovatel" osoba;
MATKA       "Matka"       osoba MATKA;
OTEC        "Otec"        osoba OTEC;


@NARIZENIDOHLEDU   "Naøízení dohledu" DOHLED ##STDNAVRH;
@POUSTAV           "PO a ústavní péèe" DOHLED ##STDNAVRH;
@POZMENA           "PO a zmìna výchovy" DOHLED ##STDNAVRH;
@PO                "PO" DOHLED ##STDNAVRH;
@NEMOCNICE         "Propuštìní do péèe matky" DOHLED ##STDNAVRH;

@REJSTRIK_SVERENYCH "Rejstøík dìtí svìøených do výchovy jiné fyzické osobì než rodièe" REJSTRIK
    "Rejstøík svìøených dìtí: Datum pøijetí: <<PRIJETI>>: <<Osoba DITE>>, odpovìdná osoba: <<Osoba OSOBA>>"
;
rejstrik "{{1:1:1:2:1:1:2:1:2:2:1:1 "
         "Poøadové èíslo:: Datum zápisu:: Spisová znaèka OM"
         ":: Jméno a pøíjmení dítìte:: Rodné èíslo:: Forma péèe"
         ":: Jméno a pøíjmení odpovìdné osoby (pøíbuzenský vztah):: Rodné èíslo odpovìdné osoby"
         ":: Místo trvalého pobytu:: Oznaèení orgánu&è.j. rozhodnutí&datum pr. moci"
         ":: Datum pøijetí dítìte do péèe:: Datum ukonèení péèe",
         "<<PC>>:: <<DATUM>>:: <<SPIS>>:: <<Osoba DITE>>:: <<OsobaRC DITE>>:: <<FORMA>>:: <<Osoba OSOBA>> "
         ":: <<OsobaRC OSOBA>>:: <<Adresa ADRESA>>:: <<ORGAN>>&<<CJ>>&<<NPM>>:: <<PRIJETI>>:: <<UKONCENI>>"
		PRIJETI UKONCENI
;
DITE     "Dítì" osoba DITE;
FORMA    "Forma péèe" text;
OSOBA    "Odpovìdná osoba" osoba;
ADRESA   " místo trvalého pobytu" adresa auto;
VZTAH    "Pøíbuzenský vztah" text;
ORGAN    "Oznaèení orgánu" text;
CJ       "È.j. rozhodnuti" text +;
NPM      "Datum právní moci" date;
PRIJETI  "Datum pøijetí do péèe" date;
UKONCENI "Datum ukonèení péèe" date;
