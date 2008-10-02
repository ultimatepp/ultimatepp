topic "One";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a17;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[{_}%EN-US 
[s0;%- One&]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 T][@(64) >__][@(0.0.255) class][@(64) _]One[@(64) _
:_][@(0.0.255) private][@(64) _][^`:`:MoveableAndDeepCopyOption^@(64) MoveableAndDeepCop
yOption][@(64) <_]One[@(64) <][@4 T][@(64) >_>_]&]
[s2; &]
[s0; [%-*C@4 T]-|Type or base class of element stored in One.&]
[s0; One is a container capable of containing none or single element 
of type specified as template argument or derived from it.&]
[s0; One is [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ moveable][*/  
]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick and 
optional deep copy] transfer semantics. Calling methods of picked 
One is logic error with exception of&]
[s0; [C+75 void ][*C+75 operator`=][C+75 (pick`_ One`& ][*C@3+75 v][C+75 )]&]
[s0; [C+75 void ][*C+75 operator<<`=][C+75 (const One`& ][*C@3+75 v][C+75 )][+75  
][/+75 (defined using DeepCopyOption template base class)]&]
[s0; [C+75 void ][*C+75 Clear][C+75 ()]&]
[s0; [C+75 bool ][*C+75 IsPicked][C+75 () const]&]
[s0; Members&]
[s0; &]
[s0;:`:`:One`:`:One`(`):%- [* One]()&]
[s2; Constructor. Constructs empty One.&]
[s0; &]
[s0;:`:`:One`:`:One`(T`*`):%- [* One](T_`*[*@3 newt])&]
[s2; Constructs One with content. Content is specified by pointer 
to object created using operator new. One takes over ownership 
of this this object. &]
[s7; [*C@3 newt]-|Object to be added.&]
[s0; &]
[s0;:`:`:One`:`:One`(pick`_`:`:One`<T`>`&`):%- [* One](pick`__[* One]<T>`&_[*@3 p])&]
[s2; Pick constructor. Transfers source One in low constant time, 
but destroys it by picking. &]
[s7; [*C@3 p]-|Source One.&]
[s0; &]
[s0;:`:`:One`:`:One`(const`:`:One`<T`>`&`,int`):%- [* One](const_[* One]<T>`&_[*@3 p], 
int)&]
[s2; Optional deep copy constructor.&]
[s6; Requires T to have deep copy constructor or optional deep copy 
constructor.&]
[s7; [*C@3 p]-|Source One.&]
[s0; &]
[s0;:`:`:One`:`:`~One`(`):%- `~[* One]()&]
[s2; Destructor.&]
[s0; &]
[s5;K:`:`:One`:`:Attach`(T`*`):%- [@(0.0.255) void][@(64) _]Attach[@(64) (T_`*][@3 data][@(64) )
]&]
[s2; Assigns element to One. (Note: if there is content in One while 
calling this function, it is destroyed).&]
[s7; [%-*C@3 data]-|Pointer to the new element created using operator 
new.&]
[s0; &]
[s5;K%- T_`*Detach[@(64) ()_pick`_]&]
[s2; Removes content giving up ownership. Client is responsible for 
deletion of content.&]
[s7; [*/ Return value]-|Pointer to the content allocated on the heap.&]
[s0; &]
[s5;K%- T_`*operator`-[@(64) ()_pick`_]&]
[s2; Same as Detach() (operator version).&]
[s7; [*/ Return value]-|Pointer to the content allocated on the heap.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Clear[@(64) ()]&]
[s2; Removes and destroys content.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]operator`=[@(64) (T_`*][@3 data][@(64) )]&]
[s2; Same as Attach(data) (operator version).&]
[s7; [%-*C@3 data]-|Pointer to the new element created using operator 
new.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]operator`=[@(64) (pick`__][^`:`:One^@(64) One][@(64) <][^T^@(64) T
][@(64) >`&_][@3 d][@(64) )]&]
[s2; Pick operator. Transfers source One in low constant time, but 
destroys it by picking.&]
[s7; [%-*C@3 d]-|Source One.&]
[s0; &]
[s5;K%- [@(0.0.255) const][@(64) _T_`*]operator`->[@(64) ()_][@(0.0.255) const]&]
[s2; Constant content access operator. Illegal if there is no content.&]
[s7; [*/ Return value]-|Returns constant pointer to content.&]
[s0; &]
[s5;K%- T_`*operator`->[@(64) ()]&]
[s2; Content access operator. Illegal if there is no content.&]
[s7; [*/ Return value]-|Returns pointer to content.&]
[s0; &]
[s5;K%- [@(0.0.255) const][@(64) _T_`*]operator`~[@(64) ()_][@(0.0.255) const]&]
[s2; Constant content pointer access.&]
[s7; [*/ Return value]-|Returns constant pointer to content or NULL 
when there is no content.&]
[s0; &]
[s5;K%- T_`*operator`~[@(64) ()]&]
[s2; Content pointer access.&]
[s7; [*/ Return value]-|Returns pointer to content or NULL when there 
is no content.&]
[s0; &]
[s5;K%- [@(0.0.255) const][@(64) _T`&_]operator`*[@(64) ()_][@(0.0.255) const]&]
[s2; Content constant reference access. Illegal if there is no content.&]
[s7; [*/ Return value]-|Constant reference to content.&]
[s0; &]
[s5;K%- T`&_operator`*[@(64) ()]&]
[s2; Content reference access. Illegal if there is no content.&]
[s7; [*/ Return value]-|Reference to content.&]
[s0; &]
[s0;%- [* template <class ][*@4 TT][* >][*@4  TT]`&_[* Create]()&]
[s2; Creates content in One. (Note: if there is content in One while 
calling this function, it is destroyed).&]
[s7; [*C@4 TT]-|Type of content. Must be equal to T or derived from 
it.&]
[s7; [*/ Return value]-|Reference to actual content (of type TT).&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]IsPicked[@(64) ()_][@(0.0.255) const]&]
[s7; [*/ Return value]-|true if One is picked.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]IsEmpty[@(64) ()_][@(0.0.255) const]&]
[s7; [*/ Return value]-|true if there is no content.&]
[s0; &]
[s5;K%- operator_[@(0.0.255) bool][@(64) ()_][@(0.0.255) const]&]
[s7; [*/ Return value]-|true if there is content.&]
[s0; ]