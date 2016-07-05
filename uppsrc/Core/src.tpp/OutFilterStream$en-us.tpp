topic "OutFilterStream";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 OutFilterStream]]}}&]
[s0;i448;a25;kKO9;@(0.0.255) &]
[s1;:OutFilterStream`:`:class: [@(0.0.255)3 class][3 _][*3 OutFilterStream][3 _:_][@(0.0.255)3 p
ublic][3 _][*@3;3 Stream]&]
[s2;%% Adapter Stream that glues an output stream with some filtering 
object, typically of compression/decompression class. Output 
stream can also be omitted, in that case OutFilterStream is useful 
to convert anything capable of consuming data into Stream.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Member List]]}}&]
[s3; &]
[s5;:OutFilterStream`:`:Close`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Close]()&]
[s2;%% Finalizes operations. Does [*/ not] closes the output stream. 
Repeated call does nothing.&]
[s3; &]
[s3; &]
[s4; &]
[s5;:OutFilterStream`:`:out: [_^Stream^ Stream]_`*[* out]&]
[s2;%% Pointer to the output stream. Can be NULL.&]
[s3; &]
[s4; &]
[s5;:OutFilterStream`:`:Filter: [_^Callback2^ Callback2]<[@(0.0.255) const]_[@(0.0.255) voi
d]_`*, [@(0.0.255) int]>_[* Filter]&]
[s2;%% Callback to filter input function. This is called when OutFilterStream 
needs to output a chunk of buffered data.&]
[s3; &]
[s4; &]
[s5;:OutFilterStream`:`:End: [_^Callback^ Callback]_[* End]&]
[s2;%% Callback to filter finalization. Called on closing the stream.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:OutFilterStream`:`:WhenPos: [_^Upp`:`:Event^ Event]<[_^Upp`:`:int64^ int64]>_
[* WhenPos]&]
[s2;%% This is invoked when OutFilterStream passes data to the filter. 
Parameter is number of unfiltered bytes so far. Useful for progress 
indicators.&]
[s3; &]
[s4; &]
[s5;:OutFilterStream`:`:Out`(const void`*`,int`): [@(0.0.255) void]_[* Out]([@(0.0.255) con
st]_[@(0.0.255) void]_`*[*@3 ptr], [@(0.0.255) int]_[*@3 size])&]
[s2;%% Method serving as filter output. Basically performs out`->Put(ptr, 
size), also keeps track of output bytes written (see GetCount).&]
[s3;%% &]
[s4; &]
[s5;:OutFilterStream`:`:GetCount`(`)const: [@(0.0.255) int64]_[* GetCount]()_[@(0.0.255) co
nst]&]
[s2;%% Returns a number of bytes written to the output stream (if 
OutFilterStream is used for compression, this is the compressed 
size). Note that this number is `'complete`' only after the OutFilterStream 
is closed, because of buffering of data chunks.&]
[s3; &]
[s4;%% &]
[s5;:OutFilterStream`:`:Set`(Stream`&`,F`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 F
]>_[@(0.0.255) void]_[* Set]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 out`_], 
[*@4 F][@(0.0.255) `&]_[*@3 filter])&]
[s2;%% Sets the output stream and filter. Filter must have WhenOut 
callback which is connected to Out method and Put and End methods 
that are connected to Filter and End callbacks.&]
[s3;%% &]
[s4;%% &]
[s5;:OutFilterStream`:`:OutFilterStream`(`): [* OutFilterStream]()&]
[s2;%% Default constructor.&]
[s3;%% &]
[s4;%% &]
[s5;:OutFilterStream`:`:OutFilterStream`(Stream`&`,F`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 F]>_[* OutFilterStream]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 in], 
[*@4 F][@(0.0.255) `&]_[*@3 filter])&]
[s2;%% Equivalent of default constructor followed by Set.&]
[s3;%% &]
[s4; &]
[s5;:OutFilterStream`:`:`~OutFilterStream`(`): [@(0.0.255) `~][* OutFilterStream]()&]
[s2;%% Calls Close.&]
[s0;%% ]]