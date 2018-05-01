topic "CtrlRetriever";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 CtrlRetriever]]}}&]
[s0; &]
[s1;:Upp`:`:CtrlRetriever`:`:class: [@(0.0.255)3 class][3 _][*3 CtrlRetriever]&]
[s2;%% Small helper class used to map data to widget (usually in 
context of dialog). Usually scenario is that before opening dialog, 
data are are put into widget through CtrlRetriever Put method 
(it has operator() alias). Then, after user edits data in dialog, 
they are moved back from widget to variables using CtrlRetriever`::Retrieve 
method.&]
[s2;%% &]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:CtrlRetriever`:`:Put`(Upp`:`:Ctrl`&`,T`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[@(0.0.255) void]_[* Put]([_^Upp`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 ctrl], 
[*@4 T][@(0.0.255) `&]_[*@3 val])&]
[s5;:Upp`:`:CtrlRetriever`:`:operator`(`)`(Upp`:`:Ctrl`&`,T`&`): [@(0.0.255) template]_
<[@(0.0.255) class]_[*@4 T]>_[_^Upp`:`:CtrlRetriever^ CtrlRetriever][@(0.0.255) `&]_[* oper
ator()]([_^Upp`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 ctrl], [*@4 T][@(0.0.255) `&]_[*@3 val])&]
[s2;%% Assigns [%-*@3 val] to [%-*@3 ctrl] and adds this to widget `- 
data mappings (to be used by Retrieve).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CtrlRetriever`:`:Set`(`): [@(0.0.255) void]_[* Set]()&]
[s2;%% Reassigns values to widgets.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:CtrlRetriever`:`:Retrieve`(`): [@(0.0.255) void]_[* Retrieve]()&]
[s2;%% Moves data mapped using Put from widgets back to variables.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:CtrlRetriever`:`:operator`<`<`(Upp`:`:Event`<`>`): [_^Upp`:`:Event^ Event]<
>_[* operator<<]([_^Upp`:`:Event^ Event]<>_[*@3 cb])&]
[s2;%% Adds [%-*@3 cb] event to WhenAction events of all mapped widgets.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CtrlRetriever`:`:operator`^`=`(Upp`:`:Event`<`>`): [_^Upp`:`:Event^ Event]<
>_[* operator`^`=]([_^Upp`:`:Event^ Event]<>_[*@3 cb])&]
[s2;%% Sets WhenAction of all mapped widgets to [%-*@3 cb].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CtrlRetriever`:`:Reset`(`): [@(0.0.255) void]_[* Reset]()&]
[s2;%% Resets CtrlRetriever to initial state `- all mapping between 
widgets and data are removed and CtrlRetriever can be used for 
another set of data/widgets.&]
[s3; &]
[s0;%% ]]