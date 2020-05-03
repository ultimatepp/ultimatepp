TITLE("Array processing functions")
REF("Calc$CaddAA")
REF("Calc$CarrayIndex")
REF("Calc$CcountA")
REF("Calc$CleftAN")
REF("Calc$CmidAN")
REF("Calc$CmidANN")
REF("Calc$CrightAN")
REF("Calc$CsumA")
REF("Calc$CsumASC")
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
"[s14;:Calc`$CaddAA: operator `+  (pole hodnot, pole hodnot) [S ®] pole hodnot&][s0; "
"&][s14;:Calc`$CarrayIndex: operator `[`]  (pole hodnot, integer) [S ®] any data "
"type&][s0; &][s0; [* Example]: mesic(27.5.1984) `= 5&][s0; The binary operator [* "
"`*] evaluates the [/ lambda`-expression] for each element of the original array and "
"returns an array composed of the results. The output array has always the same number "
"of elements as the input array. During evaluation of the lambda`-expression for each "
"input arra")
TOPIC_TEXT(
"y element the calculator variable [* I] is set to the index of the current array "
"element (first array element has index 0) and the variable [* A] contains the value "
"of the array element being processed.&][s14;:Calc`$CcountA: count (pole hodnot) [S "
"®] integer&][s14;:Calc`$CleftAN: [%00-00 left (pole hodnot, integer) ][%00-00S ®][%00-00 "
" pole hodnot]&][s0; &][s14;:Calc`$CmidAN: [%00-00 mid (pole hodnot, integer) ][%00-00S "
"®][%00-00  pole hodnot]&][s0; &][s14;:Calc`$CmidANN: [%00-00 mid (pole hodnot, integer, "
"integer) ][%00-00S ®][%00-00  pole hodnot]&][s0; &][s14;:Calc`$CrightAN: [%00-00 "
"right (pole hodnot, integer) ][%00-00S ®][%00-00  pole hodnot]&][s0; &][s14;:Calc`$CsumA: "
"sum (pole hodnot) [S ®] any data type&][s0; &][s14;:Calc`$CsumASC: [%00-00 sum(pole "
"hodnot, string ][%00-00/ identifier][%00-00 , lambda`-výraz) ][%00-00S ®][%00-00 "
" any data type]&][s0; Function adds / concatenates values calculated from the members "
"of its input array using a given [/ lambda`-expression]. [/ Lambda`-expressions] "
"may use the given [/ identifier] to access the current array element. The calculated "
"values must be of one of the supported types:&][s0; &][s0;i150;O0;+-83 [* numbers] "
"`- function returns the arithmetic sum of all such numbers; elements containing the "
"[* null] value are ignored&][s0;i150;O0;+-83 [* strings] `- function returns the "
"string concatenation of all string values&][s0;i150;O0;+-83 [* arrays] `- function "
"returns the array concatenation of all arrays&][s0;*+-83 &][s0;+-83 [*2 Example:][2 "
" ]`[1, 4, 7`].sum(`\"X`\", #(X`*X)) `= 66, `[1, 4, 7`].sum(`\"T`\", #(T.text)) `= "
"`\"147`\"&][s0; Function converts an array to its standard textual representation "
"in the format&][s0; &][s0; `[[/ first element, second element, third element...]`]&][s0; "
"&][s0; [* Example:]  `[0.0, 1.0, 2.5`].text `= `\"`[0, 1, 2.5`]`\"&][s0; &][s0; ]")
