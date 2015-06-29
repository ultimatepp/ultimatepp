topic "Assist++ C++ parser directives";
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
[b83;*4 $$10,11#07864147445237544204411237157677:title]
[b42;a42;2 $$11,11#45413000475342174754091244180557:text]
[l321;t246;C@5;1 $$12,12#20902679421464641399138805415013:code]
[{_}%EN-US 
[s10; Assist`+`+ C`+`+ parser directives&]
[s11; Like many other similar tools, Assist`+`+, for performance reasons, 
does not implement full C`+`+ syntax (in other words, it has 
to `"cheat`" on many places). As was already mentioned above, 
the most important drawback of current implementation is that 
it does not pre`-process files, ignores includes and macro expansions. 
Note that errors encountered during parsing are silently ignored 
(unless performed via `"Rescan code`" operation, then errors 
are listed in console).&]
[s11; To resolve some of problems this might cause, Assist supports 
simple directives that are passed to it via `'//`' comments:&]
[ {{3850:6150 [s12;l32; [*+75 //`$`-]]
:: [s11; Code past this directive will [*/ not] be passed to parser.]
:: [s12;l32; [*+75 //`$`+]]
:: [s11; Code past this directive will be passed to parser (stops [*C@5+75 //`$`-]).]
:: [s12;l32; [*+75 //`$_][*/+75 code]&]
[s12;l32; [A@0+75 example:][*+75  //`$ void Foo(Bar`&)]]
:: [s11; code will be passed to parser (adds code that is not part of 
compiled sources)]}}&]
[s0; ]]