topic "Layout functions";
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
[{_}%EN-US 
[ {{10000@(113.42.0) [s0;%- [*@7;4 Layout functions]]}}&]
[s3; &]
[s0; &]
[s5;:InitLayout`(Ctrl`&`,L`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 L]>_[@(0.0.255) v
oid]_[* InitLayout]([_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 ctrl], [*@4 L][@(0.0.255) `&]_[*@3 layou
t])&]
[s2; Assigns layout parameters to member Ctrl variables of [@3 layout], 
and adds them to the specified [*@3 ctrl].&]
[s7; [*C@4 L]-|Class (or structure) with Ctrl variables. Ctrl variables 
must have public access.&]
[s7; [%-*C@3 ctrl]-|Parent Ctrl.&]
[s7; [%-*C@3 layout]-|[*C@4 L] instance.&]
[s3; &]
[s4;%- &]
[s5;:CtrlLayout`(T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) void
]_[* CtrlLayout]([*@4 T][@(0.0.255) `&]_[*@3 ctrl])&]
[s2; Assigns layout parameters to member variables. Required layout 
must be one of base classes of T.&]
[s7; [*C@4 T]-|Parent Ctrl type.&]
[s7; [%-*C@3 ctrl]-|Parent Ctrl.&]
[s3; &]
[s4;%- &]
[s5;:CtrlLayout`(T`&`,const char`*`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_
[@(0.0.255) void]_[* CtrlLayout]([*@4 T][@(0.0.255) `&]_[*@3 ctrl], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 title])&]
[s2; Calls CtrlLayout and then calls Title method of [*@3 ctrl ](assigning 
a caption text to the TopWindow).&]
[s7; [*C@4 T]-|Parent Ctrl type.&]
[s7; [%-*C@3 ctrl]-|Parent Ctrl.&]
[s7; [%-*C@3 title]-|Window title.&]
[s3; &]
[s4;%- &]
[s5;:CtrlLayoutOK`(T`&`,const char`*`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>
_[@(0.0.255) void]_[* CtrlLayoutOK]([*@4 T][@(0.0.255) `&]_[*@3 ctrl], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 title])&]
[s2; Calls CtrlLayout and then assigns Acceptor(IDOK) to the [/ ok] 
member Ctrl variable and makes it the default button (activated 
by Enter).&]
[s7; [*C@4 T]-|Parent Ctrl type.&]
[s7; [%-*C@3 ctrl]-|Parent Ctrl.&]
[s7; [%-*C@3 title]-|Window title.&]
[s3; &]
[s4;%- &]
[s5;:CtrlLayoutCancel`(T`&`,const char`*`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
]>_[@(0.0.255) void]_[* CtrlLayoutCancel]([*@4 T][@(0.0.255) `&]_[*@3 ctrl], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 title])&]
[s2; Calls CtrlLayout and then assigns Rejector(IDCANCEL) to the 
[/ cancel] member Ctrl variable and makes it the default cancel 
button (activated by Esc).&]
[s7; [*C@4 T]-|Parent Ctrl type.&]
[s7; [%-*C@3 ctrl]-|Parent Ctrl.&]
[s7; [%-*C@3 title]-|Window title.&]
[s3; &]
[s4;%- &]
[s5;:CtrlLayoutOKCancel`(T`&`,const char`*`):%- [@(0.0.255) template]_<[@(0.0.255) class]_
[*@4 T]>_[@(0.0.255) void]_[* CtrlLayoutOKCancel]([*@4 T][@(0.0.255) `&]_[*@3 ctrl], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 title])&]
[s2; Calls CtrlLayoutOK and then assigns Rejector(IDCANCEL) to the 
[/ cancel] member Ctrl variable and makes it the default cancel 
button (activated by Esc).&]
[s7; [*C@4 T]-|Parent Ctrl type.&]
[s7; [%-*C@3 ctrl]-|Parent Ctrl.&]
[s7; [%-*C@3 title]-|Window title.&]
[s3; &]
[s4;%- &]
[s5;:CtrlLayoutExit`(T`&`,const char`*`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
]>_[@(0.0.255) void]_[* CtrlLayoutExit]([*@4 T][@(0.0.255) `&]_[*@3 ctrl], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 title])&]
[s2; Calls CtrlLayoutOK and then assigns Acceptor(IDEXIT) to the 
[/ exit] member Ctrl variable and makes it the default button (activated 
by Enter).&]
[s7; [*C@4 T]-|Parent Ctrl type.&]
[s7; [%-*C@3 ctrl]-|Parent Ctrl.&]
[s7; [%-*C@3 title]-|Window title.&]
[s3; &]
[s0; ]]