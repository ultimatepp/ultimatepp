topic "InFilterStream";
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
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 InFilterStream]]}}&]
[s0;i448;a25;kKO9;@(0.0.255) &]
[s1;:InFilterStream`:`:class: [@(0.0.255)3 class][3 _][*3 InFilterStream][3 _:_][@(0.0.255)3 pub
lic][3 _][*@3;3 Stream]&]
[s2;%% Adapter Stream that glues an input stream with some filtering 
object, typically of compression/decompression class.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:InFilterStream`:`:in: [_^Stream^ Stream]_`*[* in]&]
[s2;%% Pointer to input stream that serves as the source of data 
to be filtered.&]
[s3; &]
[s4; &]
[s5;:InFilterStream`:`:Filter: [_^Callback2^ Callback2]<[@(0.0.255) const]_[@(0.0.255) void
]_`*, [@(0.0.255) int]>_[* Filter]&]
[s2;%% Callback to filter input function.&]
[s3; &]
[s4; &]
[s5;:InFilterStream`:`:End: [_^Callback^ Callback]_[* End]&]
[s2;%% Callback to filter finalization.&]
[s3; &]
[s4; &]
[s5;:InFilterStream`:`:More: [_^Gate^ Gate]_[* More]&]
[s2;%% Callback used for alternate mode of operation where instead 
of input stream and filter only single data source is used.&]
[s3; &]
[s4; &]
[s5;:InFilterStream`:`:Out`(const void`*`,int`): [@(0.0.255) void]_[* Out]([@(0.0.255) cons
t]_[@(0.0.255) void]_`*[*@3 ptr], [@(0.0.255) int]_[*@3 size])&]
[s2;%% Method serving as filter output.&]
[s3;%% &]
[s4;%% &]
[s5;:InFilterStream`:`:Set`(Stream`&`,F`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 F
]>_[@(0.0.255) void]_[* Set]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 in`_], 
[*@4 F][@(0.0.255) `&]_[*@3 filter])&]
[s2;%% Sets the input stream and filter. Filter must have WhenOut 
callback which is connected to Out method and Put and End methods 
that are connected to Filter and End callbacks.&]
[s3;%% &]
[s4; &]
[s5;:InFilterStream`:`:SetSize`(int64`): [@(0.0.255) void]_[* SetSize]([_^int64^ int64]_[*@3 s
ize])&]
[s2;%% Sets the [%-*@3 size] of Stream that then reported by GetSize 
method. This is important in situations where code requires to 
know the size if input stream (e.g. serialization) but it is 
not known because filter does not provide such information.&]
[s3;%% &]
[s4;%% &]
[s5;:InFilterStream`:`:InFilterStream`(`): [* InFilterStream]()&]
[s2;%% Default constructor.&]
[s3;%% &]
[s4;%% &]
[s5;:InFilterStream`:`:InFilterStream`(Stream`&`,F`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 F]>_[* InFilterStream]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 in], 
[*@4 F][@(0.0.255) `&]_[*@3 filter])&]
[s2;%% Equivalent of default constructor followed by Set.&]
[s0;%% ]]