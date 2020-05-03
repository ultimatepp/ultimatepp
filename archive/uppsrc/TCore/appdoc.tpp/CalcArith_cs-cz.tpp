TITLE("Aritmetické operace")
REF("Calc$CaddNN")
REF("Calc$CceilN")
REF("Calc$CceilNN")
REF("Calc$CceilrNN")
REF("Calc$CdivNN")
REF("Calc$CfloorN")
REF("Calc$CfloorNN")
REF("Calc$CfloorrNN")
REF("Calc$CgeNN")
REF("Calc$CgtNN")
REF("Calc$ChypotNN")
REF("Calc$CleNN")
REF("Calc$CltNN")
REF("Calc$CmaxNN")
REF("Calc$CminNN")
REF("Calc$CminmaxNNN")
REF("Calc$CminusN")
REF("Calc$CmodNN")
REF("Calc$CmulNN")
REF("Calc$CplusN")
REF("Calc$CroundN")
REF("Calc$CroundNN")
REF("Calc$CroundrNN")
REF("Calc$CsgnN")
REF("Calc$CsqrN")
REF("Calc$CsqrtN")
REF("Calc$CsubNN")
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
"[s14; abs (reálné číslo) [S ®] reálné číslo&][s0; &][s0; Absolutní hodnota "
"čísla. Pokud je číslo kladné, jeho absolutní hodnota je mu rovna. Absolutní "
"hodnotou záporného čísla je číslo opačné.&][s0; &][s0; [* Příklad: ]abs(`-10.5) "
"`= 10.5, abs(12) `= 12.&][s14;:Calc`$CaddNN: [%00-00 operátor `+  (reálné číslo, "
"reálné číslo) ][%00-00S ®][%00-00  reálné číslo]&][s0; Operátor sečte "
"zadanou dvojici reálných čísel. Pokud je některý z parametrů [* null], je "
"výsled")
TOPIC_TEXT(
"kem součtu opět [* null]. &][s0; &][s0; [* Příklad: ]5`+7`=12&][s14;:Calc`$CceilN: "
"[%00-00 ceil (reálné číslo) ][%00-00S ®][%00-00  reálné číslo]&][s0; Zaokrouhlení "
"směrem nahoru. Funkce nalezne nejmenší celé číslo, které není menší než "
"původní [/ hodnota].&][s0; &][s0; [* Příklad: ]ceil(`-2.5) `= `-2&][s14;:Calc`$CceilNN: "
"[%00-00 ceil (reálné číslo ][%00-00/ hodnota][%00-00 , celé číslo ][%00-00/ "
"počet`_desetinných`_míst][%00-00 ) ][%00-00S ®][%00-00  reálné číslo]&][s0; "
"Zaokrouhlení směrem nahoru na zadaný počet desetinných míst. Funkce nalezne "
"nejmenší reálné číslo, které je celistvým násobkem hodnoty [* 10_`*`*_][/ "
"`-počet_míst] a_není menší než původní [/ hodnota].&][s0; &][s0; [* Příklad: "
"]ceil(47.32, 1) `= 47.4, ceil(117, `-1) `= 120.&][s14;:Calc`$CceilrNN: [%00-00 ceilr(reálné "
"číslo ][%00-00/ hodnota][%00-00 , celé číslo ][%00-00/ platných`_číslic][%00-00 "
") ][%00-00S ®][%00-00  reálné číslo]&][s0; [Izaokrouhlování`; číslo`, zaokrouhlení; "
"Zaokrouhlení] směrem nahoru na zadaný počet platných číslic. Funkce nalezne "
"nejmenší reálné číslo, které je celistvým násobkem hodnoty [* 10_`*`*_][/ "
"(ceil(log(hodnota))`-platných`_číslic)] a_není menší než původní [/ hodnota].&][s0; "
"&][s0; [* Příklad: ]ceilr(47.32, 3) `= 47.4, ceilr(117, 1) `= 200.&][s14;:Calc`$CdivNN: "
"[%00-00 operátor /  (reálné číslo, reálné číslo) ][%00-00S ®][%00-00  reálné "
"číslo]&][s0; Dělení. Pokud je na pravé straně operátoru nula, výpočet výrazu "
"ohlásí chybu [* dělení nulou].&][s0; &][s0; [* Příklad: ]3.7 / 2.5 `= 1.48.&][s14;:Calc`$CfloorN: "
"[%00-00 floor (reálné číslo) ][%00-00S ®][%00-00  reálné číslo]&][s0; Zaokrouhlení "
"směrem dolů. Funkce nalezne nejvyšší celé číslo, které není vyšší než "
"původní [/ hodnota].&][s0; &][s0; [* Příklad: ]floor(`-2.5) `= `-3&][s14;:Calc`$CfloorNN: "
"[%00-00 floor (reálné číslo ][%00-00/ hodnota][")
TOPIC_TEXT(
"%00-00 , celé číslo ][%00-00/ počet`_desetinných`_míst][%00-00 ) ][%00-00S "
"®][%00-00  reálné číslo]&][s0; Zaokrouhlení směrem dolů na zadaný počet "
"desetinných míst. Funkce nalezne nejvyšší reálné číslo, které je celistvým "
"násobkem hodnoty [* 10_`*`*_][/ `-počet_míst] a_není vyšší než původní "
"[/ hodnota].&][s0; &][s0; [* Příklad: ]floor(47.32, 1) `= 47.3, floor(117, `-1) "
"`= 110.&][s14;:Calc`$CfloorrNN: [%00-00 floorr(reálné číslo ][%00-00/ hodnota][%00-00 "
", celé číslo ][%00-00/ platných`_číslic][%00-00 ) ][%00-00S ®][%00-00  reálné "
"číslo]&][s0; Zaokrouhlení směrem dolů na zadaný počet platných číslic. "
"Funkce nalezne nejvyšší reálné číslo, které je celistvým násobkem hodnoty "
"[* 10_`*`*_][/ (ceil(log(hodnota))`-platných`_číslic)] a_není vyšší než původní "
"[/ hodnota].&][s0; &][s0; [* Příklad: ]floorr(47.32, 3) `= 47.3, floorr(117, 1) "
"`= 100.&][s14;:Calc`$CgeNN: operátor >`=  (reálné číslo, reálné číslo) [S "
"®] logická hodnota&][s0; Operátor porovná dvojici hodnot a_vrátí hodnotu 1, "
"pokud nerovnost platí, respektive 0, pokud nerovnost neplatí. Texty se porovnávají "
"lexikograficky ([/ slovníkové] uspořádání).&][s14;:Calc`$CgtNN: operátor > "
" (reálné číslo [/ n1], reálné číslo [/ n2]) [S ®] logická hodnota&][s0; "
"Operátor porovná dvojici hodnot a_vrátí hodnotu 1, pokud nerovnost platí, respektive "
"0, pokud nerovnost neplatí. Texty se porovnávají lexikograficky ([/ slovníkové] "
"uspořádání).&][s14;:Calc`$ChypotNN: [%00-00 hypot (reálné číslo ][%00-00/ "
"x][%00-00 , reálné číslo ][%00-00/ y][%00-00 ) ][%00-00S ®][%00-00  reálné "
"číslo]&][s0; Délka přepony pravoúhlého trojúhelníka. Výpočet přepony se "
"provádí podle Pythagorovy věty: [* hypot(][*/ x][* , ][*/ y][* ) `= sqrt(][*/ "
"x][*  `* ][*/ x][*  `+ ][*/ y][*  `* ][*/ y][* )].&][s0; &][s0; [* Příklad]: hypot(3, "
"4) `= 5&][s14;:Calc`$CleNN: [%00-00 operátor <`=  (reálné čísl")
TOPIC_TEXT(
"o ][%00-00/ x][%00-00 , reálné číslo ][%00-00/ y][%00-00 ) ][%00-00S ®][%00-00 "
" logická hodnota]&][s0; Operátor porovná dvojici čísel a_vrátí hodnotu 1 ([/ "
"pravda]), pokud [* x <`= y].&][s0; &][s0; [* Příklad: ](10 <`= 20) `= 1&][s14;:Calc`$CltNN: "
"[%00-00 operátor <  (reálné číslo, reálné číslo) ][%00-00S ®][%00-00  logická "
"hodnota]&][s0; Operátor porovná dvojici čísel a_vrátí hodnotu 1 ([/ pravda]), "
"pokud [* x < y].&][s0; &][s0; [* Příklad: ](10 < 20) `= 1&][s14;:Calc`$CmaxNN: "
"max (reálné číslo, reálné číslo) [S ®] reálné číslo&][s0; Funkce vrátí "
"maximum ze dvou reálných čísel. Pokud je některé z čísel rovno [* null], "
"je výsledkem funkce také [* null].&][s14;:Calc`$CminNN: [%00-00 min (reálné číslo, "
"reálné číslo) ][%00-00S ®][%00-00  reálné číslo]&][s0; Funkce vrátí minimum "
"ze dvou reálných čísel. Pokud je některé z čísel rovno [* null], je výsledkem "
"funkce také [* null].&][s0; &][s0; [* Příklad: ]min(`-15, `-16) `= `-16&][s14;:Calc`$CminmaxNNN: "
"[%00-00 minmax (reálné číslo ][%00-00/ x][%00-00 , reálné číslo ][%00-00/ "
"xmin][%00-00 , reálné číslo ][%00-00/ xmax][%00-00 ) ][%00-00S ®][%00-00  reálné "
"číslo]&][s0; Funkce vrátí hodnotu [* min(max(x, xmin), xmax)]. Pokud je některý "
"z parametrů roven [* null], je výsledek také [* null].&][s0; &][s0; [* Příklad: "
"]minmax(3, 5, 10) `= 5, minmax(7, 5, 10) `= 7, minmax(20, 5, 10) `= 10&][s14;:Calc`$CminusN: "
"[%00-00 operátor `-  (reálné číslo) ][%00-00S ®][%00-00  reálné číslo]&][s0; "
"Výsledkem aplikace unárního operátoru `-  je číslo opačné k zadanému reálnému "
"číslu.&][s0; &][s0; [* Příklad: ]`-(2 `* 2) `= `-4&][s0; Funkce vrátí počet "
"minut ze zadaného parametru typu datum / čas.&][s14;:Calc`$CmodNN: [%00-00 operátor "
"%  (reálné číslo ][%00-00/ a][%00-00 , reálné číslo ][%00-00/ b][%00-00 ) "
"][%00-00S ®][%00-00  reálné číslo]&][s0; Zbytek po dělení. Operátor určí "
"nejmenš�")
TOPIC_TEXT(
"� nezáporné číslo ze všech čísel, která lze získat přičtením nebo odečtením "
"celočíselného násobku hodnoty [/ b] k_hodnotě [/ a]. Pokud je na pravé straně "
"operátoru nula, výpočet výrazu ohlásí chybu [* dělení nulou].&][s0; &][s0; "
"[* Příklad]: 10.5 % 2.4 `= 0.9 (10.5 `= 4 `* 2.4 `+ 0.9).&][s0; Funkce vrátí "
"číslo měsíce v roce pro zadané datum.&][s0; &][s0; [* Příklad: ]month(27.5.1984) "
"`= 5&][s0; Operátor pro každý prvek zadaného pole vyhodnotí [/ lambda`-výraz] "
"a vrátí pole obsahující výsledky těchto výpočtů. Výsledné pole má vždy "
"stejný počet prvků jako vstupní pole. Při vyhodnocování [/ lambda`-výrazu] "
"je pro každý prvek původního pole proměnná kalkulátoru [* I] nastavená na "
"index tohoto prvku v poli (první prvek pole má index 0), proměnná [* A] obsahuje "
"hodnotu zpracovávaného prvku.&][s0; &][s0; [* Příklad: ]`[1, 2, 3, 4`] `* #(A "
"`* A) `= `[1, 4, 9, 16`]&][s14;:Calc`$CmulNN: [%00-00 operátor `*  (reálné číslo, "
"reálné číslo) ][%00-00S ®][%00-00  reálné číslo]&][s0; Součin reálných "
"čísel.&][s0;* &][s0; [* Příklad: ]17 `* 15 `= 255.&][s14;:Calc`$CplusN: [%00-00 "
"operátor `+  (reálné číslo) ][%00-00S ®][%00-00  reálné číslo]&][s0; Operátor "
"ponechá reálné číslo beze změny. Slouží jako symetrický protějšek k unárnímu "
"operátoru `- (minus).&][s0; &][s0; [* Příklad: ]`+(4`+5) `= 9&][s14;:Calc`$CroundN: "
"[%00-00 round (reálné číslo) ][%00-00S ®][%00-00  reálné číslo]&][s0; Zaokrouhlení "
"na nejbližší celé číslo. Pokud zadané číslo leží přesně v_polovině "
"vzdálenosti mezi nejbližšími celými čísly, zaokrouhlí se směrem nahoru.&][s0; "
"&][s0; [* Příklad: ]round(`-2.5) `= `-2, round(47.32) `= 47&][s14;:Calc`$CroundNN: "
"[%00-00 round (reálné číslo ][%00-00/ hodnota][%00-00 , celé číslo ][%00-00/ "
"počet`_desetinných`_míst][%00-00 ) ][%00-00S ®][%00-00  reálné číslo]&][s0; "
"Zaokrouhlení na zadaný počet desetin")
TOPIC_TEXT(
"ných míst (0, pokud není zadáno). Směr zaokrouhlení je k_nejbližšímu reálnému "
"číslu, které je celistvým násobkem hodnoty [* 10_`*`*_][/ `-počet_míst]. Pokud "
"zadané číslo leží přesně v_polovině vzdálenosti mezi nejbližšími takovými "
"čísly, zaokrouhlí se směrem nahoru.&][s0; &][s0; [* Příklad]: round(`-2.5) "
"`= `-2, round(47.32, 1) `= 47.3, round(117, `-1) `= 120.&][s14;:Calc`$CroundrNN: "
"[%00-00 roundr(reálné číslo ][%00-00/ hodnota][%00-00 , celé číslo ][%00-00/ "
"platných`_číslic][%00-00 ) ][%00-00S ®][%00-00  reálné číslo]&][s0; Zaokrouhlení "
"na zadaný počet platných číslic. Směr zaokrouhlení je k_nejbližšímu reálnému "
"číslu, které je celistvým násobkem hodnoty [* 10_`*`*_][/ (ceil(log(hodnota))`-platných`_číslic)]. "
"Pokud zadané číslo leží přesně v_polovině vzdálenosti mezi nejbližšími "
"takovými čísly, zaokrouhlí se směrem nahoru.&][s0; &][s0; [* Příklad: ]roundr(47.32, "
"2) `= 47, roundr(117, 1) `= 100.&][s0; Funkce doplní řetězec [* text] mezerami "
"zprava na počet znaků [* délka]. Pokud je [* délka] menší nebo rovna délce "
"původního řetězce [* text], funkce vrátí vstupní text beze změny.&][s0; &][s0;/ "
"[*/ Příklad: ][/ rpad(`\"ahoj`\", 8) `= `\"ahoj    `\" ](text [* ahoj] následovaný "
"čtyřmi mezerami)&][s14;:Calc`$CsgnN: [%00-00 sgn (reálné číslo ][%00-00/ x][%00-00 "
") ][%00-00S ®][%00-00  celé číslo]&][s0; Znaménko. Výpočet znaménka probíhá "
"podle následující tabulky:&][s0; &][s0; &][s0; &][s0; [* Příklad]: sgn(`-13.5) "
"`= `-1, sgn(0) `= 0, sgn(10) `= `+1.&][s14;:Calc`$CsqrN: [%00-00 sqr (reálné číslo "
"][%00-00/ x][%00-00 ) ][%00-00S ®][%00-00  reálné číslo]&][s0; Druhá mocnina. "
"Funkce vrátí hodnotu [* sqr(][*/ x][* ) `= ][*/ x][*  `* ][*/ x].&][s0; &][s0; "
"[* Příklad]: sqr(12) `= 144&][s14;:Calc`$CsqrtN: [%00-00 sqrt (reálné číslo "
"][%00-00/ x][%00-00 ) ][%00-00S ®][%00-00  reálné číslo]&][s0; Druhá odmocnina. "
"Funkce vrátí ta")
TOPIC_TEXT(
"kovou hodnotu [*/ a], pro kterou [*/ x][*  `= ][*/ a][*  `* ][*/ a]. Pokud je parametr "
"funkce záporný, výpočet výrazu skončí s chybou chyba při výpočtu v reálných "
"číslech.&][s0; &][s0; [* Příklad: ]sqrt(81) `= 9&][s14;:Calc`$CsubNN: [%00-00 "
"operátor `-  (reálné číslo, reálné číslo) ][%00-00S ®][%00-00  reálné "
"číslo]&][s0; Operátor odečte dvě reálná čísla. Pokud je libovolné z čísel "
"rovno [* null], výsledkem operátoru je opět [* null].&][s0; &][s0; [* Příklad: "
"]7 `- (`-13) `= 20&][s0; &][s0; &][s0; &][s0; &][s0; &][s0; ]")
