topic "About Stream";
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
[s0; [*R6 About Stream]&]
[s0; [/ (some loosely organized comments about Stream by Konstantin 
Hartwich)]&]
[s0;/ &]
[s0;i150;O0; Stream represents only a logical `'cut out piece`' or 
a finite snapshot of a per definition concurrent, infinite data 
stream, beeing processed, handled or available to some extent. 
hence the pos variable, it it is indicating the current offset 
or pos`'ition of the data chunk represented by your Stream instance, 
from the logical start or beginning of stream.&]
[s0; &]
[s0;i150;O0; Stream is basicly only an interface class with some 
pointers to ref some memory space somewhere. it usually does 
not contain (own in U`+`+ terms) the data itself. Thats why it 
may have MemStream, StringStream, FileStream etc.. `'they`' access 
or even contain (StringBuffer) the buffer and are using the pointers 
from Stream base to handle it. The buffer referenced by Stream 
is a current chunk of data, `*entirely`* accessable in your stream.&]
[s0; &]
[s0;i150;O0; Stream is unidirectional per definition and should be 
used as such. In contrast to other Stream implementations, Upp 
Stream brings in all to be used both as Input or as Output stream. 
these 2 modes are supported in one single instance, but should`'t 
be used at same time. Nevertheless, it does not produce ASSERT, 
Exception or errror messages if one tries to Put and Get stuff 
from same Stream, it simply might not be logical or what you 
expect or want, because Stream uses only one ptr to represent 
current `'head`' position for reading or writing. (thus it is 
not intrinsically possible to use a MemStream as a Circular Buffer, 
which would be nice. (btw, how about implementing such one?) 
These 2 Modes can be differed using the API functions IsStoring() 
/ IsLoading(). The Modes are set using SetStoring() / SetLoading() 
and are normally set automatically, depending on how you created 
the stream instance.&]
[s0; &]
[s0;i150;O0; in both modes, the extension of the buffer marks the 
accessible space for `*entire`* reading or writing, using rdlim 
and wrlim pointers. for reading, it means the current available, 
readable data chunk. For writing, it means the `*already allocated`* 
data for beeing able to write to. When reading, ptr meaning the 
curent read position. buffer to ptr is data already processed 
by read. the space from ptr till rdlim meaning the still to read 
data. When writing, buffer till ptr meaning the data, already 
written to the space, ptr till wrlim the space free to fill (before 
a Flush should be triggered, if supported, or is triggered automatically, 
ie. FileStream)&]
[s0; &]
[s0;i150;O0; Serializing stuff to Stream is quite cool. in other 
implementations, Stream has a split interface for serializing 
and deserializing stuff, so the user had to keep track which 
order the elements go and use 2 different functions basicly, 
for Serializing and Deserializing. upp stream puts away this 
head ache. it uses 1 interface, and handles the difference about 
serialize/deserialize internally `& implicitly with the help 
of IsLoading() / IsStoring()..the user benefits from this only 
having to specify `*one`* functionional place, that determines 
the order of serialization and that is maintained the same on 
both directions, hurray. (drawback, one cant deserialize from 
a `'const Stream `&`', because the function needs a `'Stream 
`&`', but the case where this happens is to be neglacted). since 
this is somehow unusual, one needs to get familiar with the Stream.&]
[s0; &]
[s0;i150;O0; A global template operator%(Stream `&, T `&) enables 
your class to be serialized as well by the user, simply calling 
stream % yourclass. to make this happen, provide a `'void Serialize(Stream`& 
s)`' function in your class, where you define the the behaviour 
and order of de/serialization of your content (other data types 
also beeing serializable). after deserializing your class, finishing 
init of your class should be done. this makes handling of data 
transport using Socket quite easy, but is not yet implemented 
Smile&]
[s0; &]
[s0;i150;O0; You can easily implement own buffered Streams, it provides 
a mainly protected interface. Stream provides a rich default 
serialization interface to serialize all kind of stuff, including 
NTL containers...(recursively calling serialize on them then 
as well..)&]
[s0; &]
[s0;i150;O0; One should know pretty well what kind of Stream to use 
what for.. i.e. a MemStream cant be used as a self growing Buffer, 
it handles only a previously provided chunk of memory which cant 
grow just like that (because the memory chunk cant grow). for 
this purpose, use StringStream.&]
[s0; &]
[s0;i150;O0; The basic [/ implementation] of a Stream is this: a Stream 
refers to a chunk of memory, represented by `'buffer`' pointer. 
it implicitly stores its size with the rdlim and wrlim pointers, 
representing the extension of the buffer for reading and writing 
operations. every API function asumes that it can store or read 
its data directly in this chunk of data, at ptr location, normally 
whitout invoking any kind of flush or the like, advancing the 
ptr though. But sooner or later it will have consumed it`'s space 
(reacing respective rdlim, means having read all, or wrlim, meaning 
having written all). then it will claim some `'upper level`' 
action to either provide more data, done by advanceing the snapshot 
position in the read case, or writing out stored data and mark 
it as free again. this is done invoking `_Get(..) or `_Put(..). 
in other words... `_Put normally should take care of processing 
the full buffer by flushing it somehow, process the data provided, 
that didnt fit in the full buffer, and rewinding the ptr and 
adjusting wrlim, declaring buffer empty. `_Get typically claims 
some more data to be made available inside the Stream, maybe 
by copying some data in provided empty buffer first, then remainder 
in buffer and again rewinding the ptr and adjusting rdlim. this 
behaviour is to be defined somehow, and is special for any kind 
of stream.&]
[s0; &]
[s0;i150;O0; Flushing behavior is only used for write side, or out 
buffers. it is not invoked by generic Stream implementation by 
default. but higher level Streams use it in to do exacely this. 
flushing queued data to the underying destination (File Streams 
only so far).&]
[s0; &]
[s0;i150;O0; there are several helper functions around handling Streams, 
even copying, which is normally not possible just like that, 
or stuff like providing a version for streamed data and some 
constraints on version (min, max) when serializing. take a look 
in Stream.cpp.&]
[s0; &]
[s0;i150;O0; OutStream uses a small internal buffer to accumulate 
things, before it forwards them to a to be user implemented Out(..) 
function, which should take care of processing it somehow (sending 
somewhere or what ever..)&]
[s0; &]
[s0;i150;O0; TeeStream is an OutStream that uses internal buffer 
again, and when time has come, pushes it to 2 other streams..(so 
a little `'data latency`' is expected..if you want to make the 
data be available at once, call Flush() after your operations..&]
[s0; ]]