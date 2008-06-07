TITLE("Logical operations")
REF("Calc$CbitAndNN")
REF("Calc$CbitNotN")
REF("Calc$CbitOrNN")
REF("Calc$CbitXorNN")
REF("Calc$CeqVV")
REF("Calc$Cis_dateV")
REF("Calc$Cis_nullV")
REF("Calc$Cis_numberV")
REF("Calc$Cis_textV")
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
"[s0; &][s14;:Calc`$CbitAndNN: [%00-00 operator `&  (integer, integer) ][%00-00S ®][%00-00 "
" integer]&][s0; Bit product. The result is the integer whose binary representation "
"consists of ones on those positions on which there are ones in binary representation "
"of both arguments.&][s0; &][s0; [* Example]: 11 `& 13 `= 9 (1011 `& 1101 `= 1001).&][s14;:Calc`$CbitNotN: "
"[%00-00 operator `~  (integer) ][%00-00S ®][%00-00  integer]&][s0; Bit negation. "
"The result is the integer whose binary representation consists of ")
TOPIC_TEXT(
"ones on those positions on which there are zeroes and vice versa in binary representation "
"of the argument. As integer calculations are performed in 0`'s complement code, it "
"is always true that [* %01`~x_`= `-1 `- x%01].&][s0; &][s0; [* Example]: `~14 `= "
"`-15.&][s14;:Calc`$CbitOrNN: [%00-00 operator `|  (integer, integer) ][%00-00S ®][%00-00 "
" integer]&][s0; Bit addition. The result is the integer whose binary representation "
"consists of 1`'s on those positions on which there is 1 in binary representation "
"of any of its arguments.&][s0; &][s0; [* Example]: 1 `| 4 `= 5 (001 `| 100 `= 101).&][s14;:Calc`$CbitXorNN: "
"[%00-00 operator `^  (integer, integer) ][%00-00S ®][%00-00  integer]&][s0; Bit "
"symmetric difference ([* xor]). The result of the operation is an integer whose binary "
"representation consists of 1`'s on those positions on which the digits in binary "
"representation of its arguments differ.&][s0; &][s0; [* Example]: 9 `^ 15 `= 6 (1001 "
"`^ 1111 `= 0110).&][s14;:Calc`$CeqVV: operator `=`=  (any data type, any data type) "
"[S ®] logical value&][s0; &][s14;:Calc`$Cis`_dateV: [%00-00 is`_date (any data type) "
"][%00-00S ®][%00-00  integer]&][s0; The function returns the logical value 1 ([* "
"true]) if its argument is a date. If the argument is of a different type or if it "
"contains the null value, the function returns 0 ([* false]).&][s0; &][s0; [* Example]: "
"is`_date(10.5) `= 0, is`_date(23.5.1997) `= 1, is`_date(`\"hi`\") `= 0&][s14;:Calc`$Cis`_nullV: "
"[%00-00 is`_null (any data type) ][%00-00S ®][%00-00  integer]&][s0; The function "
"returns the logical value 1 ([* true]) if its parameter is of type[* null]; otherwise "
"the logical value 0 ([* false]) is returned.&][s0; &][s0; [* Example]: is_null(10.5) "
"`= 0, is_null(null) `= 1&][s14;:Calc`$Cis`_numberV: [%00-00 is`_number (any data "
"type) ][%00-00S ®][%00-00  integer]&][s0; The function checks whether its argument "
"is a number (integer, real number, or a logical value). If the argument is a number "
"type, the function returns logi")
TOPIC_TEXT(
"cal value 1 (true).&][s0; &][s0; [* Example]: is number(10.5) `= 1, is number(23.5.1997) "
"`= 0, is number(`\"hi`\") `= 0&][s14;:Calc`$Cis`_textV: [%00-00 is`_text (any data "
"type) ][%00-00S ®][%00-00  integer]&][s0; The function returns logical value 1 ([* "
"true]) if its argument is a string. If the parameter is of a different type or is "
"empty, the function returns logical value 0 (false).&][s0; &][s0; [* Example]: is`_text(10.5) "
"`= 0, is`_text(23.5.1997) `= 0, is`_text(`\"hi`\") `= 1&][s0; &][s14;:Calc`$CneVV: "
"[%00-00 operator !`=  (any data type, any data type) ][%00-00S ®][%00-00  logical "
"value]&][s0; Equality / inequality. The operator [* `=`=] (equality) will return "
"the logical value 1 ([* true]), if operands on both sides are of the same type and "
"are identical. The operator [* !`=] (inequality) will always return the opposite "
"logical value than operator [* `=`=].&][s0; &][s0; [* Note]: This definition of equality "
"implies that if, for instance, on the one side of the comparison is the text `\"300`\" "
"and the number 300 on the other, the operator [* `=`=] will return the logical value "
"[* false], even though the text on the left`-hand side is identical with the text "
"representation of the number on the right`-hand side. If you want to compare values "
"of different types, convert them first into the same type (e.g. using the [* text] "
"(viz str._96) function).&][s14;:Calc`$CsalNN: [%00-00 operator <<  (integer, integer "
"][%00-00/ bit`_count][%00-00 ) ][%00-00S ®][%00-00  integer]&][s0; Arithmetic shift "
"to the left. The result is the integer whose binary representation can be obtained "
"by shifting the binary representation of the left operand [/ bit`_count] times to "
"the left (towards higher bits), adding zeroes to the right. As integer calculations "
"are performed in 0`'s complement code, it is always true (except when the result "
"overflows valid integer range) that [* a_<< b_`= a_`* 2 `*`* b].&][s0; &][s0; [* "
"Example]: 3 << 2 `= 12 (11 << 2 `= 1100).&][s14;:Calc`$CsarNN: [%00-00 o")
TOPIC_TEXT(
"perator >>  (integer, integer ][%00-00/ bit`_count][%00-00 ) ][%00-00S ®][%00-00 "
" integer]&][s0; Arithmetic shift to the right. The result is the integer whose binary "
"representation will be made by shifting the binary representation of its left operand "
"to the right (towards lower bits) [/ bit`_count] times, discarding lowest bits as "
"necessary and duplicating the highest bit (sign bit) of the left operand. As integer "
"calculations are performed in 0`'s complement code, the arithmetic shift to the right "
"by a bit equals division by two with rounding towards minus infinity.&][s0; &][s0; "
"[* Example]: 12 >> 2 `= 3, `-14 >> 3 `= `-2.&][s0; Function returns number of seconds "
"of its date/time argument.&][s0; ]")
