topic "Date formatting and scanning";[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[l288;a17;*1 $$7,7#70004532496200323422659154056402:requirement]
[{_}%EN-US 
[s3; [%00-00 Date formatting and scanning]&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) const][%00-00 _][%00-00@(0.0.255) char][%00-00 _`*][%00-00@0 StrToDate][%00-00 (
][%00-00^`:`:Date^ Date][%00-00 `&_][%00-00@3 d][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00@(0.0.255) c
har][%00-00 _`*][%00-00@3 s][%00-00 )]&]
[s2; Scans a string for a Date. Order of day, month and year is specified 
using SetDateScan. In place of month both number and text is 
accepted `- text must match abbreviated or full name of month.&]
[s4; [%00-00*C@3 d]-|Found date.&]
[s4; [%00-00*C@3 s]-|String to scan.&]
[s4; [*/ Return value]-|NULL if no date is found in s or position right 
after the date.&]
[s0;3 &]
[s5;K [%00-00^`:`:String^ String][%00-00 _][%00-00@0 Format][%00-00 (][%00-00^`:`:Date^ Date][%00-00 _
][%00-00@3 date][%00-00 )]&]
[s2; Formats date. Date is formated using the standard Format function, 
where the string set by SetDateFormat is used as formating string, 
and the three supplied integer value arguments are year, month 
and day.&]
[s4; [%00-00*C@3 date]-|Date to format.&]
[s4; [*/ Return value]-|Formatted date.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) int][%00-00 _][%00-00@0 CharFilterDate][%00-00 (][%00-00@(0.0.255) int][%00-00 _
][%00-00@3 c][%00-00 )]&]
[s2; Character filter for dates. Its behaviour is specified by SetDateFilter 
function.&]
[s4; [%00-00*C@3 c]-|Character to filter.&]
[s4; [*/ Return value]-|Filtered character.&]
[s0; &]
[s0; [%00-00 void_][%00-00* SetDateFormat][%00-00 (const_char_`*][%00-00*@3 fmt][%00-00 )]&]
[s2; Sets formating string for dates.&]
[s4; [%00-00*C@3 fmt]-|Formating string.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 SetDateScan][%00-00 (][%00-00@(0.0.255) const][%00-00 _
][%00-00@(0.0.255) char][%00-00 _`*][%00-00@3 scan][%00-00 )]&]
[s2; Sets date scan string `- this string represents order of day, 
month and year for StrToDate function.&]
[s4; [%00-00*C@3 scan]-|Date scan string.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 SetDateFilter][%00-00 (][%00-00@(0.0.255) cons
t][%00-00 _][%00-00@(0.0.255) char][%00-00 _`*][%00-00@3 seps][%00-00 )]&]
[s2; Specifies CharFilterDate behaviour. Digits are always allowed 
in CharFilterDate. If there is `"a`" at the beginning of seps 
string, letters are allowed. If there is `"A`", letters are allowed 
and converted to upper`-case. After this optional character, 
list of characters allowed as separator follows. `"`\a`" after 
character designates `"amend`" character `- if used, all characters 
up to next `"amend`" character are replaced by it.&]
[s4; [%00-00*C@3 seps]-|Filter`-string.&]
[s0;3 &]
[s0; ]