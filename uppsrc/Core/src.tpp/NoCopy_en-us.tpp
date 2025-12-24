topic "NoCopy";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 NoCopy]]}}&]
[s1;:Upp`:`:NoCopy: [@(0.0.255)3 class][3  ][*3 NoCopy]&]
[s0; Utility base class to disable copy semantics.&]
[s0; &]
[s0; Inheriting from this class prevents derived classes from being 
copied or copy`-assigned. The copy constructor and copy assignment 
operator are declared as deleted.&]
[s0; &]
[s0; This is useful for classes that manage unique resources such 
as file handles, mutexes, threads or memory ownership.&]
[s3; ]]