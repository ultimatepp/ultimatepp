topic "Supported operators";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s0; [* Debugger expressions syntax]&]
[s0;@(0.0.255)1 &]
[s0; [*@(0.0.255)1 0x][1  prefix for hex (e.g. 0xfe)]&]
[s0; [*@(0.0.255)1 0][1  prefix for octal numbers (e.g. 0337)]&]
[s0; [@(0.0.255)1 `' `'][1  ASCII code for character (e.g. `'a`', `'`\n`')]&]
[s0; [*@(0.0.255)1 `::][1  can be used to specify scope]&]
[s0; [*@(0.0.255)1 this][1  variable exists in methods]&]
[s0;1 &]
[ {{2165:7835^@7 [s0; [1 Operator (by priority)]]
:: [s0; [1 Note]]
::@2 [s0; [*@(0.0.255)1 . `-> `[`] ()]]
:: [s0; [1 Postfix operators. `'.`' and `'`->`' are synonyms (work based 
on actual context). ][*@(0.0.255)1 ()][1  has the same meaning as 
][*@(0.0.255)1 `[`]][1  unless the value is recognized high`-level 
map type (e.g. VectorMap) `- in that case it returns the key 
while ][*@(0.0.255)1 `[`]][1  returns the value. For recognized high`-level 
types, omitting argument of ][*@(0.0.255)1 `[`] ()][1  references 
the last item in the list, while negative argument is addressing 
elements from the last element; `-1 is the last element, `-2 
second last element etc...]]
:: [s0; [*@(0.0.255)1 `- `+ `* `& ! #]]
:: [s0; [1 Unary operators. Operator ][*@(0.0.255)1 #][1  represents a count 
of elements in recognized high`-level type (e.g. Vector`::GetCount() 
or std`::vector`::size()).]]
:: [s0; [*@(0.0.255)1 `* / %]]
:: [s0; [1 Binary operators.]]
:: [s0; [*@(0.0.255)1 `+ `-]]
:: [s0; [1 Binary operators.]]
:: [s0; [*@(0.0.255)1 `=`= !`= < > <`= >`=]]
:: [s0; [1 Note that all of these have the same priority. It is possible 
to directly compare pointer with integer.]]
:: [s0; [*@(0.0.255)1 `&`&]]
:: [s0; [1 Logical AND.]]
:: [s0; [*@(0.0.255)1 `|`|]]
:: [s0; [1 Logical OR]]}}&]
[s0;1 ]]