topic "LZ4 compression";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 LZ4CompressStream ]]}}&]
[s3; &]
[s1;:Upp`:`:LZ4CompressStream`:`:class: [@(0.0.255)3 class][3 _][*3 LZ4CompressStream][3 _:_][@(0.0.255)3 p
ublic][3 _][*@3;3 Stream]&]
[s2;%% Provides [^https`:`/`/github`.com`/Cyan4973`/lz4^ LZ4] compression. 
LZ4 is extremely fast (400MB/s per CPU core) compression algorithm 
by Yann Collet, with average compression ration at around 60%. 
Compressed stream follows [^https`:`/`/docs`.google`.com`/document`/d`/1cl8N1bmkTdIpPLtnlzbBSFAdUeyNo5fwfHbHU7VRNWY`/edit^ L
Z4 frame format] (and thus can be decompressed by commandline 
routines provided by LZ4 author, Yann Collet).&]
[s0;i448;a25;kKO9;:noref:@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:LZ4CompressStream`:`:Co`(bool`): [@(0.0.255) void]_[* Co]([@(0.0.255) bool]_[*@3 b
]_`=_[@(0.0.255) true])&]
[s2;%% Activates multithreaded mode. The parallelization is completely 
encapsulated, nothing special is required from calling thread.&]
[s3;%% &]
[s4;%% &]
[s5;:Upp`:`:LZ4CompressStream`:`:Open`(Upp`:`:Stream`&`): [@(0.0.255) void]_[* Open]([_^Upp`:`:Stream^ S
tream][@(0.0.255) `&]_[*@3 out`_])&]
[s5;:Upp`:`:LZ4CompressStream`:`:LZ4CompressStream`(Upp`:`:Stream`&`): [* LZ4CompressSt
ream]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out])&]
[s2;%% Sets the output stream where compressed data are stored.&]
[s0;*@7;4%% &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 LZ4DecompressStream ]]}}&]
[s3; &]
[s1;:Upp`:`:LZ4DecompressStream`:`:class: [@(0.0.255)3 class][3 _][*3 LZ4DecompressStream][3 _
:_][@(0.0.255)3 public][3 _][*@3;3 Stream]&]
[s2;%% Provides LZ4 decompression. It is guaranteed to decompress 
stream compressed by plugin/lz4 and [/ most] (but not all) of lz4`-frame 
format compliant streams.&]
[s9; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3;%% &]
[s5;:Upp`:`:LZ4DecompressStream`:`:Co`(bool`): [@(0.0.255) void]_[* Co]([@(0.0.255) bool]_[*@3 b
]_`=_[@(0.0.255) true])&]
[s2;%% Activates multithreaded mode. The parallelization is completely 
encapsulated, nothing special is required from calling thread.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:LZ4DecompressStream`:`:Open`(Upp`:`:Stream`&`): [@(0.0.255) bool]_[* Open]([_^Upp`:`:Stream^ S
tream][@(0.0.255) `&]_[*@3 in])&]
[s5;:Upp`:`:LZ4DecompressStream`:`:LZ4DecompressStream`(Upp`:`:Stream`&`): [* LZ4Decomp
ressStream]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in])&]
[s2;%% Sets the input stream from where compressed data are read.&]
[s0;*@7;4%% &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 LZ4 Compress/Decompress functions]]}}&]
[s3; &]
[s5;:Upp`:`:LZ4Compress`(Upp`:`:Stream`&`,Upp`:`:Stream`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:int64^ i
nt64]_[* LZ4Compress]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out], 
[_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ i
nt64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s5;:Upp`:`:LZ4Decompress`(Upp`:`:Stream`&`,Upp`:`:Stream`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:int64^ i
nt64]_[* LZ4Decompress]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out], 
[_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ i
nt64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s5;:Upp`:`:LZ4Compress`(Upp`:`:Stream`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* LZ4Compress]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], 
[_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 progress])&]
[s5;:Upp`:`:LZ4Decompress`(Upp`:`:Stream`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* LZ4Decompress]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], 
[_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 progress])&]
[s5;:Upp`:`:LZ4Compress`(const void`*`,Upp`:`:int64`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* LZ4Compress]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], 
[_^Upp`:`:int64^ int64]_[*@3 len], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], 
[_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s5;:Upp`:`:LZ4Compress`(const Upp`:`:String`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* LZ4Compress]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 s],
 [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=
_Null)&]
[s5;:Upp`:`:LZ4Decompress`(const void`*`,Upp`:`:int64`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* LZ4Decompress]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], 
[_^Upp`:`:int64^ int64]_[*@3 len], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], 
[_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s5;:Upp`:`:LZ4Decompress`(const Upp`:`:String`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* LZ4Decompress]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 s
], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_
`=_Null)&]
[s5;:Upp`:`:CoLZ4Compress`(Upp`:`:Stream`&`,Upp`:`:Stream`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:int64^ i
nt64]_[* CoLZ4Compress]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out], 
[_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ i
nt64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s5;:Upp`:`:CoLZ4Decompress`(Upp`:`:Stream`&`,Upp`:`:Stream`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:int64^ i
nt64]_[* CoLZ4Decompress]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out], 
[_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ i
nt64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s5;:Upp`:`:CoLZ4Compress`(Upp`:`:Stream`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* CoLZ4Compress]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], 
[_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 progress])&]
[s5;:Upp`:`:CoLZ4Decompress`(Upp`:`:Stream`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* CoLZ4Decompress]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], 
[_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 progress])&]
[s5;:Upp`:`:CoLZ4Compress`(const void`*`,Upp`:`:int64`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* CoLZ4Compress]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], 
[_^Upp`:`:int64^ int64]_[*@3 len], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], 
[_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s5;:Upp`:`:CoLZ4Compress`(const Upp`:`:String`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* CoLZ4Compress]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 s
], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_
`=_Null)&]
[s5;:Upp`:`:CoLZ4Decompress`(const void`*`,Upp`:`:int64`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* CoLZ4Decompress]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], 
[_^Upp`:`:int64^ int64]_[*@3 len], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], 
[_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s5;:Upp`:`:CoLZ4Decompress`(const Upp`:`:String`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* CoLZ4Decompress]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 s
], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_
`=_Null)&]
[s2;%% Simple LZ4 compression/decompression functions. [%-*@3 out] 
[%-*@3 in] parameters are input / ouput streams and function returns 
the number of bytes stored to [%-*@3 out], otherwise input is provided 
as memory block [%-*@3 data] of [%-*@3 len] bytes or as input String 
[%-*@3 data] and output is String return value. [%-*@3 progress] 
can be used to to track progress of operation and eventually 
interrupt it by returning true. Functions with Co prefix run 
multithreaded.&]
[s2;%% &]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:IsLZ4`(Upp`:`:Stream`&`): [@(0.0.255) bool]_[* IsLZ4]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&
]_[*@3 s])&]
[s2;%% Checks the Stream for magic number identifying LZ4 stream. 
Seeks back after the check. Returns true if magic number was 
detected.&]
[s3;%% &]
[s3;%% &]
[s0;%% ]]