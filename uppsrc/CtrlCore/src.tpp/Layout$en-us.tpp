topic "Layout functions";[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[{_}%EN-US 
[s3;%- Layout functions&]
[s0; &]
[s0; &]
[s5;K:`:`:InitLayout`(`:`:Ctrl`&`,L`&`):%- template_<[@(0.0.255) class]_[@4 L]>__[@(0.0.255) v
oid]_[@0 InitLayout]([^`:`:Ctrl^ Ctrl]`&_[@3 ctrl], [^L^ L]`&_[@3 layout])&]
[s2; Assigns layout parameters to member Ctrl variables of [@3 layout], 
and adds them to the specified [*@3 ctrl].&]
[s4; [*C@4 L]-|Class (or structure) with Ctrl variables. Ctrl variables 
must have public access.&]
[s4; [%-*C@3 ctrl]-|Parent Ctrl.&]
[s4; [%-*C@3 layout]-|[*C@4 L] instance.&]
[s0; &]
[s5;K:`:`:CtrlLayout`(T`&`):%- template_<[@(0.0.255) class]_[@4 T]>__[@(0.0.255) void]_[@0 Ct
rlLayout]([^T^ T]`&_[@3 ctrl])&]
[s2; Assigns layout parameters to member variables. Required layout 
must be one of base classes of T.&]
[s4; [*C@4 T]-|Parent Ctrl type.&]
[s4; [%-*C@3 ctrl]-|Parent Ctrl.&]
[s0; &]
[s5;K:`:`:CtrlLayout`(T`&`,const char`*`):%- template_<[@(0.0.255) class]_[@4 T]>__[@(0.0.255) v
oid]_[@0 CtrlLayout]([^T^ T]`&_[@3 ctrl], [@(0.0.255) const]_[@(0.0.255) char]_`*[@3 title])&]
[s2; Calls CtrlLayout and then calls Title method of [*@3 ctrl ](assigning 
a caption text to the TopWindow).&]
[s4; [*C@4 T]-|Parent Ctrl type.&]
[s4; [%-*C@3 ctrl]-|Parent Ctrl.&]
[s4; [%-*C@3 title]-|Window title.&]
[s0; &]
[s5;K:`:`:CtrlLayoutOK`(T`&`,const char`*`):%- template_<[@(0.0.255) class]_[@4 T]>__[@(0.0.255) v
oid]_[@0 CtrlLayoutOK]([^T^ T]`&_[@3 ctrl], [@(0.0.255) const]_[@(0.0.255) char]_`*[@3 title])
&]
[s2; Calls CtrlLayout and then assigns Acceptor(IDOK) to the [/ ok] 
member Ctrl variable and makes it the default button (activated 
by Enter).&]
[s4; [*C@4 T]-|Parent Ctrl type.&]
[s4; [%-*C@3 ctrl]-|Parent Ctrl.&]
[s4; [%-*C@3 title]-|Window title.&]
[s0; &]
[s5;K:`:`:CtrlLayoutCancel`(T`&`,const char`*`):%- template_<[@(0.0.255) class]_[@4 T]>__
[@(0.0.255) void]_[@0 CtrlLayoutCancel]([^T^ T]`&_[@3 ctrl], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[@3 title])&]
[s2; Calls CtrlLayout and then assigns Rejector(IDCANCEL) to the 
[/ cancel] member Ctrl variable and makes it the default cancel 
button (activated by Esc).&]
[s4; [*C@4 T]-|Parent Ctrl type.&]
[s4; [%-*C@3 ctrl]-|Parent Ctrl.&]
[s4; [%-*C@3 title]-|Window title.&]
[s0; &]
[s5;K:`:`:CtrlLayoutOKCancel`(T`&`,const char`*`):%- template_<[@(0.0.255) class]_[@4 T]>
__[@(0.0.255) void]_[@0 CtrlLayoutOKCancel]([^T^ T]`&_[@3 ctrl], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[@3 title])&]
[s2; Calls CtrlLayoutOK and then assigns Rejector(IDCANCEL) to the 
[/ cancel] member Ctrl variable and makes it the default cancel 
button (activated by Esc).&]
[s4; [*C@4 T]-|Parent Ctrl type.&]
[s4; [%-*C@3 ctrl]-|Parent Ctrl.&]
[s4; [%-*C@3 title]-|Window title.&]
[s0; &]
[s5;K:`:`:CtrlLayoutExit`(T`&`,const char`*`):%- template_<[@(0.0.255) class]_[@4 T]>__[@(0.0.255) v
oid]_[@0 CtrlLayoutExit]([^T^ T]`&_[@3 ctrl], [@(0.0.255) const]_[@(0.0.255) char]_`*[@3 titl
e])&]
[s2; Calls CtrlLayoutOK and then assigns Acceptor(IDEXIT) to the 
[/ exit] member Ctrl variable and makes it the default button (activated 
by Enter).&]
[s4; [*C@4 T]-|Parent Ctrl type.&]
[s4; [%-*C@3 ctrl]-|Parent Ctrl.&]
[s4; [%-*C@3 title]-|Window title.&]
[s0; ]