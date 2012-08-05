topic "CoWork";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 CoWork]]}}&]
[s3; &]
[s1;:CoWork`:`:class: [@(0.0.255)3 class][3 _][*3 CoWork]&]
[s9;%% This class is indented as loop`-parallelization tool. Whenever 
loop iterations are independent (they do not share any data between 
iterations), CoWork can be used to relatively easily spawn loop 
iterations over threads and thus over CPU cores. Note that previous 
statement does [* not] preclude CoWork iterations to share data 
at all `- sharing data using Mutex or similar serialization mechanisms 
still works. CoWork works with fixed`-size global thread pool, 
which is created during initialization phase (first CoWork constructor 
called). No more thread are created or destroyed during normal 
work. Nesting of CoWork instances is also possible. Of course, 
not only loop iterations can be parallelized, whenever there 
are two or more actions that can run in parallel, you can use 
CoWork.&]
[s9;%% Single`-threaded implementation simply performs all actions 
submitted by Do in sequence.&]
[s3;%% &]
[s0;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s4;H0; &]
[s5;:CoWork`:`:Do`(Callback`): [@(0.0.255) void]_[* Do]([_^Callback^ Callback]_[*@3 cb])&]
[s2;%% Adds [%-*@3 cb] to be performed by any thread.&]
[s3;%% &]
[s4; &]
[s5;:CoWork`:`:operator`&`(Callback`): [_^CoWork^ CoWork][@(0.0.255) `&]_[* operator`&]([_^Callback^ C
allback]_[*@3 cb])&]
[s2;%% Same as Do([%-*@3 cb].); return `*this;&]
[s3;%% &]
[s4; &]
[s5;:CoWork`:`:Finish`(`): [@(0.0.255) void]_[* Finish]()&]
[s2;%% Waits until all jobs submitted using Do (or operator`&) are 
finished.&]
[s3;%% &]
[s0;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Destructor detail]]}}&]
[s3; &]
[s5;:CoWork`:`:`~CoWork`(`): [@(0.0.255) `~][* CoWork]()&]
[s2;%% Calls Finish().&]
[s3; &]
[s0; ]]