TITLE("Date/time operations")
REF("Calc$CaddDN")
REF("Calc$CaddND")
REF("Calc$Cadd_monthsDN")
REF("Calc$Cadd_yearsDN")
REF("Calc$Cdate0")
REF("Calc$CdateN")
REF("Calc$CdateNN")
REF("Calc$CdateNNN")
REF("Calc$Cday_of_weekD")
REF("Calc$Cfirst_day_of_monthD")
REF("Calc$CgeTT")
REF("Calc$Clast_day_of_monthD")
REF("Calc$CleTT")
REF("Calc$CltTT")
REF("Calc$CmaxTT")
REF("Calc$CminTT")
REF("Calc$CminmaxTTT")
REF("Calc$CsubDN")
REF("Calc$Cyears_betweenDD")
REF("Calc$CgtTT")
REF("Calc$CsubDD")
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
"[s14;:Calc`$CaddDN: operator `+  (date/time, real number) [S ®] date/time&][s0; "
"&][s14;:Calc`$CaddND: [%00-00 operator `+  (real number, date/time) ][%00-00S ®][%00-00 "
" date/time]&][s0; The unary operator [* `+] leaves its argument unchanged. The operator "
"serves only as a symmetric complement of the operator [* `-] to increase readability "
"of certain formulas.&][s0; &][s0; Binary operator [* `+] adds up its operands. In "
"case of strings, the strings will be concatenated. When a number is added to a date "
"valu")
TOPIC_TEXT(
"e, the date will be moved forward or back by given number of days (forward if it "
"is positive).&][s0; &][s0; [* Example]: 27 `+ 35 `= 62, `\"a piece `\" `+ `\"of text`\" "
"`= `\"a piece of text`\", 27.12.1999 `+ 7 `= 3.1.2000&][s14;:Calc`$Cadd`_monthsDN: "
"[%00-00 add`_months (date, integer ][%00-00/ number`_of`_months][%00-00 ) ][%00-00S "
"®][%00-00  date]&][s0; The function adds given [/ number`_of`_months] to the date. "
"If the target month has fewer days than is the day number in the original date, the "
"function returns last day of the target month.&][s0; &][s0; [* Example]: add`_months(29.2.1996, "
"12) `= 28.2.1997&][s14;:Calc`$Cadd`_yearsDN: [%00-00 add`_years (date, integer ][%00-00/ "
"number`_of`_years][%00-00 ) ][%00-00S ®][%00-00  date]&][s0; The function adds given "
"[/ number`_of`_years] to its date argument. If the original date is Feb 29 in a leap "
"year and the target year is not a leap one, the day number is adjusted to 28.&][s0; "
"&][s0; [* Example]: add`_years(29.2.1992, 3) `= 28.2.1995&][s14;:Calc`$Cdate0: [%00-00 "
"date ][%00-00S ®][%00-00  date]&][s0; &][s14;:Calc`$CdateN: [%00-00 date (integer "
"][%00-00/ day][%00-00 ) ][%00-00S ®][%00-00  date]&][s0; &][s14;:Calc`$CdateNN: "
"[%00-00 date (integer ][%00-00/ day][%00-00 , integer ][%00-00/ month][%00-00 ) ][%00-00S "
"®][%00-00  date]&][s0; &][s14;:Calc`$CdateNNN: [%00-00 date (integer ][%00-00/ day][%00-00 "
", integer ][%00-00/ month][%00-00 , integer ][%00-00/ year][%00-00 ) ][%00-00S ®][%00-00 "
" date]&][s0; The function composes a date value from its arguments. The form without "
"parameters returns current system date. The form with the parameter [/ day]  returns "
"given day in the current month. The two parameter`-form, [/ day] and [/ month], returns "
"given day and month in the current system year. The three argument form composes "
"an arbitrary date as specified by day, month, and year number.&][s0; &][s0; [* Example]: "
"date `= 20.06.2003, date(10) `= 10.06.2003, date(15, 7) `= 15.07.2003, date(25, 1, "
"1974) `= 25.01.1974&][s0; Th")
TOPIC_TEXT(
"e function returns the day number of its argument.&][s0; &][s0;* Example: day(27.5.1984) "
"`= 27&][s14;:Calc`$Cday`_of`_weekD: [%00-00 day`_of`_week (date) ][%00-00S ®][%00-00 "
" integer]&][s0; The function returns the index of day in week for its date argument. "
"The functions returns 0 for Sunday, 1 for Monday etc. through 6 for Saturday.&][s0; "
"&][s0; [* Example]: day`_of`_week(20.06.2003) `= 5&][s14;:Calc`$Cfirst`_day`_of`_monthD: "
"[%00-00 first`_day`_of`_month (date ][%00-00/ d][%00-00 ) ][%00-00S ®][%00-00  date]&][s0; "
"The function returns first day in month defined by its parameter d.&][s0; &][s0; "
"[* Example]: first`_day`_of`_month(18.3.1994) `= 1.3.1994&][s14;:Calc`$CgeTT: [%00-00 "
"operator >`=  (date/time, date/time) ][%00-00S ®][%00-00  logical value]&][s0; &][s14;:Calc`$Clast`_day`_of`_monthD: "
"[%00-00 last`_day`_of`_month (date ][%00-00/ d][%00-00 ) ][%00-00S ®][%00-00  date]&][s0; "
"The function returns last day in month defined by its parameter d.&][s0; &][s0; [* "
"Example]: last`_day`_of`_month(3.2.1999) `= 28.2.1999&][s14;:Calc`$CleTT: [%00-00 "
"operator <`=  (date/time, date/time) ][%00-00S ®][%00-00  logical value]&][s14;:Calc`$CltTT: "
"operator <  (date/time, date/time) [S ®] logical value&][s0; &][s0; &][s14;:Calc`$CmaxTT: "
"max (date/time, date/time) [S ®] date/time&][s0; Minimum. The function will return "
"the bigger of its arguments. Texts are compared lexicographically (the function will "
"return text which would be closer to the end in a dictionary).&][s14;:Calc`$CminTT: "
"min (date/time, date/time) [S ®] date/time&][s0; Minimum. The function will return "
"the smaller of its arguments. Texts are compared lexicographically (the function "
"will return text which would be closer to the beginning in a dictionary).&][s14;:Calc`$CminmaxTTT: "
"minmax (date/time, date/time, date/time) [S ®] date/time&][s0; &][s14;:Calc`$CsubDN: "
"operator `-  (date/time [/ d], real number [/ number`_of`_days]) [S ®] date/time&][s14;:Calc`$Cyears`_betweenDD: "
"[%00-00 years`_between (date ][%")
TOPIC_TEXT(
"00-00/ d1][%00-00 , date ][%00-00/ d2][%00-00 ) ][%00-00S ®][%00-00  integer]&][s0; "
"The function returns number of years between its date arguments (the age of a person "
"born on [/ d1] at the date of [/ d2]).&][s0; &][s0; [* Example]: years_between(25.1.1974, "
"27.12.1999) `= 25&][s0; &][s14;:Calc`$CgtTT: [%00-00 operator >  (date/time, date/time) "
"][%00-00S ®][%00-00  logical value]&][s0; &][s0; Function returns the number of "
"hours of its date/time argument.&][s14;:Calc`$CsubDD: [%00-00 operator `-  (date/time "
"][%00-00/ date1][%00-00 , date/time ][%00-00/ date2][%00-00 ) ][%00-00S ®][%00-00 "
" real number]&][s0; Unary operator [* `-] reverses the sign of its numerical argument.&][s0; "
"&][s0; Binary operator [* `-] subtracts its operands. For numerical operands, the "
"result is the ordinary difference. When an integer is subtracted from a date, the "
"date is moved backward the specified number of days. When subtracting two dates, "
"the result is the number of days between them. The number is positive if the left "
"date comes after the right date.&][s0; &][s0; [* Example]: 137 `- 95 `= 42, 27.12.1999 "
"`- 5 `= 22.12.1999, 4.1.2000 `- 28.12.1999 `= 7&][s0; ]")
