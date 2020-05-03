TITLE("Textové operace")
REF("Calc$CaddSS")
REF("Calc$CascS")
REF("Calc$CcatSSS")
REF("Calc$CchrN")
REF("Calc$CcountNS")
REF("Calc$CftextN")
REF("Calc$CftextNI")
REF("Calc$CgeSS")
REF("Calc$CgtSS")
REF("Calc$CitextNN")
REF("Calc$CleSS")
REF("Calc$CleftSN")
REF("Calc$ClenS")
REF("Calc$CllpadSN")
REF("Calc$ClpadSN")
REF("Calc$ClpadSNS")
REF("Calc$CltSS")
REF("Calc$CmaxSS")
REF("Calc$CmidSN")
REF("Calc$CmidSNN")
REF("Calc$CminSS")
REF("Calc$CminmaxSSS")
REF("Calc$CmulNS")
REF("Calc$CmulSN")
REF("Calc$CnlsN")
REF("Calc$CnlsNN")
REF("Calc$CnlsrN")
REF("Calc$CnlsrNN")
REF("Calc$CnumberS")
REF("Calc$CnumberSN")
REF("Calc$CreplaceSSS")
REF("Calc$CrightSN")
REF("Calc$CrpadSNS")
REF("Calc$CrrpadSN")
REF("Calc$CrrpadSNS")
REF("Calc$CstringIndex")
REF("Calc$CtextD")
REF("Calc$CtextN")
REF("Calc$CtextS")
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
"[s14;:Calc`$CaddSS: [%00-00 operátor `+  (řetězec, řetězec) ][%00-00S ®][%00-00 "
" řetězec]&][s0; Operátor spojí dva řetězce tak, že je zařadí za sebe.&][s0; "
"&][s0; [* Příklad: ]`\"ahoj`\" `+`\" světe`\" `= `\"ahoj světe`\"&][s14;:Calc`$CascS: "
"[%00-00 asc (řetězec) ][%00-00S ®][%00-00  celé číslo]&][s0; Funkce vrátí "
"ASCII`-kód prvního znaku zadaného textu. Pokud je text prázdný, funkce vrátí "
"hodnotu 0.&][s0; &][s0; [* Příklad: ]asc(`\"ahoj`\") `= 97 [/ (kód znaku `'a`')].&][s0; "
"Fun")
TOPIC_TEXT(
"kce spočítá průměr hodnot ze zadaného pole. Vstupní pole musí obsahovat "
"číselné hodnoty, případně hodnoty [* null], které se ovšem při výpočtu "
"průměru ignorují.&][s0; &][s0; [* Příklad: ]avg(`[10, 20, 24`]) `= 17&][s14;:Calc`$CcatSSS: "
"[%00-00 cat (řetězec ][%00-00/ text`_1][%00-00 , řetězec ][%00-00/ mezi][%00-00 "
", řetězec ][%00-00/ text`_2][%00-00 ) ][%00-00S ®][%00-00  řetězec]&][s0; Funkce "
"spojí zadané řetězce [/ text`_1] a [/ text`_2]. Pokud jsou oba řetězce neprázdné, "
"vloží se mezi ně řetězec [/ mezi].&][s0; &][s0; [* Příklad: ]cat(`\"ahoj`\", "
"`\" `\", `\"světe`\") `= `\"ahoj světe`\", cat(`\"`\", `\" `\", `\"světe`\") `= "
"`\"světe`\"&][s14;:Calc`$CchrN: [%00-00 chr (celé číslo ][%00-00/ ascii][%00-00 "
") ][%00-00S ®][%00-00  řetězec]&][s0; Funkce vrátí jednoznakový řetězec, "
"obsahující znak s_ASCII`-kódem [/ ascii].&][s0; &][s0; [* Příklad: ]chr(66) "
"`= `\"B`\".&][s14;:Calc`$CcountNS: [%00-00 count (celé číslo, řetězec) ][%00-00S "
"®][%00-00  řetězec]&][s0; Funkce vrátí textovou reprezentaci desítkového zápisu "
"číselné hodnoty s připojeným podstatným jménem. V řetězci podstatného jména "
"lze lomítkem oddělit přípony pro singulár, plurál a genitiv plurálu, které "
"se v českém jazyce používají k vyjádření počtu 1, 2/3/4 a 5 (podle poslední "
"číslice počtu).&][s0; &][s0; [* Příklad: ]count(62, `\"koz/a/y/`\") `= `\"62 "
"kozy`\", count(61, `\"koz/a/y/`\") `= `\"61 koza`\".&][s0; Funkce vrátí aktuální "
"systémové datum v_místním čase.&][s14;:Calc`$CftextN: [%00-00 ftext (reálné "
"číslo) ][%00-00S ®][%00-00  řetězec]&][s0; Funkce vrátí zadané [/ reálné "
"číslo] zaokrouhlené na nejbližší celé číslo a převedené na text.&][s0; "
"&][s0; [* Příklad: ]ftext(133.59) `= `\"134`\"&][s14;:Calc`$CftextNI: [%00-00 ftext "
"(reálné číslo, celé číslo) ][%00-00S ®][%00-00  řetězec]&][s0; Funkce převede "
"číslo na text se zadaným počtem desetinných míst. Pokud není počet m")
TOPIC_TEXT(
"íst zadán, desetinná místa se ignorují.&][s14;:Calc`$CgeSS: [%00-00 operátor "
">`=  (řetězec ][%00-00/ text1][%00-00 , řetězec ][%00-00/ text2][%00-00 ) ][%00-00S "
"®][%00-00  logická hodnota]&][s0; Funkce porovná zadané řetězce a vrátí logickou "
"hodnotu [/ pravda], pokud v lexikografickém uspořádání (ve slovníku) text [* "
"řetězec1] následuje po řetězci [* řetězci2,] nebo pokud jsou oba řetězce "
"shodné.&][s0; &][s0; [* Příklad: ]`\"ovoce`\" >`= `\"banán`\" `= 1&][s14;:Calc`$CgtSS: "
"[%00-00 operátor >  (řetězec ][%00-00/ text1][%00-00 , řetězec ][%00-00/ text2][%00-00 "
") ][%00-00S ®][%00-00  logická hodnota]&][s0; Funkce porovná zadané řetězce "
"a vrátí logickou hodnotu [/ pravda], pokud v lexikografickém uspořádání (ve "
"slovníku) text [* text1] následuje po řetězci [* text2].&][s0; &][s0; [* Příklad: "
"](`\"ovoce`\" > `\"banán`\") `= 1, (`\"ovoce`\" > `\"ovoce`\") `= 0&][s14;:Calc`$CitextNN: "
"[%00-00 itext(celé číslo ][%00-00/ hodnota][%00-00 , celé číslo ][%00-00/ šířka][%00-00 "
") ][%00-00S ®][%00-00  řetězec]&][s0; Funkce vrátí řetězec obsahující desítkový "
"zápis zadaného celého čísla [* hodnota]. Pokud počet znaků tohoto zápisu "
"je menší než hodnota parametru [* šířka], doplní se zápis čísla zleva mezerami "
"na šířku [* šířka].&][s0; &][s0; [* Poznámka: ]pokud je textový zápis čísla "
"delší než [* šířka], funkce vrátí celý řetězec (oříznutí na zadanou "
"šířku se neprovede). &][s0; &][s0; [* Příklad: ]itext(138, 5) `= `\"  138`\"&][s14;:Calc`$CleSS: "
"[%00-00 operátor <`=  (řetězec, řetězec) ][%00-00S ®][%00-00  logická hodnota]&][s0; "
"Funkce porovná zadané řetězce a vrátí logickou hodnotu [/ pravda], pokud v "
"lexikografickém uspořádání (ve slovníku) text [* řetězec1] předchází před "
"řetězcem [* řetězci2] nebo pokud jsou oba řetězce shodné.&][s0; &][s0; [* "
"Příklad: ]`\"banán`\" < `\"ovoce`\" `= 1&][s14;:Calc`$CleftSN: [%00-00 left (řetězec "
"][%00-")
TOPIC_TEXT(
"00/ text][%00-00 , celé číslo ][%00-00/ počet`_znaků][%00-00 ) ][%00-00S ®][%00-00 "
" řetězec]&][s0; Funkce vrátí zadaný [/ počet znaků] ze začátku zadaného "
"řetězce [/ text]. Pokud je počet znaků menší nebo roven 0, výsledkem funkce "
"je prázdný řetězec. Pokud je počet znaků větší než délka řetězce, funkce "
"vrátí zadaný řetězec beze změny.&][s0; &][s0; [* Příklad: ]left(`\"ahoj světe`\", "
"6) `= `\"ahoj s`\"&][s14;:Calc`$ClenS: [%00-00 len (řetězec) ][%00-00S ®][%00-00 "
" celé číslo]&][s0; Funkce vrátí délku (počet znaků) textu, který je jejím "
"parametrem. Pokud je text prázdný, výsledkem funkce je hodnota 0.&][s0; &][s0; "
"[* Příklad: ]len(`\"pokusný text`\") `= 12.&][s14;:Calc`$CllpadSN: [%00-00 llpad(řetězec "
"][%00-00/ text][%00-00 , celé číslo ][%00-00/ délka][%00-00 ) ][%00-00S ®][%00-00 "
" řetězec]&][s0; Funkce doplní řetězec [* text] zleva na počet znaků [* délka] "
"opakovaným předřazováním řetězce [* výplň]. Pokud je [* délka] menší "
"než délka původního řetězce [* text], funkce vrátí [* délka] znaků z konce "
"řetězce [* text].&][s0; &][s0; [* Příklad: ]lpad(`\"ahoj`\", 15, `\"svete`\") "
"`= `\"esvetesveteahoj`\"&][s0; &][s0; Funkce doplní řetězec [* text] zleva na "
"počet znaků [* délka] mezerami. Pokud je [* délka] menší než délka původního "
"řetězce [* text], funkce vrátí [* délka] znaků z konce řetězce [* text]..&][s0; "
"&][s0; [* Příklad: ]llpad(`\"ahoj svete`\", 8) `= `\"oj svete`\"&][s14;:Calc`$ClpadSN: "
"[%00-00 lpad(řetězec ][%00-00/ text][%00-00 , celé číslo ][%00-00/ délka][%00-00 "
") ][%00-00S ®][%00-00  řetězec]&][s0; Funkce doplní řetězec [* text] zleva "
"na počet znaků [* délka] mezerami. Pokud je [* délka] menší nebo rovna délce "
"původního řetězce [* text], funkce vrátí vstupní text beze změny.&][s0; &][s0; "
"[* Příklad: ]lpad(`\"ahoj`\", 8) `= `\"    ahoj`\"&][s0; &][s14;:Calc`$ClpadSNS: "
"[%00-00 lpad(řetězec ][%00-00/ text][%00-00 , celé č�")
TOPIC_TEXT(
"�slo ][%00-00/ délka][%00-00 , řetězec ][%00-00/ výplň][%00-00 ) ][%00-00S "
"®][%00-00  řetězec]&][s0; Funkce doplní řetězec [* text] zleva na počet znaků "
"[* délka] opakovaným předřazováním řetězce [* výplň]. Pokud je [* délka] "
"menší nebo rovna délce původního řetězce [* text] nebo pokud je řetězec "
"[* výplň] prázdný, funkce vrátí vstupní text beze změny.&][s0; &][s0; [* "
"Příklad: ]lpad(`\"ahoj`\", 15, `\"svete`\") `= `\"esvetesveteahoj`\"&][s14;:Calc`$CltSS: "
"[%00-00 operátor <  (řetězec, řetězec) ][%00-00S ®][%00-00  logická hodnota]&][s0; "
"Funkce porovná zadané řetězce a vrátí logickou hodnotu [/ pravda], pokud v "
"lexikografickém uspořádání (ve slovníku) text [* řetězec1] předchází před "
"řetězcem [* řetězci2].&][s0; &][s0; [* Příklad: ]`\"banán`\" < `\"ovoce`\" "
"`= 1&][s14;:Calc`$CmaxSS: [%00-00 max (řetězec, řetězec) ][%00-00S ®][%00-00 "
" řetězec]&][s0; Funkce vrátí lexikografické maximum ze dvou řetězců, tedy "
"ten z nich, který by byl ve slovníku blíže konci.&][s0; &][s0; [* Příklad: "
"]max(`\"rok`\", `\"rám`\") `= `\"rok`\".&][s14;:Calc`$CmidSN: [%00-00 mid (řetězec "
"][%00-00/ text][%00-00 , celé číslo ][%00-00/ začátek][%00-00 ) ][%00-00S ®][%00-00 "
" řetězec]&][s0; Funkce vrátí všechny znaky z řetězce [* text] počínaje indexem "
"[* začátek]. První znak řetězce má index 0.&][s0; &][s0; [* Příklad: ]mid(`\"Ahoj "
"světe!`\", 5) `= `\"světě!`\"&][s14;:Calc`$CmidSNN: [%00-00 mid (řetězec ][%00-00/ "
"text][%00-00 , celé číslo ][%00-00/ začátek][%00-00 , celé číslo ][%00-00/ "
"počet`_znaků][%00-00 ) ][%00-00S ®][%00-00  řetězec]&][s0; Funkce vrátí zadaný "
"[/ počet znaků] z_řetězce [/ text] od pozice [/ začátek]. První znak v_řetězci "
"má poziční číslo 0, druhý znak číslo 1 atd., poslední znak má pozici [* "
"len(][*/ text][*  `- 1)].&][s0; &][s0; [* Příklad: ]mid(`\"ahoj světe`\", 3, 3) "
"`= `\"j_s`\"&][s0; Funkce vrátí minimum z hodnot v daném poli. Pokud ")
TOPIC_TEXT(
"pole obsahuje řetězce, provede se jejich lexikografické srovnání.&][s0; &][s0; "
"[* Příklad: ]min(`[8, 5, 13, 2, 9`]) `= 2&][s14;:Calc`$CminSS: [%00-00 min (řetězec, "
"řetězec) ][%00-00S ®][%00-00  řetězec]&][s0; Funkce vrátí minimum z_dvou řetězců "
"v lexikografickém uspořádání, tedy ten z obou řetězců, který by ve slovníku "
"byl blíže začátku.&][s0; &][s0; [* Příklad: ]min(`\"pole`\", `\"Rudolf`\") "
"`= `\"pole`\"&][s14;:Calc`$CminmaxSSS: [%00-00 minmax (řetězec, řetězec, řetězec) "
"][%00-00S ®][%00-00  řetězec]&][s0; Funkce vrátí hodnotu [* min(max(x, xmin), "
"xmax)]. Porovnávání se ve všech případech provádí lexikograficky, tedy jako "
"ve slovníku. Pokud je některý z parametrů roven [* null], je výsledek také "
"[* null].&][s14;:Calc`$CmulNS: [%00-00 operátor `*  (celé číslo ][%00-00/ počet][%00-00 "
", řetězec ][%00-00/ text][%00-00 ) ][%00-00S ®][%00-00  řetězec]&][s0; Opakování "
"textu. Výsledkem součinu řetězce a_celého čísla je řetězec, který vznikne "
"opakováním původního řetězce [/ text] tolikrát, kolik udává [/ počet]. "
"Pokud je [/ počet] <`= 0, výsledkem operace je prázdný řetězec.&][s0; &][s0; "
"[* Příklad: ]4 `* `\"znak`\" `= `\"znakznakznakznak`\".&][s14;:Calc`$CmulSN: [%00-00 "
"operátor `*  (řetězec ][%00-00/ text][%00-00 , celé číslo ][%00-00/ počet][%00-00 "
") ][%00-00S ®][%00-00  řetězec]&][s0; Opakování textu. Výsledkem součinu řetězce "
"a_celého čísla je řetězec, který vznikne opakováním původního řetězce "
"[* text] tolikrát, kolik udává [* počet]. Pokud je [* počet <`= 0], výsledkem "
"operace je prázdný řetězec.&][s0; &][s0; [* Příklad: ]`\"znak`\" `* 5 `= `\"znakznakznakznakznak`\".&][s14;:Calc`$CnlsN: "
"[%00-00 nls (reálné číslo ][%00-00/ číslo][%00-00 ) ][%00-00S ®][%00-00  řetězec]&][s0; "
"Funkce vrátí řetězec s_desítkovým zápisem zadaného celého čísla s_dodržením "
"národních zvyklostí&][s14;:Calc`$CnlsNN: [%00-00 nls (reálné číslo")
TOPIC_TEXT(
" ][%00-00/ číslo][%00-00 , celé číslo ][%00-00/ des`_míst][%00-00 ) ][%00-00S "
"®][%00-00  řetězec]&][s0; Převod číselné hodnoty na text s využitím národních "
"zvyklostí pro formátování čísel (oddělovače tisíců, oddělovač desetinné "
"části). Pokud není zadán počet desetinných míst, dosadí se běžná hodnota "
"3. Pokud je počet desetinných míst kladný, z desetinné části čísla se odstraní "
"nevýznamné nuly; pokud je počet míst záporný, nevýznamné nuly se v čísle "
"ponechají.&][s0; &][s0; [* Příklad: ]nls(1272.3, `-2) `= `\"1 272,30`\"&][s14;:Calc`$CnlsrN: "
"[%00-00 nlsr(reálné číslo ][%00-00/ číslo][%00-00 ) ][%00-00S ®][%00-00  řetězec]&][s0; "
"Převod číselné hodnoty na text s využitím národních zvyklostí pro formátování "
"čísel (oddělovače tisíců, oddělovač desetinné části). Číslo se zaokrouhlí "
"na tři platné číslice a nevýznamné nuly se ignorují.&][s0; &][s0; [* Příklad: "
"]nlsr(1272.3) `= `\"1 272`\"&][s14;:Calc`$CnlsrNN: [%00-00 nlsr(reálné číslo "
"][%00-00/ číslo][%00-00 , celé číslo ][%00-00/ platných`_číslic][%00-00 ) "
"][%00-00S ®][%00-00  řetězec]&][s0; Převod číselné hodnoty na text s využitím "
"národních zvyklostí pro formátování čísel (oddělovače tisíců, oddělovač "
"desetinné části) na zadaný počet [/ platných číslic]. Pokud je počet platných "
"číslic kladný, z_desetinné části čísla se odstraní nevýznamné nuly; pokud "
"je počet míst záporný, nevýznamné nuly se v_čísle ponechají.&][s0; &][s0; "
"[* Příklad: ]nlsr(1272.3, `-3) `= `\"1 270`\", nlsr(1.2023, `-3) `= `\"1,20`\"&][s14;:Calc`$CnumberS: "
"[%00-00 number (řetězec) ][%00-00S ®][%00-00  reálné číslo]&][s0; Funkce načte "
"ze zadaného řetězce desítkový zápis reálného čísla a toto číslo vrátí "
"jako svůj výsledek. Pokud řetězec neobsahuje zápis čísla, funkce ohlásí "
"chybu při výpočtu.&][s0; &][s0; [* Příklad: ]number(`\"10.8`\") `= 10.8&][s0;* "
"&][s14")
TOPIC_TEXT(
";:Calc`$CnumberSN: [%00-00 number (řetězec ][%00-00/ text][%00-00 , reálné číslo "
"][%00-00/ jinak][%00-00 ) ][%00-00S ®][%00-00  reálné číslo]&][s0; Funkce převede "
"textovou reprezentaci reálného čísla na hodnotu tohoto čísla. Varianta funkce "
"s jedním parametrem v případě chybného zápisu čísla ohlásí při vyhodnocení "
"chybu; funkce s dvěma parametry vrátí v případě chyby nebo prázdného řetězce "
"číselnou hodnotu [/ jinak], zadanou jako druhý parametr.&][s0; &][s0; [* Příklad]: "
"number(`\"1.2e3`\") `= 1200&][s14;:Calc`$CreplaceSSS: [%00-00 replace(řetězec ][%00-00/ "
"text][%00-00 , řetězec ][%00-00/ vyhledat][%00-00 , řetězec ][%00-00/ nahradit][%00-00 "
") ][%00-00S ®][%00-00  řetězec]&][s0; Funkce vyhledá v_řetězci [/ text] výskyty "
"textu [/ vyhledat] a_nahradí je textem [/ nahradit]. Pokud je řetězec [/ nahradit] "
"prázdný, funkce z_řetězce [/ text] odstraní výskyty textu [/ vyhledat].&][s0; "
"&][s0;~~~3200; [* Příklad:] replace(`\"člověk člověku vlkem`\", `\"člověk`\", "
"`\"vlk`\") `= `\"vlk vlku vlkem`\"&][s14;:Calc`$CrightSN: [%00-00 right (řetězec "
"][%00-00/ text][%00-00 , celé číslo ][%00-00/ počet`_znaků][%00-00 ) ][%00-00S "
"®][%00-00  řetězec]&][s0; Funkce vrátí zadaný [/ počet znaků] z_konce řetězce "
"[/ text]. Pokud je počet znaků menší nebo roven 0, výsledkem funkce je prázdný "
"řetězec. Pokud je počet znaků větší než délka řetězce, funkce vrátí "
"zadaný řetězec beze změny.&][s0; &][s0; [* Příklad]: right(`\"ahoj světe`\", "
"7) `= `\"j_světe`\"&][s14;:Calc`$CrpadSNS: [%00-00 rpad(řetězec ][%00-00/ text][%00-00 "
", celé číslo ][%00-00/ délka][%00-00 , řetězec ][%00-00/ výplň][%00-00 ) "
"][%00-00S ®][%00-00  řetězec]&][s0; Funkce doplní řetězec [* text] zprava na "
"počet znaků [* délka] opakovaným přidáváním řetězce [* výplň]. Pokud "
"je [* délka] menší nebo rovna délce původního řetězce [* text] nebo pokud "
"je řetězec [* výplň] prázdný, funkce vr�")
TOPIC_TEXT(
"�tí vstupní text beze změny.&][s0; &][s0; [* Příklad: ]rpad(`\"ahoj`\", 15, "
"`\"svete`\") `= `\"ahojsvetesvetes`\"&][s0;* &][s14;:Calc`$CrrpadSN: [%00-00 rrpad(řetězec "
"][%00-00/ text][%00-00 , celé číslo ][%00-00/ délka][%00-00 ) ][%00-00S ®][%00-00 "
" řetězec]&][s0; Funkce doplní řetězec [* text] mezerami zprava na počet znaků "
"[* délka]. Pokud je [* délka] menší než délka původního řetězce [* text], "
"funkce vrátí pouze [* délka] prvních znaků řetězce [* text].&][s0; &][s0; "
"[* Příklad: ]rpad(`\"ahoj světe`\", 6) `= `\"ahoj s`\"&][s0; &][s14;:Calc`$CrrpadSNS: "
"[%00-00 rrpad(řetězec ][%00-00/ text][%00-00 , celé číslo ][%00-00/ délka][%00-00 "
", řetězec ][%00-00/ výplň][%00-00 ) ][%00-00S ®][%00-00  řetězec]&][s0; Funkce "
"doplní řetězec [* text] zprava na počet znaků [* délka] opakovaným přidáváním "
"řetězce [* výplň]. Pokud je [* délka] menší nebo rovna délce původního "
"řetězce [* text], funkce vrátí pouze [* délka] prvních znaků řetězce [* "
"text].&][s0; &][s0; [* Příklad:] rpad(`\"ahoj`\", 15, `\"svete`\") `= `\"ahojsvetesvetes`\"&][s14;:Calc`$CstringIndex: "
"[%00-00 operátor `[`]  (řetězec, celé číslo) ][%00-00S ®][%00-00  řetězec]&][s0; "
"Operátor vrátí řetězec obsahující znak v_řetězci na dané pozici. Znaky "
"v řetězci jsou číslovány od 0, takže první znak má index 0 a poslední (délka "
"řetězce `- 1).&][s0; &][s0; [* Příklad: ]`\"ahoj`\"`[2`] `= `\"o`\"&][s14;:Calc`$CtextD: "
"[%00-00 text (datum/čas) ][%00-00S ®][%00-00  řetězec]&][s0; Převod hodnoty "
"na text. Při převodu čísla na text je výsledkem textová reprezentace tohoto "
"čísla, pořízená na 10 platných číslic. Pokud je parametrem text, funkce ponechá "
"tento text beze změny. Při převodu data na text se do řetězce datum uloží "
"v běžném tvaru [* den.měsíc.rok].&][s0; &][s0; [* Příklad]: text(1 / 3) `= "
"`\"0.3333333333`\", text(`\"ahoj`\") `= `\"ahoj`\", text(26.7.1971) `= `\"26.07.1971`\"&][s14;:Calc`$CtextN: "
"[%00-00")
TOPIC_TEXT(
" text (reálné číslo) ][%00-00S ®][%00-00  řetězec]&][s0; Funkce vrátí řetězec "
"obsahující textový zápis zadaného reálného čísla.&][s0; &][s0; [* Příklad: "
"]text(`-370.05) `= `\"`-370.05`\"&][s14;:Calc`$CtextS: [%00-00 text (řetězec) ][%00-00S "
"®][%00-00  řetězec]&][s0; Funkce vrátí jako výsledek svůj parametr beze změny.&][s0; "
"&][s0; [* Příklad: ]text(`\"Ahoj světe!`\") `= `\"Ahoj světe!`\"&][s0; &][s0; "
"&][s0; &][s0; &][s0; ]")
