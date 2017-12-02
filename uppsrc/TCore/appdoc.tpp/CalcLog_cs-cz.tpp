TITLE("Logické operace")
REF("Calc$CbitAndNN")
REF("Calc$CbitNotN")
REF("Calc$CbitOrNN")
REF("Calc$CbitXorNN")
REF("Calc$CeqVV")
REF("Calc$ClogNotN")
REF("Calc$ClogNotS")
REF("Calc$CneVV")
REF("Calc$CsalNN")
REF("Calc$CsarNN")
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
"[s14;:Calc`$CbitAndNN: [%00-00 operátor `&  (celé číslo, celé číslo) ][%00-00S "
"®][%00-00  celé číslo]&][s0; Bitový součin. Výsledkem operace je celé číslo, "
"jehož dvojková reprezentace obsahuje jedničky na těch pozicích, na kterých "
"jsou ve dvojkové reprezentaci obou argumentů jedničky.&][s0; &][s0; [* Příklad: "
"]11 `& 13 `= 9 (1011 `& 1101 `= 1001).&][s14;:Calc`$CbitNotN: [%00-00 operátor `~ "
" (celé číslo) ][%00-00S ®][%00-00  celé číslo]&][s0; Bitová negace. Výsledkem "
"oper")
TOPIC_TEXT(
"ace je celé číslo, jehož dvojková reprezentace obsahuje jedničky právě na "
"těch pozicích, kde jsou ve dvojkové reprezentaci argumentu nuly a_naopak. Vzhledem "
"k_tomu, že výpočty s_celými čísly probíhají v_doplňkovém kódu, platí: "
"[* %01`~x_`= `-1 `- x%01].&][s0; &][s0; [* Příklad: ]`~14 `= `-15.&][s14;:Calc`$CbitOrNN: "
"[%00-00 operátor `|  (celé číslo, celé číslo) ][%00-00S ®][%00-00  celé "
"číslo]&][s0; Bitový součet. Výsledkem operace je celé číslo, jehož dvojková "
"reprezentace obsahuje jedničky na těch pozicích, na kterých je ve dvojkové reprezentaci "
"alespoň jednoho z_argumentů jednička.&][s0; &][s0; [* Příklad: ]1 `| 4 `= 5 "
"(001 `| 100 `= 101).&][s14;:Calc`$CbitXorNN: [%00-00 operátor `^  (celé číslo, "
"celé číslo) ][%00-00S ®][%00-00  celé číslo]&][s0; Bitový symetrický rozdíl "
"([* xor]). Výsledkem operace je celé číslo, jehož dvojková reprezentace obsahuje "
"jedničky na těch pozicích, na kterých se ve dvojkové reprezentaci obou argumentů "
"číslice liší.&][s0; &][s0; [* Příklad: ]9 `^ 15 `= 6 (1001 `^ 1111 `= 0110).&][s0; "
"Funkce načte proměnnou výrazového systému nebo zavolá funkci zadanou textovým "
"identifikátorem. V_případě funkce je možno jako další parametry zadat parametry "
"funkce.&][s0; &][s0; [* Příklad: ]call(`\"mid`\", `\"ahoj`\", 2, 2) `= `\"oj`\"&][s14;:Calc`$CeqVV: "
"[%00-00 operátor `=`=  (libovolný typ, libovolný typ) ][%00-00S ®][%00-00  logická "
"hodnota]&][s0; Operátor porovná dvě libovolné hodnoty. Výsledkem je [/ pravda] "
"(1), pokud se hodnoty rovnají, jinak [/ nepravda].&][s0; &][s0; [* Upozornění:] "
"při porovnávání se nejprve testují datové typy porovnávaných objektů. Pokud "
"se tyto typy neshodují, funkce vrátí hodnotu [/ nepravda]. Například test `\"10`\" "
"`=`= 10 vrátí logickou hodnotu nepravda z_důvodu neshody typů, přestože řetězec "
"na levé straně obsahuje desítkový zápis čísla na pravé straně operátoru.&][s14;:Calc`$ClogNot")
TOPIC_TEXT(
"N: [%00-00 operátor !  (reálné číslo) ][%00-00S ®][%00-00  logická hodnota]&][s0; "
"Logická negace. Výsledkem operace je logická hodnota 1 ([* pravda]), pokud argument "
"je nulový (jedná se o_nulové číslo nebo o_logickou hodnotu [* nepravda]). Pokud "
"je argument nenulový nebo obsahuje logickou hodnotu [* pravda], výsledkem logické "
"negace je hodnota 0 ([* nepravda]). Pokud je parametrem operátoru řetězec, je "
"výsledkem hodnota 1 ([* pravda]), když tento řetězec je prázdný.&][s0; &][s0; "
"[* Příklad: ]!10 `= 0, !0 `= 1, !`\"ahoj`\" `= 0&][s14;:Calc`$ClogNotS: [%00-00 "
"operátor !  (řetězec) ][%00-00S ®][%00-00  logická hodnota]&][s0; Logická negace. "
"Výsledkem operace je logická hodnota 1 ([* pravda]), pokud argumentem je prázdný "
"řetězec nebo hodnota [* null]. V opačném případě je výsledkem operátoru "
"[/ nepravda].&][s0; &][s0; [* Příklad: ]!`\"ahoj`\" `= 0, !`\"`\" `= 1&][s14;:Calc`$CneVV: "
"[%00-00 operátor !`=  (libovolný typ, libovolný typ) ][%00-00S ®][%00-00  logická "
"hodnota]&][s0; Rovnost / nerovnost. Operátor [* `=`=] (rovnost) vrátí logickou "
"hodnotu 1 ([* pravda]), pokud operandy na obou stranách jsou stejného typu a_jsou "
"shodné. Operátor [* !`=] (nerovnost) vrací vždy opačnou logickou hodnotu než "
"operátor [* `=`=].&][s0; &][s0; [* Upozornění]: z_této definice rovnosti vyplývá, "
"že pokud například na jedné straně porovnání stojí text [* `\"300`\"] a_na "
"druhé straně číslo [* 300], operátor [* `=`=] vrátí logickou hodnotu [* nepravda], "
"přestože text na levé straně je shodný s_textovou reprezentací čísla na pravé "
"straně. Pokud chcete porovnávat hodnoty různých typů, převeďte je nejprve "
"na společný typ, například pomocí funkce [*^dpp`:`/`/AppDoc`/`/`:`:`/`/Calc`$CtextN^ "
"text].&][s14;:Calc`$CsalNN: [%00-00 operátor <<  (celé číslo, celé číslo ][%00-00/ "
"počet`_bitů][%00-00 ) ][%00-00S ®][%00-00  celé číslo]&][s0; Aritmetický posuv "
"vlevo. Výsledkem operace je celé č")
TOPIC_TEXT(
"íslo, jehož dvojková reprezentace vznikne posuvem dvojkové reprezentace čísla "
"na levé straně operátoru vlevo (směrem k_vyšším bitům) o_počet bitů určený "
"operandem na pravé straně. Vzhledem k_tomu, že celočíselné výpočty probíhají "
"v_doplňkovém kódu, je [* a_<< b_`= a_`* 2 `*`* b].&][s0; &][s0; [* Příklad]: "
"3 << 2 `= 12 (11 << 2 `= 1100).&][s14;:Calc`$CsarNN: [%00-00 operátor >>  (celé "
"číslo, celé číslo ][%00-00/ počet`_bitů][%00-00 ) ][%00-00S ®][%00-00  celé "
"číslo]&][s0; Aritmetický posuv vpravo. Výsledkem operace je celé číslo, jehož "
"dvojková reprezentace vznikne posuvem dvojkové reprezentace čísla na levé straně "
"operátoru vpravo (směrem k_nižším bitům) o_počet bitů určený operandem "
"na pravé straně. Nejvyšší (znaménkový) bit čísla se při posuvu kopíruje "
"do nižších bitů, nejnižší bity čísla se ztratí. Vzhledem k_tomu, že celočíselné "
"výpočty probíhají v_doplňkovém kódu, odpovídá aritmetický posuv o_bit vpravo "
"dělení dvěma se zaokrouhlováním směrem dolů.&][s0; &][s0; [* Příklad]: 13 "
">> 2 `= 3, `-14 >> 3 `= `-2.&][s0; Funkce vrátí počet sekund ze zadaného údaje "
"typu datum/čas.&][s0; &][s0; &][s0; &][s0; &][s0; ]")
