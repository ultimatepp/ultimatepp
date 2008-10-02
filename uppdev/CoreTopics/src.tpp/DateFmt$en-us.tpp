topic "Date formatting and scanning";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a17;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[{_}%EN-US 
[s0;%- Date formatting and scanning&]
[s0; &]
[s5;K%- [@(0.0.255) const][@(64) _][@(0.0.255) char][@(64) _`*]StrToDate[@(64) (][^`:`:Date^@(64) D
ate][@(64) `&_][@3 d][@(64) , ][@(0.0.255) const][@(64) _][@(0.0.255) char][@(64) _`*][@3 s][@(64) )
]&]
[s2; Scans a string for a Date. Order of day, month and year is specified 
using SetDateScan. In place of month both number and text is 
accepted `- text must match abbreviated or full name of month.&]
[s7; [%-*C@3 d]-|Found date.&]
[s7; [%-*C@3 s]-|String to scan.&]
[s7; [*/ Return value]-|NULL if no date is found in s or position right 
after the date.&]
[s0; &]
[s5;K%- [^`:`:String^ String]_Format[@(64) (][^`:`:Date^@(64) Date][@(64) _][@3 date][@(64) )]&]
[s2; Formats date. Date is formated using the standard Format function, 
where the string set by SetDateFormat is used as formating string, 
and the three supplied integer value arguments are year, month 
and day.&]
[s7; [%-*C@3 date]-|Date to format.&]
[s7; [*/ Return value]-|Formatted date.&]
[s0; &]
[s5;K%- [@(0.0.255) int][@(64) _]CharFilterDate[@(64) (][@(0.0.255) int][@(64) _][@3 c][@(64) )]&]
[s2; Character filter for dates. Its behaviour is specified by SetDateFilter 
function.&]
[s7; [%-*C@3 c]-|Character to filter.&]
[s7; [*/ Return value]-|Filtered character.&]
[s0; &]
[s0;%- void_[* SetDateFormat](const_char_`*[*@3 fmt])&]
[s2; Sets formating string for dates.&]
[s7; [%-*C@3 fmt]-|Formating string.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]SetDateScan[@(64) (][@(0.0.255) const][@(64) _][@(0.0.255) char
][@(64) _`*][@3 scan][@(64) )]&]
[s2; Sets date scan string `- this string represents order of day, 
month and year for StrToDate function.&]
[s7; [%-*C@3 scan]-|Date scan string.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]SetDateFilter[@(64) (][@(0.0.255) const][@(64) _][@(0.0.255) ch
ar][@(64) _`*][@3 seps][@(64) )]&]
[s2; Specifies CharFilterDate behaviour. Digits are always allowed 
in CharFilterDate. If there is `"a`" at the beginning of seps 
string, letters are allowed. If there is `"A`", letters are allowed 
and converted to upper`-case. After this optional character, 
list of characters allowed as separator follows. `"`\a`" after 
character designates `"amend`" character `- if used, all characters 
up to next `"amend`" character are replaced by it.&]
[s7; [%-*C@3 seps]-|Filter`-string.&]
[s0; &]
[s0; ]