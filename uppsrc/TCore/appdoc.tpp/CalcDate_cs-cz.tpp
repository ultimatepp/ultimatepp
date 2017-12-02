TITLE("Funkce pro práci s datem / časem")
REF("Calc$CaddDN")
REF("Calc$CaddND")
REF("Calc$Cadd_monthsDN")
REF("Calc$Cadd_yearsDN")
REF("Calc$CdateN")
REF("Calc$CdateNN")
REF("Calc$CdateNNN")
REF("Calc$Cday_of_weekD")
REF("Calc$Cfirst_day_of_monthD")
REF("Calc$CgeTT")
REF("Calc$CgtTT")
REF("Calc$Clast_day_of_monthD")
REF("Calc$CleTT")
REF("Calc$CltTT")
REF("Calc$CmaxTT")
REF("Calc$CminTT")
REF("Calc$CminmaxTTT")
REF("Calc$CsubDD")
REF("Calc$CsubDN")
REF("Calc$CtimeNNNNNN")
REF("Calc$Cyears_betweenDD")
TOPIC_TEXT(
"[2 $$0,0#00000000000000000000000000000000:Default][b100;a50;K* $$1,0#23046025415494101427921008701515:Paragraph][i167;b50;a25;kKN1;~~~>.3917;* "
"$$2,0#52185223490867151055420261384346:Toc1`.Heading1][i333;kN11;~~~>.3917;* $$3,0#36545033421193513457240054113541:Toc2`.Heading2][i500;kN111;~~~>.3917; "
"$$4,0#65092512416424580164828413186219:Toc3`.Heading3][i167;b50;a25;kKNA;~~~>.3917;* "
"$$5,5#92291332474308709450111162202673:AppToc1`.AppHeading1][i333;kNA1;~~~>.3917;* "
"$$6,15#61887903412252554674480251236992:AppToc2`.AppHeading2][i500;kNA11;~~~>.3917; "
"$$7,7#03533551421653050355120437860335:AppToc3`.AppHeading3][i500;b100;a100;KN1;*4 "
"$$8,0#94145093439740438411010082165944:Heading1][i500;b100;a50;kKN11;*+117 $$9,0#10901425421210535753260366156531:Heading2][i500;b100;a25;kKN111;* "
"$$10,0#53209141402041366431858192225431:Heading3][i500;b100;a100;KNA;*4 $$11,8#32912651424211307887950041245434:AppHeading1][i500;b100;a50;kKNA1;*+117 "
"$$12,8#51232055453458524230328032001901:AppHeading2][i500;b100;a25;kKNA11;* $$13,8#34108020439428305453439845463579:AppHeading3][b100;a50;k*4 "
"$$14,0#49239115434362547028389207250014:Heading][b50;a25;k~~~>.3917;* $$15,15#40743698422276745552070108434023:Toc`.Heading][b100;a50;K~~~>.3917;*+117 "
"$$16,16#15422850471211852294098111138263:Topics1][l167;b50;a50;K~~~>.3917;* $$17,17#35241722455500657380289596243808:Topics2][l333;~~~>.3917; "
"$$18,18#31041344429742018840308064013200:Topics3][b83;a42;K* $$19,0#46440151113795363000000595230845:Paragraph][{_}%EN-US "
"[s14;:Calc`$CaddDN: [%00-00 operátor `+  (datum/čas, reálné číslo) ][%00-00S "
"®][%00-00  datum/čas]&][s0; Operátor posune zadané datum o_daný počet dní "
"(dopředu, pokud je číslo kladné). Počet dní může být i neceločíselný, "
"v_tom případě se posune i číslo sekundy, minuty a hodiny.&][s0; &][s0; [* Příklad: "
"]27.12.1999 `+ 7 `= 3.1.2000&][s14;:Calc`$CaddND: [%00-00 operátor `+  (reálné "
"číslo, datum/čas) ][%00-00S ®][%00-00  datum/čas]&][s0; Operátor posune zadané "
"datum o_d")
TOPIC_TEXT(
"aný počet dní (dopředu, pokud je číslo kladné). Počet dní může být i "
"neceločíselný, v_tom případě se posune i číslo sekundy, minuty a hodiny.&][s0; "
"&][s0; [* Příklad: ]27.12.1999 `+ 7 `= 3.1.2000&][s14;:Calc`$Cadd`_monthsDN: [%00-00 "
"add`_months (datum, celé číslo ][%00-00/ počet`_měsíců][%00-00 ) ][%00-00S "
"®][%00-00  datum]&][s0; Funkce přičte k datu zadaný [/ počet měsíců]. Pokud "
"cílový měsíc má méně dní, než je číslo dne v měsíci v původním datu, "
"nastaví se číslo dne na číslo posledního dne v cílovém měsíci.&][s0; &][s0; "
"[* Příklad: ]add`_months(29.2.1996, 12) `= 28.2.1997&][s14;:Calc`$Cadd`_yearsDN: "
"[%00-00 add`_years (datum, celé číslo ][%00-00/ počet`_let][%00-00 ) ][%00-00S "
"®][%00-00  datum]&][s0; Funkce přičte k datu zadaný [/ počet let]. Pokud původní "
"datum je 29.2. v přestupném roce a cílový rok není přestupný, změní se číslo "
"dne na 28.&][s0; &][s0; [* Příklad: ]add`_years(29.2.1992, 3) `= 28.2.1995&][s14;:Calc`$CdateN: "
"[%00-00 date (celé číslo ][%00-00/ den][%00-00 ) ][%00-00S ®][%00-00  datum]&][s0; "
"Funkce vrátí aktuální systémové datum, ve kterém zamění číslo dne hodnotou "
"zadaného parametru [* den].&][s0; &][s0; Viz také:&][s0;i150;O0;*^dpp`:`/`/AppDoc`/`/`:`:`/`/Calc`$CdateNN^ "
"date (celé číslo [/ den], celé číslo [/ měsíc]) [S ®] datum&][s0;i150;O0;*^dpp`:`/`/AppDoc`/`/`:`:`/`/Calc`$CdateNNN^ "
"date (celé číslo [/ den], celé číslo [/ měsíc], celé číslo [/ rok]) [S "
"®] datum&][s14;:Calc`$CdateNN: [%00-00 date (celé číslo ][%00-00/ den][%00-00 "
", celé číslo ][%00-00/ měsíc][%00-00 ) ][%00-00S ®][%00-00  datum]&][s0; Funkce "
"vrátí aktuální systémové datum, ve kterém zamění číslo dne hodnotou zadaného "
"parametru [/ den] a číslo měsíce hodnotou parametru [/ měsíc].&][s14;:Calc`$CdateNNN: "
"[%00-00 date (celé číslo ][%00-00/ den][%00-00 , celé číslo ][%00-00/ měsíc][%00-00 "
", celé číslo ][%00-00/ rok][%00-0")
TOPIC_TEXT(
"0 ) ][%00-00S ®][%00-00  datum]&][s0; Funkce sestaví datum ze zadaných položek. "
"Funkce bez parametru vrátí dnešní datum, zjištěné ze systému. Funkce s_jedním "
"parametrem [/ den] vrátí den se zadaným číslem v_aktuálním měsíci. Funkce "
"se dvěma parametry [/ den], [/ měsíc] vrátí den se zadaným číslem dne a_měsíce "
"v_aktuálním roce. Varianta se třemi parametry pouze sestaví datum ze zadaných "
"součástí.&][s0;* &][s0; [* Příklad: ]date `= 19.06.2003, date(10) `= 10.06.2003, "
"date(15, 7) `= 15.07.2003, date(25, 1, 1974) `= 25.01.1974&][s0; [Idatum`, funkce; "
"Funkce] vrátí číslo dne v měsíci pro zadané datum.&][s0; &][s0; [* Příklad: "
"]den(27.5.1984) `= 27&][s14;:Calc`$Cday`_of`_weekD: [%00-00 day`_of`_week (datum) "
"][%00-00S ®][%00-00  celé číslo]&][s0; Funkce vrátí index dne v_týdnu pro "
"zadané datum. Prvním dnem v_týdnu s_indexem 0 je pro tuto funkci [* neděle], "
"posledním dnem je [* sobota] s_indexem 6.&][s0; &][s0; [* Příklad: ]day`_of`_week(19.06.2003) "
"`= 4&][s14;:Calc`$Cfirst`_day`_of`_monthD: [%00-00 first`_day`_of`_month (datum ][%00-00/ "
"d][%00-00 ) ][%00-00S ®][%00-00  datum]&][s0; Funkce vrátí datum prvního dne "
"v měsíci určeném parametrem [/ d].&][s0; &][s0; [* Příklad: ]first`_day`_of`_month(18.3.1994) "
"`= 1.3.1994&][s14;:Calc`$CgeTT: [%00-00 operátor >`=  (datum/čas ][%00-00/ dt1][%00-00 "
", datum/čas ][%00-00/ dt2][%00-00 ) ][%00-00S ®][%00-00  logická hodnota]&][s0; "
"Funkce porovná zadaná data / časy a vrátí hodnotu [/ pravda], pokud je [/ datum/čas1] "
"pozdější nebo roven času [/ datum/čas2].&][s0; &][s0; [* Příklad: ](15.1.2004 "
">`= 13.3.2003) `= 1&][s14;:Calc`$CgtTT: [%00-00 operátor >  (datum/čas ][%00-00/ "
"dt1][%00-00 , datum/čas ][%00-00/ dt2][%00-00 ) ][%00-00S ®][%00-00  logická hodnota]&][s0; "
"Funkce porovná zadaná data / časy a vrátí hodnotu [/ pravda], pokud je [/ dt1] "
"pozdější než [/ dt2].&][s0; &][s0; [* Příklad: ](15.1.2004 > 13.3.2003) `= "
"1, (15.1.2004 > ")
TOPIC_TEXT(
"15.1.2004) `= 0&][s0; Funkce vrátí počet hodin svého argumentu typu datum/čas.&][s14;:Calc`$Clast`_day`_of`_monthD: "
"[%00-00 last`_day`_of`_month (datum ][%00-00/ d][%00-00 ) ][%00-00S ®][%00-00  datum]&][s0; "
"Funkce vrátí datum posledního dne v měsíci určeném parametrem [/ d].&][s0; "
"&][s0; [* Příklad]: last`_day`_of`_month(3.2.1999) `= 28.2.1999&][s14;:Calc`$CleTT: "
"[%00-00 operátor <`=  (datum/čas, datum/čas) ][%00-00S ®][%00-00  logická hodnota]&][s0; "
"Funkce porovná zadaná data / časy a vrátí hodnotu [/ pravda], pokud je [* datum/čas1] "
"dřívější nebo roven času [* datum/čas2].&][s0; &][s0; [* Příklad: ](15.1.2003 "
"< 13.3.2003) `= 1&][s14;:Calc`$CltTT: [%00-00 operátor <  (datum/čas ][%00-00/ "
"dt1][%00-00 , datum/čas ][%00-00/ dt2][%00-00 ) ][%00-00S ®][%00-00  logická hodnota]&][s0; "
"Funkce porovná zadaná data / časy a vrátí hodnotu [/ pravda], pokud je čas "
"[* dt1] dřívější než [* dt2].&][s0; &][s0; [* Příklad: ](15.1.2003 < 13.3.2003) "
"`= 1&][s0; Funkce vrátí maximum z hodnot v daném poli. Pokud pole obsahuje řetězce, "
"provede se jejich lexikografické srovnání.&][s0; &][s0; [* Příklad: ]max(`[8, "
"5, 13, 2, 9`]) `= 13&][s14;:Calc`$CmaxTT: [%00-00 max (datum/čas, datum/čas) ][%00-00S "
"®][%00-00  datum/čas]&][s0; Funkce vrátí větší z_dvojice zadaných dat/časů "
"(pozdější čas/datum). Pokud je některý z parametrů roven [* null], je výsledkem "
"funkce také [* null].&][s0; &][s0; [* Příklad: ]max(3.5.1994, 7.8.1998) `= 7.8.1998&][s14;:Calc`$CminTT: "
"[%00-00 min (datum/čas, datum/čas) ][%00-00S ®][%00-00  datum/čas]&][s0; Funkce "
"vrátí minimum ze dvou časů (dřívější z obou časů). Pokud je některý "
"z parametrů roven [* null], je výsledkem funkce také [* null].&][s0;* &][s0; [* "
"Příklad: ]min(1.8.1995, 3.6.1997) `= 1.8.1995&][s14;:Calc`$CminmaxTTT: [%00-00 "
"minmax (datum/čas, datum/čas, datum/čas) ][%00-00S ®][%00-00  datum/čas]&][s0; "
"Funkce vrátí hodnotu [* min(max(x, xmin), x")
TOPIC_TEXT(
"max)]. Pokud je některý z parametrů roven [* null], je výsledek také [* null].&][s0; "
"&][s0; [* Příklad: ]minmax(3.1.1990, 8.8.1994, 2.5.1997) `= 8.8.1994, minmax(7.6.1995, "
"8.8.1994, 2.5.1997) `= 7.6.1995, minmax(20.12.2003, 8.8.1994, 2.5.1997) `= 2.5.1997&][s14;:Calc`$CsubDD: "
"[%00-00 operátor `-  (datum/čas ][%00-00/ datum1][%00-00 , datum/čas ][%00-00/ "
"datum2][%00-00 ) ][%00-00S ®][%00-00  reálné číslo]&][s0; Unární operátor "
"[* `-] obrátí znaménko svého argumentu.&][s0; &][s0; Binární operátor [* `-] "
"odečte své operandy. V_případě dvojice čísel je výsledkem běžný číselný "
"rozdíl hodnot. V_případě rozdílu data a_celého čísla se od data odečte zadaný "
"počet dní. Výsledkem rozdílu dvou dat je počet dní mezi nimi. Počet je kladný, "
"pokud je první datum pozdější než druhé.&][s0; &][s0; [* Příklad]: 137 `- "
"95 `= 42, 27.12.1999 `- 5 `= 22.12.1999, 4.1.2000 `- 28.12.1999 `= 7&][s14;:Calc`$CsubDN: "
"[%00-00 operátor `-  (datum/čas ][%00-00/ d][%00-00 , reálné číslo ][%00-00/ "
"počet`_dní][%00-00 ) ][%00-00S ®][%00-00  datum/čas]&][s0; Operátor odečte "
"od zadaného data/času [* d] zadaný [* počet dní]. Pokud je počet dní neceločíselný, "
"posune se i číslo hodiny, minuty a sekundy. &][s0; &][s0; [* Příklad: ]11.6.2000 "
"`+ 11.25 `= 22.6.2000 06:00:00&][s14;:Calc`$CtimeNNNNNN: [%00-00 time(celé číslo "
"][%00-00/ den][%00-00 , celé číslo ][%00-00/ měsíc][%00-00 , celé číslo ][%00-00/ "
"rok][%00-00 , celé číslo ][%00-00/ hodina][%00-00 , celé číslo ][%00-00/ minuta][%00-00 "
", celé číslo ][%00-00/ sekunda][%00-00 ) ][%00-00S ®][%00-00  datum/čas]&][s0; "
"Funkce vrátí hodnotu typu [* datum/čas] se zadaným číslem dne, hodiny, minuty "
"a sekundy.&][s0; &][s0; [* Příklad: ]time(3, 7, 2003, 11, 15, 37).text `= `\"3.7.2003 "
"11:15:37`\"&][s0; Funkce vrátí číslo roku pro zadané datum.&][s0; &][s0; [* "
"Příklad]: rok(27.5.1984) `= 1984&][s14;:Calc`$Cyears`_betweenDD: [%00-00 years`_between")
TOPIC_TEXT(
" (datum ][%00-00/ d1][%00-00 , datum ][%00-00/ d2][%00-00 ) ][%00-00S ®][%00-00 "
" celé číslo]&][s0; Funkce určí počet let, která leží mezi zadanými dvěma "
"daty (věk osoby, narozené dne [/ d1], ke dni [/ d2]).&][s0; &][s0; [* Příklad]: "
"years_between(25.1.1974, 27.12.1999) `= 25&][s0; &][s0; &][s0; &][s0; ]")
