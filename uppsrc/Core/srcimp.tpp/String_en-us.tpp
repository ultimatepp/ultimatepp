topic "String and WString";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[l416;2 $$7,7#55548704457842300043401641954952:nested`-desc]
[l288;i448;a25;kO9;*2 $$8,8#64691275497409617375831514634295:nested`-class]
[{_}%EN-US 
[s3; String and WString&]
[s0; Core implementation of String and WString is totally independent.&]
[s0; &]
[s0; Core implementation is represented by String0 and WString0 classes. 
Common methods shared by both are then added using AString template 
(which inherits either String0 or WString0). Implementation is 
then finalized in String and WString final classes.&]
[s0; &]
[s6; String&]
[s0; String uses three step implementation to maximize performance.&]
[s0; &]
[s0; Strings up to 14 characters (bytes) can be stored using small`-string 
optimization inside String value. sizeof(String) is 16; first 
14 bytes (0`- 13) in this case are character data, byte 14 is 
reserved for zero terminator (and is always zero for this kind), 
byte 15 is the string len. Also, for this first kind of string, 
all characters len ... 14 are strictly kept zero `- this allows 
very fast implementation of equality comparison.&]
[s0; &]
[s0; For strings up to 31 characters, bytes 0`-7 are used for pointer 
to character data. Character data are allocated as 32 bytes block 
(one byte for zero terminator); String implementation is directly 
connected with heap allocator to bypass size processing here 
and achieve maximum performance. Length of String is stored in 
the bytes 8`-11. Byte 14 is in this case 31 (`"MEDIUM`") `- it 
is the maximum number allowed to be stored in the buffer. Note 
that non`-zero value in byte 14 signals that string is not `"small`". 
When copying String of this size, new memory block is always 
allocated and data is copied. With direct link to heap allocator, 
this is about as fast as using reference counting in single threaded 
mode and faster in multithreaded.&]
[s0; &]
[s0; Finally, if size exceeds 31 characters, String uses reference 
counting. Layout is similar to `"MEDIUM`", but memory block is 
of variable length and preceded with reference count and the 
size of block (`"alloc`") `- maximum number of characters the 
block can store. Note that as an optimization, for blocks smaller 
than 255 characters, this number is also stored in byte 14 for 
faster retrieval. If block is larger, 255 is in byte 14 and the 
`"alloc`" size has to be retrieved from the memory block.&]
[s0; &]
[s6; WString&]
[s0; WString implementation does not use small string optimization, 
first step is 23 wchars stored in 48 bytes memory block without 
reference counting, for larger string once again reference counting 
is applied. Sizes are also directly stored in `"int`" member 
variables.&]
[s0; ]]