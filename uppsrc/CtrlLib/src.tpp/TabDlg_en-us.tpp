topic "TabDlg";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_} 
[ {{10000@3 [s0; [*@(229)4 TabDlg]]}}&]
[s3; &]
[s1;:TabDlg`:`:class: [@(0.0.255)3 class][3 _][*3 TabDlg][3 _:_][@(0.0.255)3 public][3 _][*@3;3 TopW
indow]&]
[s9;%% TabDlg is a class intended to simplify creation of standard 
multi`-tabbed dialogs `- dialogs with at most 4 predefined buttons 
and TabCtrl.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Field List]]}}&]
[s3; &]
[s5;:TabDlg`:`:tabctrl: [_^TabCtrl^ TabCtrl]_[* tabctrl]&]
[s2;%% TabCtrl used in dialog. Presented in interface to allow modification 
of properties.&]
[s3; &]
[s4; &]
[s5;:TabDlg`:`:ok: [_^Button^ Button]_[* ok]&]
[s5;:TabDlg`:`:cancel: [_^Button^ Button]_[* cancel]&]
[s5;:TabDlg`:`:exit: [_^Button^ Button]_[* exit]&]
[s5;:TabDlg`:`:apply: [_^Button^ Button]_[* apply]&]
[s2;%% Dialog buttons. Presented in interface to allow modification 
of properties.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:TabDlg`:`:Add`(T`&`,const char`*`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T][@(0.0.255) >
]_[_^TabCtrl`:`:Item^ TabCtrl`::Item][@(0.0.255) `&]_[* Add]([*@4 T][@(0.0.255) `&]_[*@3 tab],
 [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text])&]
[s5;:TabDlg`:`:Add`(T`&`,const Image`&`,const char`*`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T][@(0.0.255) >]_[_^TabCtrl`:`:Item^ TabCtrl`::Item][@(0.0.255) `&]_[* Add]([*@4 T][@(0.0.255) `&
]_[*@3 tab], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 name])&]
[s2;%% Adds [*@3 tab] dialog pane to the TabCtrl tab named [*@3 text], 
optionally with [%-*@3 img]. The size of dialog window is determined 
by calling GetRect() for individual [*@3 tab]s (note that CtrlLayout 
functions set this to the designed size).&]
[s3; &]
[s4; &]
[s5;:TabDlg`:`:operator`(`)`(T`&`,const char`*`): [@(0.0.255) template]_<[@(0.0.255) clas
s]_[*@4 T][@(0.0.255) >]_[_^TabDlg^ TabDlg][@(0.0.255) `&]_[* operator()]([*@4 T][@(0.0.255) `&
]_[*@3 tab], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text])&]
[s5;:TabDlg`:`:operator`(`)`(T`&`,const Image`&`,const char`*`): [@(0.0.255) template]_
<[@(0.0.255) class]_[*@4 T][@(0.0.255) >]_[_^TabDlg^ TabDlg][@(0.0.255) `&]_[* operator()]([*@4 T
][@(0.0.255) `&]_[*@3 tab], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 txt])&]
[s2; Operator versions of [* Add] for more compact code.&]
[s3; &]
[s4; &]
[s5;:TabDlg`:`:OK`(`): [_^TabDlg^ TabDlg][@(0.0.255) `&]_[* OK]()&]
[s5;:TabDlg`:`:Cancel`(`): [_^TabDlg^ TabDlg][@(0.0.255) `&]_[* Cancel]()&]
[s5;:TabDlg`:`:OKCancel`(`): [_^TabDlg^ TabDlg][@(0.0.255) `&]_[* OKCancel]()&]
[s5;:TabDlg`:`:Exit`(`): [_^TabDlg^ TabDlg][@(0.0.255) `&]_[* Exit]()&]
[s5;:TabDlg`:`:Apply`(`): [_^TabDlg^ TabDlg][@(0.0.255) `&]_[* Apply]()&]
[s2; These methods add buttons to dialog.&]
[s3; &]
[s0; ]