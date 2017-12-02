topic "Zstd compression";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 ZstdCompressStream]]}}&]
[s3; &]
[s1;:Upp`:`:ZstdCompressStream`:`:class: [@(0.0.255)3 class][3 _][*3 ZstdCompressStream][3 _:_
][@(0.0.255)3 public][3 _][*@3;3 Stream]&]
[s2;%% [^https`:`/`/github`.com`/Cyan4973`/zstd^ Zstandard] is a very 
fast (200MB/s per CPU core) compression algorithm with good compression 
ratio (average 50%) by Yann Collet. File compressed using ZstdCompressStream 
can be decompressed by Zstd commandline utilities.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:ZstdCompressStream`:`:Co`(bool`): [@(0.0.255) void]_[* Co]([@(0.0.255) bool]_[*@3 b
]_`=_[@(0.0.255) true])&]
[s2;%% Activates multithreaded mode. The parallelization is completely 
encapsulated, nothing special is required from calling thread.&]
[s3;%% &]
[s4;%% &]
[s5;:Upp`:`:ZstdCompressStream`:`:Open`(Upp`:`:Stream`&`,int`): [@(0.0.255) void]_[* Open
]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out], [@(0.0.255) int]_[*@3 level]_`=_[@3 1])
&]
[s2;%%  [%-*@3 out] [%-*@3 level] .&]
[s3;%% &]
[s4;%% &]
[s5;:Upp`:`:ZstdCompressStream`:`:ZstdCompressStream`(Upp`:`:Stream`&`,int`): [* ZstdCo
mpressStream]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out], [@(0.0.255) int]_[*@3 lev
el]_`=_[@3 1])&]
[s2;%% Sets the output stream where compressed data are stored. [%-*@3 level] 
is the compression level `- higher levels provide better compression 
but slower speed. For fast compression, level 1 is recommended.&]
[s0;*@7;4%% &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 ZstdDecompressStream]]}}&]
[s3; &]
[s1;:Upp`:`:ZstdDecompressStream`:`:class: [@(0.0.255)3 class][3 _][*3 ZstdDecompressStream
][3 _:_][@(0.0.255)3 public][3 _][*@3;3 Stream]&]
[s2;%% Provides Zstd decompression.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3;%% &]
[s5;:Upp`:`:ZstdDecompressStream`:`:Co`(bool`): [@(0.0.255) void]_[* Co]([@(0.0.255) bool]_
[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% Activates multithreaded mode. The parallelization is completely 
encapsulated, nothing special is required from calling thread.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:ZstdDecompressStream`:`:Open`(Upp`:`:Stream`&`): [@(0.0.255) bool]_[* Open]([_^Upp`:`:Stream^ S
tream][@(0.0.255) `&]_[*@3 in])&]
[s5;:Upp`:`:ZstdDecompressStream`:`:ZstdDecompressStream`(Upp`:`:Stream`&`): [* ZstdDec
ompressStream]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in])&]
[s2;%% Sets the input stream from where compressed data are read.&]
[s3;%% &]
[s0;%% &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 Zstd Compress/Decompress functions]]}}&]
[s3; &]
[s5;:Upp`:`:ZstdCompress`(Upp`:`:Stream`&`,Upp`:`:Stream`&`,Upp`:`:EventGate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:int64^ i
nt64]_[* ZstdCompress]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out], 
[_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], [_^Upp`:`:EventGate^ EventGate]<[_^Upp`:`:int64^ i
nt64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:Upp`:`:ZstdDecompress`(Upp`:`:Stream`&`,Upp`:`:Stream`&`,Upp`:`:EventGate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:int64^ i
nt64]_[* ZstdDecompress]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out], 
[_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], [_^Upp`:`:EventGate^ EventGate]<[_^Upp`:`:int64^ i
nt64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:Upp`:`:ZstdCompress`(const void`*`,Upp`:`:int64`,Upp`:`:EventGate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* ZstdCompress]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], 
[_^Upp`:`:int64^ int64]_[*@3 len], [_^Upp`:`:EventGate^ EventGate]<[_^Upp`:`:int64^ int64
], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:Upp`:`:ZstdCompress`(const Upp`:`:String`&`,Upp`:`:EventGate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* ZstdCompress]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 s],
 [_^Upp`:`:EventGate^ EventGate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 p
rogress]_`=_[@(0.0.255) false])&]
[s5;:Upp`:`:ZstdDecompress`(const void`*`,Upp`:`:int64`,Upp`:`:EventGate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* ZstdDecompress]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], 
[_^Upp`:`:int64^ int64]_[*@3 len], [_^Upp`:`:EventGate^ EventGate]<[_^Upp`:`:int64^ int64
], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:Upp`:`:ZstdDecompress`(const Upp`:`:String`&`,Upp`:`:EventGate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* ZstdDecompress]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 s
], [_^Upp`:`:EventGate^ EventGate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 p
rogress]_`=_[@(0.0.255) false])&]
[s5;:Upp`:`:CoZstdCompress`(Upp`:`:Stream`&`,Upp`:`:Stream`&`,Upp`:`:EventGate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:int64^ i
nt64]_[* CoZstdCompress]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out], 
[_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], [_^Upp`:`:EventGate^ EventGate]<[_^Upp`:`:int64^ i
nt64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:Upp`:`:CoZstdDecompress`(Upp`:`:Stream`&`,Upp`:`:Stream`&`,Upp`:`:EventGate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:int64^ i
nt64]_[* CoZstdDecompress]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out], 
[_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], [_^Upp`:`:EventGate^ EventGate]<[_^Upp`:`:int64^ i
nt64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:Upp`:`:CoZstdCompress`(const void`*`,Upp`:`:int64`,Upp`:`:EventGate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* CoZstdCompress]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], 
[_^Upp`:`:int64^ int64]_[*@3 len], [_^Upp`:`:EventGate^ EventGate]<[_^Upp`:`:int64^ int64
], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:Upp`:`:CoZstdCompress`(const Upp`:`:String`&`,Upp`:`:EventGate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* CoZstdCompress]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 s
], [_^Upp`:`:EventGate^ EventGate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 p
rogress]_`=_[@(0.0.255) false])&]
[s5;:Upp`:`:CoZstdDecompress`(const void`*`,Upp`:`:int64`,Upp`:`:EventGate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* CoZstdDecompress]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], 
[_^Upp`:`:int64^ int64]_[*@3 len], [_^Upp`:`:EventGate^ EventGate]<[_^Upp`:`:int64^ int64
], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:Upp`:`:CoZstdDecompress`(const Upp`:`:String`&`,Upp`:`:EventGate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* CoZstdDecompress]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 s
], [_^Upp`:`:EventGate^ EventGate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 p
rogress]_`=_[@(0.0.255) false])&]
[s2;%% Simple Zstd compression/decompression functions. [%-*@3 out] 
[%-*@3 in] parameters are input / ouput streams and function returns 
the number of bytes stored to [%-*@3 out], otherwise input is provided 
as memory block [%-*@3 data] of [%-*@3 len] bytes or as input String 
[%-*@3 data] and output is String return value. [%-*@3 progress] 
can be used to to track progress of operation and eventually 
interrupt it by returning true. Functions with Co prefix run 
multithreaded.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:IsZstd`(Upp`:`:Stream`&`): [@(0.0.255) bool]_[* IsZstd]([_^Upp`:`:Stream^ Strea
m][@(0.0.255) `&]_[*@3 s])&]
[s2;%% Checks the Stream for magic number identifying Zstd stream. 
Seeks back after the check. Returns true if magic number was 
detected.&]
[s3;%% &]
[s0;%% ]]