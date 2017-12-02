TITLE("Arithmetical operations")
REF("Calc$CabsN")
REF("Calc$CaddNN")
REF("Calc$CceilN")
REF("Calc$CceilNN")
REF("Calc$CceilrNN")
REF("Calc$CdivNN")
REF("Calc$CfloorN")
REF("Calc$CfloorNN")
REF("Calc$CfloorrNN")
REF("Calc$CgeNN")
REF("Calc$ChypotNN")
REF("Calc$CleNN")
REF("Calc$ClogNotN")
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
REF("Calc$CgtNN")
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
"[s14;:Calc`$CabsN: [%00-00 abs (real number) ][%00-00S ®][%00-00  real number]&][s0; "
"Absolute value. If the number is positive, its absolute value is equal to it. The "
"absolute value of a negative number is its inverse.&][s0; &][s0; [* Example]: abs(`-10.5) "
"`= 10.5, abs(12) `= 12.&][s14;:Calc`$CaddNN: operator `+  (real number, real number) "
"[S ®] real number&][s0; &][s0; &][s14;:Calc`$CceilN: [%00-00 ceil (real number) "
"][%00-00S ®][%00-00  real number]&][s0; &][s14;:Calc`$CceilNN: [%00-00 ceil (real "
"number")
TOPIC_TEXT(
" ][%00-00/ value][%00-00 , integer ][%00-00/ decimals][%00-00 ) ][%00-00S ®][%00-00 "
" real number]&][s0; Rounding down (towards plus infinity) to the stated number of "
"decimal places (0 by default). The function returns the lowest integral multiple "
"of [* 10_`*`*_][/ `-decimals] not lower than the original [/ value].&][s0; &][s0; "
"[* Example]: ceil(`-2.5) `= `-2, ceil(47.32, 1) `= 47.4, ceil(117, `-1) `= 120.&][s14;:Calc`$CceilrNN: "
"[%00-00 ceilr(real number ][%00-00/ value][%00-00 , integer ][%00-00/ significant`_digits][%00-00 "
") ][%00-00S ®][%00-00  real number]&][s0; Rounding up (towards plus infinity) to "
"the stated number of significant digits. The function returns the lowest integral "
"multiple of [* 10_`*`*_][/ (ceil(log(value))`-significant`_digits)] not lower than "
"the original [/ value].&][s0; &][s0;* Example: ceilr(47.32, 3) `= 47.4, ceilr(117, "
"1) `= 200.&][s14;:Calc`$CdivNN: [%00-00 operator /  (real number, real number) ][%00-00S "
"®][%00-00  real number]&][s0; Real number division. If the right operand equals "
"zero, the computation aborts with the error [* division by zero].&][s0; &][s0; [* "
"Example]: 3.7 / 2.5 `= 1.48.&][s14;:Calc`$CfloorN: [%00-00 floor (real number) ][%00-00S "
"®][%00-00  real number]&][s0; &][s14;:Calc`$CfloorNN: [%00-00 floor (real number "
"][%00-00/ value][%00-00 , integer ][%00-00/ decimals][%00-00 ) ][%00-00S ®][%00-00 "
" real number]&][s0; Rounding down (towards minus infinity) to the stated number of "
"decimal places (0 by default). The function returns the highest integral multiple "
"of [* 10_`*`*_][/ `-decimals] not greater than the original [/ value].&][s0; &][s0; "
"[* Example]: floor(`-2.5) `= `-3, floor(47.32, 1) `= 47.3, floor(117, `-1) `= 110.&][s14;:Calc`$CfloorrNN: "
"[%00-00 floorr(real number ][%00-00/ value][%00-00 , integer ][%00-00/ significant`_digits][%00-00 "
") ][%00-00S ®][%00-00  real number]&][s0; Rounding down (towards minus infinity) "
"to the stated number of significant digits. The function returns the highest integral "
"multiple ")
TOPIC_TEXT(
"of [* 10_`*`*_][/ (ceil(log(value))`-significant`_digits) ]not greater than the "
"original [/ value].&][s0; &][s0;* Example: floorr(47.32, 3) `= 47.3, floorr(117, "
"1) `= 100.&][s14;:Calc`$CgeNN: [%00-00 operator >`=  (real number, real number) ][%00-00S "
"®][%00-00  logical value]&][s0; The operator will compare its operands returning "
"the value 1 if the relation holds, 0 when not. String values are compared lexicographically "
"([/ dictionary] organization).&][s14;:Calc`$ChypotNN: [%00-00 hypot (real number "
"][%00-00/ x][%00-00 , real number ][%00-00/ y][%00-00 ) ][%00-00S ®][%00-00  real "
"number]&][s0; The length of the hypotenuse of the right`-angled triangle. The hypotenuse "
"is calculated using the Pythagoras theorem, [* hypot(][*/ x][* , ][*/ y][* ) `= sqrt(][*/ "
"x][*  `* ][*/ x][*  `+ ][*/ y][*  `* ][*/ y][* )].&][s0; &][s0; [* Example]: hypot(3, "
"4) `= 5&][s14;:Calc`$CleNN: operator <`=  (real number, real number) [S ®] logical "
"value&][s0; The operator will compare its operands returning the value 1 if the relation "
"holds, 0 when not. String values are compared lexicographically ([/ dictionary] organization).&][s14;:Calc`$ClogNotN: "
"[%00-00 operator !  (real number) ][%00-00S ®][%00-00  logical value]&][s0; Logical "
"negation. The result is the logical value 1 ([* true]), if the argument is zero (zero "
"number or the logical value [* false]). If the argument is nonzero or if it contains "
"the logical value [* true], the result of logical negation is the value 0 ([* false]). "
"If the operator ! is applied to a string, the result will be 1 ([* true]) when and "
"only when the string is empty.&][s0; &][s0; [* Example]: !10 `= 0, !0 `= 1, !`\"hello`\" "
"`= 0&][s14;:Calc`$CltNN: operator <  (real number, real number) [S ®] logical value&][s0; "
"The operator will compare its operands returning the value 1 if the relation holds, "
"0 when not. String values are compared lexicographically ([/ dictionary] organization).&][s14;:Calc`$CmaxNN: "
"max (real number, real number) [S ®] real number&][s0; &]")
TOPIC_TEXT(
"[s14;:Calc`$CminNN: min (real number, real number) [S ®] real number&][s0; &][s14;:Calc`$CminmaxNNN: "
"minmax (real number, real number, real number) [S ®] real number&][s0; &][s14;:Calc`$CminusN: "
"[%00-00 operator `-  (real number) ][%00-00S ®][%00-00  real number]&][s0; &][s0; "
"Function returns number of minutes of its date/time argument.&][s14;:Calc`$CmodNN: "
"[%00-00 operator %  (real number ][%00-00/ a][%00-00 , real number ][%00-00/ b][%00-00 "
") ][%00-00S ®][%00-00  real number]&][s0; Division remainder. The operator determines "
"the least non`-negative real number which can be obtained by adding or subtraction "
"any integral multiples of [/ b] to value [/ a]. If [/ b] zero, the computation aborts "
"with the error [* division by zero].&][s0; &][s0; [* Example]: 10.5 % 2.4 `= 0.9 "
"(10.5 `= 4 `* 2.4 `+ 0.9).&][s0; The function returns the month number of its argument.&][s0; "
"&][s14;:Calc`$CmulNN: [%00-00 operator `*  (real number, real number) ][%00-00S ®][%00-00 "
" real number]&][s0; The product of two real numbers.&][s0; &][s0; [* Example]: 17 "
"`* 15 `= 255.&][s14;:Calc`$CplusN: operator `+  (real number) [S ®] real number&][s0; "
"&][s0; &][s14;:Calc`$CroundN: [%00-00 round (real number) ][%00-00S ®][%00-00  real "
"number]&][s0; &][s14;:Calc`$CroundNN: [%00-00 round (real number ][%00-00/ value][%00-00 "
", integer ][%00-00/ decimals][%00-00 ) ][%00-00S ®][%00-00  real number]&][s0; Rounding "
"to the stated number of decimal places (0 by default). The direction of rounding "
"is towards the nearest integral multiple of [* 10_`*`*_][/ `-decimals]. If the number "
"lies exactly in the middle between two such numbers, it is rounded up (towards positive "
"infinity).&][s0; &][s0; [* Example]: round(`-2.5) `= `-2, round(47.32, 1) `= 47.3, "
"round(117, `-1) `= 120.&][s14;:Calc`$CroundrNN: [%00-00 roundr(real number ][%00-00/ "
"value][%00-00 , integer ][%00-00/ significant`_digits][%00-00 ) ][%00-00S ®][%00-00 "
" real number]&][s0; Rounding to the stated number of significant digits. The direction "
"o")
TOPIC_TEXT(
"f rounding is towards the nearest integral multiple of [* 10_`*`*_][/ (ceil(log(value)`-significant`_digits)]. "
"If the number lies exactly in the middle between two such nearest numbers, it is "
"rounded up (towards positive infinity).&][s0; &][s0;* Example: roundr(47.32, 3) `= "
"47.3, roundr(117, 1) `= 100.&][s0; &][s14;:Calc`$CsgnN: [%00-00 sgn (real number "
"][%00-00/ x][%00-00 ) ][%00-00S ®][%00-00  integer]&][s0; Sign. The sign computation "
"is based on the following table:&][s0; &][s0; &][s0; &][s0; [* Example]: sgn(`-13.5) "
"`= `-1, sgn(0) `= 0, sgn(10) `= `+1.&][s14;:Calc`$CsqrN: [%00-00 sqr (real number "
"][%00-00/ x][%00-00 ) ][%00-00S ®][%00-00  real number]&][s0; Square. The function "
"returns the value [* sqr(][*/ x][* ) `= ][*/ x][*  `* ][*/ x].&][s0; &][s0; [* Example]: "
"sqr(12) `= 144&][s14;:Calc`$CsqrtN: [%00-00 sqrt (real number ][%00-00/ x][%00-00 "
") ][%00-00S ®][%00-00  real number]&][s0; [%00-00 Squared root.]&][s0; &][s14;:Calc`$CsubNN: "
"operator `-  (real number, real number) [S ®] real number&][s0; &][s14;:Calc`$CgtNN: "
"operator >  (real number, real number) [S ®] logical value&][s0; The operator will "
"compare its operands returning the value 1 if the relation holds, 0 when not. String "
"values are compared lexicographically ([/ dictionary] organization).&][s0; &][s0; "
"]")
