topic "Buffer";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a17;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[{_}%EN-US 
[s0; Buffer&]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 T][@(64) >__][@(0.0.255) class][@(64) _]Buffer[@(64) _
:_][@(0.0.255) private][@(64) _][^`:`:Moveable^@(64) Moveable][@(64) <_]Buffer[@(64) <][@4 T][@(64) >
_>_]&]
[s0; class_[* Buffer]&]
[s2; &]
[s0; [%-*C@4 T]-|Type of elements stored in Buffer.&]
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
[s0; [C+75 void Alloc(int size);]&]
[s0; [C+75 void Alloc(int size, const T`& in);]&]
[s0; [C+75 void Clear();]&]
[s0; [C+75 void operator`=(pick`_ Buffer`& v);]&]
[s0;*C@4 &]
[s0;%- [%%/ Derived from][%%  ]Moveable< Buffer<T> > &]
[s0; &]
[s0;:`:`:Buffer`:`:Buffer`(`):%- [* Buffer]()&]
[s2; Constructs an empty buffer.&]
[s0; &]
[s0;:`:`:Buffer`:`:Buffer`(int`):%- [* Buffer](int_[*@3 size])&]
[s2; Constructs the Buffer.&]
[s6; [@4 T][@0  must have default constructor.]&]
[s7; [%-*C@3 size]-|Number of elements.&]
[s0; &]
[s0;:`:`:Buffer`:`:Buffer`(int`,const T`&`):%- [* Buffer](int_[*@3 size], 
const_T`&_[*@3 init])&]
[s2; Constructs the Buffer initializing the elements to the specified 
value.&]
[s6; [@4 T][@0  must have deep copy constructor.]&]
[s7; [%-*C@3 size]-|Number of elements.&]
[s7; [%-*C@3 init]-|Initialization value.&]
[s0; &]
[s0;:`:`:Buffer`:`:Buffer`(pick`_`:`:Buffer`&`):%- [* Buffer](pick`__[* Buffer]`&_[*@3 v])&]
[s2; Pick constructor.&]
[s7; [%-*C@3 v]-|Source Buffer.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]operator`=[@(64) (pick`__][^`:`:Buffer^@(64) Buffer][@(64) `&
_][@3 v][@(64) )]&]
[s2; Pick operator.&]
[s7; [%-*C@3 v]-|Source buffer to be picked.&]
[s0; &]
[s5;K%- operator_T`*()&]
[s7; [*/ Return value]-|Returns a pointer to the first element of the 
Buffer or NULL if the Buffer is empty.&]
[s0; &]
[s5;K%- operator_[@(0.0.255) const][@(64) _T`*()_][@(0.0.255) const]&]
[s7; [*/ Return value]-|Returns a pointer to the first element of the 
Buffer or NULL if the Buffer is empty.&]
[s0; &]
[s5;K%- T_`*operator`~[@(64) ()]&]
[s7; [*/ Return value]-|Returns a pointer to the first element of the 
Buffer or NULL if the Buffer is empty.&]
[s0; &]
[s5;K%- [@(0.0.255) const][@(64) _T_`*]operator`~[@(64) ()_][@(0.0.255) const]&]
[s7; [*/ Return value]-|Returns a pointer to the first element of the 
Buffer or NULL if the Buffer is empty.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Alloc[@(64) (][@(0.0.255) int][@(64) _][@3 size][@(64) )]&]
[s2; Clears the buffer and allocates it with the new size. All current 
elements are lost.&]
[s6; [@4 T][@0  must have default constructor.]&]
[s7; [%-*C@3 size]-|Number of elements.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Alloc[@(64) (][@(0.0.255) int][@(64) _][@3 size][@(64) , 
][@(0.0.255) const][@(64) _T`&_][@3 in][@(64) )]&]
[s2; Clears the buffer and allocates it with the new size, using 
the initialization value. All current elements are lost.&]
[s6; [@4 T][@0  must have deep copy constructor.]&]
[s7; [%-*C@3 size]-|Number of elements.&]
[s7; [%-*C@3 in]-|Initialization value.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Clear[@(64) ()]&]
[s0; Clears the buffer to the same state as default constructor. 
All current elements are destroyed.]