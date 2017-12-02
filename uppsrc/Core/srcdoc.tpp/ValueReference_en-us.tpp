topic "Rules for references to Value internal maps elements";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[a83;*R6 $$10,12#31310162474203024125188417583966:caption]
[b83;*2 $$11,12#07864147445237544204411237157677:title]
[b42;a42;2 $$12,12#45413000475342174754091244180557:text]
[l321;t246;C@5;1 $$13,13#20902679421464641399138805415013:code]
[{_}%EN-US 
[s10; Rules for references to ValueArray, ValueMap and Value elements&]
[s0; Value is originally designed as non`-mutable concrete Value 
of different concrete types.&]
[s0; &]
[s0; However, as Value is commonly used to store complex hierarchies 
(using ValueArray/ValueMap), it is in the end very useful to 
be able to reference ValueArray/ValueMap elements directly. Validity 
of such references is restricted by following rule:&]
[s0; &]
[s0; Reference obtained from Value/ValueArray/ValueMap (by Value`::At, 
Value`::GetAdd, ValueArray`::At, ValueMap`::GetAdd and derivative 
methods) are [* only valid until the next operation on originating 
Value `- including just reading it.]&]
[s0; &]
[s0; Examples of invalid code:&]
[s0; &]
[s13; &]
[s13; Value m;&]
[s13; Value`& x `= m(`"key`");&]
[s13; x `= m; // using m as source invalidates x&]
[s13; ....&]
[s13; Value m;&]
[s13; Value`& x `= m(`"key`");&]
[s13; Value`& y `= m(`"key2`"); // Invalidates x&]
[s13; x `= 123; // undefined&]
[s13; ....&]
[s13; Value m;&]
[s13; Value`& x `= m.At(1);&]
[s13; Value m2 `= m; // Invalidates x&]
[s13; x `= `"fail`"; // undefined&]
[s0; ]]