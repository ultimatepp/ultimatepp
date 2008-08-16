topic "Text formatting";[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[{_}%EN-US 
[s3; Text formatting&]
[s5;K:`:`:Format`(const char`*`,const`:`:Vector`<`:`:Value`>`&`): [%00-00^`:`:String^ S
tring][%00-00 _][%00-00@0 Format][%00-00 (][%00-00@(0.0.255) const][%00-00 _][%00-00@(0.0.255) c
har][%00-00 _`*][%00-00@3 format][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00^`:`:Vector^ V
ector][%00-00 <][%00-00^`:`:Value^ Value][%00-00 >`&_][%00-00@3 args][%00-00 )]&]
[s5; [%00-00^`:`:String^ String][%00-00 _][%00-00@0 Format][%00-00 (][%00-00@(0.0.255) const][%00-00 _
][%00-00@(0.0.255) char][%00-00 _`*][%00-00@3 format][%00-00 , ][%00-00^`:`:Value^ Value][%00-00 _
][%00-00@3 arg][%00-00,@3 1][%00-00@3  ][%00-00@4 `[][%00-00@0 , ][%00-00^`:`:Value^ Value][%00-00 _
][%00-00@3 arg][%00-00,@3 n][%00-00@4 `]...][%00-00@0  ][%00-00 )]&]
[s5;K:`:`:Format`(int`,const char`*`,const`:`:Vector`<`:`:Value`>`&`): [%00-00^`:`:String^ S
tring][%00-00 _][%00-00@0 Format][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 language
][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00@(0.0.255) char][%00-00 _`*][%00-00@3 s][%00-00 ,
 ][%00-00@(0.0.255) const][%00-00 _][%00-00^`:`:Vector^ Vector][%00-00 <][%00-00^`:`:Value^ V
alue][%00-00 >`&_][%00-00@3 v][%00-00 )]&]
[s5; [%00-00^`:`:String^ String][%00-00 _][%00-00@0 Format][%00-00 (][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 language][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00@(0.0.255) char][%00-00 _
`*][%00-00@3 format][%00-00 , ][%00-00^`:`:Value^ Value][%00-00 _][%00-00@3 arg][%00-00,@3 1][%00-00@3  
][%00-00@4 `[][%00-00@0 , ][%00-00^`:`:Value^ Value][%00-00 _][%00-00@3 arg][%00-00,@3 n][%00-00@4 `]
...][%00-00@0  ][%00-00 )]&]
[s2; &]
[s0; Format forms output text based on [*@3 format], inserting actual 
arguments to placeholders. Argument values are converted to text 
using formatters. U`+`+ specifies set of standard formatters; 
application can freely register their own formatters for specific 
Value types too.&]
[s0; &]
[s0; Note that the variable number of Value arguments is implemented 
by overloading the Format up to 20 parameters.&]
[s0; &]
[s0; Placeholders start with [* %] and have format:&]
[s0; &]
[s0;*@4 [@0 %]`[[@(0.0.255) commands]`]`[[@(0.0.255) options]`]`[[@(0.0.255) formatter`-id]`]
`[[@(0.0.255) ``]`]&]
[s0; &]
[s0; [*@(0.0.255) commands] are interpreted by Format routine (not 
specific formatter). Each command sequence ends with character 
specifying the kind of command, this delimits it from options 
and/or formatter.&]
[s0; &]
[s0; Available commands:&]
[s0; &]
[ {{1785:8215^ [s0;* [/@(0.0.255) position]:]
::= [s0; Seeks to an argument at [*/@(0.0.255) position]. Allows to `"reorganize`" 
ordering of arguments, useful with translations.]
::^ [s0;* [/@(0.0.255) width]<]
::= [s0; Places formatter result into field with [*/@(0.0.255) width] characters, 
aligns left.]
::^ [s0;* [/@(0.0.255) width]>]
::= [s0; Places formatter result into field with [*/@(0.0.255) width] characters, 
aligns right.]
::^ [s0;* [/@(0.0.255) width]`=]
::= [s0; Places formatter result into field with [*/@(0.0.255) width] characters, 
aligns to center.]
::^ [s0;* `[[/@(0.0.255) text]`]`~]
::= [s0; If argument is [* Null], uses [*/@(0.0.255) text] instead of formatter 
result.]}}&]
[s0; &]
[s0; [*@(0.0.255) formatter`-id][*  ]must consist of alpha characters 
only, unlike C identifiers, digits or `'`_`' are not allowed. 
Everything between [*@(0.0.255) commands] and [*@(0.0.255) formatter`-id] 
is considered to be [*@(0.0.255) options] and passed to formatter. 
Note that [*@(0.0.255) formatter`-id] is Value type specific `- 
the same name can specify different formatter depending on Value 
type. [*@(0.0.255) formatter`-id] is case`-sensitive.&]
[s0; &]
[s0; Character [* `*] in [*@(0.0.255) options] section is replaced by 
argument converted using AsString.&]
[s0; &]
[s0; If [*@(0.0.255) options] are to contain alpha characters, they 
need to be escaped using [* `[ `]] to distinguish options from 
formatter`-id.&]
[s0; &]
[s0; Placeholder can end either by non`-alpha character or by [* ``]. 
formatter`-id can be left empty; in that case Format uses AsString 
to convert Value to text (defined in RichValue interface) `- 
the most trivial placeholder is therefore [* %``].&]
[s0; &]
[s0; While Format implements all of classic printf formatter, please 
notice two incompatibilities:&]
[s0; &]
[s0;i150;O0; All arguments of Format must be convertible (and are 
converted) to Value. On the positive side, Value performs natural 
conversions like double `-> int, so it is possible to e.g. use 
%d for double value.&]
[s0; &]
[s0;i150;O0; formatter`-id `"eats`" all alpha characters. This is 
a problem when non`-placeholder alpha character is to follow 
placeholder, e.g. %dpt `- this has to be written as %d``pt (`` 
delimits the formatter`-id).&]
[s0; &]
[s0; &]
[s0;*/3 Standard formatters&]
[s0; &]
[s0;* default formatter&]
[s0; &]
[s0; If formatter`-id is empty, Value is converted using AsString 
(implemented in RichValue interface).&]
[s0; &]
[s0; &]
[s0;* printf formatters&]
[s0; &]
[s0; Most printf formatters are supported:&]
[s0; &]
[s0;* c d i o x X ld li lo lx lX e E f g G s&]
[s0; &]
[s0; Please refer to printf documentation for the description.&]
[s0; &]
[s0; &]
[s0;* switch formatter&]
[s0; &]
[s0; This is special number formatter (registered for double, int 
and int64 values). [*@(0.0.255) options] of switch formatter contain 
a list of values and respective texts `- a text for actual argument 
is printed. [*@(0.0.255) formatter`-id] is [* s].&]
[s0; &]
[s0; The format of switch [*@(0.0.255) options] is&]
[s0; &]
[s0;*@4 `[[@(0.0.255) modulo][@0 %]`]`[[@(0.0.255) case][@0 :][@(0.0.255) text][*@0 ;]`]...`[[@(0.0.255) d
efault]`]&]
[s0; &]
[ {{1395:8605 [s0;*@(0.0.255) modulo]
:: [s0; If this optional part is present, modulo of argument is used 
for switch cases.]
:: [s0;*@(0.0.255) case]
:: [s0; Numeric case.]
:: [s0;*@(0.0.255) text]
:: [s0; Text for given numeric case.]
:: [s0;*@(0.0.255) default]
:: [s0; Default text when no case is matched.]}}&]
[s0; &]
[s0; Note that as text usually contains letters, whole switch options 
section is almost always escaped using [* `[] [* `]].&]
[s0; &]
[s0; &]
[s0;* simple integer formatters&]
[s0; &]
[s0; These formatters are registered for double, int and int64 values.&]
[s0; &]
[ {{1426:8574h1;@(204) [s0;*@(0.0.255) formatter`-id]
:: [s0; Description]
::@2 [s0;* [%00-00 month]]
:: [s0;C Lower`-case month name.]
:: [s0;* [%00-00 Month]]
:: [s0;C Month name with first letter upper`-case, rest lower`-case.]
:: [s0;* [%00-00 MONTH]]
:: [s0;C Upper`-case month name.]
:: [s0;* [%00-00 mon]]
:: [s0;C Abbreviated lower`-case month name.]
:: [s0;* [%00-00 Mon]]
:: [s0;C Abbreviated month name, first letter upper`-case, rest lower`-case.]
:: [s0;* [%00-00 MON]]
:: [s0;C Abbreviated upper`-case month name.]
:: [s0;* [%00-00 day]]
:: [s0;C Lower`-case day name.]
:: [s0;* [%00-00 Day]]
:: [s0;C Day name with first letter upper`-case, rest lower`-case.]
:: [s0;* [%00-00 DAY]]
:: [s0;C Upper`-case day name.]
:: [s0;* [%00-00 dy]]
:: [s0;C Abbreviated lower`-case day name.]
:: [s0;* [%00-00 Dy]]
:: [s0;C Abbreviated day name, first letter upper`-case, rest lower`-case.]
:: [s0;* [%00-00 DY]]
:: [s0;C Abbreviated upper`-case day name.]
:: [s0;* [%00-00 tw]]
:: [s0;C 12`-hour modulo format.]
:: [s0;* [%00-00 a]]
:: [s0;C Letter format, 1: a, 2: b, ... 26: z, 27: aa, ...]
:: [s0;* [%00-00 A]]
:: [s0;C Letter format, 1: a, 2: b, ... 26: z, 27: aa, ...]
:: [s0;* [%00-00 r]]
:: [s0;C Lower`-case roman numbers.]
:: [s0;* [%00-00 R]]
:: [s0;C Upper`-case roman numbers.]}}&]
[s0; &]
[s0; &]
[s0;* alternate real number formatters&]
[s0; &]
[s0;O9;~~~.352; [* n]-|fixed decimals&]
[s0;O9;~~~.352; [* v]-|valid decimals&]
[s0;O9;~~~.352; [* ne], [* ve]-|force exponential notation&]
[s0;O9;~~~.352; [* nf], [* vf]-|force fixed notation&]
[s0;O9;~~~.352; [* nl], [* vl]-|language`-based formatting&]
[s0; &]
[s0; The format of [*@(0.0.255) options] is&]
[s0; &]
[s0;*@4 `[[@0 `+]`]`[`[[@0 `-]`][@(0.0.255) digits]`]`[[@0 !]`]`[[@0 `^]`[[@0 `+]`][@(0.0.255) expd
ig]`]&]
[s0; &]
[s0;O9;~~~.576; `+-|always prepend sign&]
[s0;O9;~~~.576; [*@4 `[][* `-][*@4 `]][*@(0.0.255) digits][@(0.0.255) -|]number 
of decimals to print (negative `= left of decimal point, default 
`= 6)&]
[s0;O9;~~~.576; [* !]-|keep insignificant zeros&]
[s0;O9;~~~.576; [* `^]-|exponent options:&]
[s0;l160;O9;~~~.640; [* `+]-|always prepend sign to exponent&]
[s0;l160;O9;~~~.640; [*@(0.0.255) expdig][@(0.0.255) -|]exponent padding 
width&]
[s0; &]
[s0; &]
[s0; &]
[s0;*/3 [%00-00 Examples of standard formatters]&]
[s0; &]
[ {{5311:4689 [s0; [%00-00 Format(`"][%00-00* %d][%00-00 , ][%00-00* %s][%00-00 `", 123, 
`"TEXT`")]]
:: [s0;C [%00-00 123, TEXT]]
:: [s0; [%00-00 Format(`"][%00-00* %2:s][%00-00 , ][%00-00* %1:d][%00-00 `", 123, 
`"TEXT`")]]
:: [s0;C [%00-00 TEXT, 123]]
:: [s0; [%00-00 Format(`"][%00-00* %010d][%00-00 `", 123)]]
:: [s0;C [%00-00 0000000123]]
:: [s0; [%00-00 Format(`"][%00-00* %0`*d][%00-00 `", 11, 123)]]
:: [s0;C [%00-00 00000000123]]
:: [s0; [%00-00 Format(`"`|][%00-00* %20<d][%00-00 `|`", 123)]]
:: [s0;C [%00-00 `|123                 `|]]
:: [s0; [%00-00 Format(`"`|][%00-00* %20>d][%00-00 `|`", 123)]]
:: [s0;C [%00-00 `|                 123`|]]
:: [s0; [%00-00 Format(`"`|][%00-00* %20`=d][%00-00 `|`", 123)]]
:: [s0;C [%00-00 `|        123         `|]]
:: [s0; [%00-00 Format(`"][%00-00* %d``][%00-00 pt`", 123)]]
:: [s0;C [%00-00 123pt]]
:: [s0; [%00-00 Format(`"][%00-00* %`[empty`]`~d][%00-00 , ][%00-00* %`[empty`]`~d][%00-00 `", 
123, Null)]]
:: [s0;C [%00-00 123, empty]]
:: [s0; [%00-00 Format(`"][%00-00* %``][%00-00 `", 123)]]
:: [s0;C [%00-00 123]]
:: [s0; [%00-00 Format(`"][%00-00* %c][%00-00 `", 65)]]
:: [s0;C [%00-00 A]]
:: [s0; [%00-00 Format(`"][%00-00* %d][%00-00 `", 123)]]
:: [s0;C [%00-00 123]]
:: [s0; [%00-00 Format(`"][%00-00* %i][%00-00 `", 123)]]
:: [s0;C [%00-00 123]]
:: [s0; [%00-00 Format(`"][%00-00* %o][%00-00 `", 123)]]
:: [s0;C [%00-00 173]]
:: [s0; [%00-00 Format(`"][%00-00* %x][%00-00 `", 123)]]
:: [s0;C [%00-00 7b]]
:: [s0; [%00-00 Format(`"][%00-00* %X][%00-00 `", 123)]]
:: [s0;C [%00-00 7B]]
:: [s0; [%00-00 Format(`"][%00-00* %e][%00-00 `", 1.23)]]
:: [s0;C [%00-00 1.230000e`+000]]
:: [s0; [%00-00 Format(`"][%00-00* %E][%00-00 `", 1.23)]]
:: [s0;C [%00-00 1.230000E`+000]]
:: [s0; [%00-00 Format(`"][%00-00* %f][%00-00 `", 1.23)]]
:: [s0;C [%00-00 1.230000]]
:: [s0; [%00-00 Format(`"][%00-00* %g][%00-00 `", 1.23)]]
:: [s0;C [%00-00 1.23]]
:: [s0; [%00-00 Format(`"][%00-00* %G][%00-00 `", 1.23)]]
:: [s0;C [%00-00 1.23]]
:: [s0; [%00-00 Format(`"][%00-00* %n][%00-00 `", 1.23)]]
:: [s0;C [%00-00 1.23]]
:: [s0; [%00-00 Format(`"][%00-00* %1n][%00-00 `", 1.23)]]
:: [s0;/ [%00-00/C 1.2 ][%00-00 (explicit number of decimals)]]
:: [s0; [%00-00 Format(`"][%00-00* %3!n][%00-00 `", 1.23)]]
:: [s0;/ [%00-00/C 1.230 ][%00-00 (force insignificant zeros)]]
:: [s0; [%00-00 Format(`"][%00-00* %ne][%00-00 `", 1.23)]]
:: [s0;C [%00-00 1.23e0]]
:: [s0; [%00-00 Format(`"][%00-00* %2`^`+3ne][%00-00 `", 1.23)]]
:: [s0;/ [%00-00/C 1.2e`+000 ][%00-00 (exponent formatting)]]
:: [s0; [%00-00 Format(`"][%00-00* %nf][%00-00 `", 1.23e30)]]
:: [s0;C [%00-00 1230000000000000000000000000000]]
:: [s0; [%00-00 Format(`"][%00-00* %nl][%00-00 `", 1.23)]]
:: [s0;C [%00-00 1,23]]
:: [s0; [%00-00 Format(`"][%00-00* %v][%00-00 `", 1.23)]]
:: [s0;C [%00-00 1.23]]
:: [s0; [%00-00 Format(`"][%00-00* %2v][%00-00 `", 1.23)]]
:: [s0;/ [%00-00/C 1.2 ][%00-00 (2 valid digits)]]
:: [s0; [%00-00 Format(`"][%00-00* %2v][%00-00 `", 123)]]
:: [s0;/ [%00-00/C 120 ][%00-00 (2 valid digits)]]
:: [s0; [%00-00 Format(`"][%00-00* %ve][%00-00 `", 1.23)]]
:: [s0;C [%00-00 1.23e0]]
:: [s0; [%00-00 Format(`"][%00-00* %vf][%00-00 `", 1.23e30)]]
:: [s0;C [%00-00 1230000000000000000000000000000]]
:: [s0; [%00-00 Format(`"][%00-00* %vl][%00-00 `", 1.23)]]
:: [s0;C [%00-00 1,23]]
:: [s0; [%00-00 Format(`"][%00-00* %`[1:one;2:two;3:three;another`]s][%00-00 `", 
2)]]
:: [s0;C [%00-00 two]]
:: [s0; [%00-00 Format(`"][%00-00* %`[1:one;2:two;3:three;another`]s][%00-00 `", 
20)]]
:: [s0;C [%00-00 another]]
:: [s0; [%00-00 Format(`"][%00-00* %`[3%1:one;2:two;3:three;another`]s][%00-00 `", 
20)]]
:: [s0;C [%00-00 two]]
:: [s0; [%00-00 Format(`"][%00-00* %month][%00-00 `", 6)]]
:: [s0;C [%00-00 june]]
:: [s0; [%00-00 Format(`"][%00-00* %Month][%00-00 `", 6)]]
:: [s0;C [%00-00 June]]
:: [s0; [%00-00 Format(`"][%00-00* %MONTH][%00-00 `", 6)]]
:: [s0;C [%00-00 JUNE]]
:: [s0; [%00-00 Format(`"][%00-00* %mon][%00-00 `", 6)]]
:: [s0;C [%00-00 jun]]
:: [s0; [%00-00 Format(`"][%00-00* %Mon][%00-00 `", 6)]]
:: [s0;C [%00-00 Jun]]
:: [s0; [%00-00 Format(`"][%00-00* %MON][%00-00 `", 6)]]
:: [s0;C [%00-00 JUN]]
:: [s0; [%00-00 Format(`"][%00-00* %day][%00-00 `", 6)]]
:: [s0;C [%00-00 saturday]]
:: [s0; [%00-00 Format(`"][%00-00* %Day][%00-00 `", 6)]]
:: [s0;C [%00-00 Saturday]]
:: [s0; [%00-00 Format(`"][%00-00* %DAY][%00-00 `", 6)]]
:: [s0;C [%00-00 SATURDAY]]
:: [s0; [%00-00 Format(`"][%00-00* %dy][%00-00 `", 6)]]
:: [s0;C [%00-00 sa]]
:: [s0; [%00-00 Format(`"][%00-00* %Dy][%00-00 `", 6)]]
:: [s0;C [%00-00 Sa]]
:: [s0; [%00-00 Format(`"][%00-00* %DY][%00-00 `", 6)]]
:: [s0;C [%00-00 SA]]
:: [s0; [%00-00 Format(`"][%00-00* %tw][%00-00 `", 0)]]
:: [s0;C [%00-00 12]]
:: [s0; [%00-00 Format(`"][%00-00* %tw][%00-00 `", 5)]]
:: [s0;C [%00-00 5]]
:: [s0; [%00-00 Format(`"][%00-00* %tw][%00-00 `", 15)]]
:: [s0;C [%00-00 3]]
:: [s0; [%00-00 Format(`"][%00-00* %0tw][%00-00 `", 15)]]
:: [s0;C [%00-00 03]]
:: [s0; [%00-00 Format(`"][%00-00* %a][%00-00 `", 1)]]
:: [s0;C [%00-00 a]]
:: [s0; [%00-00 Format(`"][%00-00* %a][%00-00 `", 123)]]
:: [s0;C [%00-00 es]]
:: [s0; [%00-00 Format(`"][%00-00* %A][%00-00 `", 1)]]
:: [s0;C [%00-00 A]]
:: [s0; [%00-00 Format(`"][%00-00* %A][%00-00 `", 123)]]
:: [s0;C [%00-00 ES]]
:: [s0; [%00-00 Format(`"][%00-00* %r][%00-00 `", 8)]]
:: [s0;C [%00-00 viii]]
:: [s0; [%00-00 Format(`"][%00-00* %R][%00-00 `", 1231)]]
:: [s0;C [%00-00 MCCXXXI]]
:: [s0; [%00-00 Format(`"][%00-00* %``][%00-00 `", GetSysDate())]]
:: [s0;C [%00-00 08/25/2006]]
:: [s0; [%00-00 Format(`"][%00-00* %``][%00-00 `", GetSysTime())]]
:: [s0;C [%00-00 08/25/2006 20:37:09]]
:: [s0; [%00-00 Format(`"][%00-00* %``][%00-00 `", `"text`")]]
:: [s0;C [%00-00 text]]
:: [s0; [%00-00 Format(`"][%00-00* %``][%00-00 `", GetSysDate())]]
:: [s0;C [%00-00 25.08.2006]]}}&]
[s0; &]
[s6; [%00-00 Registering custom formatters]&]
[s0; &]
[s5;K:`:`:Formatter`:`:typedef: [%00-00 typedef_][%00-00^`:`:String^ String][%00-00 _(`*For
matter)(][%00-00@(0.0.255) const][%00-00 _][%00-00^`:`:Formatting^ Formatting][%00-00 `&_
fmt)]&]
[s2; Formatter has to have form of function with single [^`:`:Formatting^ Formatting] 
argument.&]
[s0; &]
[s1;K:`:`:Formatting`:`:struct:@0 [%00-00@(0.0.255) struct][%00-00@(64) _][%00-00 Formattin
g]&]
[s2; This structure passes all informations to format Value argument 
to the formatter.&]
[s0; &]
[s5;K:`:`:Formatting`:`:language:@0 [%00-00@(0.0.255) int][%00-00@(64) _][%00-00 language]&]
[s2; Language of resulting text.&]
[s0; &]
[s5;K:`:`:Formatting`:`:arg:@0 [%00-00^`:`:Value^@(64) Value][%00-00@(64) _][%00-00 arg]&]
[s2; Actual argument.&]
[s0; &]
[s5;K:`:`:Formatting`:`:format:@0 [%00-00^`:`:String^@(64) String][%00-00@(64) _][%00-00 fo
rmat]&]
[s2; Formatting [*@(0.0.255) options].&]
[s0; &]
[s5;K:`:`:Formatting`:`:id:@0 [%00-00^`:`:String^@(64) String][%00-00@(64) _][%00-00 id]&]
[s2; Formatter`-id.&]
[s0; &]
[s0; &]
[s5;K:`:`:RegisterFormatter`(int`,const char`*`,`:`:Formatter`): [%00-00@(0.0.255) void
][%00-00 _][%00-00@0 RegisterFormatter][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 ty
pe][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00@(0.0.255) char][%00-00 _`*][%00-00@3 i
d][%00-00 , ][%00-00^`:`:Formatter^ Formatter][%00-00 _][%00-00@3 f][%00-00 )]&]
[s2; Registers formatter for specific Value [%00-00*@3 type]. If [%00-00*@3 type] 
is VALUE`_V, formatter is applied to all Value types if no formatter 
for specific type is specified.&]
[s0; &]
[s5;K:`:`:RegisterNumberFormatter`(const char`*`,`:`:Formatter`): [%00-00@(0.0.255) voi
d][%00-00 _][%00-00@0 RegisterNumberFormatter][%00-00 (][%00-00@(0.0.255) const][%00-00 _][%00-00@(0.0.255) c
har][%00-00 _`*][%00-00@3 id][%00-00 , ][%00-00^`:`:Formatter^ Formatter][%00-00 _][%00-00@3 f
][%00-00 )]&]
[s2; Registers formatter for int, double and int64 types.&]
[s0; &]
[s5;K:`:`:RegisterStringFormatter`(const char`*`,`:`:Formatter`): [%00-00@(0.0.255) voi
d][%00-00 _][%00-00@0 RegisterStringFormatter][%00-00 (][%00-00@(0.0.255) const][%00-00 _][%00-00@(0.0.255) c
har][%00-00 _`*][%00-00@3 id][%00-00 , ][%00-00^`:`:Formatter^ Formatter][%00-00 _][%00-00@3 f
][%00-00 )]&]
[s2; Registers formatter for String and WString types.&]
[s0; &]
[s5;K:`:`:RegisterDateTimeFormatter`(const char`*`,`:`:Formatter`): [%00-00@(0.0.255) v
oid][%00-00 _][%00-00@0 RegisterDateTimeFormatter][%00-00 (][%00-00@(0.0.255) const][%00-00 _
][%00-00@(0.0.255) char][%00-00 _`*][%00-00@3 id][%00-00 , ][%00-00^`:`:Formatter^ Formatte
r][%00-00 _][%00-00@3 f][%00-00 )]&]
[s2; Registers formatter for Date and Time types.&]
[s0; &]
[s5;K:`:`:RegisterValueFormatter`(const char`*`,`:`:Formatter`): [%00-00@(0.0.255) void
][%00-00 _][%00-00@0 RegisterValueFormatter][%00-00 (][%00-00@(0.0.255) const][%00-00 _][%00-00@(0.0.255) c
har][%00-00 _`*][%00-00@3 id][%00-00 , ][%00-00^`:`:Formatter^ Formatter][%00-00 _][%00-00@3 f
][%00-00 )]&]
[s2; Registers formatter to be applied when no formatter for specific 
type is specified.&]
[s0; ]