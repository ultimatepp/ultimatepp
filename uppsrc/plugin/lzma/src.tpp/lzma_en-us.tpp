topic "LZMA";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 LZMA]]}}&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Function List]]}}&]
[s3; &]
[s5;:LZMACompress`(Stream`&`,Stream`&`,int64`,Gate2`<int64`,int64`>`,int`): [_^int64^ i
nt64]_[* LZMACompress]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 out], [_^Stream^ Stream][@(0.0.255) `&
]_[*@3 in], [_^int64^ int64]_[*@3 size], [_^Gate2^ Gate2]<[_^int64^ int64], 
[_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false], [@(0.0.255) int]_[*@3 lvl]_`=_[@3 6])
&]
[s5;:LZMACompress`(Stream`&`,Stream`&`,Gate2`<int64`,int64`>`,int`): [_^int64^ int64]_[* L
ZMACompress]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 out], [_^Stream^ Stream][@(0.0.255) `&]_
[*@3 in], [_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) f
alse], [@(0.0.255) int]_[*@3 lvl]_`=_[@3 6])&]
[s5;:LZMACompress`(const void`*`,int64`,Gate2`<int64`,int64`>`,int`): [_^String^ String
]_[* LZMACompress]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], 
[_^int64^ int64]_[*@3 len], [_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progres
s]_`=_[@(0.0.255) false], [@(0.0.255) int]_[*@3 lvl]_`=_[@3 6])&]
[s5;:LZMACompress`(const String`&`,Gate2`<int64`,int64`>`,int`): [_^String^ String]_[* LZ
MACompress]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s], 
[_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false], 
[@(0.0.255) int]_[*@3 lvl]_`=_[@3 6])&]
[s2;%% Compresses data using lzma from input stream [%-*@3 in], memory 
[%-*@3 data][%- , ][%-*@3 len] or input string [%-*@3 s] to output stream 
[%-*@3 out] (in this case returns a number of bytes written or 
negative integer to signal error) or into String return value 
(returns IsVoid String on error). [%-*@3 progress] can be used 
to track progress of operation, returning true cancels operation. 
[%-*@3 lvl] can be used to tune compression ratio, can be 1..9 
`- values greater than 6 however might take quite a long time 
and a lot of memory (hunderds of MB).&]
[s3;%% &]
[s4;%% &]
[s5;:LZMADecompress`(Stream`&`,Stream`&`,Gate2`<int64`,int64`>`): [_^int64^ int64]_[* LZM
ADecompress]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 out], [_^Stream^ Stream][@(0.0.255) `&]_
[*@3 in], [_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) f
alse])&]
[s5;:LZMADecompress`(const void`*`,int64`,Gate2`<int64`,int64`>`): [_^String^ String]_[* L
ZMADecompress]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], [_^int64^ int64]_[*@3 len
], [_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false
])&]
[s5;:LZMADecompress`(const String`&`,Gate2`<int64`,int64`>`): [_^String^ String]_[* LZMAD
ecompress]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s], 
[_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s2;%% Decompresses data in lzma format from input stream [%-*@3 in], 
memory [%-*@3 data][%- , ][%-*@3 len] or input string [%-*@3 s] to output 
stream [%-*@3 out] (in this case returns a number of bytes written 
or negative integer to signal error) or into String return value 
(returns IsVoid String on error). [%-*@3 progress] can be used 
to track progress of operation, returning true cancels operation.&]
[s3; &]
[s4; &]
[s5;:LZMACompressFile`(const char`*`,const char`*`,Gate2`<int64`,int64`>`,int`): [@(0.0.255) b
ool]_[* LZMACompressFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 dstfile], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 srcfile], [_^Gate2^ Gate2]<[_^int64^ int64], 
[_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false], [@(0.0.255) int]_[*@3 lvl]_`=_[@3 6])
&]
[s5;:LZMACompressFile`(const char`*`,Gate2`<int64`,int64`>`,int`): [@(0.0.255) bool]_[* L
ZMACompressFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 srcfile], 
[_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress], 
[@(0.0.255) int]_[*@3 lvl]_`=_[@3 6])&]
[s2;%% Compresses file into lzma format. If [%-*@3 dstfile] is not 
present, the name is created by appending .lzma extension to 
[%-*@3 srcfile]. Returns true on success. [%-*@3 lvl] can be used 
to tune compression ratio, can be 1..9 `- values greater than 
6 however might take quite a long time and a lot of memory (hunderds 
of MB).&]
[s3;%% &]
[s4;%% &]
[s5;:LZMADecompressFile`(const char`*`,const char`*`,Gate2`<int64`,int64`>`): [@(0.0.255) b
ool]_[* LZMADecompressFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 dstfile], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 srcfile], [_^Gate2^ Gate2]<[_^int64^ int64], 
[_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:LZMADecompressFile`(const char`*`,Gate2`<int64`,int64`>`): [@(0.0.255) bool]_[* LZMA
DecompressFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 srcfile], 
[_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress])&]
[s2;%% Decompresses file from lzma format. If [%-*@3 dstfile] is not 
present, the name is created by removing .lzma extension to [%-*@3 srcfile]. 
If [%-*@3 srcfile] does not have .lzma extension, function returns 
false to signal error and does nothing. Returns true on success.&]
[s3;%% ]]