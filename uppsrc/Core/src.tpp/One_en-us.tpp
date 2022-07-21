topic "One";
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
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 One]]}}&]
[s3; &]
[s1;:noref: [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][@(0.0.255)3 >]&]
[s1;:One`:`:class: [@(0.0.255) class]_[* One]_:_[@(0.0.255) private]_[*@3 MoveableAndDeepCopyO
ption][@(0.0.255) <]_[* One][@(0.0.255) <][*@4 T][@(0.0.255) >]_>_&]
[s8;%% [%-*C@4 T]-|Type or base class of element stored in One.&]
[s0;%% &]
[s9;%% One is a container capable of containing none or single element 
of type specified as template argument or derived from it.&]
[s9;%% The container is similar to [^http`:`/`/en`.cppreference`.com`/w`/cpp`/memory`/unique`_ptr^ s
td`::unique`_ptr]., but unlike it, it is treated more like container 
than smart pointer. For example, it propagates constantness of 
One to contained element.&]
[s9;%% One is [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ moveable][*/  
]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick and 
optional clone] transfer semantics.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor List]]}}&]
[s0;0 &]
[s5;:One`:`:One`(`): [* One]()&]
[s2;%% Default constructor. Constructs empty One.&]
[s3; &]
[s4; &]
[s5;:One`:`:One`(T`*`): [* One]([*@4 T]_`*[*@3 newt])&]
[s2;%% Constructs One with content [%-*@3 newt]. Content is specified 
by pointer to object created using operator new. One takes over 
ownership of this this object.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:One`:`:One`(Upp`:`:One`<TT`>`&`&`): [@(0.0.255) template]_<[@(0.0.255) class]_
[*@4 TT]>_[* One]([* One]<[*@4 TT]>`&`&_[*@3 p])&]
[s2;%% Pick constructor. [%-*@4 TT] must be the same as [%-*@4 T] or 
derived from [%-*@4 T].&]
[s3;%% &]
[s4; &]
[s5;:One`:`:One`(const One`<T`>`&`,int`): [* One]([@(0.0.255) const]_[* One][@(0.0.255) <][*@4 T
][@(0.0.255) >`&]_[*@3 p], [@(0.0.255) int])&]
[s6;%% Optional deep copy constructor.&]
[s2;%% Requires T to have deep copy constructor or optional deep 
copy constructor.&]
[s3;%% &]
[s4; &]
[s5;:One`:`:`~One`(`): [@(0.0.255) `~][* One]()&]
[s2;%% Default destructor.&]
[s3; &]
[s1;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Method List]]}}&]
[s0;0 &]
[s5;:One`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2; Removes the element stored in [* One].&]
[s3;%% &]
[s4; &]
[s5;:One`:`:IsPicked`(`)const: [@(0.0.255) bool]_[* IsPicked]()_[@(0.0.255) const]&]
[s2;%% Returns [* true] if [* One] has been picked.&]
[s3;%% &]
[s4; &]
[s5;:One`:`:Attach`(T`*`): [@(0.0.255) void]_[* Attach]([*@4 T]_`*[*@3 data])&]
[s2;%% Assigns element to One. (Note: if there is content in One while 
calling this function, it is destroyed). [%-*C@3 data] must be 
pointer to the new content created using operator new.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:One`:`:Detach`(`): [*@4 T]_`*[* Detach]()&]
[s2;%% Removes content giving up ownership. Client is responsible 
for deletion of content. If One is empty, return value is NULL, 
otherwise the return value is pointer to the content allocated 
on the heap.&]
[s3; &]
[s4; &]
[s5;:One`:`:operator`=`(T`*`): [@(0.0.255) void]_[* operator`=]([*@4 T]_`*[*@3 data])&]
[s7;%% Same as Attach([%-*@3 data]) (operator version).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:One`:`:operator`=`(Upp`:`:One`<TT`>`&`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 TT]>_[@(0.0.255) void]_[* operator`=]([_^Upp`:`:One^ One]<[*@4 TT]>`&`&_[*@3 d])&]
[s2;%% Pick assignment. [%-*@4 TT] must be the same as [%-*@4 T] or derived 
from [%-*@4 T].&]
[s3; &]
[s4; &]
[s5;:One`:`:operator`-`>`(`)const: [@(0.0.255) const]_[*@4 T]_`*[* operator`->]()_[@(0.0.255) c
onst]&]
[s2;%% Constant content access operator. Illegal if there is no content. 
Returns constant pointer to content.&]
[s3; &]
[s4; &]
[s5;:One`:`:operator`-`>`(`): [*@4 T]_`*[* operator`->]()&]
[s2;%% Content access operator. Illegal if there is no content. Returns 
pointer to content.&]
[s3; &]
[s4; &]
[s5;:One`:`:operator`~`(`)const: [@(0.0.255) const]_[*@4 T]_`*[* operator`~]()_[@(0.0.255) co
nst]&]
[s5;:Upp`:`:One`:`:Get`(`)const: [@(0.0.255) const]_[*@4 T]_`*[* Get]()_[@(0.0.255) const]&]
[s2;%% Constant content pointer access. Returns constant pointer 
to content or NULL when there is no content.&]
[s3; &]
[s4; &]
[s5;:One`:`:operator`~`(`): [*@4 T]_`*[* operator`~]()&]
[s5;:Upp`:`:One`:`:Get`(`): [*@4 T]_`*[* Get]()&]
[s2;%% Content pointer access. Returns pointer to content or NULL 
when there is no content.&]
[s3; &]
[s4; &]
[s5;:One`:`:operator`*`(`)const: [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* operator`*]()_[@(0.0.255) c
onst]&]
[s2;%% Content constant reference access. Illegal if there is no 
content. Returns constant reference to content.&]
[s3; &]
[s4; &]
[s5;:One`:`:operator`*`(`): [*@4 T][@(0.0.255) `&]_[* operator`*]()&]
[s2;%% Content reference access. Illegal if there is no content. 
Returns reference to content.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:One`:`:Create`(Args`&`&`.`.`.args`): [@(0.0.255) template]_<[@(0.0.255) class
]_[*@4 TT], [@(0.0.255) class...]_[*@4 Args]>_[*@4 TT][@(0.0.255) `&]_[* Create]([*@4 Args][@(0.0.255) `&
`&...]_args)&]
[s2;%% Creates [%-*@4 TT] content in One. If there is content in One 
while calling this function, it is destroyed. Additional arguments 
can be specified for [%-*@4 TT ]constructor.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:One`:`:Create`(`): [*@4 T][@(0.0.255) `&]_[* Create]()&]
[s2;%% Creates [%-*@4 T] content in One (default constructed).&]
[s3; &]
[s4; &]
[s5;:One`:`:Is`(`)const: [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 TT]>_[@(0.0.255) bool
]_[* Is]()_[@(0.0.255) const]&]
[s2;%% Returns true if One contains object of type [%-*@4 TT] or derived 
from [%-*@4 TT]. [%-*@4 T] must be polymorphic.&]
[s3; &]
[s4; &]
[s5;:One`:`:IsEmpty`(`)const: [@(0.0.255) bool]_[* IsEmpty]()_[@(0.0.255) const]&]
[s2;%% Returns [* true] if there is no content.&]
[s3; &]
[s4; &]
[s5;:One`:`:operator bool`(`)const: [* operator_bool]()_[@(0.0.255) const]&]
[s2;%% Returns [* true] if there is content.&]
[s0; &]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 MakeOne]]}}&]
[s0; &]
[s5;:Upp`:`:MakeOne`(Args`&`&`.`.`.`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T], 
[@(0.0.255) class...]_[*@4 Args]>_[_^Upp`:`:One^ One]<[*@4 T]>_[* MakeOne]([*@4 Args][@(0.0.255) .
..]_args)&]
[s2;%% This simple utility function creates One<T> instance.&]
[s0; ]]