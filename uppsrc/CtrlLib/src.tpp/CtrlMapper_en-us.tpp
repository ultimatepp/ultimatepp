topic "CtrlMapper";
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
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 CtrlMapper]]}}&]
[s1;@(0.0.255)3 &]
[s1;:Upp`:`:CtrlMapper`:`:class: [@(0.0.255)3 class][3 _][*3 CtrlMapper]&]
[s2;%% Trivial utility class for mapping values to widgets and back. 
It is sort of similar to CtrlRetriever, but does immediate transfer 
of values based on flag&]
[s2;%% &]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:CtrlMapper`:`:operator`(`)`(Upp`:`:Ctrl`&`,T`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[_^Upp`:`:CtrlMapper^ CtrlMapper][@(0.0.255) `&]_[* operator()]([_^Upp`:`:Ctrl^ C
trl][@(0.0.255) `&]_[*@3 ctrl], [*@4 T][@(0.0.255) `&]_[*@3 val])&]
[s2;%% Sets value of [%-*@3 ctrl] to [%-*@3 val] or [%-*@3 val] to [%-*@3 ctrl] 
based on current direction setting.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CtrlMapper`:`:operator`(`)`(Upp`:`:Ctrl`&`,T`&`,const T`&`): [@(0.0.255) te
mplate]_<[@(0.0.255) class]_[*@4 T]>_[_^Upp`:`:CtrlMapper^ CtrlMapper][@(0.0.255) `&]_[* op
erator()]([_^Upp`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 ctrl], [*@4 T][@(0.0.255) `&]_[*@3 val], 
[@(0.0.255) const] [*@4 T][@(0.0.255) `&]_[*@3 f])&]
[s2;%% Similar to two parameter operator(), but before setting [%-*@3 val] 
to [%-*@3 ctrl], it multiplies it by [%-*@3 f] and before retrieving, 
divides by [%-*@3 f]. Useful as simple conversion tool when e.g. 
the value is to be stored in meters but edited as kilometers.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:CtrlMapper`:`:ToCtrls`(`): [_^Upp`:`:CtrlMapper^ CtrlMapper][@(0.0.255) `&]_[* T
oCtrls]()&]
[s2;%% Sets direction flag to move values to widgets.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:CtrlMapper`:`:ToValues`(`): [_^Upp`:`:CtrlMapper^ CtrlMapper][@(0.0.255) `&]_
[* ToValues]()&]
[s2;%% Sets direction flag to move widgets to values&]
[s3; &]
[s0;%% ]]