topic "Zlib encapsulation";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 Zlib]]}}&]
[s3; &]
[s1;:Zlib`:`:class: [@(0.0.255)3 class][3 _][*3 Zlib]&]
[s2;%% This class encapsulates zlib library to provide compression/decompression 
utility.&]
[s2;%% Encapsulation is specifically designed to allow streaming 
mode of operations. Input data are fed using Put method and can 
be either obtained at output as partial String, or through output 
callback. If using String, it is also always possible to clear 
this output partial String to conserve memory.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Zlib`:`:WhenOut: [_^Callback2^ Callback2]<[@(0.0.255) const]_[@(0.0.255) void]_`*, 
[_^dword^ int]>_[* WhenOut]&]
[s2;%% Output callback. Represents `"consumer function`": When there 
are output data available, they are passed out using this callback. 
Default value stores data into output String inside Zlib.&]
[s3; &]
[s4; &]
[s5;:Zlib`:`:Compress`(`): [@(0.0.255) void]_[* Compress]()&]
[s2;%% Sets Zlib into compression mode (`"deflate`" in zlib terminology).&]
[s3; &]
[s4; &]
[s5;:Zlib`:`:Decompress`(`): [@(0.0.255) void]_[* Decompress]()&]
[s2;%% Sets Zlib into decompression mode (`"inflate`" in zlib terminology).&]
[s3; &]
[s4; &]
[s5;:Zlib`:`:Put`(const void`*`,int`): [@(0.0.255) void]_[* Put]([@(0.0.255) const]_[@(0.0.255) v
oid]_`*[*@3 ptr], [@(0.0.255) int]_[*@3 size])&]
[s5;:Zlib`:`:Put`(const String`&`): [@(0.0.255) void]_[* Put]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 s])&]
[s2;%% Puts a block of data to be processed (compressed or decompressed 
based on mode) by Zlib.&]
[s3;%% &]
[s4; &]
[s5;:Zlib`:`:End`(`): [@(0.0.255) void]_[* End]()&]
[s2;%% Signals the end of input stream.&]
[s3; &]
[s4; &]
[s5;:Zlib`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%% Resets Zlib to initial state (throws out data).&]
[s3; &]
[s4; &]
[s5;:Zlib`:`:Get`(`)const: [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[* Get]()_[@(0.0.255) c
onst]&]
[s5;:Zlib`:`:operator const String`&`(`)const: [@(0.0.255) operator][* _const_String`&]()
_[@(0.0.255) const]&]
[s5;:Zlib`:`:operator`~`(`)const: [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[* ope
rator`~]()_[@(0.0.255) const]&]
[s2;%% Returns current output String (it only gets created if WhenOut 
is not redirected somewhere else).&]
[s3; &]
[s4; &]
[s5;:Zlib`:`:ClearOut`(`): [@(0.0.255) void]_[* ClearOut]()&]
[s2;%% Clears output String. Can be called anytime.&]
[s3; &]
[s4; &]
[s5;:Zlib`:`:GetCRC`(`)const: [_^dword^ dword]_[* GetCRC]()_[@(0.0.255) const]&]
[s2;%% Returns CRC of uncompressed stream, if CRC mode is active.&]
[s3; &]
[s4; &]
[s5;:Zlib`:`:IsError`(`)const: [@(0.0.255) bool]_[* IsError]()_[@(0.0.255) const]&]
[s2;%% Returns true if there was error processing input.&]
[s3; &]
[s4; &]
[s5;:Zlib`:`:GetGZipName`(`)const: [_^String^ String]_[* GetGZipName]()_[@(0.0.255) const]&]
[s2;%% If GZip mode is active and Gzip header is processed, returns 
the name of file.&]
[s3; &]
[s4; &]
[s5;:Zlib`:`:GetGZipComment`(`)const: [_^String^ String]_[* GetGZipComment]()_[@(0.0.255) c
onst]&]
[s2;%% If GZip mode is active and Gzip header is processed, returns 
the gzip comment.&]
[s3; &]
[s4; &]
[s5;:Zlib`:`:GZip`(bool`): [_^Zlib^ Zlib][@(0.0.255) `&]_[* GZip]([@(0.0.255) bool]_[*@3 gzip`_
]_`=_[@(0.0.255) true])&]
[s2;%% Activates GZip mode. In this mode Zlib is able to directly 
process .gzip file format.&]
[s3;%% &]
[s4; &]
[s5;:Zlib`:`:Header`(bool`): [_^Zlib^ Zlib][@(0.0.255) `&]_[* Header]([@(0.0.255) bool]_[*@3 hd
r`_]_`=_[@(0.0.255) true])&]
[s2;%% Activates zlib header (see zlib documentation for details).&]
[s3;%% &]
[s4; &]
[s5;:Zlib`:`:NoHeader`(`): [_^Zlib^ Zlib][@(0.0.255) `&]_[* NoHeader]()&]
[s2;%% Same as Header(false).&]
[s3; &]
[s4; &]
[s5;:Zlib`:`:CRC`(bool`): [_^Zlib^ Zlib][@(0.0.255) `&]_[* CRC]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) t
rue])&]
[s2;%% Activates CRC mode. In this mode, Zlib gathers CRC of uncompressed 
data that can be at the end obtained by GetCRC method.&]
[s3;%% &]
[s4; &]
[s5;:Zlib`:`:NoCRC`(`): [_^Zlib^ Zlib][@(0.0.255) `&]_[* NoCRC]()&]
[s2;%% Same as CRC(false).&]
[s3; &]
[s4; &]
[s5;:Zlib`:`:ChunkSize`(int`): [_^Zlib^ Zlib][@(0.0.255) `&]_[* ChunkSize]([@(0.0.255) int]_[*@3 n
])&]
[s2;%% Sets the maximum amount of input or output data to be processed 
in single zlib call.&]
[s3;%% &]
[s4; &]
[s5;:Zlib`:`:Level`(int`): [_^Zlib^ Zlib][@(0.0.255) `&]_[* Level]([@(0.0.255) int]_[*@3 compre
ssion`_lvl])&]
[s2;%% Sets the compresison level. Must be 1..9. Default level is 
6.&]
[s3;%% &]
[s4; &]
[s5;:Zlib`:`:Zlib`(`): [* Zlib]()&]
[s2;%% Constructor.&]
[s3; &]
[s4; &]
[s5;:Zlib`:`:`~Zlib`(`): [@(0.0.255) `~][* Zlib]()&]
[s2;%% Destructor.&]
[s3;%% &]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 Compression / decompression functions]]}}&]
[s3; &]
[s5;:ZCompress`(Stream`&`,Stream`&`,int64`,Gate2`<int64`,int64`>`,bool`): [_^int64^ int
64]_[* ZCompress]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 out], [_^Stream^ Stream][@(0.0.255) `&
]_[*@3 in], [_^int64^ int64]_[*@3 size], [_^Gate2^ Gate2]<[_^int64^ int64], 
[_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false], [@(0.0.255) bool]_[*@3 hdr]_`=_[@(0.0.255) t
rue])&]
[s5;:ZCompress`(Stream`&`,Stream`&`,Gate2`<int64`,int64`>`): [_^int64^ int64]_[* ZCompres
s]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 out], [_^Stream^ Stream][@(0.0.255) `&]_[*@3 in], 
[_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:ZCompress`(const void`*`,int64`,Gate2`<int64`,int64`>`): [_^String^ String]_[* ZComp
ress]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], [_^int64^ int64]_[*@3 len], 
[_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:ZCompress`(const String`&`,Gate2`<int64`,int64`>`): [_^String^ String]_[* ZCompress](
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s], [_^Gate2^ Gate2]<[_^int64^ int6
4], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s2;%% Compresses raw data using zlib from input stream [%-*@3 in], 
memory [%-*@3 data][%- , ][%-*@3 len] or input string [%-*@3 s] to output 
stream [%-*@3 out] (in this case returns a number of bytes written 
or negative integer to signal error) or into String return value 
(returns IsVoid String on error). [%-*@3 progress] can be used 
to track progress of operation, returning true cancels operation. 
Parameter [%-*@3 hdr] can be used to switch`-off zlib header (see 
zlib documentation for details).&]
[s3;%% &]
[s4; &]
[s5;:ZDecompress`(Stream`&`,Stream`&`,int64`,Gate2`<int64`,int64`>`,bool`): [_^int64^ i
nt64]_[* ZDecompress]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 out], [_^Stream^ Stream][@(0.0.255) `&
]_[*@3 in], [_^int64^ int64]_[*@3 size], [_^Gate2^ Gate2]<[_^int64^ int64], 
[_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false], [@(0.0.255) bool]_[*@3 hdr]_`=_[@(0.0.255) t
rue])&]
[s5;:ZDecompress`(Stream`&`,Stream`&`,int64`,Gate2`<int64`,int64`>`): [_^int64^ int64]_
[* ZDecompress]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 out], [_^Stream^ Stream][@(0.0.255) `&
]_[*@3 in], [_^int64^ int64]_[*@3 size], [_^Gate2^ Gate2]<[_^int64^ int64], 
[_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:ZDecompress`(Stream`&`,Stream`&`,Gate2`<int64`,int64`>`): [_^int64^ int64]_[* ZDecom
press]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 out], [_^Stream^ Stream][@(0.0.255) `&]_[*@3 in
], [_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false
])&]
[s5;:ZDecompress`(const void`*`,int64`,Gate2`<int64`,int64`>`): [_^String^ String]_[* ZDe
compress]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], [_^int64^ int64]_[*@3 len], 
[_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:ZDecompress`(const String`&`,Gate2`<int64`,int64`>`): [_^String^ String]_[* ZDecompr
ess]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s], [_^Gate2^ Gate2]<[_^int64^ i
nt64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s2;%% Decompresses raw data using zlib from input stream [%-*@3 in], 
memory [%-*@3 data][%- , ][%-*@3 len] or input string [%-*@3 s] to output 
stream [%-*@3 out] (in this case returns a number of bytes written 
or negative integer to signal error) or into String return value 
(returns IsVoid String on error). [%-*@3 progress] can be used 
to track progress of operation, returning true cancels operation. 
Parameter [%-*@3 hdr] can be used to switch`-off zlib header (see 
zlib documentation for details).&]
[s3;%% &]
[s4;%% &]
[s5;:GZCompress`(Stream`&`,Stream`&`,int64`,Gate2`<int64`,int64`>`): [_^int64^ int64]_[* G
ZCompress]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 out], [_^Stream^ Stream][@(0.0.255) `&]_[*@3 i
n], [_^int64^ int64]_[*@3 size], [_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 pr
ogress]_`=_[@(0.0.255) false])&]
[s5;:GZCompress`(Stream`&`,Stream`&`,Gate2`<int64`,int64`>`): [_^int64^ int64]_[* GZCompr
ess]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 out], [_^Stream^ Stream][@(0.0.255) `&]_[*@3 in],
 [_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])
&]
[s5;:GZCompress`(const void`*`,int`,Gate2`<int64`,int64`>`): [_^String^ String]_[* GZComp
ress]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], [@(0.0.255) int]_[*@3 len], 
[_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:GZCompress`(const String`&`,Gate2`<int64`,int64`>`): [_^String^ String]_[* GZCompres
s]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s], [_^Gate2^ Gate2]<[_^int64^ i
nt64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s2;%% Compresses raw data into gzip file format from input stream 
[%-*@3 in], memory [%-*@3 data][%- , ][%-*@3 len] or input string [%-*@3 s] 
to output stream [%-*@3 out] (in this case returns a number of 
bytes written or negative integer to signal error) or into String 
return value (returns IsVoid String on error). [%-*@3 progress] 
can be used to track progress of operation, returning true cancels 
operation.&]
[s3;%% &]
[s4;%% &]
[s5;:GZDecompress`(Stream`&`,Stream`&`,int64`,Gate2`<int64`,int64`>`): [_^int64^ int64]_
[* GZDecompress]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 out], [_^Stream^ Stream][@(0.0.255) `&
]_[*@3 in], [_^int64^ int64]_[*@3 size], [_^Gate2^ Gate2]<[_^int64^ int64], 
[_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:GZDecompress`(Stream`&`,Stream`&`,Gate2`<int64`,int64`>`): [_^int64^ int64]_[* GZDec
ompress]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 out], [_^Stream^ Stream][@(0.0.255) `&]_[*@3 i
n], [_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) fals
e])&]
[s5;:GZDecompress`(const void`*`,int`,Gate2`<int64`,int64`>`): [_^String^ String]_[* GZDe
compress]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], [@(0.0.255) int]_[*@3 len], 
[_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:GZDecompress`(const String`&`,Gate2`<int64`,int64`>`): [_^String^ String]_[* GZDecom
press]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s], 
[_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s2;%% Decompresses raw data into gzip file format from input stream 
[%-*@3 in], memory [%-*@3 data][%- , ][%-*@3 len] or input string [%-*@3 s] 
to output stream [%-*@3 out] (in this case returns a number of 
bytes written or negative integer to signal error) or into String 
return value (returns IsVoid String on error). [%-*@3 progress] 
can be used to track progress of operation, returning true cancels 
operation.&]
[s3;%% &]
[s4;%% &]
[s5;:GZCompressFile`(const char`*`,const char`*`,Gate2`<int64`,int64`>`): [@(0.0.255) b
ool]_[* GZCompressFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 dstfile], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 srcfile], [_^Gate2^ Gate2]<[_^int64^ int64], 
[_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:GZCompressFile`(const char`*`,Gate2`<int64`,int64`>`): [@(0.0.255) bool]_[* GZCompre
ssFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 srcfile], [_^Gate2^ Gate2]<[_^int64^ in
t64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s2;%% Compresses file into gzip format. If [%-*@3 dstfile] is not 
present, the name is created by appending .gz extension to [%-*@3 srcfile]. 
Returns true on success.&]
[s3;%% &]
[s4;%% &]
[s5;:GZDecompressFile`(const char`*`,const char`*`,Gate2`<int64`,int64`>`): [@(0.0.255) b
ool]_[* GZDecompressFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 dstfile], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 srcfile], [_^Gate2^ Gate2]<[_^int64^ int64], 
[_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:GZDecompressFile`(const char`*`,Gate2`<int64`,int64`>`): [@(0.0.255) bool]_[* GZDeco
mpressFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 srcfile], [_^Gate2^ Gate2]<[_^int64^ i
nt64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s2;%% Decompresses file from gzip format. If [%-*@3 dstfile] is not 
present, the name is created by removing .gz extension to [%-*@3 srcfile]. 
If [%-*@3 srcfile] does not have .gz extension, function returns 
false to signal error and does nothing. Returns true on success.&]
[s0;%% ]]