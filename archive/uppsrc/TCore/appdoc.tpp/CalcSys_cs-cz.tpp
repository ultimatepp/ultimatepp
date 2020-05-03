TITLE("Systémové operace")
REF("Calc$Cis_nullV")
REF("Calc$Cis_numberV")
REF("Calc$Cis_textV")
REF("Calc$Cnull0")
REF("Calc$CnvlVV")
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
"[s14; [%00-00 is`_date (libovolný typ) ][%00-00S ®][%00-00  celé číslo]&][s0; "
"Funkce vrátí logickou hodnotu 1 ([* pravda]), když zadaný parametr je typu [* "
"datum]. Pokud je parametr jiného typu nebo obsahuje prázdnou hodnotu, funkce vrátí "
"hodnotu 0 ([* nepravda]).&][s0; &][s0; [* Příklad: ]is`_date(10.5) `= 0, is`_date(23.5.1997) "
"`= 1, is`_date(`\"ahoj`\") `= 0&][s14;:Calc`$Cis`_nullV: [%00-00 is`_null (libovolný "
"typ) ][%00-00S ®][%00-00  celé číslo]&][s0; Funkce vrátí logickou hodnotu 1 ")
TOPIC_TEXT(
"([* pravda]), když její parametr je typu [* null]jinak vrátí logickou hodnotu "
"0 ([* nepravda]).&][s0; &][s0; [* Příklad: ]is`_null(10.5) `= 0, is`_null(null) "
"`= 1&][s14;:Calc`$Cis`_numberV: [%00-00 is`_number (libovolný typ) ][%00-00S ®][%00-00 "
" celé číslo]&][s0; Funkce otestuje, zda je zadaný parametr typu [* číslo] (celé "
"nebo reálné číslo, případně výsledek některé z_logických operací). Pokud "
"parametr je typu [* číslo], funkce vrátí logickou hodnotu 1 ([* pravda]).&][s0; "
"&][s0; [* Příklad: ]is`_number(10.5) `= 1, is`_number(23.5.1997) `= 0, is`_number(`\"ahoj`\") "
"`= 0&][s14;:Calc`$Cis`_textV: [%00-00 is`_text (libovolný typ) ][%00-00S ®][%00-00 "
" celé číslo]&][s0; Funkce vrátí logickou hodnotu 1 ([* pravda]), když zadaný "
"parametr je typu [* řetězec]. Pokud je parametr jiného typu nebo obsahuje prázdnou "
"hodnotu, funkce vrátí hodnotu 0 ([* nepravda]).&][s0; &][s0; [* Příklad: ]is`_text(10.5) "
"`= 0, is`_text(23.5.1997) `= 0, is`_text(`\"ahoj`\") `= 1&][s0; Funkce vrátí řetězec "
"obsahující desítkový zápis zadaného celého čísla [* hodnota]. Pokud počet "
"znaků tohoto zápisu je menší než hodnota parametru [* šířka], doplní se "
"zápis čísla zleva číslicemi 0 (nula) na šířku [* šířka].&][s0; &][s0; "
"[* Poznámka: ]pokud je textový zápis čísla delší než [* šířka], funkce "
"vrátí celý řetězec (oříznutí na zadanou šířku se neprovede). &][s0; &][s0; "
"[* Příklad: ]itext0(138, 5) `= `\"00138`\"&][s0; Funkce vrátí řetězec obsahující "
"desítkový zápis zadaného celého čísla. Pokud je parametr [* null], funkce "
"vrátí prázdný řetězec.&][s0; &][s0; [* Příklad: ]itext(138) `= `\"138`\"&][s14;:Calc`$Cnull0: "
"[%00-00 null ][%00-00S ®][%00-00  prázdná hodnota]&][s0; Prázdná hodnota. Při "
"výpočtech operací a_volání funkcí se prázdná hodnota [/ propaguje] z_podvýrazu: "
"pokud libovolný z_operandů nebo libovolný z_parametrů funkce obsahuje prázdnou "
"hodnotu, výsledkem op")
TOPIC_TEXT(
"erace nebo funkce je opět prázdná hodnota. Výjimkou z_tohoto pravidla jsou speciální "
"operátory [* ?:], [* `|`|], [* `&`&], [* `=`=], [* !`=] a_dále funkce [* is_number], "
"[* is_text], [* is_date] a_[* is_null], které prázdné hodnoty zpracovávají speciálním "
"způsobem.&][s0; &][s0; [* Příklad]: 10 `+ null `= null&][s14;:Calc`$CnvlVV: [%00-00 "
"nvl (libovolný typ, libovolný typ) ][%00-00S ®][%00-00  libovolný typ]&][s0; "
"Funkce vrátí hodnotu svého prvního parametru, tato hodnota není prázdná ([* "
"null]). Pokud první parametr obsahuje prázdnou hodnotu, funkce vrátí hodnotu "
"druhého parametru.&][s0; &][s0; [* Příklad]: nvl(`\"prvni`\", `\"druhy`\") `= "
"`\"prvni`\", nvl(`\"`\", `\"druhy`\") `= `\"druhy`\"&][s0; Ludolfovo číslo [S p] "
"určuje polovinu délky obvodu jednotkové kružnice ([* l_`= 2][*S p][* r]).&][s0; "
"&][s0; [* Příklad: ]cos(60 `* (pi / 180)) `= 0.5&][s0; &][s0; ]")
