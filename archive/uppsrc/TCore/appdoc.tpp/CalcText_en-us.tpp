TITLE("Text processing functions")
REF("Calc$CaddSS")
REF("Calc$CascS")
REF("Calc$CcatSSS")
REF("Calc$CchrN")
REF("Calc$CcountNS")
REF("Calc$CftextN")
REF("Calc$CftextNI")
REF("Calc$CgeSS")
REF("Calc$CgtSS")
REF("Calc$CleSS")
REF("Calc$CleftSN")
REF("Calc$ClenS")
REF("Calc$ClogNotS")
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
REF("Calc$CnumberS")
REF("Calc$CnumberSN")
REF("Calc$CrightSN")
REF("Calc$CstringIndex")
REF("Calc$CtextS")
REF("Calc$CtextN")
REF("Calc$CtextD")
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
"[s14;:Calc`$CaddSS: operator `+  (string, string) [S ®] string&][s0; &][s14;:Calc`$CascS: "
"[%00-00 asc (string) ][%00-00S ®][%00-00  integer]&][s0; The function will return "
"ASCII`-code of the first character of its string argument. If the argument is empty, "
"the function will return 0.&][s0; &][s0; [* Example]: asc(`\"hello`\") `= 104 [/ "
"(ASCII code of the character `'h`')].&][s14;:Calc`$CcatSSS: [%00-00 cat (string ][%00-00/ "
"text`_1][%00-00 , string ][%00-00/ join][%00-00 , string ][%00-00/ text`_2][%00-00 "
") ")
TOPIC_TEXT(
"][%00-00S ®][%00-00  string]&][s0; The function concatenates its arguments [/ text`_1] "
"and [/ text`_2]. If neither string is empty, the string [/ join] is inserted between "
"them.&][s0; &][s0; [* Example]: cat(`\"hello`\", `\" `\", `\"world`\") `= `\"hello "
"world`\", cat(`\"`\", `\" `\", `\"world`\") `= `\"world`\"&][s14;:Calc`$CchrN: [%00-00 "
"chr (integer ][%00-00/ ascii][%00-00 ) ][%00-00S ®][%00-00  string]&][s0; The function "
"will return one`-character string containing the character with the ASCII`-code ascii.&][s0; "
"&][s0; [* Example]: chr(66) `= `\"B`\".&][s14;:Calc`$CcountNS: [%00-00 count (integer, "
"string) ][%00-00S ®][%00-00  string]&][s0; The function returns text representation "
"of decimal notation of its numeric value together with the attached noun. In the "
"noun string, the forward slash can separate the suffixes of singular and plural used "
"in English to distinguish between a single object and multiple objects.&][s0; &][s0; "
"[* Example]: count(1, `\"m/ouse/ice`\") `= `\"1 mouse`\", count(2, `\"m/ouse/ice`\") "
"`= `\"2 mice`\".&][s14;:Calc`$CftextN: ftext (real number) [S ®] string&][s0; &][s14;:Calc`$CftextNI: "
"ftext (real number, integer) [S ®] string&][s0; Funkce převede číslo na text "
"se zadaným počtem desetinných míst. Pokud není počet míst zadán, desetinná "
"místa se ignorují.&][s14;:Calc`$CgeSS: operator >`=  (string, string) [S ®] logical "
"value&][s0; &][s14;:Calc`$CgtSS: operator >  (string, string) [S ®] logical value&][s0; "
"&][s14;:Calc`$CleSS: operator <`=  (string, string) [S ®] logical value&][s0; &][s14;:Calc`$CleftSN: "
"[%00-00 left (string ][%00-00/ text][%00-00 , integer ][%00-00/ character`_count][%00-00 "
") ][%00-00S ®][%00-00  string]&][s0; The function returns first [/ character`_count] "
"characters of its argument [/ text]. If the number of characters is less or equal "
"to 0, the function returns an empty string. If the character count is greater than "
"the string length, the function will return the original argument unchanged.&][s0; "
"&][s0; [* Example]: left(")
TOPIC_TEXT(
"`\"hello world`\", 7) `= `\"hello w`\"&][s14;:Calc`$ClenS: [%00-00 len (string) "
"][%00-00S ®][%00-00  integer]&][s0; The function will return the length (number "
"of characters) of its string argument. If the string is empty the function returns "
"0.&][s0; &][s0; [* Example]: len(`\"simple text`\") `= 11.&][s14;:Calc`$ClogNotS: "
"operator !  (string) [S ®] logical value&][s14;:Calc`$CltSS: operator <  (string, "
"string) [S ®] logical value&][s0; &][s14;:Calc`$CmaxSS: max (string, string) [S "
"®] string&][s0; &][s14;:Calc`$CmidSN: [%00-00 mid (string ][%00-00/ text][%00-00 "
", integer ][%00-00/ beginning][%00-00 ) ][%00-00S ®][%00-00  string]&][s0; &][s14;:Calc`$CmidSNN: "
"[%00-00 mid (string ][%00-00/ text][%00-00 , integer ][%00-00/ beginning][%00-00 "
", integer ][%00-00/ character`_count][%00-00 ) ][%00-00S ®][%00-00  string]&][s0; "
"The function returns [/ character`_count] characters from the string [/ text] starting "
"at position [/ beginning]. The first character in the string has the position number "
"0, the second character has the number 1 etc., the last character has the position "
"[* len(][*/ text][*  `- 1)]. If the character count is omitted the function returns "
"all characters from the position [/ beginning] until the end of the string.&][s0; "
"&][s0; [* Example]: mid(`\"hello world`\", 4, 3) `= `\"o w`\"&][s14;:Calc`$CminSS: "
"min (string, string) [S ®] string&][s0; &][s14;:Calc`$CminmaxSSS: minmax (string, "
"string, string) [S ®] string&][s0; &][s14;:Calc`$CmulNS: [%00-00 operator `*  (integer "
"][%00-00/ count][%00-00 , string ][%00-00/ text][%00-00 ) ][%00-00S ®][%00-00  string]&][s0; "
"Text repetition. The result of a multiplying a [* text] value by an integer value "
"[* count] is a string containing the original [* text] repeated [* count] times. "
"When [* count] <`= 0, the result is the empty string.&][s0; &][s0; [* Example]: `\"text`\" "
"`* 5 `= `\"texttexttexttexttext`\".&][s14;:Calc`$CmulSN: [%00-00 operator `*  (string "
"][%00-00/ text][%00-00 , integer ][%00-00/ count][%00-00 ) ][%00-00S �")
TOPIC_TEXT(
"�][%00-00  string]&][s0; &][s14;:Calc`$CnlsN: [%00-00 nls (real number ][%00-00/ "
"value][%00-00 ) ][%00-00S ®][%00-00  string]&][s0; &][s14;:Calc`$CnlsNN: [%00-00 "
"nls (real number ][%00-00/ value][%00-00 , integer ][%00-00/ decimals][%00-00 ) ][%00-00S "
"®][%00-00  string]&][s0; Converts its numeric value to text according to current "
"regional settings (delimiters of thousands, delimiter of decimal part). If number "
"of decimals is omitted, the default value 3 is used. If the number of decimals is "
"positive, non`-significant zeroes will be deleted from the decimal part of the number. "
"If the number of decimal positions is negative, non`-significant zeroes will be retained "
"in the number. &][s0; &][s0; [* Example]: nls(1272.3, `-2) `= `\"1 272,30`\"&][s0; "
"Converts its numeric value to text according to current regional settings (delimiters "
"of thousands, delimiter of decimal part). The number is rounded to 3 significant "
"digits and insignificant zeroes are ignored.&][s0; &][s0;* Example: nlsr(1272.3) "
"`= `\"1 272.3`\"&][s0; Converts its numeric value to text according to current regional "
"settings (delimiters of thousands, delimiter of decimal part) with given number of "
"[/ significant digits]. If the number of digits is positive, non`-significant zeroes "
"will be deleted from the decimal part of the number. If the number of digits is negative, "
"non`-significant zeroes will be retained in the number. &][s0; &][s0;* Example: nlsr(1272.3, "
"`-2) `= `\"1 300`\", nlsr(1.999, `-2) `= `\"2.00`\"&][s14;:Calc`$CnumberS: [%00-00 "
"number (string) ][%00-00S ®][%00-00  real number]&][s0; &][s14;:Calc`$CnumberSN: "
"[%00-00 number (string ][%00-00/ text][%00-00 , real number ][%00-00/ default][%00-00 "
") ][%00-00S ®][%00-00  real number]&][s0; The function converts text representation "
"of a real numeric constant to the value of this number. If the text doesn`'t contain "
"a valid text representation of a real number, the form with one parameter aborts "
"the computation with the error [/ `'invalid numeric format`']")
TOPIC_TEXT(
" whereas the form with two arguments merely returns its second argument [/ default].&][s0; "
"&][s0; [* Example]: number(`\"1.2e3`\") `= 1200&][s14;:Calc`$CrightSN: [%00-00 right "
"(string ][%00-00/ text][%00-00 , integer ][%00-00/ character`_count][%00-00 ) ][%00-00S "
"®][%00-00  string]&][s0; The function returns last [/ character`_count] characters "
"of its string argument [/ text]. If the number of characters is less or equal to "
"0, the function returns an empty string. If the character count is greater than the "
"string length, the function returns its [/ text] argument unchanged.&][s0; &][s0; "
"[* Example]: right(`\"hello world`\", 7) `= `\"o world`\"&][s14;:Calc`$CstringIndex: "
"operator `[`]  (string, integer) [S ®] string&][s0; &][s14;:Calc`$CtextS: text (string) "
"[S ®] string&][s0; &][s0; &][s14;:Calc`$CtextN: text (real number) [S ®] string&][s0; "
"&][s0; &][s14;:Calc`$CtextD: [%00-00 text (date/time) ][%00-00S ®][%00-00  string]&][s0; "
"Value`-to`-text conversion. If a number is converted into a text, its result is the "
"text representation of the number with 10 significant digits. If the parameter is "
"a text, the text will be left unchanged by the function. In case of date`-to`-text "
"conversion, the date will be saved in the string in the usual form day.month.year.&][s0; "
"&][s0; [* Example]: text(1 / 3) `= `\"0.3333333333`\", text(`\"hello`\") `= `\"hello`\", "
"text(26.7.1971) `= `\"26.07.1971`\"&][s0; &][s0; ]")
