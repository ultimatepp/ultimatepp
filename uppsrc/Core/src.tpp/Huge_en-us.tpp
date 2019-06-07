topic "Huge";
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
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 Huge]]}}&]
[s3;%- &]
[s1;:Upp`:`:Huge`:`:class:%- [@(0.0.255)3 class][3 _][*3 Huge]&]
[s2; This class is intended for dealing of large raw binary data. 
The main use is supposed to be in serialization loading, where 
it is used to preload large binary data (e.g. String or Image) 
as means for protection against invalid input `- Huge is storing 
data in 1MB chunks, so is likely to run to the end of stream 
without allocating invalid amounts of data.&]
[s2; To explain further, the problem we are trying to solve here 
is&]
[s2; &]
[s2; [C void Foo`::Serialize(Stream`& s)]&]
[s2; [C `{]&]
[s2; [C    int len;]&]
[s2; [C    s % len;]&]
[s2; [C    if(s.IsLoading()) `{]&]
[s2; [C        ptr `= new byte`[len`];]&]
[s2; [C        s.GetAll(ptr, len);]&]
[s2; [C    `}]&]
[s2; [C    .....]&]
[s2; [C `}]&]
[s2; &]
[s2; now if input stream is invalid, the allocation is likely to 
fail. We could prevent this by examining the amount of remaining 
data in the stream, but for certain class of streams (e.g. compressed 
ones) we do not have that info. [* Huge] solves the problem by 
serving as temporary buffer for loaded data:&]
[s2; &]
[s2; [C void Foo`::Serialize(Stream`& s)]&]
[s2; [C `{]&]
[s2; [C    int len;]&]
[s2; [C    s % len;]&]
[s2; [C    if(s.IsLoading()) `{]&]
[s2; [C -|       Huge h;]&]
[s2; [C        s.GetAll(h, len);]&]
[s2; [C        ptr `= new byte`[h.GetSize()`];]&]
[s2; [C        h.Get(ptr);]&]
[s2; [C    `}]&]
[s2; [C    .....]&]
[s2; [C `}]&]
[s2;C &]
[s2; Now if stream remaining length is less than [C len], [* Huge] performs 
allocation just in 1MB chunks and the end of stream will be reached 
without allocating too much data. GetAll will then fail with 
LoadingError. The downside of this step is the fact that data 
get copied one more time and that for particular element, twice 
as much memory is required to load it, however, usually these 
are negligible.&]
[s0;i448;a25;kKO9;:noref:@(0.0.255)%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:Upp`:`:Huge`:`:CHUNK:%- [@(0.0.255) enum]_[* CHUNK]_`=_[@3 1024]_`*_[@3 1024]_&]
[s2; This is the size of allocation chunk for [* Huge], currently at 
1MB.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Huge`:`:AddChunk`(`):%- [_^Upp`:`:byte^ byte]_`*[* AddChunk]()&]
[s2; Adds a chunk and returns a pointer to it. Client code must respect 
the [* CHUNK] size.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Huge`:`:Finish`(int`):%- [@(0.0.255) void]_[* Finish]([@(0.0.255) int]_[*@3 last`_
chunk`_size])&]
[s2; After adding a last chunk, [* Huge] is `'closed`' with calling 
this method with the size of its [%-*@3 last`_chunk`_size]. Usually, 
[* Huge] gets data from some source (like Stream) which returns 
full sized chunks until the last one.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Huge`:`:GetSize`(`)const:%- [_^size`_t^ size`_t]_[* GetSize]()_[@(0.0.255) cons
t]&]
[s2; Returns the number of bytes stored.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Huge`:`:Get`(void`*`,size`_t`,size`_t`)const:%- [@(0.0.255) void]_[* Get]([@(0.0.255) v
oid]_`*[*@3 t`_], [_^size`_t^ size`_t]_[*@3 pos], [_^size`_t^ size`_t]_[*@3 sz])_[@(0.0.255) c
onst]&]
[s2; Reads data into target pointer.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Huge`:`:Get`(void`*`):%- [@(0.0.255) void]_[* Get]([@(0.0.255) void]_`*[*@3 t])&]
[s2; Same as Get(t, 0, GetSize()).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Huge`:`:Get`(`)const:%- [_^Upp`:`:String^ String]_[* Get]()_[@(0.0.255) const]&]
[s2; Returns data as String. There must be less than 2GB of data 
or operation fails with Panic.&]
[s3;%- ]]