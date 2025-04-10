topic "Low Level Data Functions";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 Low Level Data Functions]]}}&]
[s0;%% &]
[s0;%% Some of these functions extend or supplement classical C mem`* 
functions to wider datatypes or eventually specific condition, 
using heavy inlining and SIMD code to optimize performance. Other 
provide endianness support and/or raw access to memory (SwapEndian/Poke/Peek).&]
[s0;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Function List]]}}&]
[s3;%% &]
[s5;:Upp`:`:memset8`(void`*`,Upp`:`:byte`,size`_t`): [@(0.0.255) void]_[* memset8]([@(0.0.255) v
oid]_`*[*@3 p], [_^Upp`:`:byte^ byte]_[*@3 data], [_^size`_t^ size`_t]_[*@3 count])&]
[s5;:Upp`:`:memset16`(void`*`,Upp`:`:word`,size`_t`): [@(0.0.255) void]_[* memset16]([@(0.0.255) v
oid]_`*[*@3 p], [_^Upp`:`:word^ word]_[*@3 data], [_^size`_t^ size`_t]_[*@3 count])&]
[s5;:Upp`:`:memset32`(void`*`,Upp`:`:dword`,size`_t`): [@(0.0.255) void]_[* memset32]([@(0.0.255) v
oid]_`*[*@3 p], [_^Upp`:`:dword^ dword]_[*@3 data], [_^size`_t^ size`_t]_[*@3 count])&]
[s5;:Upp`:`:memset64`(void`*`,Upp`:`:qword`,size`_t`): [@(0.0.255) void]_[* memset64]([@(0.0.255) v
oid]_`*[*@3 p], [_^Upp`:`:qword^ qword]_[*@3 data], [_^size`_t^ size`_t]_[*@3 count])&]
[s5;:Upp`:`:memset128`(void`*`,Upp`:`:m128`,size`_t`): [@(0.0.255) void]_[* memset128]([@(0.0.255) v
oid]_`*[*@3 t], [_^Upp`:`:m128^ m128]_[*@3 data], [_^size`_t^ size`_t]_[*@3 count])&]
[s2;%% Similar functionality to memset for 8, 16, 32, 64, 128 bit: 
sets the value of [%-*@3 count] elements starting at [%-*@3 p]. memset8 
[/ might] be faster for small datablocks, other functions [/ should] 
be faster than the manual implementation.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:memcpy8`(void`*`,const void`*`,size`_t`): [@(0.0.255) void]_[* memcpy8]([@(0.0.255) v
oid]_`*[*@3 p], [@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 q], [_^size`_t^ size`_t]_[*@3 cou
nt])&]
[s5;:Upp`:`:memcpy16`(void`*`,const void`*`,size`_t`): [@(0.0.255) void]_[* memcpy16]([@(0.0.255) v
oid]_`*[*@3 p], [@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 q], [_^size`_t^ size`_t]_[*@3 cou
nt])&]
[s5;:Upp`:`:memcpy32`(void`*`,const void`*`,size`_t`): [@(0.0.255) void]_[* memcpy32]([@(0.0.255) v
oid]_`*[*@3 p], [@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 q], [_^size`_t^ size`_t]_[*@3 cou
nt])&]
[s5;:Upp`:`:memcpy64`(void`*`,const void`*`,size`_t`): [@(0.0.255) void]_[* memcpy64]([@(0.0.255) v
oid]_`*[*@3 p], [@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 q], [_^size`_t^ size`_t]_[*@3 cou
nt])&]
[s5;:Upp`:`:memcpy128`(void`*`,const void`*`,size`_t`): [@(0.0.255) void]_[* memcpy128]([@(0.0.255) v
oid]_`*[*@3 p], [@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 q], [_^size`_t^ size`_t]_[*@3 cou
nt])&]
[s2;%% Similar functionality to memcpy for 8, 16, 32, 64, 128 bit: 
copies the memory block of [%-*@3 count] elements from [%-*@3 q] 
to [%-*@3 p]. memset8 [/ might] be faster for small datablocks, other 
functions [/ should] be faster than the manual implementation.&]
[s0;%% &]
[s4; &]
[s5;:Upp`:`:memcpy`_t`(void`*`,const T`*`,size`_t`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[@(0.0.255) void]_[* memcpy`_t]([@(0.0.255) void]_`*[*@3 t], 
[@(0.0.255) const]_[*@4 T]_`*[*@3 s], [_^size`_t^ size`_t]_[*@3 count])&]
[s2;%% This function selects appropriate memcpy`* function based 
on sizeof(T) to copy [%-*@3 count] of [%-*@4 T] elements. Useful 
for template algorithms.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:inline`_memeq8`_aligned`(const void`*`,const void`*`,size`_t`): [@(0.0.255) b
ool]_[* inline`_memeq8`_aligned]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 p], 
[@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 q], [_^size`_t^ size`_t]_[*@3 count])&]
[s5;:Upp`:`:inline`_memeq16`_aligned`(const void`*`,const void`*`,size`_t`): [@(0.0.255) b
ool]_[* inline`_memeq16`_aligned]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 p], 
[@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 q], [_^size`_t^ size`_t]_[*@3 count])&]
[s5;:Upp`:`:inline`_memeq32`_aligned`(const void`*`,const void`*`,size`_t`): [@(0.0.255) b
ool]_[* inline`_memeq32`_aligned]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 p], 
[@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 q], [_^size`_t^ size`_t]_[*@3 count])&]
[s5;:Upp`:`:inline`_memeq64`_aligned`(const void`*`,const void`*`,size`_t`): [@(0.0.255) b
ool]_[* inline`_memeq64`_aligned]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 p], 
[@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 q], [_^size`_t^ size`_t]_[*@3 count])&]
[s5;:Upp`:`:inline`_memeq128`_aligned`(const void`*`,const void`*`,size`_t`): [@(0.0.255) b
ool]_[* inline`_memeq128`_aligned]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 t], 
[@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 s], [_^size`_t^ size`_t]_[*@3 count])&]
[s2;%% These functions compare the memory blocks at [%-*@3 p][%-  and 
][%-*@3 q] of [%-*@3 count] 8, 16, 32, 64, 128 bit elements and return 
true if they are equal. Memory must be correctly aligned for 
the element size. These functions [/ should] be faster than memcmp 
for small blocks.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:memeq8`(const void`*`,const void`*`,size`_t`): [@(0.0.255) bool]_[* memeq8]([@(0.0.255) c
onst]_[@(0.0.255) void]_`*[*@3 p], [@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 q], 
[_^size`_t^ size`_t]_[*@3 count])&]
[s5;:Upp`:`:memeq16`(const void`*`,const void`*`,size`_t`): [@(0.0.255) bool]_[* memeq16](
[@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 p], [@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 q
], [_^size`_t^ size`_t]_[*@3 count])&]
[s5;:Upp`:`:memeq32`(const void`*`,const void`*`,size`_t`): [@(0.0.255) bool]_[* memeq32](
[@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 p], [@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 q
], [_^size`_t^ size`_t]_[*@3 count])&]
[s5;:Upp`:`:memeq64`(const void`*`,const void`*`,size`_t`): [@(0.0.255) bool]_[* memeq64](
[@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 p], [@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 q
], [_^size`_t^ size`_t]_[*@3 count])&]
[s5;:Upp`:`:memeq128`(const void`*`,const void`*`,size`_t`): [@(0.0.255) bool]_[* memeq12
8]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 p], [@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 q
], [_^size`_t^ size`_t]_[*@3 count])&]
[s2;%% Similar to memcmp(p, q, count `* size) `=`= 0 and probably 
about the same speed. Provided mostly for completeness.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:memeq8len`(const void`*`,const void`*`,size`_t`): size`_t 
[* memeq8len]([@(0.0.255) const] [@(0.0.255) void] [@(0.0.255) `*][*@3 p], 
[@(0.0.255) const] [@(0.0.255) void] [@(0.0.255) `*][*@3 q], size`_t 
[*@3 count])&]
[s2;%% Returns the [/ position] such that [/ position ]< [*@3 count ][@(0.0.255) `&`&] 
[*@3 p][@(0.0.255) `[][/ position][@(0.0.255) `] !`=] [*@3 q][@(0.0.255) `[][/ position][@(0.0.255) `]
]. If no such position exists, returns [*@3 count]. In other words 
returns count of bytes at p and q that are equal.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:memcnt8`(const void`*`,dword`,size`_t`): size`_t [* memcnt8]([@(0.0.255) cons
t] [@(0.0.255) void] [@(0.0.255) `*][*@3 s], dword [*@3 value], size`_t 
[*@3 sz])&]
[s5;:Upp`:`:memcnt16`(const void`*`,dword`,size`_t`): size`_t [* memcnt16]([@(0.0.255) co
nst] [@(0.0.255) void] [@(0.0.255) `*][*@3 s], dword [*@3 value], size`_t 
[*@3 sz])&]
[s5;:Upp`:`:memcnt32`(const void`*`,dword`,size`_t`): size`_t [* memcnt32]([@(0.0.255) co
nst] [@(0.0.255) void] [@(0.0.255) `*][*@3 s], dword [*@3 value], size`_t 
[*@3 sz])&]
[s2;%% Counts the number of items equal to [%-*@3 value].&]
[s3; &]
[s4; &]
[s5;:Upp`:`:memhash`(const void`*`,size`_t`): [_^Upp`:`:hash`_t^ hash`_t]_[* memhash]([@(0.0.255) c
onst]_[@(0.0.255) void]_`*[*@3 ptr], [_^size`_t^ size`_t]_[*@3 size])&]
[s2;%% Computes a non`-cryptographic hash of memory block. &]
[s3; &]
[s4; &]
[s5;:Upp`:`:FoldHash`(Upp`:`:qword`): [_^Upp`:`:dword^ dword]_[* FoldHash]([_^Upp`:`:qword^ q
word]_[*@3 h])&]
[s2;%% This functions `"hashes all bits together`". One purpose is 
to bring the entropy of higher bits down so that the hash can 
be limited by masking, other purpose is to provide hash for integer 
numbers.&]
[s2;%% &]
[s4; &]
[s5;:Upp`:`:SignificantBits`(Upp`:`:dword`): [@(0.0.255) int]_[* SignificantBits]([_^Upp`:`:dword^ d
word]_[*@3 x])&]
[s5;:Upp`:`:SignificantBits64`(Upp`:`:uint64`): [@(0.0.255) int]_[* SignificantBits64]([_^Upp`:`:uint64^ u
int64]_[*@3 x])&]
[s2;%% Returns the number of bits required to store the value. E.g. 
for binary value 1010, this value is 4. For 0, the value is 0.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:FitsInInt64`(double`): [@(0.0.255) bool]_[* FitsInInt64]([@(0.0.255) double]_[*@3 x
])&]
[s2;%% Returns true if [%-*@3 x] can be converted to int64 without 
loosing the precision.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CountBits`(dword`): [@(0.0.255) int] [* CountBits]([_^Upp`:`:dword^ dword] 
[*@3 mask])&]
[s5;:Upp`:`:CountBits64`(uint64`): [@(0.0.255) int] [* CountBits64]([_^Upp`:`:uint64^ uint6
4] [*@3 mask])&]
[s2;%% Returns the total number of set bits in a given bit [%-*@3 mask].&]
[s3; &]
[s4; &]
[s5;:Upp`:`:CountTrailingZeroBits`(dword`): [@(0.0.255) int] [* CountTrailingZeroBits](dw
ord [*@3 x])&]
[s5;:Upp`:`:CountTrailingZeroBits64`(uint64`): [@(0.0.255) int] [* CountTrailingZeroBits6
4](uint64 [*@3 x])&]
[s2; [%% Returns the index of first non`-zero bit in (from least significant 
bit). If] [*@3 x] is zero, the result is undefined.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Peek16le`(const void`*`): [@(0.0.255) int]_[* Peek16le]([@(0.0.255) const]_[@(0.0.255) v
oid]_`*[*@3 ptr])&]
[s5;:Upp`:`:Peek32le`(const void`*`): [@(0.0.255) int]_[* Peek32le]([@(0.0.255) const]_[@(0.0.255) v
oid]_`*[*@3 ptr])&]
[s5;:Upp`:`:Peek64le`(const void`*`): [_^Upp`:`:int64^ int64]_[* Peek64le]([@(0.0.255) cons
t]_[@(0.0.255) void]_`*[*@3 ptr])&]
[s2;%% Reads little endian value from unaligned [%-*@3 ptr].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Poke16le`(const void`*`,int`): [@(0.0.255) void]_[* Poke16le]([@(0.0.255) const
]_[@(0.0.255) void]_`*[*@3 ptr], [@(0.0.255) int]_[*@3 val])&]
[s5;:Upp`:`:Poke32le`(const void`*`,int`): [@(0.0.255) void]_[* Poke32le]([@(0.0.255) const
]_[@(0.0.255) void]_`*[*@3 ptr], [@(0.0.255) int]_[*@3 val])&]
[s5;:Upp`:`:Poke64le`(const void`*`,Upp`:`:int64`): [@(0.0.255) void]_[* Poke64le]([@(0.0.255) c
onst]_[@(0.0.255) void]_`*[*@3 ptr], [_^Upp`:`:int64^ int64]_[*@3 val])&]
[s2;%% Writes [%-*@3 val] to unaligned [%-*@3 ptr] as little endian.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Peek16be`(const void`*`): [@(0.0.255) int]_[* Peek16be]([@(0.0.255) const]_[@(0.0.255) v
oid]_`*[*@3 ptr])&]
[s5;:Upp`:`:Peek32be`(const void`*`): [@(0.0.255) int]_[* Peek32be]([@(0.0.255) const]_[@(0.0.255) v
oid]_`*[*@3 ptr])&]
[s5;:Upp`:`:Peek64be`(const void`*`): [_^Upp`:`:int64^ int64]_[* Peek64be]([@(0.0.255) cons
t]_[@(0.0.255) void]_`*[*@3 ptr])&]
[s2;%% Reads big endian value from unaligned [%-*@3 ptr].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Poke16be`(const void`*`,int`): [@(0.0.255) void]_[* Poke16be]([@(0.0.255) const
]_[@(0.0.255) void]_`*[*@3 ptr], [@(0.0.255) int]_[*@3 val])&]
[s5;:Upp`:`:Poke32be`(const void`*`,int`): [@(0.0.255) void]_[* Poke32be]([@(0.0.255) const
]_[@(0.0.255) void]_`*[*@3 ptr], [@(0.0.255) int]_[*@3 val])&]
[s5;:Upp`:`:Poke64be`(const void`*`,Upp`:`:int64`): [@(0.0.255) void]_[* Poke64be]([@(0.0.255) c
onst]_[@(0.0.255) void]_`*[*@3 ptr], [_^Upp`:`:int64^ int64]_[*@3 val])&]
[s2;%% Writes [%-*@3 val] to unaligned [%-*@3 ptr] as big endian.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SwapEndian16`(Upp`:`:word`): [_^Upp`:`:word^ word]_[* SwapEndian16]([_^Upp`:`:word^ w
ord]_[*@3 v])&]
[s5;:Upp`:`:SwapEndian16`(Upp`:`:int16`): [_^Upp`:`:int16^ int16]_[* SwapEndian16]([_^Upp`:`:int16^ i
nt16]_[*@3 v])&]
[s5;:Upp`:`:SwapEndian32`(Upp`:`:dword`): [_^Upp`:`:dword^ dword]_[* SwapEndian32]([_^Upp`:`:dword^ d
word]_[*@3 v])&]
[s5;:Upp`:`:SwapEndian32`(int`): [@(0.0.255) int]_[* SwapEndian32]([@(0.0.255) int]_[*@3 v])&]
[s5;:Upp`:`:SwapEndian64`(Upp`:`:uint64`): [_^Upp`:`:uint64^ uint64]_[* SwapEndian64]([_^Upp`:`:uint64^ u
int64]_[*@3 v])&]
[s5;:Upp`:`:SwapEndian64`(Upp`:`:int64`): [_^Upp`:`:int64^ int64]_[* SwapEndian64]([_^Upp`:`:int64^ i
nt64]_[*@3 v])&]
[s2;%% Returns the [%-*@3 v] with swapped order of bytes.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:EndianSwap`(Upp`:`:word`&`): [@(0.0.255) void]_[* EndianSwap]([_^Upp`:`:word^ w
ord][@(0.0.255) `&]_[*@3 v])&]
[s5;:Upp`:`:EndianSwap`(Upp`:`:int16`&`): [@(0.0.255) void]_[* EndianSwap]([_^Upp`:`:int16^ i
nt16][@(0.0.255) `&]_[*@3 v])&]
[s5;:Upp`:`:EndianSwap`(Upp`:`:dword`&`): [@(0.0.255) void]_[* EndianSwap]([_^Upp`:`:dword^ d
word][@(0.0.255) `&]_[*@3 v])&]
[s5;:Upp`:`:EndianSwap`(int`&`): [@(0.0.255) void]_[* EndianSwap]([@(0.0.255) int`&]_[*@3 v])
&]
[s5;:Upp`:`:EndianSwap`(Upp`:`:int64`&`): [@(0.0.255) void]_[* EndianSwap]([_^Upp`:`:int64^ i
nt64][@(0.0.255) `&]_[*@3 v])&]
[s5;:Upp`:`:EndianSwap`(Upp`:`:uint64`&`): [@(0.0.255) void]_[* EndianSwap]([_^Upp`:`:uint64^ u
int64][@(0.0.255) `&]_[*@3 v])&]
[s2;%% Swaps the order of bytes of [%-*@3 v].&]
[s3;%% ]]