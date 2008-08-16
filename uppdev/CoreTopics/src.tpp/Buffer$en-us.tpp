topic "Buffer";[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[l288;a17;*1 $$7,7#70004532496200323422659154056402:requirement]
[{_}%EN-US 
[s3; Buffer&]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 T][%00-00 >__][%00-00@(0.0.255) c
lass][%00-00 _][%00-00@0 Buffer][%00-00 _:_][%00-00@(0.0.255) private][%00-00 _][%00-00^`:`:Moveable^ M
oveable][%00-00 <_][%00-00@0 Buffer][%00-00 <][%00-00@4 T][%00-00 >_>_]&]
[s0;* [* class_]Buffer&]
[s2; &]
[s0; [%00-00*C@4 T]-|Type of elements stored in Buffer.&]
[s0; &]
[s0; Buffer is a simple class used to manage a fixed size plain old 
C dynamically allocated vector of elements of a specified type. 
The size of the buffer is specified as a constructor parameter 
and it can be also reallocated with new size, while loosing all 
current data.&]
[s0; Buffer is a [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ moveable][*/  
]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick] (only) 
transfer semantics. Calling methods of picked Buffer is logic 
error with the exceptions of&]
[s0;C+75 void Alloc(int size);&]
[s0;C+75 void Alloc(int size, const T`& in);&]
[s0;C+75 void Clear();&]
[s0;C+75 void operator`=(pick`_ Buffer`& v);&]
[s0;*C@4 &]
[s0; [/ Derived from] [%00-00 Moveable< Buffer<T> > ]&]
[s0;3 &]
[s0;:`:`:Buffer`:`:Buffer`(`): [%00-00* Buffer][%00-00 ()]&]
[s2; Constructs an empty buffer.&]
[s0;3 &]
[s0;:`:`:Buffer`:`:Buffer`(int`): [%00-00* Buffer][%00-00 (int_][%00-00*@3 size][%00-00 )]&]
[s2; Constructs the Buffer.&]
[s7; [@4 T] must have default constructor.&]
[s4; [%00-00*C@3 size]-|Number of elements.&]
[s0;3 &]
[s0;:`:`:Buffer`:`:Buffer`(int`,const T`&`): [%00-00* Buffer][%00-00 (int_][%00-00*@3 size][%00-00 ,
 const_T`&_][%00-00*@3 init][%00-00 )]&]
[s2; Constructs the Buffer initializing the elements to the specified 
value.&]
[s7; [@4 T] must have deep copy constructor.&]
[s4; [%00-00*C@3 size]-|Number of elements.&]
[s4; [%00-00*C@3 init]-|Initialization value.&]
[s0;3 &]
[s0;:`:`:Buffer`:`:Buffer`(pick`_`:`:Buffer`&`): [%00-00* Buffer][%00-00 (pick`__][%00-00* B
uffer][%00-00 `&_][%00-00*@3 v][%00-00 )]&]
[s2; Pick constructor.&]
[s4; [%00-00*C@3 v]-|Source Buffer.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 operator`=][%00-00 (pick`__][%00-00^`:`:Buffer^ B
uffer][%00-00 `&_][%00-00@3 v][%00-00 )]&]
[s2; Pick operator.&]
[s4; [%00-00*C@3 v]-|Source buffer to be picked.&]
[s0;3 &]
[s5;K [%00-00 operator_T`*()]&]
[s4; [*/ Return value]-|Returns a pointer to the first element of the 
Buffer or NULL if the Buffer is empty.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00@(64) operator_][%00-00 const][%00-00@(64) _T`*()_][%00-00 const]&]
[s4; [*/ Return value]-|Returns a pointer to the first element of the 
Buffer or NULL if the Buffer is empty.&]
[s0;3 &]
[s5;K [%00-00 T_`*][%00-00@0 operator`~][%00-00 ()]&]
[s4; [*/ Return value]-|Returns a pointer to the first element of the 
Buffer or NULL if the Buffer is empty.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 const][%00-00@(64) _T_`*][%00-00@0 operator`~][%00-00@(64) ()_][%00-00 c
onst]&]
[s4; [*/ Return value]-|Returns a pointer to the first element of the 
Buffer or NULL if the Buffer is empty.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Alloc][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 s
ize][%00-00 )]&]
[s2; Clears the buffer and allocates it with the new size. All current 
elements are lost.&]
[s7; [@4 T] must have default constructor.&]
[s4; [%00-00*C@3 size]-|Number of elements.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Alloc][%00-00 (][%00-00@(0.0.255) int][%00-00 _][%00-00@3 s
ize][%00-00 , ][%00-00@(0.0.255) const][%00-00 _T`&_][%00-00@3 in][%00-00 )]&]
[s2; Clears the buffer and allocates it with the new size, using 
the initialization value. All current elements are lost.&]
[s7; [@4 T] must have deep copy constructor.&]
[s4; [%00-00*C@3 size]-|Number of elements.&]
[s4; [%00-00*C@3 in]-|Initialization value.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Clear][%00-00 ()]&]
[s2; Clears the buffer to the same state as default constructor. 
All current elements are destroyed.]