topic "Buffer";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 Buffer]]}}&]
[s3; &]
[s1;:noref:%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T][@(0.0.255) >]&]
[s1;:Buffer`:`:class:%- [@(0.0.255) class]_[* Buffer]_:_[@(0.0.255) private]_[*@3 Moveable][@(0.0.255) <
]_[* Buffer][@(0.0.255) <][*@4 T][@(0.0.255) >]_>_&]
[s0;%- &]
[s0; [%-*C@4 T]-|Type of elements stored in Buffer.&]
[s0; &]
[s0; Buffer is a simple class used to manage a fixed size plain old 
C dynamically allocated vector of elements of a specified type. 
The size of the buffer is specified as a constructor parameter 
and it can be also reallocated with new size, while loosing all 
current data.&]
[s0; &]
[s0; Buffer is a [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ moveable][*/  
]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick] (only) 
transfer semantics. Calling methods of picked Buffer is logic 
error with the exceptions of:&]
[s0; [C+75 void Alloc(int size);]&]
[s0; [C+75 void Alloc(int size, const T`& in);]&]
[s0; [C+75 void Clear();]&]
[s0; [C+75 void operator`=(pick`_ Buffer`& v);]&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3; &]
[s5;:Buffer`:`:Buffer`(`):%- [* Buffer]()&]
[s2; Constructs an empty buffer.&]
[s3;%- &]
[s4;%- &]
[s5;:Buffer`:`:Buffer`(size`_t`):%- [* Buffer](size`_t_[*@3 size])&]
[s2; Constructs the Buffer with a [%-*@3 size][%-  number of elements].&]
[s6; T must have default constructor.&]
[s3; &]
[s4;%- &]
[s5;:Buffer`:`:Buffer`(size`_t`,const T`&`):%- [* Buffer](size`_t_[*@3 size], 
[@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 init])&]
[s2; Constructs the Buffer initializing the [%-@3 size ]elements to 
the specified value [%-@3 init].&]
[s6; T must have default constructor.&]
[s3;%- &]
[s4;%- &]
[s5;:Buffer`:`:Buffer`(pick`_ Buffer`&`):%- [* Buffer]([@(0.128.128) pick`_]_[* Buffer][@(0.0.255) `&
]_[*@3 v])&]
[s2;%- [%% Pick constructor. Destroys source container ][*@3 v.]&]
[s3;%- &]
[s4;%- &]
[s5;:Buffer`:`:`~Buffer`(`):%- [@(0.0.255) `~][* Buffer]()&]
[s2; Default destructor.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Buffer`:`:Buffer`(size`_t`,std`:`:initializer`_list`<T`>`):%- [* Buffer](si
ze`_t_[*@3 size], [_^http`:`/`/en`.cppreference`.com`/w`/cpp`/utility`/initializer`_list^ s
td`::initializer`_list]<[*@4 T]>_[*@3 init])&]
[s5;:Upp`:`:Buffer`:`:Buffer`(std`:`:initializer`_list`<T`>`):%- [* Buffer]([_^http`:`/`/en`.cppreference`.com`/w`/cpp`/utility`/initializer`_list^ s
td`::initializer`_list]<[*@4 T]>_[*@3 init])&]
[s2; C`+`+ 11 initialization.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3;%- &]
[s5;:Buffer`:`:operator`=`(pick`_ Buffer`&`):%- [@(0.0.255) void]_[* operator`=]([@(0.128.128) p
ick`_]_[_^Buffer^ Buffer][@(0.0.255) `&]_[*@3 v])&]
[s2; Pick operator. Source buffer [%-*@3 v ]is destroyed.&]
[s3;%- &]
[s4;%- &]
[s5;:Buffer`:`:operator T`*`(`):%- [* operator_T`*]()&]
[s5;:Buffer`:`:operator const T`*`(`)const:%- [* operator_const_T`*]()_[@(0.0.255) const]&]
[s5;:Buffer`:`:operator`~`(`):%- [*@4 T]_`*[* operator`~]()&]
[s5;:Buffer`:`:operator`~`(`)const:%- [@(0.0.255) const]_[*@4 T]_`*[* operator`~]()_[@(0.0.255) c
onst]&]
[s5;:Upp`:`:Buffer`:`:Get`(`):%- [*@4 T]_`*[* Get]()&]
[s5;:Upp`:`:Buffer`:`:Get`(`)const:%- [@(0.0.255) const]_[*@4 T]_`*[* Get]()_[@(0.0.255) cons
t]&]
[s5;:Upp`:`:Buffer`:`:begin`(`):%- [*@4 T]_`*[* begin]()&]
[s5;:Upp`:`:Buffer`:`:begin`(`)const:%- [@(0.0.255) const]_[*@4 T]_`*[* begin]()_[@(0.0.255) c
onst]&]
[s2; Returns a pointer to the first element of the Buffer or NULL 
if the Buffer is empty.&]
[s3;%- &]
[s4;%- &]
[s5;:Buffer`:`:Alloc`(size`_t`):%- [@(0.0.255) void]_[* Alloc](size`_t_[*@3 size])&]
[s2; Clears the buffer and allocates it with the new size [%-*@3 size]. 
All current elements are lost.&]
[s6; T must have default constructor.&]
[s3; &]
[s4;%- &]
[s5;:Buffer`:`:Alloc`(size`_t`,const T`&`):%- [@(0.0.255) void]_[* Alloc](size`_t_[*@3 size
], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 in])&]
[s2; Clears the buffer and allocates it with the new size [%-*@3 size], 
using the initialization value [%-*@3 in]. All current elements 
are lost.&]
[s6; T must have deep copy constructor.&]
[s3; &]
[s4;%- &]
[s5;:Buffer`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Clears the buffer to the same state as default constructor. 
All current elements are destroyed.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Buffer`:`:IsEmpty`(`)const:%- [@(0.0.255) bool]_[* IsEmpty]()_[@(0.0.255) const
]&]
[s2; Returns true if Buffer is empty.&]
[s3;%- &]
[s0; ]]